#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>
#include <chprintf.h>
#include <stdbool.h>

#include <msgbus/messagebus.h>
#include <sensors/proximity.h>
#include <sensors/VL53L0X/VL53L0X.h>
#include <main.h>
#include <angles.h>

#include <leds.h>

#include <i2c_bus.h>
#include <motors.h>
#include "path_check.h"


void labyrinth_start(void)
{
	int number = init_prox();
    rotate_to_sensor(number);
	int distanceTOF=VL53L0X_get_dist_mm();
	if (distanceTOF<DISTANCE_TOF_MAX)
    {
        move(distance_to_step((distanceTOF)/MM_TO_CM),SPEED_STOP);
	}
    glue_shoulder();
    set_front_led(true);
    VL53L0X_stop();
}



static THD_WORKING_AREA(waMoveRight, 256);
static THD_FUNCTION(MoveRight, arg)
{
    chRegSetThreadName(__FUNCTION__);
    (void)arg;
    systime_t time;
    messagebus_topic_t *proximity_topic = messagebus_find_topic_blocking(&bus, "/proximity");
    proximity_msg_t proximity_values;


    while(1)
    {
        time = chVTGetSystemTime();

    	messagebus_topic_wait(proximity_topic, &proximity_values, sizeof(proximity_values));

    		if (to_the_left()==true)
    		{
    			glue_shoulder();
    		}
    		adapt_speed(check_shoulder());
        chThdSleepUntilWindowed(time, time + MS2ST(10));
    }
}

void move_right_start(void)
{
	chThdCreateStatic(waMoveRight, sizeof(waMoveRight), NORMALPRIO, MoveRight, NULL);
}

int init_prox(void)
{

    uint8_t number = 0;


    	int max=0;
        for (unsigned int i=0; i<NB_SENSORS; i++)
    	{
    		if (get_prox(i)>max)
    		{
                max=get_prox(i);
    			number=i;
    		}
            
        }
    return number;
}


bool to_the_left (void)
{
	return ((proximity_values.delta[SENSOR_IR1]+proximity_values.delta[SENSOR_IR8])/2 > LIM_OBSTACLE_FACE );  //Comparison between the average value and the threshold
}

int check_shoulder(void)
{

	enum sense {nothing, far, close};
	enum sense eye;
	enum sense shoulder;

    // function initialisation of values
    
	if (proximity_values.delta[SENSOR_IR2] > GOAL_IR2)
	{
		eye=close;
	}
	else if (proximity_values.delta[SENSOR_IR2] < NOISE_IR2)
	{
		eye=nothing;
	}
	else eye=far;

	if (proximity_values.delta[SENSOR_IR3] > GOAL_IR3)
	{
		shoulder=close;
	}
	else if (proximity_values.delta[SENSOR_IR3] < NOISE_IR3)
	{
		shoulder=nothing;
	}
	else shoulder=far;

	float speed;
    
    // comparisons and calculations of booleans

	if (eye==close)														// too close to the wall, has to turn a bit left
	{
		speed=(proximity_values.delta[SENSOR_IR2]-GOAL_IR2)/(GOAL_IR2-MAX_IR2);
		if (speed < - MAX_SPEED_MOD)
		{
			speed = - MAX_SPEED_MOD;
		}                            // avoid to have speed above SPEEDK
		return SPEEDK*speed;
	}
	if (eye==far && shoulder==far)                          // too far from the wall, has to turn a bit right
	{
        speed=(GOAL_IR2-proximity_values.delta[SENSOR_IR2])/(GOAL_IR2-NOISE_IR2);
        if (speed > MAX_SPEED_MOD )
        {
        	speed = MAX_SPEED_MOD;
        }                              // avoid to have speed above SPEEDK
		return  SPEEDK * speed;
	}


	if (eye==nothing)
	{
		if (shoulder==close)
		{
			return SPEED_STOP;                  //goes straight
		}
		else
		{
			return SPEEDK;                      // optimal speed to do a rotation around a point 1cm away
		}

	}

return SPEED_STOP;
}



