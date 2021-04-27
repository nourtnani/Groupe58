/*
 * walls.h
 *
 *  Created on: 22 avr. 2021
 *      Author: nour tnani & zakarya souid
 */

#ifndef WALLS_H_
#define WALLS_H_

#define SENSOR_IR1  	0
#define SENSOR_IR2  	1
#define SENSOR_IR3  	2
#define SENSOR_IR4  	3
#define SENSOR_IR5  	4
#define SENSOR_IR6  	5
#define SENSOR_IR7  	6

#define MOTEUR_DROITE 	0
#define MOTEUR_GAUCHE 	1

#define LIM_OBSTACLE 	200
#define SPEED_WALK 		180
#define SPEED_ROTATE 	150
#define SPEED_STOP 		0
#define ROTATION_COEFF	2



 /**
 * @brief   Function useful for the edges.
 * 			If it returns true, the robot should turn to the left
 */
bool to_the_left (void);

/**
 * @brief   Function useful for the edges.
 * 			If it returns true, the robot should turn to the right
*/
bool to_the_right (void);

/**
* @brief   The robot always follows the wall on its right.
*/
bool stay_on_your_right (void);

void labyrinth_start(void);

void move_right_start(void);

#endif /* WALLS_H_ */
