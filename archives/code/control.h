/** @file       src/control/control.h
    @ingroup    control
    @brief      Motion controller
    @author     Jacob Perron
    @author	Alexander Moriarty
*/

#ifndef __CTRL_CONTROL_H
#define __CTRL_CONTROL_H
#include <libplayerc++/playerc++.h>
#include <vector>
#include <iostream>
#include "actuators/motor.h"
#include "logger.h"

using namespace std;

/** A high-level controller that implements a Motor controller and
    Position2d proxy to navigate between set positions using a
    predefined map.
									      */
class Control {
  public:
    static const int INDEPENDENT = 0;
    static const int SUBSEQUENT = 1;

/** Constructor.                                                              */
    Control(PlayerCc::Position2dProxy& proxy);

/** Destructor.                                                               */
    ~Control();

/** Given the current position and goal position, calculate rotation,
    distance from current position to goal in a straight line.		      */
    void plan(player_pose2d_t position);

/** Rotates robot so that it faces the given orientation                      */
    double rotate();

/** Moves the robot foward a given distance.                                  */
    double move();

/** Sets mode robot is working in, such as INDEPENDENT or SUBSEQUENT.         */
    void setMode(int m);

/** Adds a goal to the list of coordinates to visit.                          */
    void addGoal(player_point_2d goal);

/** Determines the next goal and then set as current goal.                    */
    void nextGoal();

  private:
/** Hide default constructor.						      */
    Control();

/** Mode, INDEPENDENT, SUBSEQUENT, etc.                                       */
    int mode;

/** Used to store the information for the robots next destination.            */
    double distance;
    double direction;

/** Keeps track of the Goal Positions, along with current and past goals.     */
    vector<player_point_2d> goals;
    player_point_2d* currentgoal;
    player_point_2d* pastgoal;

/** Proxy used to determine robots current position.                          */
    PlayerCc::Position2dProxy& positionProxy;

/** Motor that is manipulated through this controller                         */
   // Motor& motor;
};

#endif
