/** @file       src/util/local.h
    @ingroup    UTIL
    @brief      Provides robots local.
    @author     Alex Moriarty <alexander@dal.ca>
    @author     Jacob Perron <perronj@yorku.ca>
*/

#ifndef __UTIL_LOCAL_H_
#define __UTIL_LOCAL_H_

#include "infs/module.h"
#include "data/position.h"
#include <libplayerc++/playerc++.h>
#include <sstream>

/** Keeps track of the robot's position in the world.
 *
 *  Utilizes PlayerCc::Position2dProxy to obtain robots up-to-date position.
 *  All classes that need to know the robots current location can do so by
 *  calling @ref getLocal .
 */
class Local : public Module {
    public:

        /** Constructor.
         *
         *  @param proxy : Reference to Player/Stage Position2dProxy.
         */
        Local(PlayerCc::Position2dProxy& proxy);

        /** Destructor. */
         ~Local();

        /** Returns the current location of the robot in the world.
         *
         *  @return The robots location.
         */
        Position getLocal();

        /** Inherited from Module. */
        std::string toString();

    private:

        /** Disable default constructor. */
        Local();

        /** Disable copy constructor. */
        Local(const Local& source);

        /** Reference to Position2dProxy. */
        PlayerCc::Position2dProxy* proxy;
};
#endif
