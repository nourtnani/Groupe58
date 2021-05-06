/*
 * walls.h
 *
 *  Created on: 22 avr. 2021
 *      Author: nour tnani & zakarya souid
 */

#ifndef WALLS_H_
#define WALLS_H_


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

bool double_turn (void);


void labyrinth_start(void);

void move_right_start(void);

void verif (void);

#endif /* WALLS_H_ */
