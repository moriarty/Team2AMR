#include "objectdetector.h"

CREATE_LOGGER("ObjectDetector");

ObjectDetector::ObjectDetector() {
    threshold = 0.25;
    LOG_CTOR << "Constructed." << std::endl;
}

ObjectDetector::~ObjectDetector() {
    LOG_DTOR << "Destructed." << std::endl;
}

bool ObjectDetector::check() {
    //int minAngle = (data.range.size() * 3) / 8;
    //int maxAngle = (data.range.size() * 5) / 8;
    for (int i = 0; i < data.range.size(); i++) {
        if (data.range[i] < threshold)
            return true;
    }
    return false;
}

void ObjectDetector::setThreshold(double t) {
    threshold = t;
}

void ObjectDetector::setRangerData(RangerData data) {
    this->data = data;
}

std::string ObjectDetector::toString() {
    return "ObjectDetector";
}
