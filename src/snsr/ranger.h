/** @file       src/snsr/ranger.h
    @ingroup    SNSR
    @brief      Player/Stage ranger proxy.
    @author     Alex Moriarty <alexander@dal.ca>
    @author     Jacob Perron <perronj@yorku.ca>
*/

#ifndef __ACTR_RANGER_H_
#define __ACTR_RANGER_H_

#include <libplayerc++/playerc++.h>
#include "infs/module.h"
#include "data/rangerdata.h"

/** The base class for all ranger modules.
 *
 *  On the call @ref getData the ranger will take its current readings and
 *  package them in a RangerData struct, which is then dealt with by higher-
 *  level classes.
 *
 *  This class completes the interaction between the robot and the
 *  Player/Stage RangerProxy.
 */
class Ranger : public Module {
    public:

        /** Constructor. */
        Ranger(PlayerCc::RangerProxy& rangerProxy);

        /** Destructor  */
        ~Ranger();

        /** Function used to retreive data from the ranger.
         *
         *  The type of data returned is dependent on the type of ranger.
         *  Therefore a RangerData struct is used so that passing data from
         *  one class to another is easily accomplished.
         *
         *  @return Information from the ranger.
         */
        RangerData getData();

        /** Inherited from Module   */
        std::string toString();

    protected:

        /** RangerProxy */
        PlayerCc::RangerProxy* rangerProxy;

    private:

        /** Hide default Contructor  */
        Ranger();

        /** Disable copy constructor. */
        Ranger(const Ranger& source);

        /** Disable assignment operator. */
        Ranger& operator=(const Ranger& source);
};
#endif
