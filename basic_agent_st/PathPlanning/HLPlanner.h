/* Taking the car position, lane width and obstacles on the
road i.e. cone, generate a path to the lookahead distance 
iteratively*/
#ifndef RRT_HL_PLANNER_H
#define RRT_HL_PLANNER_H

/* STL Includes */
#include <deque>
#include <vector>
#include <map>

#include <math.h>

#include <iostream>
#include <cmath>
#include <random>
#include <cassert>

// defines
#define GOAL_SAMPLING_PROB 0.05
#define INF 1e18

using namespace std;

// typedef struct RoutePoint
// {
//     double x;
//     double y;
// }Point;

/* Header file from a project found online
Credits
Author: Nikhil Chandak
Link:  https://github.com/nikhilchandak/Rapidly-Exploring-Random-Trees.git*/
// BEGIN

// Type of data type to be used for all calculations (Ex: long double)
#define ftype double 

/*  NOTE: Most of the calculations are done using EPS as a factor of difference
    since double/long double doesn't store floating point values precisely (limited precision) */
const ftype EPS = 1e-6;

struct Point {
    ftype x, y;
    Point() {}
    Point(ftype x, ftype y): x(x), y(y) {}
    Point& operator+=(const Point &t) {
        x += t.x;
        y += t.y;
        return *this;
    }
    Point& operator-=(const Point &t) {
        x -= t.x;
        y -= t.y;
        return *this;
    }
    Point& operator*=(ftype t) {
        x *= t;
        y *= t;
        return *this;
    }
    Point& operator/=(ftype t) {
        x /= t;
        y /= t;
        return *this;
    }
    Point operator+(const Point &t) const {
        return Point(*this) += t;
    }
    Point operator-(const Point &t) const {
        return Point(*this) -= t;
    }
    Point operator*(ftype t) const {
        return Point(*this) *= t;
    }
    Point operator/(ftype t) const {
        return Point(*this) /= t;
    }
    ftype dot(const Point &t) const {
    	return (x*t.x + y*t.y); 
    }
    ftype cross(const Point& t) const { 
        return x * t.y - y * t.x;
    }
    ftype cross(const Point& a, const Point& b) const {
        return (a - *this).cross(b - *this); 
    }
    ftype distance(const Point &t) const {
    	const double x_diff = x - t.x, y_diff = y - t.y ; 
    	return sqrt(x_diff * x_diff + y_diff * y_diff);
    }
    Point steer(const Point& t, ftype DELTA) {
	    if(this->distance(t) < DELTA) {
	        return t ; 
	    }
	    else {
	        double theta = atan2(t.y - y, t.x - x);
	        return Point(x + DELTA * cos(theta), y + DELTA * sin(theta));
	    }
	}
    bool operator==(const Point& rhs) const
    {
        return fabs(x - rhs.x) < EPS and fabs(y - rhs.y) < EPS ; // or another approach as above
    }
};

// END

typedef struct Obstacle
{
    double x;
    double y;
    double width;
    double length;
}Obs;

typedef struct Vehicle
{
    double width;
    double length;
}Veh;


class HLPlanner
{
private:
    /* data */
    double lane_width;              // Width of the lane
    double target_dist;             // Lookahead distance
    Veh vehicle;                    // Length and width of the vehicle
    vector < Obs > obstacles;       // Obstacles in lookahead distance
    deque < Point > route;          // Route of points to the target
    vector < Point > nodes;         // Nodes in the tree - V
    vector < int > parent;          // Parent of each node - Parent V
    map <int, int> edges;           // Parent and destination of each node - Edges E

    Point start = {0,0};            // Start point
    Point target;                   // Target point
    bool is_route = false;          // Route planned flag
    bool is_static = false;         // Static obstacles planning flag

    // RRT* parameters
    double s_max;                   // Maximum distance to extend an edge
    double goal_radius = 0.5;       // Radius of the goal region
    int max_iter;                   // Maximum number of iterations

    // Private member functions
    Point randomPoint();
    bool checkTargetReached();

public:
    HLPlanner(double lane_width, double lookahead_dist);
    HLPlanner();
    ~HLPlanner();

    // Setter functions
    void setVehicle(Veh vehicle);

    void setLaneWidth(double width);
    void setTarget(Point target);
    void setSMax(double s_max);
    void setMaxIter(int max_iter);

    // Getter functions
    Point getTarget();

    void addObstacle(Obs obs);
    bool clearObstacles();
    bool planRoute(vector < Point > &nodes);


};

#endif // RRT_HL_PLANNER_H