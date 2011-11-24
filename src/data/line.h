/** @file       src/data/line.h
    @ingroup    Data
    @brief      Line struct.
    @author     Alex Moriarty <alexander@dal.ca>
    @author     Jacob Perron <perronj@yorku.ca>
*/
#include <math.h>
#include "position.h"

#ifndef __DATA_LINE_H_
#define __DATA_LINE_H_

/** Represents a line in 2d space.
 *
 *  The line is represented the form Ax+By+C=0. The struct simply stores
 *  the values of 'A', 'B', and 'C'.
 */
struct Line {

    double A;

    double B;

    double C;

/** Default constructor, initializes all components with zeros. */
    Line() : A(0), B(0), C(0) { };

/** Constructor */
    Line(double A, double B, double C) : A(A), B(B), C(C) { };
/** Constructor */
    Line(Position p1, Position p2) {
        double dx = p2.x - p1.x;
        double dy = p2.y - p1.y;
        A = -dy;
        B = dx;
        C = -(p1.y*dx - p1.x*dy);
    }
/** Distance from line to point. */
    double calcDistTo(Position p1){
        double d = (std::abs(A*p1.x+B*p1.y+C))/(sqrt(A*A + B*B));
        return d;
    }
};
#endif
