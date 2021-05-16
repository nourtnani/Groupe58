/*
 * angles.h
 *
 *  Created on: 29 avr. 2021
 *      Author: nourtnani & zakaryasouid
 *
 */

/*
 * The goal of this module is to control the geometric properties of the program:
 * It allows the rotation of the motor
 * It allows the motor to move forward
 */

#ifndef ANGLES_H_
#define ANGLES_H_

/**
* @brief   convert an angle to a number of step the robot should do to rotate
*/
int angle_to_step(int angle);

/**
* @brief  	Function used to convert a distance into a number of steps
*/
int distance_to_step(float distance);

/**
* @brief   Function used to make the robot rotate of a given angle
* 			The function uses angle_to_step to get a number of step and enter a
* 			while loop, stopping when the the step counter reaches the value corresponding
* 			to the desired angle
*/
void rotate_to_angle(int angle);

/**
* @brief   Function used to tell the robot to put his head where a given sensor previously was
* 			Uses rotate_to_angle and the different angles where each sensor is located.
*/
void rotate_to_sensor(int sensor);


/**
 * @brief Function controlling the motors to turn the robot so IR3 is the closest to an object
 * 			uses the rotate to angle functions and check every 20° if IR3 is returning the highest value
 * 			keep rotating until it is the case
 */
void glue_shoulder(void);


/**
* @brief   Function dealing with the speed of the robot and compensating its direction
* 			The function has a few different modes, allowing to take different decisions
* 			and applying the correction factor depending on what the front and side sensors are feeling.
* 			Speed_correction allows angular correction so the robot can follow a wall.
*/
void adapt_speed (int16_t speed_correction);

/**
* @brief   Function used to tell the robot to go straight for a specified number of step
* 			Speed_correction allows angular correction so the robot can follow a wall.
*/
void move (float valeur , int16_t speed_correction);


#endif /* ANGLES_H_ */
