#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "msgbus/messagebus.h"
#include "sensors/proximity.h"
#include "parameter/parameter.h"


//constants for the differents parts of the project
#define SENSOR_IR1  			0
#define SENSOR_IR2  			1
#define SENSOR_IR3  			2
#define SENSOR_IR4  			3
#define SENSOR_IR5  			4
#define SENSOR_IR6  			5
#define SENSOR_IR7  			6
#define SENSOR_IR8 				7

#define A_SENSOR_IR1			-20
#define A_SENSOR_IR2 			-50
#define A_SENSOR_IR3 			-90
#define A_SENSOR_IR4 			-150
#define A_SENSOR_IR5		 	150
#define A_SENSOR_IR6	 	 	90
#define A_SENSOR_IR7		 	50
#define A_SENSOR_IR8		 	20
#define A_TURN_RIGHT			-90
#define A_TURN_LEFT 			90

#define KEEP_STRAIGHT			0
#define TURN_LEFT				1
#define TURN_RIGHT				2
#define STOP					3

#define LIM_OBSTACLE 			300
#define LIM_OBSTACLE_FACE		1600
#define NOISE 					100
#define NOISE_IR3				300

#define SPEED_WALK 				200
#define SPEED_STOP 				0
#define ROTATION_COEFF			2

#define DEMI_CERCLE				180
#define CERCLE 					360
#define AVANCE_POUR_TOURNER 	150
#define TOUR_COMPLET			1300.

/** Robot wide IPC bus. */
extern messagebus_t bus;

extern parameter_namespace_t parameter_root;


void SendUint8ToComputer(uint8_t* data, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif
