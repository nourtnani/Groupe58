/*
 * path_check.h
 *
 *  Created on: 22 avr. 2021
 *      Author: nour tnani & zakarya souid
 */


/*
 * the goal of this module is to check the conditions of proximity of the robot
 * if the robot has a wall on its right, it should jeep following it
 * if it finds a wall in front of it, it turns left
 * if it arrives at the edge of a wall, it should turn right
 * if it finds nothing after turning right, it should turn right again
 * it should also always be in a certain distance from the wall. the range of this proximity is controlled.
 */

#ifndef PATH_CHECK_H_
#define PATH_CHECK_H_


uint8_t init_prox(void);
 /**
 * @brief   Function useful for the edges.
 * 			If it returns true, the robot should turn to the left
 */
bool to_the_left (void);


/**
* @brief   The robot always follows the wall on its right.
*/
bool stay_on_your_right (void);

bool see_nothing (void) ;

int check_shoulder(void) ;

void verif (void);

void labyrinth_start(void);

void move_right_start(void);

#endif /* PATH_CHECK_H_ */
