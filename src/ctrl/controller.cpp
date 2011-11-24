#include "controller.h"

CREATE_LOGGER("Controller");

Controller::Controller(Motor& m) : pe(m), oa(m) {
    LOG_CTOR << "Constructed." << std::endl;
}

Controller::~Controller() {
    LOG_DTOR << "Destructed." << std::endl;
}

void Controller::setRangerData(RangerData data) {
    od.setRangerData(data);
}

void Controller::setRangerData(RangerData *data) {
    if (data != NULL) {
        //Use first ranger only
        setRangerData(data[0]);
    }
    else
        MAKE_LOG << "No ranger data passed!" << std::endl;
}

void Controller::update() {
    //a brain-dead robot.
}

void Controller::setMotor(Motor& m) {
    pe.setMotor(m);
}

void Controller::youAreHere(Position pos) {
    //Brain-dead robot doesn't care.
}

void Controller::executeCommand(const Command cmd) {
    //Brain-dead robot doesn't know how to execute commands.
}

std::string Controller::toString() {
    return "Brain-Dead Controller";
}
