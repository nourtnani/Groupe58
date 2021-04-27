#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <ch.h>
#include <hal.h>
#include "memory_protection.h"
#include <usbcfg.h>
#include <main.h>
#include <motors.h>
#include <leds.h>
#include <walls.h>
#include <sensors/proximity.h>
#include <msgbus/messagebus.h>
#include <chmtx.h>
#include <chprintf.h>
#include <i2c_bus.h>


void SendUint8ToComputer(uint8_t* data, uint16_t size) 
{
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)"START", 5);
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)&size, sizeof(uint16_t));
	chSequentialStreamWrite((BaseSequentialStream *)&SD3, (uint8_t*)data, size);
}

static void serial_start(void)
{
	static SerialConfig ser_cfg = {
	    115200,
	    0,
	    0,
	    0,
	};

	sdStart(&SD3, &ser_cfg); // UART3.
}

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

//messagebus_topic_t *prox_topic = messagebus_find_topic_blocking(&bus, "/proximity");

//proximity_msg_t prox_values;

int main(void)
{
    halInit();
    chSysInit();
    serial_start();

    mpu_init();
    i2c_start();
    proximity_start();
    motors_init();


    /** Inits the Inter Process Communication bus. */
    messagebus_init(&bus, &bus_lock, &bus_condvar);


    messagebus_topic_t *prox_topic = messagebus_find_topic_blocking(&bus, "/proximity");
    proximity_msg_t prox_values;
    int16_t speed = 180;
    int16_t arret = 0;

    while (1)
    {
    	chprintf((BaseSequentialStream *)&SD3, "je suis ici ");

    	messagebus_topic_wait(prox_topic, &prox_values, sizeof(prox_values));

    	// Read proximity sensors.
    	//chprintf((BaseSequentialStream *)&SD3, "PROXIMITY\r\n");
    	//chprintf((BaseSequentialStream *)&SD3, "%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d\r\n\n", prox_values.delta[0], prox_values.delta[1], prox_values.delta[2], prox_values.delta[3], prox_values.delta[4], prox_values.delta[5], prox_values.delta[6], prox_values.delta[7]);
    	//chprintf((BaseSequentialStream *)&SD3, "AMBIENT\r\n");
    	//chprintf((BaseSequentialStream *)&SD3, "%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d\r\n\n", prox_values.ambient[0], prox_values.ambient[1], prox_values.ambient[2], prox_values.ambient[3], prox_values.ambient[4], prox_values.ambient[5], prox_values.ambient[6], prox_values.ambient[7]);

    	chprintf((BaseSequentialStream *) &SD3, "%d;",prox_values.delta[SENSOR_IR3]);
    	chprintf((BaseSequentialStream *) &SD3, "%d;",prox_values.delta[SENSOR_IR4]);
    	if (prox_values.delta[SENSOR_IR3] > LIM_OBSTACLE )
    	{
    		set_front_led(1);
    		right_motor_set_speed (speed);
    		left_motor_set_speed (speed);
    	}
    	else if  ( (prox_values.delta[SENSOR_IR4] > (LIM_OBSTACLE *3/4)) & ( prox_values.delta[SENSOR_IR3] < LIM_OBSTACLE) )
    	{
    		set_front_led(0);
    		set_body_led (1);
    		right_motor_set_speed (speed);
    		left_motor_set_speed (speed);
    	}
    	else
    	{
    		set_front_led(0);
    		set_body_led (0);
    		right_motor_set_speed (arret);
    		left_motor_set_speed (arret);
    	}
    }

   // *prox_topic = messagebus_find_topic_blocking(&bus, "/proximity");

    /** Inits the Inter Process Communication bus. */
    /*
    messagebus_init(&bus, &bus_lock, &bus_condvar);

    messagebus_topic_t *prox_topic = messagebus_find_topic_blocking(&bus, "/proximity");
    proximity_msg_t prox_values;

  //  parameter_namespace_declare(&parameter_root, NULL, NULL);

    motors_init();
    int16_t speed = 180;
    proximity_start ();

    while (1)
    {
    	//right_motor_set_speed (speed);
    	//left_motor_set_speed (-speed);
    	set_body_led (1) ;

    	messagebus_topic_wait(prox_topic, &prox_values, sizeof(prox_values));
    	//verif_prox();

    	// Read proximity sensors.
    	chprintf((BaseSequentialStream *)&SD3, "PROXIMITY\r\n");
    	chprintf((BaseSequentialStream *)&SD3, "%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d\r\n\n", prox_values.delta[0], prox_values.delta[1], prox_values.delta[2], prox_values.delta[3], prox_values.delta[4], prox_values.delta[5], prox_values.delta[6], prox_values.delta[7]);
    	chprintf((BaseSequentialStream *)&SD3, "AMBIENT\r\n");
    	chprintf((BaseSequentialStream *)&SD3, "%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d\r\n\n", prox_values.ambient[0], prox_values.ambient[1], prox_values.ambient[2], prox_values.ambient[3], prox_values.ambient[4], prox_values.ambient[5], prox_values.ambient[6], prox_values.ambient[7]);

    	//chprintf((BaseSequentialStream *) &SD3, "%d;",get_prox(SENSOR_IR3));
    	if (prox_values.delta[SENSOR_IR3] < LIM_OBSTACLE )
    	{
    		set_front_led(1);
    	}

    }


    messagebus_init(&bus, &bus_lock, &bus_condvar);

	chprintf((BaseSequentialStream *) &SD3, "main1 " );
    proximity_start();

	chprintf((BaseSequentialStream *) &SD3, "main2 " );
    systime_t time;


    while (1)
    {
    	chprintf((BaseSequentialStream *) &SD3, "main " );
    	//time = chVTGetSystemTime();
    	verif_prox();

    	//100Hz
    	    	//chThdSleepUntilWindowed(time, time + MS2ST(10));
    }



    motors_init();


    int16_t speed = 180;
    systime_t time;

    while (1)
    {
    	time = chVTGetSystemTime();
    	right_motor_set_speed(speed);
    	left_motor_set_speed(- speed);



    	//100Hz
    	chThdSleepUntilWindowed(time, time + MS2ST(10));
    }


*/


    /* Infinite loop. */
  /*  while (1) {
    	//waits 1 second
        chThdSleepMilliseconds(1000);
    }
*/
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
