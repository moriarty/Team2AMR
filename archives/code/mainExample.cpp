#include <libplayerc++/playerc++.h>
// test edit

#define USE_MATH_DEFINES
#include <cmath>

// Needed for random number generation
#include <stdlib.h>
#include <time.h>

// This defines gHostname, gPort and gIndex
#include "args.h"

#include "actuators/motor.h"
#include "hri/console.h"

// Gain access to all the class names in libplayerc++
using namespace PlayerCc;

Motion CalculateMotion(RangerProxy& rp)
{
  const double TURNRATE = 30 * M_PI / 180;
  const double SPEED = 0.5;
  const double MIN_DISTANCE_TO_OBSTACLE = 0.75;
  static unsigned int numLPValues = rp.GetRangeCount();
  static unsigned int minAngle = (numLPValues * 3) / 8;
  static unsigned int maxAngle = (numLPValues * 5) / 8;

  // Process laser data and extract interesting information
  double minDistance = rp.GetRange(minAngle);
  for(unsigned int i = minAngle; i < maxAngle; i++)
  {
    double range = rp.GetRange(i);
    if(range < minDistance)
      minDistance = range;
  }

  // Compute motion based on acquired information
  static int turnDirection = 1;
  if(minDistance < MIN_DISTANCE_TO_OBSTACLE)
  {
    return Motion(0, TURNRATE * turnDirection);
  }
  else
  {
    if(rand() % 100 == 1)
      turnDirection *= -1;
    return Motion(SPEED, 0);
  }
}

int main(int argc, char **argv)
{
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

  // Initialize random number generator
  srand(time(NULL));

  bool continueOperation = true;

  // Start infinite read-think-act superloop
  while(continueOperation)
  {
    robot.Read(); // this blocks until new data from the stage come
    continueOperation = console.Update();
    motor.SetMotion(CalculateMotion(rangerProxy));
    motor.Update();
  }

  // Cleanup. Now there is nothing to do, the proxies get destroyed once we
  // leave this code block. However if you have created any objects with the new
  // operator, you should delete them.
}
