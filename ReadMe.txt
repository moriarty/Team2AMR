"Textual Commands" (2011-11-03)
    Task 1: 5 hours.
    Task 2: 7 hours.
    Task 3: 3 hours.    

    Added Functionality:
        -Complex data structure Path.
        -Utility class PathPlanner.
        -MotionCommand controller.
        -New and improved Console.
        -Console Manual (below)

"Wall-follower" (2011-11-10)
    Task 1: 7 hour
    Task 2: 0.5 hour

    Added Functionality:
        -"In-Console" logging. (Along with use of 'clear' command)
        -Planning state to MotionCommand using 'plan' and 'endplan' commands.
        -Separate log file for class construction and destruction.
        -Fixed differential drive.
        -WallFollower controller.

"Bug-2" (2011-11-17)
    Task 1: 2 hours
    Task 2: 1 hour
    Task 3:

    Added Functionality:
        -Bug controller (with Bug-2 algorithm).
        -Integrated bug-2 algorithm with MotionCommand controller.

Planned Edits/Functionality:
    -Add more functionality to PathPlanner...(fix PathPlanner so that it is the
     only place where Path objects are created).
    -Make class Local maybe utility class easily accessed by all other modules.
    -Have ObjectAvoider alert user through console.
    -Make Console prettier.

CONSOLE MANUAL

--------------------------------------------------------------------------------

LEGEND:

<...> - Represents a mandatory command argument. Without it command will not
        function.

[...] - Represents an optional command.


BASIC COMMANDS:

start - Switches on the robot. Note the robot will continue to execute its last
        instruction while the power was off. Hence instructions can be given
        before starting the robot.

stop - Switches off the robot. Does not kill commands waiting to be processed by
       the controller.

load <type> [...] - Sets the type of controller for the robot. The optional
                      arguments are controller type dependent. See the list of
                      controllers below for details.

clear - Clears the "in-console" log history.
 
exit - Exits the program. 


LIST OF CONTROLLERS: 

braindead (bd) - The most basic controller. Does nothing.

motioncommand (mc) - A simple controller that interacts with the world based entirely
                on user input via the console, with the exception of its object
                avoidence system halting the motor when expecting a crash.
 
commandlist { move, turn, goto, plan, endplan }

move <distance> [direction] - Moves the robot a set distance in meters either
                              "forward" or "backward". If no direction specified
                              forward movement is assumed.

turn <degrees> - Turns the robot on the spot measured in degrees relative to the
                 robot.

goto <x> <y> <rotation> - Moves the robot to the specified location, x and y as
                          coordinates, and rotation measured in degrees relative
                          to the world.

plan - Puts the controller into "planning mode". In this state all motion
       commands are stored in a single Path for later execution.

endplan - Takes the controller out of the planning state. The robot will
          start executing any motion commands issued during the planning state,
          and will continue until finished or a new command is issued. A new
          command will overwrite any commands stored during the planning state.

wallfollower - A controller that finds the nearest wall and begins traveling
               along parallel to the wall. By default, the robot will keep its
               right-side parallel to the wall, but this can be changed using
               the 'follow' command.

commandlist { follow }

follow <side> - Tells the robot which side it should keep parallel to the wall.
                Side can either be 'left' or 'right'.

bug - This controller is used to implement various bug algorithms. Typically
      used inside of other controllers, it can be used as the robots sole
      controller with nothing but a 'goto' command and obstacle avoidence using
      a bug algorithm.

EXAMPLES:

Once robot is loaded into Player/Stage, one could type such sequences of
commands to the console:

    load wallfollower
    follow left
    start
    exit

    load motioncommand
    start
    move 6 forward
    stop
    turn -45
    start
    move 2 backward
    turn 45
    exit

    start
    load motioncommand
    bug2 on
    goto 5 5 0
    move 4
    stop
    exit

    load motioncommand
    plan
    move 2
    turn -45
    move 3
    turn -45
    move 1
    goto -6.5 -6.5 45
    endplan
    start
    exit
