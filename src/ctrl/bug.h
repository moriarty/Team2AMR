/** @file       src/ctrl/bug.h
    @ingroup    CTRL
    @brief      Bug Algorithm Controller.
    @author     Alex Moriarty <alexander@dal.ca>
    @author     Jacob Perron <perronj@yorku.ca>
*/

#ifndef __CTRL_BUG_H_
#define __CTRL_BUG_H_

#include "controller.h"
#include "wallfollower.h"
#include "data/line.h"

const double DIST_THRESHOLD = 0.5;

/** Robot Controller that implements Bug algorithms.
 *
 *  Currently this controller only implements the Bug-2 algorithm, but could
 *  theoretically contain more. When called on it's own as a Controller it has
 *  the ability to execute "goto" commands. The @ref update function will
 *  continue with the execution of the "goto" command via @ref bug2 algorithm.
 *
 *  Ideally this controller will be called inside of another as a means of
 *  obstacle avoidence. For example inside MotionController after ObjectDetector
 *  detects an obstacle. In this case it is only necessary for @ref bug2 to be
 *  called and not the @ref update function.
 *
 *  @note The @ref update function is currently not implemented, although the
 *      @ref bug2 function is called by other classes. Until then, this class
 *      is useless implemented as a sole Controller.
 */
class Bug : public Controller {
    public:

        /** Constructor. */
        Bug(Motor& m);

        /** Destructor. */
        ~Bug();

        /** Updates the controllers actions.
         *
         *  Waits until the rangers on the robot detect an obstacle and then
         *  calls @ref bug2 algorithm. Nothing much is expected to occur unless
         *  the robot is being affected by another Controller.
         */
        void update();

        /** Continues to update the robot based on the Bug-2 algorithm.
         *
         *  Records the robots position on the first call, starts the
         *  wall-follower and continues until the provided terminating
         *  condition is met (ie. Comes into contact with line).
         *
         *  @param goal : The goal Position used to create the terminating
         *      condition, which is a 2D line.
         *
         *  @return True if the robot has reached the line, false otherwise.
         */
        bool bug2(Position pos);

        /** Passes the latest ranger data to the internal wall-follower.
         *
         *  @param data : The lastest RangerData.
         */
        void setRangerData(RangerData data);

        /** Sets the latest Position of the robot.
         *
         *  @param pos : The robot's current Position.
         */
        void youAreHere(Position pos);

        /** Executes a command from the Console.
         *
         *  Executes the "goto" command by setting a goal position and
         *  calculating and storing a line in order to get there.
         *
         *  @param cmd : Command struct created from the Console.
         */
        void executeCommand(const Command cmd);

    protected:

    /** Wall-following component of Bug. */
    WallFollower wf;

    /** Robot's current location. */
    Position robotPos;

    /** Robot's last location before activating wall-follower. */
    Position lastPos;

    /** Determines if the robot has reached its goal. */
    bool isArrived;

    /** Determines if it is the first call to bug2 algorithm. */
    bool firstCallBug2;

    private:

        /** Disable default constructor. */
        Bug();

        /** Disable copy constructor. */
        Bug(const Bug& source);

        /** Disable assignment operator. */
        Bug& operator=(const Bug& source);
};
#endif
