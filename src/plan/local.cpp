#include "local.h"

CREATE_LOGGER("Local");

Local::Local(PlayerCc::Position2dProxy& proxy) {
    this->proxy = &proxy;
    LOG_CTOR << "Constructed." << std::endl;
}

Local::~Local() {
    LOG_DTOR << "Destructed." << std::endl;
}

Position Local::getLocal() {
    return Position(proxy->GetXPos(), proxy->GetYPos(), proxy->GetYaw());
}

std::string Local::toString() {
    std::stringstream out;
    out << "X: " << proxy->GetXPos() << " ";
    out << "Y: " << proxy->GetYPos() << " ";
    out << "Yaw: " << proxy->GetYaw();
    return out.str();
}
