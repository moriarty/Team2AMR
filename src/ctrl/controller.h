/** @file       src/ctrl/controller.h
    @ingroup    CTRL
    @brief      Robot controller.
    @author     Jacob Perron <perronj@yorku.ca>
    @author     Alexander Moriarty <alexander@dal.ca>
*/

#ifndef __INFS_CONTROLLER_H_
#define __INFS_CONTROLLER_H_

#include "infs/module.h"
#include "infs/commandexecuter.h"
#include "plan/navigation.h"
#include "plan/pathexecuter.h"
#include "plan/pathplanner.h"
#include "objt/objectdetector.h"
#include "objt/objectavoider.h"
#include "data/rangerdata.h"
#include "data/position.h"


/** This class acts like the brain of the robot.
 *
 *  This is the part of the robot that decides how it thinks, ie, planning
 *  and acting. The Controller makes use of an ObjectDetector object,
 *  Navigation object, PathPlanner, and PathExecuter to conduct its control
 *  of the robot.
 *
 *  By default, when @ref update is called the ObjectDetector checks for
 *  obstacles, followed by a consultation with Navigation and possibly
 *  PathPlanner, and then the PathExecuter is either given a new path to
 *  execute or given the order to continue executing the current path.
 *
 *  This class should be extended by other, more specific, controllers. For
 *  example, a Braitenberg or Wallfollower controller.
 */
class Controller : public CommandExecuter, public Module {
    public:

        /** Constructor.
         *
         *  Should initialize Navigation, ObjectAvoider, and PathExecuter
         *  and set the motor to the passed parameter.
         *
         *  @param m : The robot motor.
         */
        Controller(Motor& m);

        /** Destructor. */
        virtual ~Controller();

        /** Takes new relavent ranger data.
         *
         *  Distributes the data to the relevant sub-classes, such as
         *  Navigation and ObjectAvoider.
         *
         *  @param data : A RangerData struct.
         */
        virtual void setRangerData(RangerData data);

        /** Takes new relavent ranger data for multiple rangers.
         *
         *  Distributes the data from the various rangers to the relevant
         *  sub-classes.
         *
         *  @note For the purposes of this basic Controller, only the first
         *      ranger in the array is utilized.
         *
         *  @param data : An array of RangerData structs.
         */
        virtual void setRangerData(RangerData *data);

        /** Updates controller actions.
         *
         *  This particular controller is "brain-dead" and so nothing actually
         *  occurs inside this version of @ref update .
         *
         *  An actual controller might do something like first check for
         *  obstacles using ObjectDetector, followed by a check-in with
         *  Navigation, and finally changing/continuing executing the plan
         *  with PlanExecuter.
         */
        virtual void update();

        /** Changes reference to a new motor for the same robot.
         *
         *  Used to provide Controllers sub-classes (ie. PathExecuter) with
         *  a reference to the robot's motor.
         *
         *  @param m : The reference to the new robot motor.
         */
        virtual void setMotor(Motor& m);

        /** Tells the controller the robots current location.
         *
         *  Function used to let components inside the Controller know about
         *  the robots new location in the world. It is up to the individual
         *  Controller to decide what components need to know about the local,
         *  such as Navigation and/or ObjectDetector.
         *
         *  @param pos : The robots current position.
         */
        virtual void youAreHere(Position pos);

        /** Returns information on this Controller.
         *
         *  @return String representation of this Controller.
         */
        virtual std::string toString();

        /** Executes a Command from the Console.
         *
         *  First checks if the command is a valid one for Controller
         *  and if so, executes. If not, then the command is passed to potential
         *  command executing componenets of the Controller.
         *
         *  @param cmd : A command from the Console.
         */
        virtual void executeCommand(const Command cmd);

    protected:
        /** ObjectDetector. */
        ObjectDetector od;

        /** ObjectAvoider. */
        ObjectAvoider oa;

        /** Navigation system. */
        Navigation nav;

        /** PathExecuter. */
        PathExecuter pe;

    private:

    /** Disable copy constructor. */
    Controller(const Controller& source);

    /** Disable assignment operator. */
    Controller& operator=(const Controller& source);

    /** Disable default constructor. */
    Controller();
};
#endif
