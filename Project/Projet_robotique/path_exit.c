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


static THD_WORKING_AREA(waFallMonitoring, 1024);
static THD_FUNCTION(FallMonitoring, arg)
{
	chRegSetThreadName(__FUNCTION__);
	(void)arg;

	systime_t time;
	messagebus_topic_t *imu_topic = messagebus_find_topic_blocking(&bus, "/imu");

	float threshold_acc = -6.5 ;
	float threshold_gyro = 0.17f ;
	float init_gyro = get_gyro_rate(X_AXIS);

	while(1)
	{
		time = chVTGetSystemTime();
        //wait for new measures to be published
        messagebus_topic_wait(imu_topic, &imu_values, sizeof(imu_values));

        chprintf((BaseSequentialStream *)&SD3, "%Ax=%6f Ay=%6f Az=%6f Gx=%6f Gy=%6f Gz=%6f\r\n",
                get_acceleration(X_AXIS), get_acceleration(Y_AXIS), get_acceleration(Z_AXIS),
				get_gyro_rate(X_AXIS), get_gyro_rate(Y_AXIS), get_gyro_rate(Z_AXIS));
        stopCurrentMelody();
        if (abs( get_gyro_rate(X_AXIS) - init_gyro)  >threshold_gyro )
        {
                adapt_speed (STOP , SPEED_STOP);
            	playMelody(MARIO_DEATH , ML_FORCE_CHANGE , NULL);
            	waitMelodyHasFinished();
                while(1)
                {
                    adapt_speed (STOP , SPEED_STOP);

                }
        }

    	else
    	{
    		stopCurrentMelody();
    	}
	}

    //100Hz
    chThdSleepUntilWindowed(time, time + MS2ST(10));

}

void fall_monitoring_start(void)
{
	chThdCreateStatic(waFallMonitoring, sizeof(waFallMonitoring), NORMALPRIO , FallMonitoring, NULL) ;
}


