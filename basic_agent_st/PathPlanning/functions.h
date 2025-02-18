#ifndef FUNTIONS_H    
#define FUNTIONS_H
// ^To make sure I don't declare any function more than once by including the header multiple times.

#include <math.h>
#include <vector>
#include <math.h>
#include <vector>
#include <deque>
#include <algorithm>
#include <numeric>

#include "HLPlanner.h"

#define COEFFS_DIM 6

using namespace std ; 


int IsCoeffZero( const double coeffs[] )
{
    double sum = 0;

    for ( size_t i=0; i<COEFFS_DIM ;i++ )
    {   
        sum += coeffs[i];
    }

    if ( sum == 0 ) return 1;
    else return 0;
}

// Function to calculate the moving average of a deque of points
deque<Point> MovingAverage(const deque<Point>& points, int sample_size)
{
    deque<Point> smoothedPoints;
    int n = points.size();

    if (sample_size < 1) return points; // Avoid division by zero or negative sample size

    for (int i = 0; i < n; ++i) 
    {
        double sumX = 0, sumY = 0;
        int count = 0;

        // Average over the sample size window
        for (int j = max(0, i - sample_size / 2); j < min(n, i + sample_size / 2 + 1); ++j) 
        {
            sumX += points[j].x;
            sumY += points[j].y;
            count++;
        }

        smoothedPoints.push_back({sumX / count, sumY / count});
    }

    return smoothedPoints;
}

// Function to perform linear regression and return (slope, y-intercept)
pair<double, double> LinearRegression(const deque<Point>& points)
{
    int n = points.size();
    if (n < 2) return {0, 0}; // Avoid division by zero
    
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;

    for (const auto& p : points) 
    {
        sumX += p.x;
        sumY += p.y;
        sumXY += p.x * p.y;
        sumX2 += p.x * p.x;
    }

    double meanX = sumX / n;
    double meanY = sumY / n;
    
    double numerator = sumXY * n - sumX * sumY;
    double denominator = sumX2 * n - sumX * sumX;

    if (denominator == 0) return {0, meanY}; // Vertical line case
    
    double slope = numerator / denominator;
    double intercept = meanY - slope * meanX;
    
    return {slope, intercept};
}

// Function to remove outliers based on a given threshold
deque<Point> RemoveOutliers(const deque<Point>& points, double slope, double intercept, double threshold)
{
    deque<Point> filteredPoints;

    for (const auto& p : points) 
    {
        double expectedY = slope * p.x + intercept;
        if (fabs(p.y - expectedY) <= threshold) 
        {
            filteredPoints.push_back(p);
        }
    }
    
    return filteredPoints;
}

// Main function to compute path with moving average and linear regression
pair<double, double> ComputePath(const deque<Point> &path, int sample_size, double outlierThreshold)
{
    // Step 1: Apply Moving Average Filter
    // deque<Point> smoothedPath = MovingAverage(path, sample_size);
    
    // Step 2: Perform Initial Linear Regression
    pair<double, double> result = LinearRegression(path);

    // Step 3: Remove Outliers
    deque<Point> filteredPath = RemoveOutliers(path, result.first, result.second, outlierThreshold);

    // Step 4: Perform Final Regression on Clean Data
    return LinearRegression(filteredPath);
}

pair<double, double> ComputePath(const Point &p1, const Point &p2)
{
    double slope = (p2.y - p1.y) / (p2.x - p1.x);
    double intercept = p1.y - slope * p1.x;
    return { slope, intercept };
}

double ComputeY(double x, double slope, double intercept)
{
    return (slope*x + intercept);
}

#endif // FUNTIONS_H