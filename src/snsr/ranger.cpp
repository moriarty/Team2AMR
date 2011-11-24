#include "ranger.h"

CREATE_LOGGER("Ranger");

Ranger::Ranger(PlayerCc::RangerProxy& proxy) {
    rangerProxy = &proxy;
    LOG_CTOR <<  "Constructed." << std::endl;
}

Ranger::~Ranger() {
    LOG_DTOR << "Destructed." << std::endl;
}

RangerData Ranger::getData() {
    int count = rangerProxy->GetRangeCount();
    RangerData data;

    for (int i = 0; i < count; i++) {
        data.range.push_back(rangerProxy->GetRange(i));
        Position p;
        p.x = rangerProxy->GetElementPose(i).px;
        p.y = rangerProxy->GetElementPose(i).py;
        p.yaw = rangerProxy->GetElementPose(i).pyaw;
        data.pos.push_back(p);
    }

    data.angleRes = rangerProxy->GetAngularRes();
    data.minAngle = rangerProxy->GetMinAngle();
    data.maxAngle = rangerProxy->GetMaxAngle();
    data.maxRange = rangerProxy->GetMaxRange();
    data.minRange = rangerProxy->GetMinRange();

    return data;
}

std::string Ranger::toString() {
    //TODO
    return "Ranger (RangerProxy)";
}
