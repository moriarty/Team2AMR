#include "objectavoider.h"

CREATE_LOGGER("ObjectAvoider");

ObjectAvoider::ObjectAvoider(Motor& motor) {
    this->motor = &motor;
    LOG_CTOR << "Constructed." << std::endl;
}

ObjectAvoider::~ObjectAvoider() {
    LOG_DTOR << "Destructed." << std::endl;
}

void ObjectAvoider::avoid() {
    motor->halt();
    MAKE_LOG << "Motor halted." << std::endl;
}

void ObjectAvoider::setPosition(Path pos) {
    this->position = pos;
}

void ObjectAvoider::setMotor(Motor& motor) {
    this->motor = &motor;
}

std::string ObjectAvoider::toString() {
    //TODO
    return "ObjectAvoider";
}
