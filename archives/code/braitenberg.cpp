/** @file	src/control/braitenberg.cpp
    @ingroup	control
    @brief	Motion controller
    @author	Jacob Perron
    @author	Alexander Moriarty
*/

#include "braitenberg.h"

CREATE_LOGGER("Braitenberg");

Braitenberg::Braitenberg( PlayerCc::Position2dProxy& pos, 
			  PlayerCc::RangerProxy& ranger
			) : positionProxy(pos), rangerProxy(ranger) {

    configuration = Braitenberg::A;
    behaviour = Braitenberg::INHIBIT;
    MAKE_LOG << "Braitenberg robot created." << std::endl;	
}		

Braitenberg::~Braitenberg() { 
    MAKE_LOG << "Braitenberg robot stopped." << std::endl;
    positionProxy.SetSpeed(0, 0);
}

void Braitenberg::setConfig(int config) {
    configuration = config;
    MAKE_LOG << "config = " << config << std::endl;
}

void Braitenberg::setBehaviour(int behave) {
    behaviour = behave;
    MAKE_LOG << "behaviour = " << behave << std::endl;
}

void Braitenberg::run() {
    double sensor1, sensor2; ///< closest reading for each half of sensor
    double speed1, speed2; ///< speeds for each wheel
   // int numSPvalues, minAngle, maxAngle, i;
    
    //get readings from front of robot 
    
    //numSPvalues = rangerProxy.GetRangeCount();
    //minAngle = (numSPvalues*3)/8;
    //maxAngle = (numSPvalues*5)/8;
    
   // MAKE_LOG << "Range count = " << numSPvalues << std::endl << std::endl;
    
    sensor1 = rangerProxy.GetRange(0);
    //get readings from first half of sensor
    //for (i = minAngle; i < (maxAngle / 2); i++) {
	//double range = rangerProxy.GetRange(i);
	//if (range < sensor1) {
	  //  sensor1 = range;
	//}
    //}
    
    sensor2 = rangerProxy.GetRange(1);
    //get readings from second half of sensor
    //for (i = (maxAngle / 2); i < maxAngle; i++) {
	//double range = rangerProxy.GetRange(i);
	//if (range < sensor2) {
	 //   sensor2 = range;
	//}
    //}
    
    MAKE_LOG << "Sensor1 = " << sensor1 << " | Sensor2 = " << sensor2 << std::endl;
    speed1 = distToSpeed(sensor1);
    speed2 = distToSpeed(sensor2);
    //Determine movement based on vehicle configuration
    if (configuration == Braitenberg::A) {
	move(speed1, speed2);
    } 
    else if (configuration == Braitenberg::B) {
	move(speed2, speed1);
    }
    else if (configuration == Braitenberg::C) {
	/// helps normalize the speed
	double const MULTIPLIER = 0.5; 
	double avg = (speed1 + speed2) / 2;
	move(speed1*MULTIPLIER + avg, speed2*MULTIPLIER + avg);
    }
    else {
	MAKE_LOG << "Problem realizing vehicle configuration." << std::endl;
	std::exit(1);
    }
}

double Braitenberg::distToSpeed(double distance) {
    double speed;
    double m = 0.5;
    speed = pow(distance*m, behaviour);
    return speed;    
}

void Braitenberg::move(double speed1, double speed2) {
    double const WIDTH = 0.5; ///< assumed distance between two wheels
    double const TURNRATE = 30 * M_PI / 180;
    double translation, 
	   rotation, 
	   diff;  ///< difference between wheel speeds
	   
    diff = speed1 - speed2;
    
    //calculate translation and rotation
    translation = (speed1 + speed2) / 2;
    rotation = atan2(diff, WIDTH) * TURNRATE;
    
    //move robot    
    positionProxy.SetSpeed(translation, rotation);
}

