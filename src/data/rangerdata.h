/** @file       src/data/rangerdata.h
    @ingroup    Data
    @brief      Ranger Data.
    @author     Alex Moriarty <alexander@dal.ca>
    @author     Jacob Perron <perronj@yorku.ca>
*/

#ifndef __DATA_RANGERDATA_H_
#define __DATA_RANGERDATA_H_

#include "position.h"

/** Holds data from a ranger.
 *
 *  Ideally this struct will hold any type of data a sensor could return
 *  using something like a template class. It is then up to the class
 *  using RangerData to know what type of data is contained inside.
 *
 *  For now RangerData is set up for data from a RangerProxy in Player/Stage.
 */
struct RangerData {

    /** Range readings. */
    std::vector<double> range;

    /** Angle of individual ranger relative to robot. */
    std::vector<Position> pos;

    /** Other RangerProxy data. */
    double angleRes, minAngle, maxAngle, minRange, maxRange;

    /** Constructor */
    RangerData(std::vector<double> r, std::vector<Position> p) : range(r), pos(p) { };

    RangerData() { };

};
#endif
