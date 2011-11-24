#include "pathexecuter.h"

CREATE_LOGGER("PathExecuter");

PathExecuter::PathExecuter(Motor& motor) {
    this->motor = &motor;
    this->alreadyFound = false;
    this->path = NULL;
    LOG_CTOR << "Constructed." << std::endl;
}

PathExecuter::~PathExecuter() {
    abandonPath();
    LOG_DTOR << "Destructed." << std::endl;
}

bool PathExecuter::executePosition() {
    if (path != NULL) {
        if (path->size() == 0) {
            motor->halt();
            abandonPath();
            MAKE_LOG << "Path Deleted." << std::endl;
        }
        else { //continue executing
			//get next Position to go to
            Position goal = path->getPosition(0);
            if (isArrived(goal)) { //check if at goal
				//take position off list
                path->removePosition(0);
            }
            else
                motor->goTo(goal);
        } //end continue executing
        return false;
    } //end if Path != NULL
    return true;
}

bool PathExecuter::executeMove(bool remove) {
    if (path != NULL) {
        MAKE_LOG << "PE executeMove()"<< std::endl;
        if (path->numOfMoves(0) == 0) { //0 moves to execute
            MAKE_LOG << "zero moves to execute" << std::endl;
            motor->halt();

            if (remove && path != NULL)
                abandonPath();

            return true;
        }
        else { //continue executing
            //get current move
            Move m = path->getMove();
            Motion motion;

            if (m.isMeters) { //if moving
                double difference = lastLocation.calcDistTo(robotLocation);

                if (m.value < 0) //if moving backwards
                    motion.x = (-1)*PathExecuter::SPEED;
                else
                    motion.x = PathExecuter::SPEED;

                if (difference >= std::abs(m.value)) { //check if arrived
                    path->removeMove();
                    lastLocation = robotLocation;
                }
                else { //continue current Move
                    motor->setMotion(motion);
                    motor->update();
                }
            }
            else { // if turning
                if (m.value < 0)
                    motion.yaw = (-1)*PathExecuter::TURNRATE;
                else
                    motion.yaw = PathExecuter::TURNRATE;

                double angleDiff = std::abs(robotLocation.yaw - lastLocation.yaw);
                MAKE_LOG << "Angle diff : " << angleDiff << std::endl;
                MAKE_LOG << "angle to move : " << m.value*(180.0/M_PI) << std::endl;
                if (angleDiff >= std::abs(m.value)) { //check if arrived
                    MAKE_LOG << "Removing move" << std::endl;
                    path->removeMove();
                    lastLocation = robotLocation;
                }
                else { //continue turning
                    MAKE_LOG << "updating" << std::endl;
                    motor->setMotion(motion);
                    motor->update();
                }
            }
        } //end continue
        return false;
    } //end if path!=NULL
    return true;
}

bool PathExecuter::execute() {
	if (path != NULL) {
        if (path->size() == 0) {
            motor->halt();
            abandonPath();
            MAKE_LOG << "Path Deleted." << std::endl;
        }
        else { //continue executing
			//get next Position to go to
            Position goal = path->getPosition(0);
            if (alreadyFound || isArrived(goal)) { //check if at goal
                alreadyFound = true;
				//now execute moves
                //if (path->numOfMoves(0) > 0)
    			if (executeMove(false)) { //take position off list
                    path->removePosition(0);
                    lastLocation = robotLocation;
                    alreadyFound = false;
                }
            }
            else
                motor->goTo(goal);
         } //end continue executing
        return false;
    } //end if Path != NULL
    return true;
}

//bool PathExecuter::move(double distance, double rotation);

void PathExecuter::setMotion(Motion m) {
    motor->setMotion(m);
    motor->update();
}

void PathExecuter::halt() {
    motor->halt();
}

void PathExecuter::setMotor(Motor& motor) {
    this->motor = &motor;
}

void PathExecuter::setPath(Path& path) {
    abandonPath();
    this->path = &path;
    this->lastLocation = robotLocation;
    this->alreadyFound = false;
}

void PathExecuter::setLocal(Position pos) {
    robotLocation = pos;
}

bool PathExecuter::isArrived(Position goal) {
    double ACCURACY = 0.10;
    double DEG_ACCURACY = 0.1;
    if (robotLocation.calcDistTo(goal) <= ACCURACY) {
        if (std::abs(robotLocation.yaw*(180.0/M_PI) - goal.yaw*(180.0/M_PI)) <= DEG_ACCURACY)
            return true;
    }
    return false;
}

void PathExecuter::abandonPath() {
    //delete old path if not already deleted
    if (path != NULL) {
        delete path;
        path = NULL;
    }
}

std::string PathExecuter::toString() {
    return "PathExecuter";
}
