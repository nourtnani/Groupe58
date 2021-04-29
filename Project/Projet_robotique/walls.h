/*
 * walls.h
 *
 *  Created on: 22 avr. 2021
 *      Author: nour tnani & zakarya souid
 */

#ifndef WALLS_H_
#define WALLS_H_

#define SENSOR_IR1  		0
#define SENSOR_IR2  		1
#define SENSOR_IR3  		2
#define SENSOR_IR4  		3
#define SENSOR_IR5  		4
#define SENSOR_IR6  		5
#define SENSOR_IR7  		6
#define SENSOR_IR8 			7 // cest lequel ?

#define A_SENSOR_IR1 -20
#define A_SENSOR_IR2 -50
#define A_SENSOR_IR3 -90
#define A_SENSOR_IR4 -150
#define A_SENSOR_IR5 150
#define A_SENSOR_IR6 90
#define A_SENSOR_IR7 50
#define A_SENSOR_IR8 20

#define MOTEUR_DROITE 		0
#define MOTEUR_GAUCHE 		1

#define LIM_OBSTACLE 		200
#define LIM_OBSTACLE_AD		LIM_OBSTACLE/2
#define SPEED_WALK 			180
#define SPEED_ROTATE 		150
#define SPEED_STOP 			0
#define ROTATION_COEFF		2
#define RAYON_EPUCK			35
#define MM_PER_STEP			0.13
#define TIME_LIM			RAYON_EPUCK/(MM_PER_STEP * SPEED_WALK)



uint8_t init_prox(void);
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

bool rotate_left (void);

int16_t *adapt_speed (void);

void labyrinth_start(void);

void move_right_start(void);

#endif /* WALLS_H_ */
