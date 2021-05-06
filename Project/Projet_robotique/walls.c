#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>
#include <chprintf.h>
#include <stdbool.h>

#include <msgbus/messagebus.h>
#include <sensors/proximity.h>
#include <main.h>
#include <walls.h>
#include <angles.h>

#include <leds.h>

#include <i2c_bus.h>
#include <motors.h>

proximity_msg_t proximity_values;

uint8_t init_prox(void)
{
	 messagebus_topic_t *prox_topic = messagebus_find_topic_blocking(&bus, "/proximity");
	 messagebus_topic_wait(prox_topic, &proximity_values, sizeof(proximity_values));

    uint8_t number = 0;
    uint8_t distance[7];

    int i = 100;
    while (i--)
    {
    	int max=0;
    	for (unsigned int i=0; i<8; i++)
    	{
    		chprintf((BaseSequentialStream *)&SD3, " prox values de %4d, == %4d \n ", i ,get_prox(i));
    		if (get_prox(i)>max)
    		{
    			chprintf((BaseSequentialStream *)&SD3, " dans la boucle ");
    			max=get_prox(i);
    			number=i;

    		}
    	}

    	chprintf((BaseSequentialStream *)&SD3, "max: %d\n", number);
    	chThdSleep(10);
    }

    return number;
}


bool to_the_left (void)
{
	chprintf((BaseSequentialStream *)&SD3, " prox values de  IR1 = %4d, de IR8 = %4d \n ", get_prox(SENSOR_IR1) ,get_prox(SENSOR_IR8));
	return ((get_prox(SENSOR_IR1) > LIM_OBSTACLE_FACE ) && (get_prox(SENSOR_IR8) > LIM_OBSTACLE_FACE ) );
}

bool to_the_right (void)
{
	chprintf((BaseSequentialStream *)&SD3, " prox values de  IR3 = %4d, de IR4 = %4d \n ", get_prox(SENSOR_IR3) ,get_prox(SENSOR_IR4));

	return ((get_prox(SENSOR_IR3) < LIM_OBSTACLE ) && (get_prox(SENSOR_IR4) > LIM_OBSTACLE_AD ) ) ;
}

bool stay_on_your_right (void)
{
	chprintf((BaseSequentialStream *)&SD3, " prox values de  IR3 = %4d \n ", get_prox(SENSOR_IR3) );

	return (get_prox(SENSOR_IR3) > LIM_OBSTACLE ) ;
}

bool double_turn (void)
{
	return ((get_prox(SENSOR_IR3) < LIM_OBSTACLE ) && (get_prox(SENSOR_IR4) < LIM_OBSTACLE_AD ) ) ;
}
void labyrinth_start(void)
{
	int number = init_prox();
    rotate_to_sensor(number);
//    avancer jusqu'à l'obstacle
    //rotate_to_sensor(SENSOR_IR6);
    while(init_prox()!=2)
    {
    	rotate_to_angle(-20);
    }
    chprintf((BaseSequentialStream *)&SD3, "max percu: %d\n", number);
    set_body_led(1);
}



static THD_WORKING_AREA(waMoveRight, 256);
static THD_FUNCTION(MoveRight, arg)
{
    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    systime_t time;
    messagebus_topic_t *proximity_topic = messagebus_find_topic_blocking(&bus, "/proximity");
    //adapt_speed (STOP);

	static unsigned int second_turn = 0;
	static unsigned int start = 0;

    while(1)
    {
        time = chVTGetSystemTime();

    	messagebus_topic_wait(proximity_topic, &proximity_values, sizeof(proximity_values));


    	while (stay_on_your_right()==true)
    	{
    		set_led(LED1, 1);
    		if (to_the_left()==true)
    		{
    			set_front_led(1);
    			adapt_speed(TURN_LEFT);
    		}
    		adapt_speed(KEEP_STRAIGHT);
    		start = 1;
    	}
    	//else if (stay_on_your_right()==false)
    	//{
    	if (start == 1)
    	{
    		avance_valeur (AVANCE_POUR_TOURNER);
    		start = 0;
    	}

    	 if (to_the_right()==true)
    	{
        	set_body_led(1);
   	    	adapt_speed(TURN_RIGHT);
   	    	second_turn = 1;
   	    	avance_valeur (AVANCE_POUR_TOURNER*3/2);
    	}
    	//}
    	 else if ((double_turn () == true) && (second_turn==1))
    	{
    		avance_valeur (AVANCE_POUR_TOURNER *3/2);
    	    adapt_speed(TURN_RIGHT);
    	   	avance_valeur (AVANCE_POUR_TOURNER * 3/2);
    	   	second_turn = 0;
    	}
    	else adapt_speed (STOP);

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
 void verif (void)
 {
	 while (1)
	 {
		chprintf((BaseSequentialStream *)&SD3, " prox values de  IR4 = %4d,  \n ", get_prox(SENSOR_IR4) );
		chThdSleep(100);
	 }


 }


