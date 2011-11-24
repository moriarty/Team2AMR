#include "motor.h"

CREATE_LOGGER("Motor");

Motor::Motor(PlayerCc::Position2dProxy& proxy) {
    positionProxy = &proxy;
    LOG_CTOR << "Constructed." << std::endl;
}

Motor::~Motor() {
    LOG_DTOR << "Destructed." << std::endl;
}

void Motor::setTranslation(double translation) {
    motion.x = translation;
}

void Motor::setRotation(double rotation) {
    motion.yaw = rotation;
}

void Motor::update() {
    positionProxy->SetSpeed(motion.x, motion.yaw);
}

void Motor::halt() {
    setMotion(0, 0);
    update();
}

void Motor::setDiffDrive(double rWheel, double lWheel, double wheelDist) {
    double translation, rotation;
    translation = (rWheel + lWheel)/2;
    if(rWheel > lWheel) {
        rotation = (rWheel - lWheel)/wheelDist;
    } else if (lWheel > rWheel) {
        rotation = (lWheel - rWheel)/wheelDist;
    } else {
        rotation = 0;
    }
    Motor::setMotion(translation, rotation);
}

void Motor::setMotion(double translation, double rotation) {
    motion.x = translation;
    motion.yaw = rotation;
}

void Motor::setMotion(Motion m) {
    motion = m;
}

void Motor::goTo(Position pos) {
    positionProxy->GoTo(pos.x, pos.y, pos.yaw);
}

std::string Motor::toString() {
    std::stringstream out;
    out << "X: "<< positionProxy->GetXPos() << " ";
    out << "Y: " << positionProxy->GetYPos() << " ";
    out << "Yaw: " << positionProxy->GetYaw() << std::endl;
    return out.str();
}
