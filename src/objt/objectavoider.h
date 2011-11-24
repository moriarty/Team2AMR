/** @file       src/objt/objectavoider.h
    @ingroup    OBJT
    @brief      Moves robot in order to avoid obstacles.
    @author     Alex Moriarty <alexander@dal.ca>
    @author     Jacob Perron <perronj@yorku.ca>
*/

#ifndef __OBJT_OBJECTAVOIDER_H_
#define __OBJT_OBJECTAVOIDER_H_

#include "infs/module.h"
#include "actr/motor.h"
#include "data/position.h"
#include "data/path.h"

/** Avoids objects and obstacles.
 *
 *  Contains the neccesary functions for avoid objects in the robots
 *  trajectory. By giving ObjectAvoider control over the Motor it can
 *  commandeer the robot in order to avoid obstacles.
 *
 *  It should be predefined by the controller that after an object is
 *  detected if ObjectDetector should pass control to ObjectAvoider.
 *  The controller can also specify ahead of time how it wants the
 *  ObjectAvoider to behave, according to predefined modes, through
 *  the ObjectDetector.
 */
class ObjectAvoider : public Module {
    public:

        /** Constructor.
         *
         *  @param m : Reference to robot motor, accessed to avoid collision
         */
        ObjectAvoider(Motor& m);

        /** Destructor. */
        ~ObjectAvoider();

        /** Moves the robot so there are no immediate obstacles in its path.
         *
         *  Based on the current Position of obstacles provided, move the robot
         *  according to the preset behaviour in order to avoid said obstacles.
         *
         *  @note: Class is not fully functional, right now only halts motor in
         *      order to stop from crashing into obstacles.
         */
        void avoid();

        /** Sets the current Position of known obstacles.
         *
         *  @param positions : The known positions of obstacles to be avoided.
         */
        void setPosition(Path positions);

        /** Sets reference to robot motor. */
        void setMotor(Motor& m);

        /** Inherited from Module. */
        std::string toString();

    private:

        /** Disable default constructor. */
        ObjectAvoider();

        /** Disable copy constructor. */
        ObjectAvoider(const ObjectAvoider& source);

        /** Disable assignment operator. */
        ObjectAvoider& operator=(const ObjectAvoider& source);

        /** Most recent known Position of obstacles. */
        Path position;

        /** Reference to motor, for purpose of manual override. */
        Motor* motor;
};
#endif
