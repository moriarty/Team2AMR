/** @file       src/control/console.h
    @ingroup    control
    @brief      controller for the robot
    @author     Alex Moriarty <alexander@dal.ca>
    @author	Jacob Perron  <perronj@yorku.ca>
*/

#include "control.h"

static const int INDEPENDENT = 0;
static const int SUBSEQUENT = 1;

int mode;

double distance;
double direction;

vector<player_point_2d> goals;
player_point_2d* currentgoal;
player_point_2d* pastgoal;

Control::Control(PlayerCc::Position2dProxy& proxy) : positionProxy(proxy) {
//	positionProxy = proxy;
//	motor = m;
	double x, y;
	x = positionProxy.GetXPos();
	y = positionProxy.GetYPos();
	player_point_2d origin;
	origin.px = x;
	origin.py = y;
	goals.push_back(origin);
}

Control::~Control() { }

void Control::plan(player_pose2d_t pos) {
	double dx, dy;
	dx = currentgoal->px - pos.px;
	dy = currentgoal->py - pos.py;
	distance = sqrt(dx*dx + dy*dy);
	direction = atan2(dy, dx);
	if( distance < 0.05 ) {
		// We've arrived at the destination,
		nextGoal();
		dx = currentgoal->px - pos.px;
		dy = currentgoal->py - pos.py;
		distance = sqrt(dx*dx + dy*dy);
		direction = atan2(dy, dx);
	}
}

void Control::nextGoal() {
  if(mode == Control::INDEPENDENT) {

  } else { //SUBSEQUENT

  }
}

double Control::move() {
  return distance;
}

double Control::rotate() {
  return direction;
}

void Control::setMode(int m) {
  mode = m;
}

void Control::addGoal(player_point_2d goal) {
	goals.push_back(goal);
}
