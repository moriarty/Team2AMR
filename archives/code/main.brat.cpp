#include <libplayerc++/playerc++.h>
// test edit

#define USE_MATH_DEFINES
#include <cmath>

// Needed for random number generation
#include <stdlib.h>
#include <time.h>
#include <math.h>

// This defines gHostname, gPort and gIndex
#include "args.h"
#include "hri/console.h"
#include "logger.h"
#include "control/braitenberg.h"

// Gain access to all the class names in libplayerc++
using namespace PlayerCc;

int main(int argc, char **argv) {
    //Create Logger and open stream
    CREATE_LOGGER("main");
    Logger::start("braiten");

    // Create a player client object
    PlayerClient robot(gHostname, gPort);

    // Subscribe to the position2d device
    Position2dProxy positionProxy(&robot, gIndex);
	
    //Subscribe to the laser device
    RangerProxy rangerProxy(&robot, 0);
  
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
    Console console;
    Braitenberg braiten(positionProxy, rangerProxy);
    braiten.setConfig(Braitenberg::A);
    braiten.setBehaviour(Braitenberg::EXCITE);

    bool continueOperation = true;

    // Start infinite read-think-act superloop
    while(continueOperation) {
        robot.Read(); // this blocks until new data from the stage come
        continueOperation = console.Update();
        braiten.run();
    }

    //Close log file
    Logger::stop();

    // Cleanup. Now there is nothing to do, the proxies get destroyed once we
    // leave this code block. However if you have created any objects with the new
    // operator, you should delete them.
}
