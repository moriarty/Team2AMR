/** @file       src/ctrl/wallfollower.h
    @ingroup    CTRL
    @brief      Wall-Follower robot controller.
    @author     Alex Moriarty <alexander@dal.ca>
    @author     Jacob Perron <perronj@yorku.ca>
*/

#ifndef __CTRL_WALLFOLLOWER_H_
#define __CTRL_WALLFOLLOWER_H_

#include "controller.h"
#include <cmath>

enum wfState {
    looking,
    found,
    executeMove,
    movingParallel,
    concave,
    convex
};

/** Robot controller that finds and follows walls.
 *
 *  This Controller finds the nearest wall to the robot, moves to the wall, and
 *  procedes to move parallel along the wall. The robot will move with it's
 *  right side facing the wall by default, otherwise specified by the user
 *  via the Console.
 *
 *  The controller could be in one of 5 states:
 *      -# Looking for wall.\n
 *         \t Looks for the nearest wall, or continues moving forward\n
 *         \t if unable to find one.
 *      -# Found wall.\n
 *         \t Approaches wall until within an appropriate distance.
 *      -# Arrived at wall.\n
 *         \t Check if parralel to wall and continue forward.
 *      -# Identified concave wall.\n
 *         \t Makes turn to otherside of the concave object.
 *      -# Identified convex wall.\n
 *         \t Waits until enough distance has been traveled forward\n
 *         \t to safely turn to the otherside of the convex object.
 */
class WallFollower : public Controller {
    public:

        /** Constructor.
         *
         *  @param motor : Reference to the robot motor.
         */
        WallFollower(Motor& motor);

        /** Destructor. */
        ~WallFollower();

        /** Updates the controllers actions.
         *
         *  The action taken depends on which one of the 5 possible states the
         *  Controller is currently.
         */
        virtual void update();

        /** Sets the latest data from the rangers to anaylze.
         *
         *  @param data : The latest RangerData.
         */
        void setRangerData(RangerData data);

        /** Sets the latest Position of the robot.
         *
         *  Passes information to PathExecuter.
         *
         *  @param pos : The Robot's current Position.
         */
        void youAreHere(Position pos);

        /** Executes a command from the Console.
         *
         *  Only one possible command for this controller is @c follow with one
         *  argument @c left or @c right depending if the user's preference.
         *
         *  @param cmd : Command struct created from the Console.
         */
        void executeCommand(const Command cmd);

        std::string toString();

    protected:

        /** Robot's current location. */
        Position robotLocation;

        /** Determines what side of the robot should be parallel to the wall. */
        bool isLeft;

        /** Keeps track of the state the controller is in. */
        wfState state;

        /** Latest ranger data. */
        RangerData data;

    private:

        /** Disable default constructor. */
        WallFollower();

        /** Disable copy constructor. */
        WallFollower(const WallFollower& source);

        /** Disable assignment operator. */
        WallFollower& operator=(const WallFollower& source);
};
#endif
