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

proximity_msg_t proximity_values;

void labyrinth_start(void)
{
	int number = init_prox();
    rotate_to_sensor(number);

	chprintf((BaseSequentialStream *)&SD3, " TOF ",VL53L0X_get_dist_mm() );
	int distanceTOF=VL53L0X_get_dist_mm();
	if (distanceTOF<100){
    move(distance_to_step((distanceTOF)/10-1),SPEED_STOP);
	}
    glue_shoulder();
    set_front_led(1);
    VL53L0X_stop();
}



static THD_WORKING_AREA(waMoveRight, 256);
static THD_FUNCTION(MoveRight, arg)
{
    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    systime_t time;
    messagebus_topic_t *proximity_topic = messagebus_find_topic_blocking(&bus, "/proximity");
    adapt_speed (STOP , check_shoulder());


    while(1)
    {
        time = chVTGetSystemTime();

    	messagebus_topic_wait(proximity_topic, &proximity_values, sizeof(proximity_values));

      //  adapt_speed (STOP , check_shoulder());

    	while (1)//(stay_on_your_right()==true)
    	{
    		if (to_the_left()==true)
    		{
    			glue_shoulder();
    		}
    		adapt_speed(KEEP_STRAIGHT , check_shoulder());

    	}


        /*
         * if the line is nearly in front of an edge, slow down to turn
         * adapt : to turn to the left, follow the right wall until a certain distance from the front wall, then turn,
         * 			to turn to the right, once it stops detecting the right wall, it moves forward till the back sensor senses the right wall, then rotates.
		*/

        //100Hz
        chThdSleepUntilWindowed(time, time + MS2ST(10));
    }
}

void move_right_start(void)
{
	chThdCreateStatic(waMoveRight, sizeof(waMoveRight), NORMALPRIO, MoveRight, NULL);
}



uint8_t init_prox(void)
{
	 messagebus_topic_t *prox_topic = messagebus_find_topic_blocking(&bus, "/proximity");
	 messagebus_topic_wait(prox_topic, &proximity_values, sizeof(proximity_values));

    uint8_t number = 0;

    int i = 100;
    while (i--)
    {
    	int max=0;
    	for (unsigned int i=0; i<8; i++)
    	{
    		if (get_prox(i)>max)
    		{
    			max=get_prox(i);
    			number=i;
    		}
    	}
    }
    return number;
}


bool to_the_left (void)
{
	return (get_prox(SENSOR_IR1)+get_prox(SENSOR_IR8) > 2*LIM_OBSTACLE_FACE );
}

int check_shoulder(void)
{

	enum sense {nothing, far, close};
	enum sense eye;
	enum sense shoulder;

	if (get_prox(SENSOR_IR2) > GOAL_IR2)
	{
		eye=close;
	}
	else if (get_prox(SENSOR_IR2) < NOISE_IR2)
	{
		eye=nothing;
	}
	else eye=far;

	if (get_prox(SENSOR_IR3) > GOAL_IR3)
	{
		shoulder=close;
	}
	else if (get_prox(SENSOR_IR3) < NOISE_IR3)
	{
		shoulder=nothing;
	}
	else shoulder=far;

	int speed;

	if (eye==close)													  // too close to the wall, turn a bit left
	{
		speed=(get_prox(SENSOR_IR2)-GOAL_IR2)/(GOAL_IR2-MAX_IR2)*SPEEDK;
		if (speed<-SPEEDK){speed=-SPEEDK;}
		return speed;
	}
	if (eye==far && shoulder==far)									// voit qu'il est un peu loin avec son oeil
																	// sent qu'il est trop loin du mur mais le voit encore
																	// turn a bit right something positive
	{
		return (GOAL_IR2-get_prox(SENSOR_IR2))/(GOAL_IR2-NOISE_IR2)*SPEEDK;
	}


	if (eye==nothing)											// voit rien avec son oeil droit
	{
		if (shoulder==close)									//mais sent avec son epaule
		{
			return SPEED_STOP;
		}
		else{
			return SPEEDK;
		}

	}

return SPEED_STOP;
}

 void verif (void)
 {
	 while (1)
	 {
		chprintf((BaseSequentialStream *)&SD3, " prox values de  IR1 = %4d,  \n ", get_prox(SENSOR_IR1) );
		chprintf((BaseSequentialStream *)&SD3, " prox values de  IR3 = %4d,  \n ", get_prox(SENSOR_IR3) );
		chprintf((BaseSequentialStream *)&SD3, " prox values de  IR2 = %4d,  \n ", get_prox(SENSOR_IR2) );
		chprintf((BaseSequentialStream *)&SD3, " prox values de  IR4 = %4d,  \n ", get_prox(SENSOR_IR4) );


		chThdSleep(100);
	 }

 }





