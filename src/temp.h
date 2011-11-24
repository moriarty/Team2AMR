/** @file       src/GROUP/FILENAME.h
    @ingroup    GROUP
    @brief      Discription.
    @author     Alex Moriarty <alexander@dal.ca>
    @author     Jacob Perron <perronj@yorku.ca>
*/

#ifndef __GROUP_FILENAME_H_
#define __GROUP_FILENAME_H_

#include "infs/module.h"

class CLASSNAME : public Module {
    public:

        /** Constructor. */
        CLASSNAME();

        /** Destructor. */
        ~CLASSNAME();

    private:

        /** Disable copy constructor. */
        CLASSNAME(const CLASSNAME& source);

        /** Disable assignment operator. */
        CLASSNAME& operator=(const CLASSNAME& source);
};
#endif
