/** @file       src/data/command.h
    @ingroup    Data
    @brief      Command struct.
    @author     Alex Moriarty <alexander@dal.ca>
    @author     Jacob Perron <perronj@yorku.ca>
*/

#ifndef __DATA_COMMAND_H_
#define __DATA_COMMAND_H_

#include <vector>

enum robotCommand {
    start,
    stop,
    load,
    gt,
    move,
    turn,
    plan,
    endplan,
    follow,
    mode,
    behave,
    bug2,
    NAC //Not A Command
};


/** Contains a command and its arguments.
 *
 *  Used to pass commands from console to the various modules.
 */
struct Command {

    robotCommand name;
    std::vector<std::string> arg;

    /** Constructor. */
    Command(robotCommand cmd) :  name(cmd) { };

    /** Default Contructor. */
    Command() : name(NAC) {};
};
#endif
