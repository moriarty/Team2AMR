/** @file       src/ctrl/robot.h
    @ingroup    CTRL
    @brief      Container for modules.
    @author     Jacob Perron <perronj@yorku.ca>
    @author     Alexander Moriarty <alexander@dal.ca>
*/

#ifndef __CTRL_ROBOT_H_
#define __CTRL_ROBOT_H_

#include <libplayerc++/playerc++.h>
#include <boost/ptr_container/ptr_vector.hpp>
#include <vector>
#include "controller.h"
#include "motioncommand.h"
#include "wallfollower.h"
#include "braitenberg.h"
#include "bug.h"
#include "infs/commandexecuter.h"
#include "util/logger.h"
//#include "actr/motor.h"
#include "snsr/ranger.h"
#include "hrio/console.h"
#include "plan/local.h"

class Motor;

/** The top-level class that contains all modules required to run a robot.
 *
 *  Used to initialize "lower-level" modules (parts) of the robot. Modules,
 *  such as rangers and motor, can then be passed to a controller (the brain),
 *  which is also inside Robot, that makes the necessary invocations
 *  for the desired operation of the robot.
 *
 *  The idea is that the robot is initialized on construction and then the
 *  "Think-Act" superloop occurs inside Robot once the @ref run function is
 *  called.
 *
 *  Currently, the player/stage library is implemented here to initialize
 *  and link the PlayerClient with the motor and rangers.
 */
class Robot : public CommandExecuter {
    public:

        /** Constructor.
         *
         *  This constructor is used for the incorporation of player/stage.
         *  It is assumed that the player (robot) is already initialized and
         *  connected with the associated ranger and motor.
         *
         *  @param robot : The robot player from the player/stage lib
         *
         *  @param ranger : A ranger onboard the robot. Other's can be
         *      added later with @ref addRanger .
         *
         *  @param motor : The motor used to drive the robot.
         */
        Robot( PlayerCc::PlayerClient& robot,
               PlayerCc::RangerProxy &rangerProxy,
               PlayerCc::Position2dProxy &positionProxy );

        /** Default contructor.
         *
         *  Instantiates a robot that wanders aimlessly about it's
         *  environment, while avoiding obstacles.
         */
        Robot();

        /** Destructor.  */
        ~Robot();

        /** Activates the robot.
         *
         *  Initiates the "Think-Act" superloop and effectively blocks main.
         *  The only way to kill the robot after this function is called is
         *  to invoke the "exit" command from the the robots personal console.
         */
        void run();

        /** Adds a ranger to the robot.
         *
         *  Should be done before invoking @ref run but may be possible
         *  through a console command.
         *
         *  @param ranger : A new Ranger object to be added to the robot.
         */
        void addRanger(Ranger& ranger);

        /** Swaps current controller for a new one.
         *
         *  The robots behaviour is changed before running or on the fly
         *  via console command.
         *
         *  @param controller : The new controller to be swapped in.
         */
        void setController(Controller& controller);

        /** Inherited from CommandExecuter. */
        void executeCommand(Command command);

    private:

        /** A reference to all rangers aboard the robot */
        boost::ptr_vector<Ranger> ranger;

        /** A reference to the Motor aboard that drives the robot. */
        Motor* motor;

        /** A reference to the controller that's called inside the superloop. */
        Controller* controller;

        /** A reference to Local. */
        Local* local;

        /** Provides human-robot interaction. Called in superloop. */
        Console console;

        /** Player from player/stage. */
        PlayerCc::PlayerClient* player;

        /** State of robot, on/off. */
        bool power;

        /** Disable copy constructor. */
        Robot(const Robot& source);

        /** Disable assignment operator. */
        Robot& operator=(const Robot& source);
};
#endif
