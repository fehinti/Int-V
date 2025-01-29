#include "HLPlanner.h"


/* Header file from a project found online
Credits
Author: Nikhil Chandak
Link:  https://github.com/nikhilchandak/Rapidly-Exploring-Random-Trees.git*/
// BEGIN

Point operator*(ftype a, Point b) {
    return b * a;
}

double distance(Point& a, Point &b) {
	const double x_diff = a.x - b.x, y_diff = a.y - b.y ; 
	return sqrt(x_diff * x_diff + y_diff * y_diff);
}

double dot(Point a, Point b) {
	return (a.x*b.x + a.y*b.y);
}

double cross(Point a, Point b) {
    return (a.x*b.y - b.x*a.y); 
}

/*  Returns a point in the direction of (p2 - p1) vector such that 
    the new point is within a DELTA distance of point1  */
Point stepNear(Point& p1, Point& p2, ftype DELTA) {
    if( (distance(p1, p2) - DELTA) <= EPS) 
        return p2 ; 
    else {
        ftype theta = atan2(p2.y - p1.y, p2.x - p1.x);
        return Point(p1.x + DELTA * cos(theta), p1.y + DELTA * sin(theta));
    }
}

// Return minimum distance between line segment vw and point p
ftype minimum_distance(Point v, Point w, Point p) {
  ftype l2 = distance(v, w);  l2 *= l2 ; // i.e. |w-v|^2 -  avoid a sqrt
  if (l2 < EPS) return distance(p, v);   // v == w case

  // Consider the line extending the segment, parameterized as v + t (w - v).
  // We find projection of point p onto the line. 
  // It falls where t = [(p-v) . (w-v)] / |w-v|^2
  // We clamp t from [0,1] to handle points outside the segment vw.
  const ftype t = max(0.0, min(1.0, dot(p - v, w - v) / l2));

  Point projection = v + t * (w - v);  // Projection falls on the segment
  return distance(p, projection);
}

struct Polygon {
	vector < Point > points ;  // Assumes clockwise/anti-clockwise points input 
    int pointCnt = 0 ; 

	void addPoint(const Point pnt) {
		points.push_back(pnt); pointCnt++ ;
	}
	vector < Point > getPoints() {
		return points ; 
	}
	bool pointInside(const Point point) { // Can be done in log(N)
	  int i, j, nvert = points.size();
	  bool c = false;
	  for(i = 0, j = nvert - 1; i < nvert; j = i++) {
	    if( ( (points[i].y >= point.y ) != (points[j].y >= point.y) ) &&
	        (point.x <= (points[j].x - points[i].x) * (point.y - points[i].y) / (points[j].y - points[i].y) + points[i].x)
	      )
	      c = !c;
	  }
	  return c;
	}
};

/*  Return true if the given line segment intersects the circle whose center
    is at location */
bool checkCollision(Point lineFrom, Point lineTo, Point location, ftype radius)
{
	location += Point(radius, radius); // Adjust location from top-left corner to center coordinates 
    ftype ab2, acab, h2;
    Point ac = location - lineFrom;
    Point ab = lineTo - lineFrom;
    ab2 = dot(ab, ab); acab = dot(ac, ab);
    ftype t = acab / ab2;

    if (t < 0) t = 0;
    else if (t > 1) t = 1;

    Point h = ((ab * t) + lineFrom) - location;
    h2 = dot(h, h); 
    return (h2 <= (radius * radius));
}

// taken from stackoverflow: https://stackoverflow.com/questions/11716268/point-in-polygon-algorithm
// this can be done in log(N) though
bool PointInPolygon(Point point, Polygon polygon) {
  vector<Point> points = polygon.getPoints();
  int i, j, nvert = points.size();
  bool c = false;

  for(i = 0, j = nvert - 1; i < nvert; j = i++) {
    if( ( (points[i].y >= point.y ) != (points[j].y >= point.y) ) &&
        (point.x <= (points[j].x - points[i].x) * (point.y - points[i].y) / (points[j].y - points[i].y) + points[i].x)
      )
      c = !c;
  }
  return c;
}

// helper function
int sign(const ftype x) { 
    return x >= 0 ? x ? 1 : 0 : -1; 
}

/*  Returns true if two line segments on the same line intersect.
    (a, b) denote the endpoints of first line segment and 
    (c, d) denotes the endpoints of the second lint segment */
bool intersectOnLine(ftype a, ftype b, ftype c, ftype d) {
    if ((a - b) > EPS) swap(a, b);
    if ((c - d) > EPS) swap(c, d);
    return max(a, c) <= min(b, d);
}

// Returns true if the two line segments (a, b) and (c, d) intersect 
bool check_intersection(const Point a, const Point b, const Point c, const Point d) {
    // Check if both line segments lie on the same line
    if (c.cross(a, d) == 0 && c.cross(b, d) == 0) 
        return intersectOnLine(a.x, b.x, c.x, d.x) && intersectOnLine(a.y, b.y, c.y, d.y);

    // Check if a and b both lie on different side of line segment CD 
    // Similarly check for c and d to lie on different side of line segment AC 
    return sign(a.cross(b, c)) != sign(a.cross(b, d)) && sign(c.cross(d, a)) != sign(c.cross(d, b));
}

/*  Returns true if the given line segment represented by ba intersects with any
    side of the polygon */
bool lineSegmentIntersectsPolygon(Point a, Point b, Polygon& polygon) {
    // PQ is merely a point not enough distance for it be line segment
    if( a.distance(b) < EPS ) 
        return PointInPolygon( (a+b)/2.0, polygon); 

    int num = polygon.pointCnt ; 
    vector < Point > points = polygon.getPoints();
    for(int i = 0; i < num; i++) {
        int nxt = i+1; if(nxt == num) nxt = 0 ;
        if(check_intersection(a, b, points[i], points[nxt])) return true ; 
    }
    return false ; 
}

// END

HLPlanner::HLPlanner(double lane_width, double target_dist):
max_iter(1000),
vehicle({1.25,2})
{
    this->lane_width = lane_width;
    this->target_dist = target_dist;
}

HLPlanner::HLPlanner(): 
lane_width(4.0),
target_dist(10.0),
max_iter(1000),
vehicle({1.25,2})
{
}

HLPlanner::~HLPlanner()
{
}

void HLPlanner::setVehicle(Veh vehicle)
{
    this->vehicle = vehicle;
}

void HLPlanner::setLaneWidth(double width)
{
    this->lane_width = width;
}

void HLPlanner::setTarget(Point target)
{
    this->target = target;
    cout << "Target set to: " << target.x << " " << target.y << endl;
}

void HLPlanner::setSMax(double s_max)
{
    this->s_max = s_max;
}

Point HLPlanner::getTarget()
{
    return this->target;
}

void HLPlanner::setMaxIter(int max_iter)
{
    this->max_iter = max_iter;
}

void HLPlanner::addObstacle(Obs obs)
{
    this->obstacles.push_back(obs);
    // cout << "Obstacle added!!! Total Obstacles: " << this->obstacles.size() << endl;
}

bool HLPlanner::clearObstacles()
{
    if ( this->obstacles.size() > 0 )
    {
        this->obstacles.clear();
        return true;
    }
    return false;
}

// Function returns a random number in margin [low, high]
template <typename T> T RNG (T low, T high)
{
    random_device rd;
    mt19937 engine{ rd() };
    uniform_real_distribution<double> dist(low, high);
    return dist(engine);
}

Point HLPlanner::randomPoint()
{
    // double sample = RNG(0.0, 1.0); // random sample to create bias towards goal
    // if ( (sample - GOAL_SAMPLING_PROB) <= EPS && !this->is_route )
    // {
    //     return {this->target->x-this->vehicle_width/2, this->target->x+this->vehicle_width/2};
    // }
    return Point{RNG(this->start.x, this->target.x), RNG(-this->lane_width/2, this->lane_width/2)};
}

bool HLPlanner::checkTargetReached()
{
    if ( distance(this->nodes.back(), this->target) < this->goal_radius )
    {
        cout << "Target reached!!! => " << this->nodes.back().x << ", " << this->nodes.back().y << endl;
        this->is_route = true;

        // Reconstruct the path
        this->route.push_front(this->nodes.back());
        int index = this->nodes.size()-1;
        while ( this->parent[index] != index )
        {
            int parent = this->parent[index];
            cout << "Parent: " << parent << " at index: " << " node: " << this->nodes[parent].x << " : " <<
                                                        this->nodes[parent].y << endl;
            this->route.push_front(this->nodes[parent]);
            index = parent;
        }
        cout << "Number of nodes in route: " << this->route.size() << endl;
        // cout << "First parent: " << parent[parent[parent.back()]] << endl;
        // int temp = parent.back(); 
        // int temp1 = parent[temp];
        // int temp2 = parent[temp1];
        // cout << "First parent: " << temp << " node: " << this->nodes[temp].x << " : " <<
        //                                                 this->nodes[temp].y << endl;
        // cout << "Second parent: " << temp1 << " node: " << this->nodes[temp1].x << " : " <<
        //                                                 this->nodes[temp1].y << endl;
        // cout << "Third parent: " << temp2 << " node: " << this->nodes[temp2].x << " : " <<
        //                                                 this->nodes[temp2].y << endl;
        
        return true;
    }
    return false;
}

// Funtion - returns true if edge (line segment) between points a and b is obstacle free
bool isEdgeObstacleFree(Point a, Point b, const vector<Obs> &obstacles)
{
    for (auto obs : obstacles)
    {
        vector<Point> points = {{obs.x+obs.width/2, obs.y+obs.length/2},
                                {obs.x-obs.width/2, obs.y+obs.length/2},
                                {obs.x-obs.width/2, obs.y-obs.length/2},
                                {obs.x+obs.width/2, obs.y-obs.length/2}};
        Polygon poly;
        for (auto p : points)
        {
            poly.addPoint(p);
        }
        if ( lineSegmentIntersectsPolygon(a, b, poly) )
        {
            return false;
        }
    }
    return true;
}

bool HLPlanner::planRoute(vector <Point> &nodes)
{
    // RRT* algorithm
    this->nodes.clear();

    // 1. Initialize the tree with the start point
    this->start = {this->vehicle.length/2, 0};

    this->nodes.push_back(start);
    this->parent.push_back(0);

    Point new_point, closest_point, next_point(0, 0);
    int closest_index, iterations = 0;
    // double min_cost = INF;

    while ( !this->is_route && iterations < this->max_iter-1 )
    {
        iterations++;
        // cout << "Number of nodes: " << this->nodes.size() << endl;
        // 2. Generate a random point in the configuration space
        new_point = randomPoint();

        // 3. Find the closest point in the tree to the new random point
        // => Extend the tree towards the random point with a step size of s_max
        // => such that the tree is ordered [..., closest_point, new_point, ...]
        // => and each edge is obstacle free
        closest_point = *this->nodes.begin();
        closest_index = 0;

        for (auto it = this->nodes.begin(); it != this->nodes.end(); it++)
        {
            Point node = *it;
 
        // 5. Check for collisions
        // => closest point in the tree to the new random point
        // => If the edge between the closest point and the new point is obstacle free
        // => then add the new point to the tree and define the edge between the closest point and the new point
            if ( node.distance(new_point) - closest_point.distance(new_point) <= EPS &&
                 isEdgeObstacleFree(node,node.steer(new_point, this->s_max),this->obstacles) )
            {
                closest_point = node;                       // used to reconstruct final path
                closest_index = it - this->nodes.begin();    // used to reconstruct final path
                // closest_index = node - this->nodes[0];    // used to reconstruct final path
            }
        }
        // 4. Steer the tree towards the random point
        next_point = stepNear(closest_point, new_point, this->s_max);
        if( !isEdgeObstacleFree(closest_point, next_point, this->obstacles) )
        {
            continue;
        }
        // 5. Add the new point to the tree
        this->nodes.push_back(next_point);
        this->parent.push_back(closest_index);
        
        // 6. Check if the target is reached
        checkTargetReached();

    }
    nodes = this->nodes;

    return true;
}
