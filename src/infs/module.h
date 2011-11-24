/** @file       src/infs/module.h
    @ingroup    INFS
    @brief      Module interface.
    @author     Jacob Perron <perronj@yorku.ca>
    @author     Alexander Moriarty <alexander@dal.ca>
*/

#ifndef __INFS_MODULE_H_
#define __INFS_MODULE_H_

#include <string>
#include "util/logger.h"

/** The base class for all modules of a robot.
 *
 *  This exists for the purpose of following the Object Orientated
 *  Programming style. Each of the robot parts would be a Module.
 *
 *  Having a Generic top level object can be handy.
 *  Two planned uses for this are:
 *      #) Standardize Logging across project.
 *          ##): All Modules include logger for logging.
 *          ##): All Modules must overwrite toString or operator<<. **
 *      #) Ability to create generic or dynamic functions that take a Module
 *         as a parametre. This likely won't be needed but the this project
 *         was not started with a clearly set Software Requirement Specification
 *         so we decided that a generic object would be best for planning ahead.
 *
 *      **Logic behind the standard toString and operator<< funtions:
 *         By creating a standard toString and operator<< functions, we plan
 *         to be able to use functions for two this:
 *              #). One could be use to print usage messages to console on incorrect commands.
 *                 (this will likely be toString)
 *              #). The other could be used to quickly output nicely formated variable values.
 *                 This would be nice for logging.
 *                 (because of how we use logger, this will likely be operator<<)
 *
 *  Any functions that should be implemented by a standard robot component,
 *  such as a sensor or actuator, should extend this interface.
 *
 */
class Module {
    public:

        /** Returns a string representation of the module.
         *
         *  The representation should include something like the name of the
         *  module and maybe the state of some of it's variables.
         *
         *  If the Module is one that interacts with the Console, and can be
         *  passed commands, then the toString should contain the command usage.
         *  Then when an invalid command is entered the command usage could be printed.
         *  Calling the "help <command>" would call the toString of <command>
         *
         * @return Module string representation.
         */
        virtual std::string toString() = 0;


};
#endif
