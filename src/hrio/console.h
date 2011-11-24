/** @file       src/hrio/console.h
    @ingroup    HRIO
    @brief      Human-robot interaction through a console.
    @author     Jacob Perron <perronj@yorku.ca>
    @author     Alexander Moriarty <alexander@dal.ca>
*/

#ifndef __HRIO_CONSOLE_H_
#define __HRIO_CONSOLE_H_

#include <sstream>
#include "PSTermIOSimple.h"
#include "data/command.h"

/** This class provides means for human-robot interaction through a console.
 *  It uses the PSTermIO library to make the user input non-blocking.
 */
class Console {
    public:

        /** Default constructor. */
        Console();

        /** Default destructor. */
        virtual ~Console();

        /** Checks if there is a new input from a user, processes it and executes
         *  commands.
         *
         *  Here we distinguish between two "states" of the console system:
         *    #) The menu is NOT displayed, NOT waiting for the user input.
         *    #) The menu is on the screen and system waits for the user input.
         *  When the system is in first state, we clear screen, display menu and ask
         *  for the user input.
         *  When the system is in second state, we check if the user input ready (which
         *  means that the user has typed something and hit "enter"). If it is ready,
         *  then we process it and execute whatever is commanded.
         *
         *  @return false if the exit command received, true if a regular command or no
         *          command received
         */
        bool update();

        /** Logs a line to the console.
         *
         *  The string provided is added to the log history of the console, part of
         *  which is displayed to console.
         *
         *  @param line : The string to be logged.
         */
        void log(const std::string line);

        /** Gets the last command issued.
         *
         *  Should be called after calling @ref isNewCommand in order to check
         *  whether or not the command is old and should not be executed.
         *
         *  @return The last Command object created by Console.
         */
        Command getCommand();

        /** Used to let the client know if there has been a new command issued.
         *
         *  @return @c true if there has been a new command since the last
         *      @ref getCommand call. @c false otherwise.
         */
        bool isNewCommand();

        /** Static method used to convert a string into a 'robotCommand' enum.
         *
         *  @param str : String to be converted into an enum.
         */
        robotCommand stoe(const std::string);

    private:

        /** Prints the main menu to the terminal. */
        void displayMenu();

    /** Prints the help menu to the terminal. */
//    void displayHelp();

    /** Prints the about menu to the terminal. */
//    void displayAbout();

    /** Prints a description of the current robot to the terminal. */
//    void displayDescription();

    /** Prints the lolcatz page to the terminal. */
//    void displayLolcats();

        /** Processes a received command.
        *  The command is represented by a string entered by user. The function parses
        *  string, recognizes the command and executes it.
        */
        void processCommand(std::string& command);

        /** Disable copy constructor. */
        Console(const Console& source);

        /** Disable assignment operator. */
        Console& operator=(const Console& source);

        /** Use of provided PSTermIO object. */
        MobileRobots::PSTermIO terminal;

        /** The last command issued through the console. */
        Command command;

        /** State depends on whether there is new command or not. */
        bool newCommand;

        /** History logging commands and other information. */
        std::vector<std::string> history;
};
#endif
