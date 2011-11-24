#include "braitenberg.h"
#include <sstream>

CREATE_LOGGER("Braitenberg");

Braitenberg::Braitenberg(Motor& m) : Controller(m) {
    motor = &m;
    config = A;
    behaviour = INHIBIT;
    LOG_CTOR << "Constructed." << std::endl;
}

Braitenberg::~Braitenberg() {
    LOG_DTOR << "Destructed." << std::endl;
}

void Braitenberg::update() {
    double leftRange = data.range[0];
    double rightRange = data.range[1];
    double leftSpeed, rightSpeed;

    switch (config) {
        case A:
            switch (behaviour) {
                case INHIBIT:
                    leftSpeed = 1/(1+leftRange);
                    rightSpeed = 1/(1+rightRange);
                    break;

                case EXCITE:
                    leftSpeed = 1+leftRange;
                    rightSpeed = 1+rightRange;
                    break;
            }
            break;

        case B:
            switch (behaviour) {
                case INHIBIT:
                    leftSpeed = 1/(1+rightRange);
                    rightSpeed = 1/(1+leftRange);
                    break;

                case EXCITE:
                    leftSpeed = 1+rightRange;
                    rightSpeed = 1+leftRange;
                    break;
            }
            break;

        case C:
            switch (behaviour) {
                case INHIBIT:
                    leftSpeed = 1/(1+rightRange) + 2/(2+leftRange);
                    rightSpeed = 1/(1+leftRange) + 2/(2+rightRange);
                    break;

                case EXCITE:
                    leftSpeed = 0.5+rightRange+leftRange;
                    rightSpeed = 0.5+leftRange+rightRange;
                    break;
            }
            break;
    } //end config switch-case

    motor->setDiffDrive(rightSpeed, leftSpeed);
    motor->update();
}

void Braitenberg::executeCommand(const Command cmd) {
    switch(cmd.name){
        case mode:
            if (cmd.arg.size() >= 2) {
                if (cmd.arg[1] == "a" || cmd.arg[1] == "A" ) {
                    config = A;
                    MAKE_LOG << "Set config to 'a'"<<std::endl;
                } else if (cmd.arg[1] == "b" || cmd.arg[1] == "B") {
                    config = B;
                    MAKE_LOG << "Set config to 'b'"<<std::endl;
                } else if (cmd.arg[1] == "c" || cmd.arg[1] == "C") {
                    config = C;
                    MAKE_LOG << "Set mode to 'c'"<<std::endl;
                } else
                    TO_CONSOLE("usage: mode braitenConfig, braitenConfig={'A', 'B' or 'C'}");
            }
            else
                TO_CONSOLE("usage: mode braitenConfig, braitenConfig={'A', 'B' or 'C'}");
            break;

        case behave:
            if (cmd.arg.size() >= 2) {
                if (cmd.arg[1] == "x" || cmd.arg[1] == "excite" ) {
                    behaviour = EXCITE;
                    MAKE_LOG << "Set behave to 'excite'"<< std::endl;
                } else if (cmd.arg[1] == "i" || cmd.arg[1] == "inhibit" ) {
                    behaviour = INHIBIT;
                    MAKE_LOG << "Set behave to 'inhibit'"<< std::endl;
                } else
                    TO_CONSOLE("usage: behave braitenBehave, braitenBehave={'excite' or 'inhibit'}");
            }
            else
                TO_CONSOLE("behave <braitenBehave>, braitenConfig = 'excite', 'inhibit'");
            break;

        default:
            break;
    }
}

void Braitenberg::setRangerData(RangerData data) {
    this->data = data;
}

std::string Braitenberg::toString() {
    return "Braitenberg Controller";
}
