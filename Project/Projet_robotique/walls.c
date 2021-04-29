#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>
#include <chprintf.h>
#include <stdbool.h>

#include <walls.h>
#include <angles.h>
#include <sensors/proximity.h>
#include <leds.h>
#include <msgbus/messagebus.h>
#include <i2c_bus.h>
#include <motors.h>
#include <main.h>

proximity_msg_t prox_values;
//messagebus_t bus;

uint8_t init_prox(void)
{
	 messagebus_topic_t *prox_topic = messagebus_find_topic_blocking(&bus, "/proximity");
	 messagebus_topic_wait(prox_topic, &prox_values, sizeof(prox_values));

    uint8_t number = 0;
    uint8_t distance[7];
   // for (int i=0; i<8; i++)
   // {
   // 	chprintf((BaseSequentialStream *)&SD3, " prox values de %4d, == %4d \n ", i ,prox_values.delta[i]);
    //    distance[i]=prox_values.delta[i];
   //     chprintf((BaseSequentialStream *)&SD3, "distance de %4d , ==, %4d \n ",i , "==" , distance[i]);
   // }
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
	return ((prox_values.delta[SENSOR_IR3] > LIM_OBSTACLE ) && (prox_values.delta[SENSOR_IR1] > LIM_OBSTACLE ) && (prox_values.delta[SENSOR_IR2] > LIM_OBSTACLE ) );
}

bool to_the_right (void)
{
	return ((prox_values.delta[SENSOR_IR3] < LIM_OBSTACLE ) && (prox_values.delta[SENSOR_IR4] > LIM_OBSTACLE_AD ) ) ;
}

bool stay_on_your_right (void)
{
	return (prox_values.delta[SENSOR_IR3] > LIM_OBSTACLE ) ;
}

bool rotate_left (void)
{
	return ((prox_values.delta[SENSOR_IR4] < LIM_OBSTACLE ) && (prox_values.delta[SENSOR_IR5] < LIM_OBSTACLE ) ) ;
}

void labyrinth_start(void)
{
	int number = init_prox();
    rotate_to_sensor(number);
//    avancer jusqu'à l'obstacle
    rotate_to_sensor(SENSOR_IR6);
    chprintf((BaseSequentialStream *)&SD3, "max percu: %d\n", number);
    set_body_led(1);
}

int16_t *adapt_speed (void)
{
	int16_t speed [2] ;

    unsigned int time = TIME_LIM ;

	if (stay_on_your_right () == true)
	{
		speed [MOTEUR_DROITE] = SPEED_WALK ;
		speed [MOTEUR_GAUCHE] = SPEED_WALK ;

		if (to_the_left () == true)
		{
			while (rotate_left () == true)
			{
				speed [MOTEUR_DROITE] = SPEED_WALK ;
				speed [MOTEUR_GAUCHE] = - SPEED_WALK ;
			}
		}
	}

	else if (to_the_right () == true)
	{
		while (time --)
		{
			speed [MOTEUR_DROITE] = SPEED_WALK ;
			speed [MOTEUR_GAUCHE] = SPEED_WALK ;
		}

		//while (rotate () == true)
		{
			speed [MOTEUR_DROITE] = - SPEED_WALK ;
			speed [MOTEUR_GAUCHE] =  SPEED_WALK ;
		}

	}
	else
	{
		speed [MOTEUR_DROITE] = SPEED_STOP ;
		speed [MOTEUR_GAUCHE] = SPEED_STOP ;
	}

	return speed;
}

static THD_WORKING_AREA(waMoveRight, 256);
static THD_FUNCTION(MoveRight, arg)
{
    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    systime_t time;

    int16_t *speed ;
    messagebus_topic_t *prox_topic = messagebus_find_topic_blocking(&bus, "/proximity");


    while(1)
    {
        time = chVTGetSystemTime();

    	chprintf((BaseSequentialStream *)&SD3, "je suis ici ");

    	messagebus_topic_wait(prox_topic, &prox_values, sizeof(prox_values));

        //computes the speed to give to the motors
        //speed is modified given the state of the motor (walk, rotate, stop)

         speed = adapt_speed () ;

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
      	right_motor_set_speed(speed[MOTEUR_DROITE]);
      	left_motor_set_speed(speed[MOTEUR_GAUCHE]);

        //100Hz
        chThdSleepUntilWindowed(time, time + MS2ST(10));
    }
}


void move_right_start(void)
{
	chThdCreateStatic(waMoveRight, sizeof(waMoveRight), NORMALPRIO, MoveRight, NULL);
}



