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
#include "path_check.h"


    imu_start();
    calibrate_gyro();
    calibrate_acc();

    static THD_WORKING_AREA(waFallMonitoring, 256);
    static THD_FUNCTION(FallMonitoring, arg)
		{
		    chRegSetThreadName(__FUNCTION__);
		    (void)arg;

		    systime_t time;
		    messagebus_topic_t *imu_topic = messagebus_find_topic_blocking(&bus, "/imu");
		    int noise_z;
		    int noise_gyr;

		    while(1){
		    	if(get_acc(2)<noise_z){
		    		if(get_gyr(0)+get_gyr(1)<noise_gyr){
		    			// turn all leds and bip
		    			set_led(0,1);
		    			set_led(1,1);
		    			set_led(2,1);
		    			set_led(3,1);

		    		}else{
		    			left_motor_set_speed(MAXSPEED);
		    			righ_motor_set_speed(MAXSPEED);
		    		}
		    		}

		    	}


		    }

    void fall_monitoring_start(void)
    {
    	chThdCreateStatic(waFallMonitoring, sizeof(waFallMonitoring), NORMALPRIO, FallMonitoring, NULL);
    }
