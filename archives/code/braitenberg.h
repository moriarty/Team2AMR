/** @file		src/control/braitenberg.h
    @ingroup	control
    @brief		Motion controller
    @author		Jacob Perron
    @author		Alexander Moriarty
*/

#ifndef __CONTROL_BRAITENBERG_H
#define __CONTROL_BRAITENBERG_H

#include <libplayerc++/playerc++.h>
#include "logger.h"

/** Controls a Braitenberg robot.
 *
 *  Based on the principle of direct mapping between two sensors and
 *  two motors. Implements three different sensor to motor
 *  configurations, selected by the client: Sensors to adjacent motor,
 *  sensors to opposite motor, and sensors to both motors. The client is
 *  also able to choose between two different behaviours of the robot,
 *  'inhibiting' or 'exciting'.
 */
class Braitenberg {
	public:

        enum BraitenConfig {
            A,
            B,
            C,
        };

        enum BraitenBehave {
            INHIBIT,
            EXCITE,
        };

		/** Constructor.
		 *
		 *  Sets default configuration to @c Braitenberg::A and default
		 *  behaviour to @c Braitenberg::INHIBIT .
		 *
		 *  @param pos : The position proxy used to set the movement of
		 * 		robot.
		 *
		 *  @param ranger : The sensor used to obtain readings for when
		 * 		the robot is activated.
		 */
		Braitenberg(PlayerCc::Position2dProxy& positionProxy,
					PlayerCc::RangerProxy& rangerProxy
				   );

		/** Deconstructor	*/
		~Braitenberg();

		/** Sets wiring configuration of robot.
		 *
		 *  @param config : Determines wiring configuration of robot.
		 * 		Either Braitenberg::A, Braitenberg::B, or Braitenberg::C.
		 */
		void setConfig(BraitenConfig config);

		/** Set the behaviour of the robot.
		 *
		 *  @param behaviour : Robot can either be set to act inhibiting,
		 *  	motor speed reduced with increase of sensor signal, or
		 * 		exciting, motor speed increased. Set to
		 *  	Braitenberg::INHIBIT or Braitenberg::EXCITE.
		 */
		void setBehaviour(BraitenBehave behaviour);

		/** Performs the necessary steps to make the robot work.
		 *
		 *  Reads sensor data, calculates motor speeds according to vehicle
		 *  configuration, and passes the speeds to the function @c move().
		 *
		 */
		void run();

	private:

		PlayerCc::Position2dProxy& positionProxy;
		PlayerCc::RangerProxy& rangerProxy;
		int configuration;
		int behaviour;

		/**Hide default constructor.	*/
		Braitenberg();

		/**Function used to convert reading distance into a speed.
		 *
		 * @param distance : The distance from an obstacle.
		 *
		 * @return Speed.
		 */
		double distToSpeed(double distance);

		/** Function used to move the robot.
		 *
		 * 	Given two speeds, using a predetermined equation, calculates
		 *  a rotation and distance and moves the robot accordingly.
		 *
		 *  @param speed1 : The speed of the first wheel.
		 *
		 *  @param speed2 : The speed of the second wheel.
		 */
		void move(double speed1, double speed2);
};
#endif
