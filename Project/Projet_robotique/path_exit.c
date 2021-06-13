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




static THD_WORKING_AREA(waFallMonitoring, 1024);
static THD_FUNCTION(FallMonitoring, arg)
{
	chRegSetThreadName(__FUNCTION__);
	(void)arg;
    imu_msg_t imu_values;
	systime_t time;
	messagebus_topic_t *imu_topic = messagebus_find_topic_blocking(&bus, "/imu");

	float init_gyro = get_gyro_rate(X_AXIS);

	while(1)
	{
		time = chVTGetSystemTime();
        //wait for new measures to be published
        messagebus_topic_wait(imu_topic, &imu_values, sizeof(imu_values));
        stopCurrentMelody();
        if (abs( get_gyro_rate(X_AXIS) - init_gyro)  > ANGULAR_ACC_DEATH )
        {
            	playMelody(MARIO_DEATH , ML_FORCE_CHANGE , NULL);
            	waitMelodyHasFinished();
        }
	}
    //100Hz
    chThdSleepUntilWindowed(time, time + MS2ST(10));
}

void fall_monitoring_start(void)
{
	chThdCreateStatic(waFallMonitoring, sizeof(waFallMonitoring), NORMALPRIO , FallMonitoring, NULL) ;
}


