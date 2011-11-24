/** @file       src/ctrl/motioncommand.h
    @ingroup    CTRL
    @brief      Controller that takes motion commands..
    @author     Alex Moriarty <alexander@dal.ca>
    @author     Jacob Perron <perronj@yorku.ca>
*/

#ifndef __CTRL_MOTIONCOMMAND_H_
#define __CTRL_MOTIONCOMMAND_H_

#include "controller.h"
#include "bug.h"

class MotionCommand : public Controller {
    public:

        /** Constructor.
         *
         *  Calls Controller constructor.
         *
         *  @param m : Reference to robot motor.
         */
        MotionCommand(Motor& m);

        /** Destructor. */
        ~MotionCommand();

        /** Updates the controller actions.
         *
         *  This function first checks for obstacles using ObjectDetector,
         *  followed by a continued execution by PathExecuter.
         */
        virtual void update();

        /** Executes a Command from the Console.
         *
         *  Pays attention to the "goto", "move", and "turn" commands.
         *
         *  @param cmd : Command from the Console.
         */
        void executeCommand(const Command cmd);

        /** Gives the Controller the robots current Position.
         *
         *  @param pos : The robots Position.
         */
        virtual void youAreHere(Position pos);

        /** Sets the latest RangerData.
         *
         *  Ranger data is passed on to ObjectDetector and the Bug controller.
         *
         *  @param data : Latest RangerData to be used.
         */
        void setRangerData(RangerData data);

        /** String representation of this Controller. */
        virtual std::string toString();

    protected:

        /** Current robot location. */
        Position robotPos;

        /** Bug controller used for Bug algorithms. */
        Bug bug;

    private:

        /** Stores a more complex path with more than one motion command. */
        Path* plannedPath;

        /** The goal position for the last motion command. */
        Position goal;

		/** Determines if planning mode is activated. */
        bool planning;

        /** Determines whether bug2 algorithm is activated. */
        bool isBug2;

        /** Determines if controller is finished completing an algorithm. */
        bool doingAlg;

        /** Disable copy constructor. */
        MotionCommand(const MotionCommand& source);

        /** Disable assignment operator. */
        MotionCommand& operator=(const MotionCommand& source);

        /** Disable default constructor. */
        MotionCommand();
};
#endif
