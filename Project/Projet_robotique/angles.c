#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>
#include <chprintf.h>

#include <walls.h>
#include <angles.h>
#include <main.h>
#include <sensors/proximity.h>
#include <leds.h>
#include <msgbus/messagebus.h>
#include <motors.h>




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
            chprintf((BaseSequentialStream *)&SD3, "compteur moteur: %d\n", right_motor_get_pos());

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
            chprintf((BaseSequentialStream *)&SD3, "compteur moteur: %d\n", right_motor_get_pos());
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
        	set_led (LED1,1);
        	rotate_to_angle(A_SENSOR_IR1);
            break;
        case SENSOR_IR2:
        	set_rgb_led (LED2,100,0,0);
            rotate_to_angle(A_SENSOR_IR2);
            break;
        case SENSOR_IR3:
        	set_led (LED3,1);
            rotate_to_angle(A_SENSOR_IR3);
            break;
        case SENSOR_IR4:
        	set_rgb_led (LED4,100,0,0);
            rotate_to_angle(A_SENSOR_IR4);
            break;
        case SENSOR_IR5:
        	set_led (LED5,1);
            rotate_to_angle(A_SENSOR_IR5);
            break;
        case SENSOR_IR6:
        	set_rgb_led (LED6,100,0,0);
            rotate_to_angle(A_SENSOR_IR6);
            break;
        case SENSOR_IR7:
        	set_led (LED7,1);
            rotate_to_angle(A_SENSOR_IR7);
            break;
        case SENSOR_IR8:
        	set_rgb_led (LED8,100,0,0);
            rotate_to_angle(A_SENSOR_IR8);
            break;
        default:
            break;
    }
}

void adapt_speed (int cas)
{
	switch (cas)
	{
	case KEEP_STRAIGHT :
		right_motor_set_speed (SPEED_WALK);
		left_motor_set_speed (SPEED_WALK);
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

void avance_valeur (int valeur)
{
	right_motor_set_pos (SPEED_STOP);
	left_motor_set_pos (SPEED_STOP);
	while (right_motor_get_pos()<= valeur)
	{
		right_motor_set_speed (SPEED_WALK);
		left_motor_set_speed (SPEED_WALK);
	}
}


void go(float distance_in_cm)
{
	int right_motor_counter=right_motor_get_pos();
    int steps = distance_to_step(distance_in_cm);

        //        turn left
        // 1 rot = 1000 steps --> tour complet = 4000 steps
        // pour faire un tour complet il faut 4000 step sur un droite et -4000 sur gauche
        right_motor_set_speed(+100);
        left_motor_set_speed(+100);
        while (right_motor_get_pos()<(right_motor_counter+steps))
        {
            chThdSleep(10);
            chprintf((BaseSequentialStream *)&SD3, "compteur moteur: %d\n", right_motor_get_pos());
        }
        right_motor_set_speed(0);
        left_motor_set_speed(0);

}


int check_shoulder(){
	float treshold_1 = 0.5; // à determiner experimentalement
	float noise_1;
	float treshold_2 = 2;
	float noise_2;
	float treshold_3 = 0.2;
	float noise_3;

	if (get_prox(1) > treshold_1){
		// turn a bit left

	}
	if (get_prox(1)< treshold_1){
		// turn a bit right
		if (get_prox(2)>treshold_2){
			return get_prox(2)-treshold_2;
		}
		if (get_prox(1)<noise_1 && get_prox(2)<noise_2){
			// routine 90° left
		} //si aucun capteur return 0
	}
	return (get_prox(1)-treshold_1);
			// * une constante pour obtenir un angle
}
