/** @file       src/data/move.h
    @ingroup    DATA
    @brief      Move struct
    @author     Jacob Perron <perronj@yorku.ca>
*/

#ifndef __DATA_MOVE_H_
#define __DATA_MOVE_H_
/** Represents one single action the robot could take.
 *
 *  i.e. Moving a set distance or turning a set angle.
 */
struct Move {
    /** The value for the move, could be in meters or radians. */
    double value;
    bool isMeters; //if not then radians

    /** Constructors. */
    Move() : value(0), isMeters(true) { };
    Move(double v, bool type) : value(v), isMeters(type) { };
};
#endif
