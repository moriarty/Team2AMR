/** @file       src/ctrl/Braitenberg.h
    @ingroup    CTRL
    @brief      Braitenberg robot controller.
    @author     Alex Moriarty <alexander@dal.ca>
    @author     Jacob Perron <perronj@yorku.ca>
*/

#ifndef __CTRL_BRAITENBERG_H_
#define __CTRL_BRAITENBERG_H_

#include "controller.h"

enum braitenConfig {
    A,
    B,
    C
};

enum braitenBehave {
    INHIBIT,
    EXCITE
};

class Braitenberg : public Controller {
    public:

        /** Constructor.
         *
         *  Calls Controller constructor.
         *
         *  @param m : Reference to robot motor.
         */
        Braitenberg(Motor& m);

        /** Destructor. */
        ~Braitenberg();

        /** Updates the controller actions.
         *
         *  Controller moves robot like according to the braitenberg robot
         *  configuration and behaviour.
         */
        virtual void update();

        /** Sets wiring configuration of robot.
		 *
		 *  @param config : Determines wiring configuration of robot.
		 * 		Either Braitenberg::A, Braitenberg::B, or Braitenberg::C.
		 */
		void setConfig(braitenConfig config);

		/** Set the behaviour of the robot.
		 *
		 *  @param behaviour : Robot can either be set to act inhibiting,
		 *  	motor speed reduced with increase of ranger signal, or
		 * 		exciting, motor speed increased. Set to
		 *  	Braitenberg::INHIBIT or Braitenberg::EXCITE.
		 */
		void setBehaviour(braitenBehave behaviour);

        /** Sets the latest data from the rangers to anaylze.
         *
         *  @param data : The latest RangerData.
         */
        void setRangerData(RangerData data);

        /** Executes a Command from the Console.
         *
         *  Pays attention to the "behave" and "mode" commands.
         *
         *  @param cmd : Command from the Console.
         */
        void executeCommand(const Command cmd);

        /** String representation of this Controller. */
        virtual std::string toString();

    protected:

        /** Braitenberg Configuration */
        braitenConfig config;

        /** Braitenberg Behaviour */
        braitenBehave behaviour;

        /** Latest ranger data. */
        RangerData data;

        /** Reference to robot motor. */
        Motor* motor;

    private:

        /** Disable copy constructor. */
        Braitenberg(const Braitenberg& source);

        /** Disable assignment operator. */
        Braitenberg& operator=(const Braitenberg& source);

        /** Disable default constructor. */
        Braitenberg();
};
#endif
