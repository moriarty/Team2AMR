/** @file       src/plan/map.h
    @ingroup    PLAN
    @brief      Map Object.
    @author     Alex Moriarty <alexander@dal.ca>
    @author     Jacob Perron <perronj@yorku.ca>
*/

#ifndef __PLAN_MAP_H_
#define __PLAN_MAP_H_

#include "infs/module.h"

/** Map 
 * 
 *  Contains known data the robot has collected from the outside world.
 *  Intended to for use calculating future paths and avoiding 
 *  wrong turns.
 **/
class Map : public Module{
    public:
        /**
        * Default constructor.
        */
        Map();
        /**
         * Destructor
         */
         ~Map();

        /** Inherited from Module   */
        std::string toString();

    private:
        /** 
        * Disable copy constructor.
        */
        Map(const Map& source);

};
#endif
