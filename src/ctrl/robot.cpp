#include "robot.h"

CREATE_LOGGER("Robot");


Robot::Robot( PlayerCc::PlayerClient &robot,
              PlayerCc::RangerProxy &rangerProxy,
              PlayerCc::Position2dProxy &positionProxy
            ) {
    //initialize variables
    ranger.push_back(new Ranger(rangerProxy));
    motor = new Motor(positionProxy);
    local = new Local(positionProxy);
    player = &robot;
    controller = new Controller(*motor);
    power = false;

    //pass console to logger for "in-console" logging
    Logger::setConsole(console);
    LOG_CTOR << "Constructed." << std::endl;
}

Robot::Robot() {
    PlayerCc::PlayerClient player(PlayerCc::PLAYER_HOSTNAME, PlayerCc::PLAYER_PORTNUM);

    //create ranger and motor classes
    PlayerCc::Position2dProxy positionProxy(&player, 0);
    PlayerCc::RangerProxy rangerProxy1(&player, 0);
    PlayerCc::RangerProxy rangerProxy2(&player, 1);

    //connect rangers/motor with player
    player.Read();
    positionProxy.RequestGeom();
    rangerProxy1.RequestConfigure();
    rangerProxy1.RequestGeom();
    rangerProxy2.RequestConfigure();
    rangerProxy2.RequestGeom();
    player.Read();

    this->player = &player;
    motor = new Motor(positionProxy);
    ranger.push_back(new Ranger(rangerProxy1));
    ranger.push_back(new Ranger(rangerProxy2));
    local = new Local(positionProxy);
    controller = new Controller(*motor);
    power = false;

    Logger::setConsole(console);
    LOG_CTOR << "Constructed." << std::endl;
}

Robot::~Robot() {
    delete controller;
    delete motor;
    delete local;
/*
    Ranger** sen = ranger.c_array();
    for (int i = 0; i < ranger.size(); i++) {
        delete sen[i];
    }
*/
    LOG_DTOR << "Destructed." << std::endl;
}

void Robot::addRanger(Ranger& ranger) {
    this->ranger.push_back(&ranger);
}

void Robot::setController(Controller& control) {
    this->controller = &control;
}

void Robot::run() {
    bool continueOperation = true;

    MAKE_LOG << "Enter superloop!" << std::endl;

    //enter superloop
    while(continueOperation) {
        //update console
        continueOperation = console.update();

        //check for new command
        if (console.isNewCommand()) {
            //get new command
            Command cmd = console.getCommand();
            //execute command
            executeCommand(cmd);
        }

        //get update from Player/Stage
        player->Read();

        int size = ranger.size();

        //read ranger data and pass to controller
        if (size == 1) {
            RangerData data = ranger[0].getData();

            //pass data to controller
            controller->setRangerData(data);
        }
        else { //multiple rangers
            RangerData data[size];
            //initialize array
            for (int i = 0; i < size; i++)
                data[i] = ranger[i].getData();

            //pass data to controller
            controller->setRangerData(data);
        } //end pass ranger data

        //pass local
        controller->youAreHere(local->getLocal());

        //if power is off skip controller update.
        if (!power)
            continue;

        //update controller
        controller->update();
    } //end superloop
} //end run

void Robot::executeCommand(const Command command) {
/*  MAKE_LOG << "Num of Args: " << command.arg.size() << std::endl;
    for (int i = 0; i < command.arg.size(); i++)
        MAKE_LOG << command.arg[i] << std::endl;
*/
    switch (command.name) {
        case start:
            //start robot
            power = true;
            MAKE_LOG << "Power On." << std::endl;
            break;

        case stop:
            //stop robot
            power = false;
            motor->halt();
            MAKE_LOG << "Power Off." << std::endl;
            break;

        case load:
            //new Controller
            Controller* newController;
            if (command.arg.size() < 2) {
                TO_CONSOLE("usage: load type");
                break;
            }
            if (command.arg[1] == "motioncommand" || command.arg[1] == "mc") {
                newController = new MotionCommand(*motor);
                MAKE_LOG << "Created MotionCommand Controller." << std::endl;
            }
            else if (command.arg[1] == "braindead" || command.arg[1] == "bd") {
                newController = new Controller(*motor);
                MAKE_LOG << "Created Brain-Dead Controller." << std::endl;
            }
            else if (command.arg[1] == "wallfollower" || command.arg[1] == "wf") {
                newController = new WallFollower(*motor);
                MAKE_LOG << "Created Wall-Follower Controller." << std::endl;
            }
            else if (command.arg[1] == "braitenberg" || command.arg[1] == "bb") {
                newController = new Braitenberg(*motor);
                MAKE_LOG << "Created Braitenberg Controller." << std::endl;
            }
            else if (command.arg[1] == "bug") {
                newController = new Bug(*motor);
                MAKE_LOG << "Created Bug Controller." << std::endl;
            }
            else { //no new controller
                MAKE_LOG << "Failed to add new Controller." << std::endl;
                TO_CONSOLE("Failed to load such a controller.");
                break;
            }

            TO_CONSOLE("Load successful.");

            //delete old controller
            delete controller;
            //assign new controller
            controller = newController;
            break;

        case NAC:
            //do nothing
            TO_CONSOLE("That's not a Command.");
            break;

        default:
            controller->executeCommand(command);
            break;
    } //end switch
} //end executeCommand

