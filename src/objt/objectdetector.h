/** @file       src/plan/objectdetector.h
    @ingroup    PLAN
    @brief      Detects objects around robot.
    @author     Alex Moriarty <alexander@dal.ca>
    @author     Jacob Perron <perronj@yorku.ca>
*/

#ifndef __PLAN_OBJECTDETECTOR_H_
#define __PLAN_OBJECTDETECTOR_H_

#include "infs/module.h"
#include "data/rangerdata.h"
#include "data/position.h"

/** Detects obstacles.
 *
 *  Given RangerData from a Controller, ObjectDetector will determine if
 *  there are any objects too close to the robot using @ref check .
 *
 *  @note ObjectDetector is planned on being expanded to potentially
 *      calculate and return the current positions of immediate objects
 */
class ObjectDetector : public Module {
    public:

        /** Constructor. */
        ObjectDetector();

        /** Destructor. */
        ~ObjectDetector();

        /** Checks for objects around robot.
         *
         *  Uses RangerData to determine if an object is too close to
         *  the robot based on the set threshold.
         *
         *  @return True if it found an object considered too close to the
         *      robot, and false otherwise.
         */
        bool check();

        /** Sets the most recent ranger readings from the robot.
         *
         *  @param data : A RangerData struct containing ranger readings.
         */
        void setRangerData(RangerData data);

        /** Sets distance threshold.
         *
         *  Sets the min distance the robot can be to an obstacle before
         *  it is possible for control to be passed to ObjectAvoider.
         *
         *  @param threshold : The min distance threshold.
         */
        void setThreshold(double threshold);

         /** Returns a string representation of this ObjectDetector. */
         std::string toString();

    private:

        /** Threshold distance for an object to be considered too close. */
        double threshold;

        /** Copy of most recent ranger data from the robot. */
        RangerData data;

        /** Disable copy constructor. */
        ObjectDetector(const ObjectDetector& source);

        /** Disable assignment operator. */
        ObjectDetector& operator=(const ObjectDetector& source);
};
#endif
