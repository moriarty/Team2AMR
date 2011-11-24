#include "wallfollower.h"

CREATE_LOGGER("WallFollower");

WallFollower::WallFollower(Motor& m) : Controller(m) {
    this->motor = &m;
    //follow with right side of robot along wall by default
    isLeft = false;
    //set controller to first state
    state = FOLLOW;
    LOG_CTOR << "Constructed." << std::endl;
}

WallFollower::~WallFollower() {
    LOG_DTOR << "Destructed." << std::endl;
}

void WallFollower::update() {
    MAKE_LOG << "Entering Update()" << std::endl;
    double front, back;
    Position frontPos, backPos;
    if (isLeft) { //left side sonars
        front = data.range[0];
        back = data.range[15];
        frontPos = data.pos[0];
        backPos = data.pos[15];
    }
    else { //right side sonars
        front = data.range[7];
        back = data.range[8];
        frontPos = data.pos[7];
        backPos = data.pos[8];
    }
    //front sonars
    double fl = data.range[3];
    double fr = data.range[4];

    int closestIndex;
    double closestReading, diff, rangerDist, dist;
    double rotation = 1;
    double turnrate = PathExecuter::TURNRATE;
    Path* path;

    switch (state) {
        case MOVE:
            MAKE_LOG << "Executing Move..." << std::endl;
            //continue executing move until finished
            if (pe.executeMove()) {
                state = FOLLOW;
            } 
            break;
        

        case FOLLOW:
            MAKE_LOG << "Wall Following..." << std::endl;

            //check for concave corner
            if (fl < 0.4 || fr < 0.4) {
                pe.halt();
                //state = CONCAVE;
                break;
            } else {
                //calc difference between distances to wall
                diff = std::abs(front - back); 
                if (diff > 2.0 && back < front){
                //    state = CONCAVE;
                    break;
                }
            }
           // if ( front > 2.5 || back > 2.5 ) {
                // My sensors aren't near a wall yet? 
                // Move forward.
             //   pe.setMotion(Motion(PathExecuter::SPEED, 0));
            //} 
            /*else if ( front > 2.0 || back > 2.0 ){
                // turn towards wall.
                rotation = 0;
                if(isLeft) { 
                    rotation = M_PI/8;
                } else
                    rotation = - M_PI/8;
            }*/
            if(front < 0.5 ){
                if(isLeft) {
                    motor->setDiffDrive(0.25,0.5);
                }else
                    motor->setDiffDrive(0.25,0.5);
            } else if ( front > 1.5 ){
                if(isLeft) {
                    motor->setDiffDrive(0.5,0.25);
                }else
                    motor->setDiffDrive(0.25,0.5);
            } else
                motor->setDiffDrive(0.5,0.5);
            
            motor->update();
            
            /*if (state = FOLLOW){
                //calc difference between rangers on robot
                rangerDist = frontPos.calcDistTo(backPos);
                MAKE_LOG << "Calculating rotation..." << std::endl;
                //calc rotation required to make parallel
                rotation += atan2(diff,rangerDist);
                MAKE_LOG << "setting turnrate..." << std::endl;
                //calc turnrate as a factor of rotation
                turnrate = rotation*1.5;
                MAKE_LOG << "Turnrate : " << turnrate << std::endl;
                //set motion
                pe.setMotion(Motion(PathExecuter::SPEED, turnrate));
                break;            
            }*/
            break;
        case CONCAVE:
            MAKE_LOG << "Turning a Concave Corner..." << std::endl;
            //stop any existing movement
            pe.halt();

            //determine appropriate turn to make
            //currently set to make 90 degree turns
            path = new Path(robotLocation);
            if (isLeft) //making right turn
                path->addMove(Move(M_PI/-4.0, false));
            else //left turn
                path->addMove(Move(M_PI/4.0, false));

            //set turn
            pe.setPath(*path);
            //change state to execute turn
            state = MOVE;
            break;

        case CONVEX:
            MAKE_LOG << "Turning a Convex Corner..." << std::endl;
            //turn until both side rangers see wall again
            //check if both rangers out of corner range
            if (front >= 2 && back >= 2) {
                pe.halt();
                MAKE_LOG << "Adding 90 degree turn" << std::endl;
                path = new Path(robotLocation);
                if (isLeft)
                    path->addMove(Move(M_PI/4.0, false));
                else
                    path->addMove(Move(M_PI/-4.0, false));
                pe.setPath(*path);
                state = MOVE;
            }
            else
                pe.setMotion(Motion(PathExecuter::SPEED / 2.0, 0)); //move forward

            break;
    } //end switch-block
} //end update

void WallFollower::executeCommand(const Command cmd) {
    switch (cmd.name) {
        case follow:
            if (cmd.arg.size() >= 2) {
                if (cmd.arg[1] == "left")
                    isLeft = true;
                else if (cmd.arg[1] == "right")
                    isLeft = false;
                else
                    TO_CONSOLE("usage: follow side, side={ 'left', 'right' }");
            }
            else
                TO_CONSOLE("usage: follow side, side={ 'left', 'right' }");
            break;

        default:
            break;
    }
}

void WallFollower::setRangerData(RangerData data) {
    this->data = data;
    od.setRangerData(data);
}

void WallFollower::youAreHere(Position p) {
    robotLocation = p;
    pe.setLocal(p);
}

std::string WallFollower::toString() {
    return "WallFollower";
}
