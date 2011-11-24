/** @file       src/infs/motor.h
    @ingroup    INFS
    @brief      Motor controller interface.
    @author     Jacob Perron <perronj@yorku.ca>
*/

#ifndef __INFS_MOTOR_H
#define __INFS_MOTOR_H

#include <sstream>
#include "infs/module.h"
#include "data/motion.h"
#include "data/position.h"
#include <libplayerc++/playerc++.h>

/** The base class for all motor modules.
 *
 *  Any motor module in a robot should extend this class. A motor
 *  object is described as being a class that manipulates the overall
 *  position of a robot. In other words, it is the part of the robot that
 *  moves it from position A to position B. The class itself should be invoked
 *  by other higher-level classes.
 *
 *  Used to interact with Player/Stage Position2dProxy.
 */
class Motor : public Module {
    public:

        /** Constructor.
         *
         *  Requires a reference to a Player/Stage Position2dProxy that should
         *  already be connected with a PlayerClient.
         *
         *  @param proxy : Position2dProxy from Player/Stage.
         */
        Motor(PlayerCc::Position2dProxy& proxy);

        /** Destructor. */
        virtual ~Motor();

        /** Updates the motor settings.
         *
         *  Applies the most recent internal settings to move the robot.
         *  Should be called when it is time to move the robot and after
         *  internal variables are set by other functions, such as
         *  @ref setTranslation and @ref setRotation .
         */
        virtual void update();

        /** Sets the translation component of movement.
         *
         *  The translation speed passed should be set internally.
         *
         *  @param translation : Translation, or distance in meters.
         */
        void setTranslation(double translation);

        /** Sets the rotation component of movement.
         *
         *  @param rotation : Rotation, in radians.
         */
        void setRotation(double rotation);

        /** Sets both components of movement.
         *
         *  @param translation : Translation, or distance in meters.
         *
         *  @param rotation : Rotation, in radians.
         */
        void setMotion(double translation, double rotation);

        /** Sets both components of movenment.
         *
         *  @param motion : A Motion stuct describing motion.
         */
        void setMotion(Motion motion);

        /** Sets Motion to simulate differential driving.
         *
         *  Assumes a robot with only two standard wheels along the same
         *  axis. Given the speeds of the right and left wheels the robot is
         *  moved accordingly.
         *
         *  @param rWheel: Right wheel speed.
         *
         *  @param lWheel: Left sheel speed.
         *
         *  @param wheelDist: Distance between wheels, makes computation more
         *      accurate if provided..
         */
        virtual void setDiffDrive(double rWheel, double lWheel, double wheelDist = 0.25 );

         /** Goes to a point in the global frame of the world.
         *
         *  Note it is up to the controller to keep the robot from crashing
         *  into obstacles.
         *
         *  @param  pos: The Position the robot should drive towards.
         */
        virtual void goTo(Position pos);

        /** Stops the motor. */
        void halt();

        /** Returns string representation of this Motor.  */
        virtual std::string toString();

    protected:

        /** The motion the robot is set to execute. */
        Motion motion;
        
        /** Reference to Position2dProxy. */
        PlayerCc::Position2dProxy* positionProxy;

    private:

        /** Disable default constructor */
        Motor();
        
        /** Disable copy constructor. */
        Motor(const Motor& source);

        /** Disable assignment operator. */
        Motor& operator=(const Motor& source);
};
#endif
