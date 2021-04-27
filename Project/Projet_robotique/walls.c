#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>
#include <chprintf.h>

#include <walls.h>
#include <sensors/proximity.h>
#include <leds.h>
#include <msgbus/messagebus.h>


proximity_msg_t prox_values;

bool to_the_left (void)
{
	return ((prox_values.delta[SENSOR_IR3] > LIM_OBSTACLE ) & (prox_values.delta[SENSOR_IR1] > LIM_OBSTACLE ) & (prox_values.delta[SENSOR_IR2] > LIM_OBSTACLE ) ) ;
}

bool to_the_right (void)
{
	return ((prox_values.delta[SENSOR_IR3] < LIM_OBSTACLE ) & (prox_values.delta[SENSOR_IR4] > LIM_OBSTACLE ) ) ;
}

bool stay_on_your_right (void)
{
	return (prox_values.delta[SENSOR_IR3] > LIM_OBSTACLE ) ;
}

void labyrinth_start(void)
{

}

uint16_t * adapt_speed ()
{
	uint16_t speed_correction [2];

	return 0;
}

static THD_WORKING_AREA(waMoveRight, 256);
static THD_FUNCTION(MoveRight, arg)
{
    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    systime_t time;

    int16_t speed = 0;
    int16_t speed_correction = 0;

    while(1)
    {
        time = chVTGetSystemTime();

        //computes the speed to give to the motors
        //speed is modified given the state of the motor (walk, rotate, stop)

        // speed = pi_regulator(get_distance_cm(), GOAL_DISTANCE);

        /*
         * if the line is nearly in front of an edge, slow down to turn
         * adapt : to turn to the left, follow the right wall until a certain distance from the front wall, then turn,
         * 			to turn to the right, once it stops detecting the right wall, it moves forward till the back sensor senses the right wall, then rotates.

        if(abs(speed_correction) < ROTATION_THRESHOLD)
        {
        	speed_correction = 0;
        }
		*/
        //applies the speed
      	right_motor_set_speed(speed + ROTATION_COEFF * speed_correction);
      	left_motor_set_speed(speed + ROTATION_COEFF * speed_correction);

        //100Hz
        chThdSleepUntilWindowed(time, time + MS2ST(10));
    }
}


void move_right_start(void)
{
	chThdCreateStatic(waMoveRight, sizeof(waMoveRight), NORMALPRIO, MoveRight, NULL);
}



