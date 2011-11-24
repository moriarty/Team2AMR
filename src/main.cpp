#include <stdlib.h>

#include <libplayerc++/playerc++.h>
#include "args.h"
#include "ctrl/robot.h"
#include "util/logger.h"
#include "docs/mainpage.h"

using namespace PlayerCc;

int main(int argc, char **argv) {
    CREATE_LOGGER("main");
    Logger::start("numbers");

    PlayerClient player(gHostname, gPort);

    // Subscribe to the position2d device
    Position2dProxy positionProxy(&player, gIndex);

    /* Subscribe to the ranger device
    *  index 0 is a laser scanner, 1 is sonar, >1 will be more sonars 
    */
    RangerProxy rangerProxy(&player, 1);

    // Actually connect proxies with player server by initiating communication
    player.Read();

    // Complete initialization of proxies by reading default properties from
    // player server
    positionProxy.RequestGeom();
    rangerProxy.RequestConfigure();
    rangerProxy.RequestGeom();

    // Do actual communication with server
    player.Read();
    // At this point the player client and device proxies are completely
    // initialized and are ready to work

    // We now create high-level modules
    {
        Robot robot(player, rangerProxy, positionProxy);
        MAKE_LOG << "Ready to run robot." << std::endl;
        robot.run();
        MAKE_LOG << "Finished running" << std::endl;
    }
    Logger::stop();

    return 0;

}
