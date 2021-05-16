/*
 * path_exit.h
 *
 *  Created on: 13 mai 2021
 *      Author: nourtnani & zakaryasouid
 */

/*
 * The goal of this module is to start singing "we are the champions by queen at the edge of the labyrinth
 */
#ifndef PATH_EXIT_H_
#define PATH_EXIT_H_

 /**
 * @brief   Starts the exiting unit
 * 			Compares the values measured by the IMU unit, y_acceleration and x_gyroscope,
 * 			with threshold values determined experimentally.
 * 			When the condition is verified, it launches the buffer, and the robot starts singing
 */
void fall_monitoring_start(void) ;



#endif /* PATH_EXIT_H_ */
