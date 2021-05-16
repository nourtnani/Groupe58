#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>
#include <chprintf.h>

#include <angles.h>
#include <main.h>
#include <sensors/proximity.h>
#include <leds.h>
#include <msgbus/messagebus.h>
#include <motors.h>
#include "path_check.h"




//https://stackoverflow.com/questions/11498169/dealing-with-angle-wrap-in-c-code
double constrainAngle(double x)
{
    x = fmod(x + DEMI_CERCLE,CERCLE);
    if (x < 0)  x += CERCLE;

    return x - DEMI_CERCLE;
}

double angle_to_step(int angle)
{
    return (TOUR_COMPLET/CERCLE)*constrainAngle(angle);
}

int distance_to_step(float distance)
{
    return (10/257.6)*distance ;
}

void rotate_to_angle(int angle)
{
    right_motor_set_pos(5000);
    int steps = angle_to_step(angle);
    if (steps<0)
    {
    	//   turn right
        //   53mm --> 166,5 de perimetre --> 4 tours complets de roue
    	//	41 mm --> 128 de perim
    	// 1 rot = 1000 steps --> tour complet = 1290 steps
    	// pour faire un tour complet il faut 1290 step sur un gauche et -1290 sur droite
        right_motor_set_speed(-SPEED_WALK);
        left_motor_set_speed(+SPEED_WALK);
        while (right_motor_get_pos()>(5000+steps))
        {
            chThdSleep(10);
          //  chprintf((BaseSequentialStream *)&SD3, "compteur moteur: %d\n", right_motor_get_pos());

        }
        right_motor_set_speed(SPEED_STOP);
        left_motor_set_speed(SPEED_STOP);
    }
    else
    {
        //        turn left
        // 1 rot = 1000 steps --> tour complet = 1290 steps
        // pour faire un tour complet il faut 1290 step sur un droite et -1290 sur gauche
        right_motor_set_speed(+SPEED_WALK);
        left_motor_set_speed(-SPEED_WALK);
        while (right_motor_get_pos()<(5000+steps))
        {
            chThdSleep(10);
          //  chprintf((BaseSequentialStream *)&SD3, "compteur moteur: %d\n", right_motor_get_pos());
        }
        right_motor_set_speed(SPEED_STOP);
        left_motor_set_speed(SPEED_STOP);
    }
}

void rotate_to_sensor(int sensor)
{
    switch (sensor)
    {
        case SENSOR_IR1:
        	rotate_to_angle(A_SENSOR_IR1);
            break;
        case SENSOR_IR2:
            rotate_to_angle(A_SENSOR_IR2);
            break;
        case SENSOR_IR3:
            rotate_to_angle(A_SENSOR_IR3);
            break;
        case SENSOR_IR4:
            rotate_to_angle(A_SENSOR_IR4);
            break;
        case SENSOR_IR5:
            rotate_to_angle(A_SENSOR_IR5);
            break;
        case SENSOR_IR6:
            rotate_to_angle(A_SENSOR_IR6);
            break;
        case SENSOR_IR7:
            rotate_to_angle(A_SENSOR_IR7);
            break;
        case SENSOR_IR8:
            rotate_to_angle(A_SENSOR_IR8);
            break;
        default:
            break;
    }
}

void adapt_speed (int cas , int16_t speed_correction)
{
	switch (cas)
	{
	case KEEP_STRAIGHT :
		right_motor_set_speed (SPEED_WALK - ROTATION_COEFF * speed_correction);
		left_motor_set_speed (SPEED_WALK + ROTATION_COEFF * speed_correction);
		break;

	case TURN_LEFT :
		rotate_to_angle (A_TURN_LEFT);
		break;

	case TURN_RIGHT :
		rotate_to_angle (A_TURN_RIGHT);
		break;

	case STOP :
		right_motor_set_speed (SPEED_STOP);
		left_motor_set_speed (SPEED_STOP);
		break;
	}

}

void avance_valeur (int valeur , int16_t speed_correction)
{
	right_motor_set_pos (SPEED_STOP);
	left_motor_set_pos (SPEED_STOP);
	while (right_motor_get_pos()<= valeur)
	{
		right_motor_set_speed (SPEED_WALK - ROTATION_COEFF * speed_correction);
		left_motor_set_speed (SPEED_WALK - ROTATION_COEFF * speed_correction);
	}
	right_motor_set_speed (SPEED_STOP);
	left_motor_set_speed (SPEED_STOP);
}



