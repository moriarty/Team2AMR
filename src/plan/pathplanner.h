/** @file       src/plan/pathplanner.h
    @ingroup    PLAN
    @brief      Path Planner Utility Class.
    @author     Alex Moriarty <alexander@dal.ca>
    @author     Jacob Perron <perronj@yorku.ca>
*/

#ifndef __PLAN_PATHPLANNER_H_
#define __PLAN_PATHPLANNER_H_

#include <math.h>
#include "data/position.h"
#include "data/path.h"
#include "util/logger.h"

/** Utility Class Containing Static Planning Functions.
 *
 *  Path Planner is a collection of functions and calculation to
 *  help plan the robots next move.
 **/
class PathPlanner {
    public:
        /** Calculates a Path from a Point to Destination
        *
        * Calculates a direct path from the Robot to the desired Destination.
        *
        * @param robot : Position to start calculating path from.
        * @param dest : Position of the goal Destination.
        * @return Path.
        */
        static Path* calcPath(Position p1 , Position dest);

        //comment later todo
        static Path* calcPath(Position p1, double dist);

        /** Returns a new Position based on a root Position and distance traveled.
         *
         *  @param p1 : Original Position.
         *  @param dist : Distance traveled.
         *
         *  @return Newly calculated Position.
         */
        static Position calcPosition(Position p1, double dist);

    private:
        /**
        * Destructor
        */
        ~PathPlanner();
        /**
        * Default constructor.
        */
        PathPlanner();
        /**
        * Disable copy constructor.
        */
        PathPlanner(const PathPlanner& source);
};
#endif
