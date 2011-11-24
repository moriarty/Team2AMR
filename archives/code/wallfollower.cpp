#include "wallfollower.h"

CREATE_LOGGER("WallFollower");

WallFollower::WallFollower(Motor& m) : Controller(m) {
    //follow with right side of robot along wall by default
    isLeft = false;
    //set controller to first state
    state = looking;
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
        case looking:
            MAKE_LOG << "Looking..." << std::endl;
            closestIndex = -1; //sonar index with closest reading
            closestReading = data.maxRange; //best reading

            //determine closest reading
            for (unsigned int i = 0; i < data.range.size(); i++) {
                if (data.range[i] < closestReading) {
                    closestReading = data.range[i];
                    closestIndex = i;
                }
            }

            //if no wall found, move forward.
            if (closestIndex == -1)
                pe.setMotion(Motion(PathExecuter::SPEED, 0));
            else {
                //stop motor, in case already moving forward
                pe.halt();
                //determine angle to pose robot
                dist = data.pos[closestIndex].yaw;
                //Set pathexecuter to turn robot
                path = new Path(robotLocation);
                path->addMove(Move(dist, false));
                pe.setPath(*path);
                //move to next state, which executes set turn
                state = found;
            }
            break;

        case found:
            MAKE_LOG << "Found..." << std::endl;
            if (pe.executeMove()) { //if finished turning
                //check forward rangers
                if (fr <= 0.4 || fl <= 0.4) { //stop, set turn
                    //pe.halt();
                    path = new Path(robotLocation);
                    //set turn to align side of robot with wall
                    //at this point it is assumed robot is more or less
                    //perpendicular to the wall, hence 90 degree turns.
                    if (isLeft)
                        path->addMove(Move(M_PI/-2.0, 0));
                    else
                        path->addMove(Move(M_PI/2.0, 0));

                    pe.setPath(*path);

                    //move to next state, which excutes set turn
                    state = executeMove;
                }
                else //continue moving forward
                    pe.setMotion(Motion(PathExecuter::SPEED, 0));
            }
            break;

        case executeMove:
            MAKE_LOG << "Executing Move..." << std::endl;
            //continue executing move until finished
            if (pe.executeMove()) {
                state = movingParallel;
            }
            break;

        case movingParallel:
            MAKE_LOG << "Moving Parallel..." << std::endl;

            //check for concave corner
            if (fl < 0.4 || fr < 0.4) {
                state = concave;
                break;
            }

            //calc difference b/t distances to wall
            diff = std::abs(front - back);
            //calc difference b/t rangers on robot
            rangerDist = frontPos.calcDistTo(backPos);

            //get furthest distance and set rotation direction
            if (back > front && front >= 0.25 && isLeft)
                rotation = -1;
            else if (front > back && back >= 0.25 && !isLeft)
                rotation = -1;

            //check for convex corner
            if (diff >= rangerDist) { //>=rangerDist else asin() below will fail
                if (rotation == -1) //means rf > rb
                    state = convex;
                else
                    diff = rangerDist;
                break;
            }

            MAKE_LOG << "Calculating rotation..." << std::endl;
            //calc rotation required to make parallel
            rotation *= asin(diff/rangerDist);
            MAKE_LOG << "setting turnrate..." << std::endl;
            //calc turnrate as a factor of rotation
            turnrate = rotation*1.5;
            MAKE_LOG << "Turnrate : " << turnrate << std::endl;
            //set motion
            pe.setMotion(Motion(PathExecuter::SPEED, turnrate));
            break;

        case concave:
            MAKE_LOG << "Concave..." << std::endl;
            //stop any existing movement
            pe.halt();

            //determine appropriate turn to make
            //currently set to make 90 degree turns
            path = new Path(robotLocation);
            if (isLeft) //making right turn
                path->addMove(Move(M_PI/-2.0, false));
            else //left turn
                path->addMove(Move(M_PI/2.0, false));

            //set turn
            pe.setPath(*path);
            //change state to execute turn
            state = executeMove;
            break;

        case convex:
            MAKE_LOG << "Convex..." << std::endl;
            //turn until both side rangers see wall again
            //check if both rangers out of corner range
            if (front >= 2 && back >= 2) {
                pe.halt();
                MAKE_LOG << "Adding 90 degree turn" << std::endl;
                path = new Path(robotLocation);
                if (isLeft)
                    path->addMove(Move(M_PI/2.0, false));
                else
                    path->addMove(Move(M_PI/-2.0, false));
                pe.setPath(*path);
                state = executeMove;
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
