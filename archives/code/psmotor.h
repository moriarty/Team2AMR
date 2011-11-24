/** @file       src/actr/psmotor.h
    @ingroup    ACTR
    @brief      Motor for Player/Stage.
    @author     Alex Moriarty <alexander@dal.ca>
    @author     Jacob Perron <perronj@yorku.ca>
*/

#ifndef __ACTR_PSMOTOR_H_
#define __ACTR_PSMOTOR_H_

#include "motor.h"
#include "data/position.h"
#include <libplayerc++/playerc++.h>

class PSMotor : public Motor {
    public:

        /** Constructor.
         *
         *  Requires a reference to a Player/Stage Position2dProxy that should
         *  already be connected with a PlayerClient.
         *
         *  @param proxy : Position2dProxy from Player/Stage.
         */
        PSMotor(PlayerCc::Position2dProxy& proxy);

        /** Destructor. */
        ~PSMotor();
        /** Goes to a point in the global frame of the world.
         *
         *  Note it is up to the controller to keep the robot from crashing
         *  into obstacles.
         *
         *  @param  pos: The Position the robot should drive towards.
         */
        virtual void goTo(Position pos);

        virtual void update();

        virtual std::string toString();

    private:

        /** Disable default constructor    */
        PSMotor();

        /** Disable copy constructor. */
        PSMotor(const PSMotor& source);

        /** Disable assignment operator. */
        PSMotor& operator=(const PSMotor& source);

    protected:

        /** Reference to Position2dProxy    */
        PlayerCc::Position2dProxy* positionProxy;
};
#endif
