#include "motioncommand.h"
#include <sstream>

CREATE_LOGGER("MotionCommand");

MotionCommand::MotionCommand(Motor& m) : Controller(m), bug(m) {
    planning = false;
    isBug2 = false;
    doingAlg = false;
    LOG_CTOR << "Constructed." << std::endl;
}

MotionCommand::~MotionCommand() {
    LOG_DTOR << "Destructed." << std::endl;
}

void MotionCommand::update() {
    if (od.check() || doingAlg) //check for near obstacles
	    if (isBug2) {
            doingAlg = true;
            if (bug.bug2(goal))
                doingAlg = false; //finished bug2 algorithm
        }
        else
            pe.halt();
    else
        pe.execute();
}

void MotionCommand::executeCommand(const Command cmd) {
    switch(cmd.name){
        case move:
            if (cmd.arg.size() >= 2) {
                //distance to move
                double distance = strtod(cmd.arg[1].c_str(), NULL);
                MAKE_LOG << "Distance from String : " << distance << std::endl;

                if (cmd.arg.size() >= 3 && cmd.arg[2] == "backward")
                    distance = distance * (-1.0);

                goal = PathPlanner::calcPosition(robotPos, distance);

                if (planning) {
                    plannedPath->addMove(Move(distance, true));
                    MAKE_LOG << "Added move to the plan." << std::endl;
                }
                else {
                    Path* path = new Path(robotPos);
                    path->addMove(Move(distance, true));
                    pe.setPath(*path);
                    MAKE_LOG << "Moving "<< distance << " meters." << std::endl;
                }
            }
            else
                TO_CONSOLE("move <distance> [direction], distance=double, direction='forward' or 'backward'");
            break;

        case turn:
            if (cmd.arg.size() >= 2) {
                //degrees from console
                double yaw = std::strtod(cmd.arg[1].c_str(),NULL);
                //convert to radians
                yaw = yaw * (M_PI/180);
                if (planning) {
                    plannedPath->addMove(Move(yaw, false));
                    MAKE_LOG << "Added turn to the plan." << std::endl;
                }
                else {
                    Path* path = new Path(robotPos);
                    path->addMove(Move(yaw, false));
                    pe.setPath(*path);
                    MAKE_LOG << "Turning " << cmd.arg[1] << " degrees." << std::endl;
                }
            }
            else
               TO_CONSOLE("turn <degrees>, degrees=double");
            break;

        case gt:
            if (cmd.arg.size() >= 4) {
                goal.x = std::strtod(cmd.arg[1].c_str(), NULL);
                goal.y = std::strtod(cmd.arg[2].c_str(), NULL);
                goal.yaw = (M_PI/180)*std::strtod(cmd.arg[3].c_str(), NULL);

                if (planning) {
                    plannedPath->addPosition(goal);
                    MAKE_LOG << "Added goto to the plan." << std::endl;
                }
                else {
                    Path* path = new Path();
                    path->addPosition(goal);
                    pe.setPath(*path);
                    MAKE_LOG << "Going to position (" << cmd.arg[1] << ", "
                    << cmd.arg[2] << ", " << cmd.arg[3] << ")" << std::endl;
                }
            }
            else
                TO_CONSOLE("goto <x> <y> <yaw>, x=double, y=double, yaw=double");
            break;

        case plan:
            planning = true;
            plannedPath = new Path(robotPos);
            break;

        case endplan:
            pe.setPath(*plannedPath);
            planning = false;
            break;

		case bug2:
			if (cmd.arg.size() >= 2) {
				if (cmd.arg[1] == "on")
					isBug2 = true;
				else if (cmd.arg[1] == "off")
					isBug2 = false;
				else
					TO_CONSOLE("usage: bug2 state, state={'on' or 'off'}");
			}
			else
				TO_CONSOLE("usage: bug2 state, state={'on' or 'off'}");

        default:
            bug.executeCommand(cmd);
            break;
    }
}

void MotionCommand::youAreHere(Position p) {
    robotPos = p;
    pe.setLocal(p);
    bug.youAreHere(p);
}

void MotionCommand::setRangerData(RangerData data) {
    od.setRangerData(data);
    bug.setRangerData(data);
}

std::string MotionCommand::toString() {
    return "MotionCommand Controller";
}
