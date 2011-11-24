/** @file       src/data/motion.h
    @ingroup    Data
    @brief      Motion struct.
    @author     Sergey Alexandrov <sergey.alexandrov@smail.inf.h-brs.de>
*/

#ifndef __DATA_MOTION_H
#define __DATA_MOTION_H

/** Represents motion of a car-like vehicle (x-speed, yaw-speed) in 2d space. */
struct Motion {
    double x;
    double yaw;

/** Default constructor, initializes both components with zeros.              */
    Motion() : x(0), yaw(0) { };
/** Constructor, initializes motion components with supplied values.          */
    Motion(double nx, double ny) : x(nx), yaw(ny) { };
};
#endif
