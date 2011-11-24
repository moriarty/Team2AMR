/** @file       src/plan/navigation.h
    @ingroup    PLAN
    @brief      navigation class
    @author     Alex Moriarty <alexander@dal.ca>
    @author     Jacob Perron <perronj@yorku.ca>
*/

#ifndef __PLAN_NAVIGATION_H_
#define __PLAN_NAVIGATION_H_

#include "infs/module.h"

/** Navigation Functions and Methods
 *
 *  The Navigation Class is intended to contain all the aspects of
 *  Planning the route and passing it back to the controller.
 *  It keeps trace of where the Robot has been, where it will go next.
 */
class Navigation : public Module {
    public:

        /** Constructor. */
        Navigation();

        /** Destructor. */
         ~Navigation();

        /** Inherited from Module. */
        std::string toString();

    private:

        /** Disable copy constructor. */
        Navigation(const Navigation& source);

        /** Disable assignment operator. */
        Navigation& operator=(const Navigation& source);

};
#endif
