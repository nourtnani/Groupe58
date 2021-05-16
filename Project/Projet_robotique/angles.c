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


/**
* @brief   Function used to convert an angle value to one between -180° and 180°.
* 			code taken from //https://stackoverflow.com/questions/11498169/dealing-with-angle-wrap-in-c-code
* 			as is.
*/

int constrainAngle(int x)
{
    x = fmod(x + DEMI_CERCLE,CERCLE);
    if (x < 0)  x += CERCLE;

    return x - DEMI_CERCLE;
}

int angle_to_step(int angle)
{
    return (TOUR_COMPLET/CERCLE)*constrainAngle(angle);
}

int distance_to_step(float distance)
{
    return (STEPS_ROT/WHEEL_PERIMETER)*distance ;
}

void rotate_to_angle(int angle)
{
    right_motor_set_pos(COUNTER_INIT);
    int steps = angle_to_step(angle);
    if (steps<0)						 //turn right
    {
        right_motor_set_speed(-SPEED_WALK);
        left_motor_set_speed(+SPEED_WALK);
        while (right_motor_get_pos()>(COUNTER_INIT+steps)) 	 {}
    }
    else 								//turn left
    {
        right_motor_set_speed(+SPEED_WALK);
        left_motor_set_speed(-SPEED_WALK);
        while (right_motor_get_pos()<(COUNTER_INIT+steps))	 {}
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

void glue_shoulder(void)
{
	while(init_prox()!=SENSOR_IR3) 		//if the sensor giving back the highest value is not the IR3, keep rotating
	{
		rotate_to_angle(A_SENSOR_IR8);
	}
}

void adapt_speed (int16_t speed_correction)
{
		right_motor_set_speed (SPEED_WALK - ROTATION_COEFF * speed_correction);
		left_motor_set_speed (SPEED_WALK + ROTATION_COEFF * speed_correction);

}

void move (float valeur , int16_t speed_correction)
{
	right_motor_set_pos (SPEED_STOP);
	left_motor_set_pos (SPEED_STOP);
	while (right_motor_get_pos()<= valeur) //same logic as rotate, we keep checking
	{
		right_motor_set_speed (SPEED_WALK - ROTATION_COEFF * speed_correction);
		left_motor_set_speed (SPEED_WALK - ROTATION_COEFF * speed_correction);
	}
	right_motor_set_speed (SPEED_STOP);
	left_motor_set_speed (SPEED_STOP);
}



