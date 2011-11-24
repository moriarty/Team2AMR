/** @file       src/plan/pathexecutre.h
    @ingroup    PLAN
    @brief      Path Execution Class.
    @author     Alex Moriarty <alexander@dal.ca>
    @author     Jacob Perron <perronj@yorku.ca>
*/

#ifndef __PLAN_PATHEXECUTER_H_
#define __PLAN_PATHEXECUTER_H_

#include <math.h>
#include "infs/module.h"
#include "actr/motor.h"
#include "data/path.h"
#include "pathplanner.h"

/** Follows a given Path.
 *
 *  PathExecuter seperates the decision making and path planning from the
 *  actual moving of the robot.
 *
 *  A Path should be created, ideally by the PathPlanner, and then supplied
 *  to this class for execution. This should be the only class, with the
 *  exception of ObjectAvoider, that should physically move the robot. This
 *  Class also handles deleting Path objects once it has exhausted executing
 *  a said Path. A controller can determine that a Path has been fully executed
 *  by checking if the pointer to the Path is NULL.
 */
class PathExecuter : public Module {
    public:

        const static double SPEED = 0.5;
        const static double TURNRATE = 0.5;

        /** Default constructor. */
        PathExecuter();

        /** Constructor.
         *
         *  @param motor : A reference to the robots motor.
         */
        PathExecuter(Motor& motor);

        /** Destructor. */
        ~PathExecuter();

        /** Sets the current Path to be executed.
         *
         *  The Path can either be read in terms of Position or in terms of
         *  Move. This is set based on the second parameter. If set to use
         *  Position then the PathExecuter simple uses the "GoTo" function
         *  from Player/Stage, otherwise it follows each Move from the first
         *  Position only.
         *
         *  @param path : A reference to the current path.
         *
         *  @param type : Decides whether to execute across positions or moves.
         */
        void setPath(Path& path);

        /** Applies a motion and updates the motor.
         *
         *  The robot will begin moving according the provided Motion.
         *
         *  @para motion : The Motion to be executed.
         */
        void setMotion(Motion motion);

        /** Stops the robot.
         *
         *  Used to stop the execution of a Motion. Calls Motor.halt().
         */
        void halt();

        /** Sets the current location of the robot.
         *
         *  @param pos : Position of robot in world.
         */
        void setLocal(Position pos);

        /** Continues to execute path by positions.
         *
         *  Resumes executing the path or starts executing a newly given path
         * 	with respect to Position structs.
         *
         *  @return True if finished executing the path, false otherwise.
         */
        bool executePosition();

        /** Continues to execute path by moves.
         *
         *  Resumes executing the path or starts executing a newly given path
         * 	with respect to Move structs.
         *
         *  @param remove : Set true (by default) to have PathExecuter delete
         *                  the Path object when finished executing.
         *
         *  @return True if finished executing the path, false otherwise.
         */
        bool executeMove(bool remove = true);

        /** Executes a single Move or a goto Position.
         *
         *  Resumes executing the path or starts executing a newly given path
         * 	with respect to both Position and Move structs. Starts with the
         *  first Position in the Path followed by the Position's list of
         * 	associated Move structs and then moves on to execute the next
         * 	Position and it's associated moves, and so on.
         *
         * 	Once a Move is finished execution it is removed from the Path.
         * 	When a Position and its following moves are all finished
         * 	execution, it is removed from the Path.
         *
         *  @return True if finished executing the path, false otherwise.
         */
        bool execute();


        /** Sets a reference to the robot motor.
         *
         *  @param motor : A reference to the Motor.
         */
        void setMotor(Motor& motor);

        /** Determines if robot is close enough to goal.
         *
         *  If the robot is within a 10% accuracy of its goal location it is
         *  considered to be arrived.
         *
         *  @param goal : The goal Position tested against the current location
         *          of the robot.
         *
         *  @return True if within accuracy, false otherwise.
         */
        bool isArrived(Position goal);

        /** Abandons the current Path.
         *
         *  Deletes the current Path and sets reference to NULL.
         */
        void abandonPath();

       // void setSpeed(double speed);

       // void setTurnrate(double rate);

        /** Inherited from Module. */
        std::string toString();

    private:

        /** Reference to the robot motor. */
        Motor* motor;

        /** Reference to the current path. */
        Path* path;

        /** Current Position of Robot. */
        Position robotLocation;

        /** The last location before current Move being executed. */
        Position lastLocation;

        /** Used as reference for subsequent execute calls on same Path. */
        bool alreadyFound;

        /** Disable copy constructor. */
        PathExecuter(const PathExecuter& source);

        /** Disable assignment operator. */
        PathExecuter& operator=(const PathExecuter& source);
};
#endif
