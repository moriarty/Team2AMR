/** @file       src/infs/commandexecuter.h
    @ingroup    INFS
    @brief      Interface for command executing modules.
    @author     Jacob Perron <perronj@yorku.ca>
    @author     Alexander Moriarty <alexander@dal.ca>
*/

#ifndef __INFS_COMMANDEXECUTER_H_
#define __INFS_COMMANDEXECUTER_H_

#include "data/command.h"

/** Implemented by classes that need to take external commands.
 *
 *  All modules that need to be able to accept commands from the console
 *  should implement this interface. This insures a module can interpret
 *  a command from the console pertaining to itself.
 */
class CommandExecuter {
    public:

        /** Interprets and executes a command.
         *
         *  Should contain a switch-block to determine what
         *  function(s) to call or changes to make within the class, or if
         *  the command is applicable in the first place.
         *
         *  @param command : A Command object containing the command and
         *      arguments, if any.
         */
        virtual void executeCommand(const Command command) = 0;
};
#endif
