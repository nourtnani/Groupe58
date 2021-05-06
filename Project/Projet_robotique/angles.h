/*
 * angles.h
 *
 *  Created on: 29 avr. 2021
 *      Author: nourtnani & zakariasouid
 *
 */

#ifndef ANGLES_H_
#define ANGLES_H_


double constrainAngle(double x) ;

double angle_to_step(int angle);

void rotate_to_angle(int angle);

void rotate_to_sensor(int sensor);

void adapt_speed (int cas);

void avance_valeur (int valeur);

#endif /* ANGLES_H_ */
