/** @file       src/data/position.h
    @ingroup    Data
    @brief      Position struct.
    @author     Alex Moriarty <alexander@dal.ca>
    @author     Jacob Perron <perronj@yorku.ca>
*/
#include <math.h>

#ifndef __DATA_POSITION_H_
#define __DATA_POSITION_H_

/** Represents a position in 2d space.
 *
 *  The position could be relative or absolute and representing the robot or
 *  another object in the world's position.
 */
struct Position
{
    double x;
    double y;
    double yaw;

    /** Default constructor, initializes all components with zeros. */
    Position() : x(0), y(0), yaw(0) { };

    /** Constructor, initializes motion components with supplied values. */
    Position(double nx, double ny, double nyaw) : x(nx), y(ny), yaw(nyaw) { };
    /** Calculates the distance between two points
    * Calculates the length of the vector bewteen two points.
    *
    * @param p1 : Position of the first point
    * @param p2 : Position of the second point
    * @return Distance.
    */
    double calcDistTo(Position p1){
        double i = x - p1.x;
        double j = y - p1.y;
        return sqrt(i*i + j*j);
    }

    /** Calculates the angle bewteen two points
    *
    * Finds the angle between position vectors of p1 and p2.
    *    -> Used dot product. now it uses??
    *
    * @param p1 : Position of point 1.
    *
    * @return Angle between positions.
    */
    double calcAngleTo(Position p1){
    // return acos((p1.y*y+p1.x*x) / (sqrt(x*x+y*y)*sqrt(p1.x*p1.x+p1.y*p1.y)));
	double yaw1 = atan2(p1.y, p1.x);
	double yaw2 = atan2(y, x);	
	return (yaw2 - yaw1);
    }
};
#endif
