/*
 * path_check.h
 *
 *  Created on: 22 avr. 2021
 *      Author: nour tnani & zakarya souid
 */


/*
 * the goal of this module is to check the conditions of proximity of the robot
 * if the robot has a wall on its right, it should keep following it
 * if it finds a wall in front of it, it turns left
 * if it arrives at the edge of a wall, it should turn right
 * if it finds nothing after turning right, it should turn right again
 * it should also always be in a certain distance from the wall. the range of this proximity is controlled.
 */

#ifndef PATH_CHECK_H_
#define PATH_CHECK_H_

 /**
 * @brief   This function compares the values measured by all the sensors
 * 			and returns the maximal value.
 * 			NOTE : the closer the obstacle is, the larger is the value transmitted by the captors.
 */
int init_prox(void);

 /**
 * @brief   Function useful for the edges.
 * 			If it returns true, the robot should turn to the left.
 * 			It compares the values measured by the TOF sensor to a goal value.
 */
bool to_the_left (proximity_msg_t *proximity_values);

/**
* @brief   This function returns a correction coefficient for the speed of the motors.
* 			It keeps the robot close enough to the walls, by comparing the values measured
* 			by IR2 and IR3 to threshold and noise values.
* 			It allows the robot to always follow the obstacles on its right, and therefore
* 			turning right when it should.
*/
int check_shoulder(proximity_msg_t *proximity_values) ;

/**
* @brief   Starts the labyrinth check unit
* 			The robots turns to face the closest obstacle,
* 			moves forward to go to it,
* 			then turns such as it becomes on its right.
*/
void labyrinth_start(void);

/**
* @brief   Starts the resolution of the labyrinth.
*/
void move_right_start(void);


#endif /* PATH_CHECK_H_ */
