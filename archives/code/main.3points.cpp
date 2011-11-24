#include <libplayerc++/playerc++.h>
// test edit

#define USE_MATH_DEFINES
#include <cmath>

// Needed for random number generation
#include <stdlib.h>
#include <time.h>

// This defines gHostname, gPort and gIndex
#include "args.h"
#include "control/control.h"
#include "actuators/motor.h"
#include "hri/console.h"

// Gain access to all the class names in libplayerc++
using namespace PlayerCc;

int main(int argc, char **argv) {
  //Create Logger and open stream
  CREATE_LOGGER("main");
  Logger::start();

  // Create a player client object
  PlayerClient robot(gHostname, gPort);

  // Subscribe to the position2d device
  Position2dProxy positionProxy(&robot, gIndex);

  // Subscribe to the laser device
  RangerProxy rangerProxy(&robot, 1);

  // Actually connect proxies with player server by initiating communication
  robot.Read();

  // Complete initialization of proxies by reading default properties from
  // player server
  positionProxy.RequestGeom();
  rangerProxy.RequestConfigure();
  rangerProxy.RequestGeom();

  // Do actual communication with server
  robot.Read();
  // At this point the player client and device proxies are completely
  // initialized and are ready to work

  // We now create high-level modules
  Motor motor(positionProxy);
  Console console;

  // Initialize Controller
  Control control(positionProxy);

  player_point_2d one;
  player_point_2d two;
  player_point_2d three;
  one.px = -6.5;
  one.py = 0.5;
  two.px = 5.5;
  two.py = -3.5;
  three.px = 5.5;
  three.py = 5.5;

  control.addGoal(one);
  control.addGoal(two);
  control.addGoal(three);

  bool continueOperation = true;

  // Start infinite read-think-act superloop
  while(continueOperation)
  {
    robot.Read(); // this blocks until new data from the stage come
    continueOperation = console.Update();

    //Plan
//    control.plan(positionProxy.
    //Rotate
    motor.SetRotationSpeed(control.rotate());
    //Move
    motor.SetTranslationSpeed(control.move());
    
 //   motor.SetMotion(CalculateMotion(rangerProxy));
 //   MAKE_LOG << "X: " << positionProxy.GetXPos() <<"Y: "<< positionProxy.GetYPos() << "yaw: "<< positionProxy.GetYaw() <<  std::endl;
    motor.Update();
  }

  //Close log file
  Logger::stop();
  
  // Cleanup. Now there is nothing to do, the proxies get destroyed once we
  // leave this code block. However if you have created any objects with the new
  // operator, you should delete them.
}
