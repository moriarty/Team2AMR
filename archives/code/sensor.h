/** @file       src/infs/sensor.h
    @ingroup    INFS
    @brief      Sensor interface.
    @author     Jacob Perron <perronj@yorku.ca>
    @author     Alexander Moriarty <alexander@dal.ca>
*/

#ifndef __INFS_SENSOR_H_
#define __INFS_SENSOR_H_

#include "module.h"
#include "data/sensordata.h"

/** The base class for all sensor modules.
 *
 *  On the call @ref getData the sensor will take its current readings and
 *  package them in a SensorData struct, which is then dealt with by higher-
 *  level classes.
 *
 *  All sensor modules of a robot should implement this interface.
 */
class Sensor {
    public:

        /** Function used to retreive data from the sensor.
         *
         *  The type of data returned is dependent on the type of sensor.
         *  Therefore a SensorData struct is used so that passing data from
         *  one class to another is easily accomplished.
         *
         *  @return Information from the sensor.
         */
        virtual SensorData getData() = 0;

        /** Inherited from Module   */
        virtual std::string toString() = 0;
};
#endif
