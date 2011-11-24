#include "console.h"
#include <time.h>
#include <iostream>
#include <algorithm>
#include <iterator>

//CREATE_LOGGER("Console");

Console::Console() {
    terminal.setTimeout(10);
    newCommand = false;
  //  MAKE_LOG << "Constructed." << std::endl;
}

Console::~Console() {
  // MAKE_LOG << "Destructed." << std::endl;
}

bool Console::update() {
  std::string line;
  if(!terminal.checkRequestPending()) {
    terminal.clrScreen();          // clear terminal screen
    write(1, "\0337\033[1;1H", 8); // move cursor to its upper-left corner
    displayMenu();
    terminal.requestLine(">");
  }
  else {
    if(terminal.getLine(line)) {   // read command, if any
      if(line == "exit")           // check if the command is "exit"
        return false;
      else if (line == "clear")
        history.clear();
      else
        processCommand(line);      // regular command, process
    }
  }
  return true;
}

Command Console::getCommand() {
    newCommand = false;
    return command;
}

bool Console::isNewCommand() {
    return newCommand;
}

void Console::displayMenu() {
    srand(time(NULL));
    std::cout << "Autonomous Mobile Robots W2011\t\t\t";
    if (rand() % 2 == 1)
        std::cout << "Jacob Perron, Alex Moriarty";
    else
        std::cout << "Alex Moriarty, Jacob Perron";

    std::cout << std::endl << std::endl
              << "Capabilities:" << std::endl
              << "\t-can load Brain-Dead controller (braindead)" << std::endl
              << "\t-can load MotionCommand controller (motioncommand)" << std::endl
              << "\t-can load WallFollower(wallfollower)" << std::endl
              << std::endl << "Coming Soon:" << std::endl
              << "\t-Braitenberg" << std::endl
              << "\t-Obstacle-Avoidence" << std::endl << std::endl;

    std::cout << "Please look at 'ReadMe.txt' for info on commands." << std::endl;
    std::cout << "Enter 'exit' to terminate robot." << std::endl;
    std::cout << "============================================================" << std::endl;

    //only maintain last 10 commands
    if (history.size() > 10)
        history.resize(10);

//    for (unsigned int i = 0; i < history.size(); i++)
//        std::cout << history[i] << std::endl;
    for (unsigned int i = history.size(); i > 0; i--)
        std::cout << history[i-1] << std::endl;
}

void Console::processCommand(std::string& line) {
    //add line to history
    history.insert(history.begin(), ">" + line);

    //split command and args
    std::vector<std::string> token;

    std::istringstream iss(line);
    std::copy( std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(),
              std::back_inserter<std::vector<std::string> >(token) );

    std::string c = token[0];
    //create Command object with enum robotCommand as name
    Command cmd(stoe(c));
    //copy remaining args, if any
    cmd.arg = token;

    command = cmd;
    newCommand = true;
} //end processCommand

/** Convienence method that turns a string into a robotCommand enum. */
robotCommand Console::stoe(const std::string str) {
    if (str == "start")
        return start;
    else if (str == "stop")
        return stop;
    else if (str == "load")
        return load;
    else if (str == "goto")
        return gt;
    else if (str == "move")
        return move;
    else if (str == "turn")
        return turn;
    else if (str == "plan")
        return plan;
    else if (str == "endplan")
        return endplan;
    else if (str == "follow")
        return follow;
    else if (str == "bug2")
		return bug2;
    else if (str == "mode")
		return mode;
    else if (str == "behave")
		return behave;
	else
        return NAC; //Not A Command
}

void Console::log(const std::string line) {
    history.insert(history.begin(), line);
}
