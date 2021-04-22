#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <usbcfg.h>
#include <main.h>
#include <motors.h>
#include <chprintf.h>
#include <leds.h>
#include <walls.h>
#include <sensors/proximity.h>



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


int main(void)
{
    halInit();
    chSysInit();
    mpu_init();

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



   /* motors_init();


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
