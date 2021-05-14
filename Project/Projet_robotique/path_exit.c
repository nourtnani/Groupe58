/*
 * path_exit.c
 *
 *  Created on: 13 mai 2021
 *      Author: nourt
 */


#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>
#include <chprintf.h>

#include <angles.h>
#include <main.h>
#include <leds.h>
#include <msgbus/messagebus.h>
#include <motors.h>
#include <sensors/imu.h>
#include <i2c_bus.h>
#include "path_check.h"
#include "path_exit.h"
#include <audio/play_melody.h>


imu_msg_t imu_values;


static THD_WORKING_AREA(waFallMonitoring, 512);
static THD_FUNCTION(FallMonitoring, arg)
{
	chRegSetThreadName(__FUNCTION__);
	(void)arg;

	systime_t time;
	messagebus_topic_t *imu_topic = messagebus_find_topic_blocking(&bus, "/imu");

	int16_t noise_z = -15000 ;
	int16_t noise_gyr_x = 7000 ;

	while(1)
	{
		time = chVTGetSystemTime();
        //wait for new measures to be published
        messagebus_topic_wait(imu_topic, &imu_values, sizeof(imu_values));

        chprintf((BaseSequentialStream *)&SD3, "%Ax=%-7d Ay=%-7d Az=%-7d Gx=%-7d Gy=%-7d Gz=%-7d\r\n",
                get_acc(X_AXIS), get_acc(Y_AXIS), get_acc(Z_AXIS),
				get_gyro(X_AXIS), get_gyro(Y_AXIS), get_gyro(Z_AXIS));

        if ((get_acc(Z_AXIS) < noise_z))
        {
        	if (get_gyro(X_AXIS) > noise_gyr_x)
        	{
                adapt_speed (STOP , SPEED_STOP);
            	playMelody(WE_ARE_THE_CHAMPIONS , ML_FORCE_CHANGE , NULL);
            	waitMelodyHasFinished();
                adapt_speed (STOP , SPEED_STOP);
        	}
        	else   stopCurrentMelody();
        }


       /* if(get_acc(Z_AXIS) > noise_z)
		    	{
		    		if((get_gyr(X_AXIS)+get_gyr(Y_AXIS)) > noise_gyr)
		    		{
		    			// turn all leds and bip
		    			set_led(0,1);
		    			set_led(1,1);
		    			set_led(2,1);
		    			set_led(3,1);

		    		}
		    		else
		    		{
		    			left_motor_set_speed(MAXSPEED);
		    			righ_motor_set_speed(MAXSPEED);
		    		}
		    	}
*/
		    }

    //100Hz
    chThdSleepUntilWindowed(time, time + MS2ST(10));

}

void fall_monitoring_start(void)
{
	chThdCreateStatic(waFallMonitoring, sizeof(waFallMonitoring), NORMALPRIO , FallMonitoring, NULL) ;
}


