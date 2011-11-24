#include "bug.h"

CREATE_LOGGER("Bug");

Bug::Bug(Motor& m) : Controller(m), wf(m) {
    isArrived = false;
    firstCallBug2 = true;
    LOG_CTOR << "Constructed." << std::endl;
}

Bug::~Bug() {
    LOG_DTOR << "Destructed." << std::endl;
}

void Bug::update() {
}

bool Bug::bug2(Position goal) {
    //on activation record position
    if (firstCallBug2 && !isArrived) {
        lastPos = robotPos;
        firstCallBug2 = false;
    }

    Line line(lastPos, goal);

    //check for terminating condition
    if (line.calcDistTo(robotPos) <= DIST_THRESHOLD
        && robotPos.calcDistTo(lastPos) > DIST_THRESHOLD) {
        pe.halt();
        isArrived = true;
        firstCallBug2 = true;
        MAKE_LOG << "Found line!" << std::endl;
    }
    else
        isArrived = false;

    if (!isArrived) //continue wall-following
        wf.update();
    return isArrived;
}

void Bug::setRangerData(RangerData data) {
    wf.setRangerData(data);
    od.setRangerData(data);
}

void Bug::youAreHere(Position pos) {
    robotPos = pos;
    pe.setLocal(pos);
    wf.youAreHere(pos);
}

void Bug::executeCommand(const Command cmd) {
    wf.executeCommand(cmd);
}
