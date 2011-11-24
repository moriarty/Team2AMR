#include "psmotor.h"

CREATE_LOGGER("PSMotor");

PSMotor::PSMotor(PlayerCc::Position2dProxy& proxy) {
    positionProxy = &proxy;
    MAKE_LOG << "Constructed." << std::endl;
}

PSMotor::~PSMotor() {
    MAKE_LOG << "Destructed." << std::endl;
}

void PSMotor::update() {
    positionProxy->SetSpeed(motion.x, motion.yaw);
}

void PSMotor::goTo(Position pos) {
    //TODO
}

std::string PSMotor::toString(){
    return "TODO: Update this later";
}
