/*
 * angles.h
 *
 *  Created on: 29 avr. 2021
 *      Author: nourtnani & zakariasouid
 *
 */

#ifndef ANGLES_H_
#define ANGLES_H_

#define A_SENSOR_IR1	-20
#define A_SENSOR_IR2 	-50
#define A_SENSOR_IR3 	-90
#define A_SENSOR_IR4 	-150
#define A_SENSOR_IR5	 150
#define A_SENSOR_IR6	 90
#define A_SENSOR_IR7	 50
#define A_SENSOR_IR8	 20


double constrainAngle(double x) ;

int angle_to_step(int angle);

void rotate_to_angle(int angle);

void rotate_to_sensor(int sensor);

#endif /* ANGLES_H_ */
