#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>
#include <chprintf.h>
#include <stdbool.h>

#include <msgbus/messagebus.h>
#include <sensors/proximity.h>
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
//    avancer jusqu'à l'obstacle
    //rotate_to_sensor(SENSOR_IR6);
  //  while(init_prox()!=2)
  //  {
   // 	rotate_to_angle(-20);
  //  }
    set_body_led(1);
}



static THD_WORKING_AREA(waMoveRight, 256);
static THD_FUNCTION(MoveRight, arg)
{
    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    systime_t time;
    messagebus_topic_t *proximity_topic = messagebus_find_topic_blocking(&bus, "/proximity");
    adapt_speed (STOP , check_shoulder());

	static unsigned int second_turn = 0;
	static unsigned int start = 0;

    while(1)
    {
        time = chVTGetSystemTime();

    	messagebus_topic_wait(proximity_topic, &proximity_values, sizeof(proximity_values));

        adapt_speed (STOP , check_shoulder());

    	while (stay_on_your_right()==true)
    	{
    		if (to_the_left()==true)
    		{
    			adapt_speed(TURN_LEFT , check_shoulder());
    		}
    		adapt_speed(KEEP_STRAIGHT , check_shoulder());
    		start = 1;
    	}
    	if (start == 1)
    	{
    		adapt_speed (KEEP_STRAIGHT, check_shoulder());
    		second_turn = 1;
    	}
		if ((see_nothing() == true) && (second_turn ==1))
		{
        	avance_valeur (AVANCE_POUR_TOURNER , SPEED_STOP);
			adapt_speed (KEEP_STRAIGHT , check_shoulder());
			second_turn = 0;
		}
 		start = 0;

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
    	//chThdSleep(10);
    }
    return number;
}


bool to_the_left (void)
{
	return ((get_prox(SENSOR_IR1) > LIM_OBSTACLE_FACE ) && (get_prox(SENSOR_IR8) > LIM_OBSTACLE_FACE ) );
}

bool stay_on_your_right (void)
{
	return (get_prox(SENSOR_IR3) > LIM_OBSTACLE ) ;
}

bool see_nothing (void)
{
	return ((get_prox(SENSOR_IR3) < NOISE_IR3 ) && (get_prox(SENSOR_IR2) < NOISE ) );
}


int check_shoulder(void)
{
	float treshold_IR2 		= 700;
	float noise_IR2 		= 160;
	float treshold_IR3 		= 2350;
	float noise_IR3 		= 250;
	float treshold_IR4 		= 400;
	float noise_IR4			= 100;
	float max_1				= 1100;
	float speedk 			= 50;

	enum sense {nothing, far, close};
	enum sense eye;
	enum sense shoulder;
	enum sense back;

	if (get_prox(SENSOR_IR2) > treshold_IR2)
	{
		eye=close;
	}
	else if (get_prox(SENSOR_IR2) < noise_IR2)
	{
		eye=nothing;
	}
	else eye=far;

	if (get_prox(SENSOR_IR3) > treshold_IR3)
	{
		shoulder=close;
	}
	else if (get_prox(SENSOR_IR3) < noise_IR3)
	{
		shoulder=nothing;
	}
	else shoulder=far;

	if (get_prox(SENSOR_IR4) > treshold_IR4)
	{
		back=close;
	}
	else if (get_prox(SENSOR_IR4) < noise_IR4)
	{
		back=nothing;
	}
	else back=far;

	if (eye==nothing && shoulder==nothing && back==nothing) 		 // he is somewhere, no obstacles
	{
		return SPEED_STOP;
	}
	if (eye==close)													  // too close to the wall, turn a bit left
	{
		return (get_prox(SENSOR_IR2)-treshold_IR2)/(treshold_IR2-max_1)*speedk;
	}
	if (eye==far && shoulder==far)									// voit qu'il est un peu loin avec son oeil
																	// sent qu'il est trop loin du mur mais le voit encore
																	// turn a bit right something positive
	{
		return (treshold_IR2-get_prox(SENSOR_IR2))/(treshold_IR2-noise_IR2)*speedk;
	}


	if (eye==nothing)											// voit rien avec son oeil droit
	{
		if (shoulder==close)									//mais sent avec son epaule
		{
			return (treshold_IR3-get_prox(SENSOR_IR3))/(treshold_IR3-noise_IR3);
		}
		if (((shoulder==far)) || ((shoulder == nothing)&& back == far))
		{
			return (treshold_IR3-get_prox(SENSOR_IR3))/(treshold_IR3-noise_IR3)*speedk*ROTATION_COEFF;
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





