#include "navigation.h"

CREATE_LOGGER("Navigation");

Navigation::Navigation() {

    LOG_CTOR << "Constructed." << std::endl;
}

Navigation::~Navigation() {

    LOG_DTOR << "Destructed." << std::endl;
}

std::string Navigation::toString() {
    return "Navigation";
}
