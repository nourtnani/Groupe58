#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <main.h>
#include <motors.h>
#include <imu.h>
#include <proximity.h>
#include <VL53L0X.h>


void init_world(void) {

	// on lance le capteur et les moteurs

	motor_init();
	VL53L0X_init(U13); // jsp quoi mettre dans les paranthèses
	uint16_t distance[36];
	// tour complet
	VL53L0X_start();
	uint16_t lrotation_counter=left_motor_get_pos();
	uint16_t rrotation_counter=left_motor_get_pos();

	while (lrotation_counter+un_tour_complet>left_motor_get_pos()){
		lrotation_counter+=un_tour_complet/36;
		left_motor_set_pos(lrotation_counter);
		left_motor_set_pos(-rrotation_counter);
		distance[i]=VL53L0X_get_dist_mm();
	}
	uint16_t min=distances(0);
	uint8_t minrank=0;
	for (int i=1; i<size(distance); i++){
		if (distance(i)<min){
			min=distance(i);
			minrank=i;
		}
	}

}

//static THD_WORKING_AREA(waInit, 256);
//static THD_FUNCTION(InitWorld, arg) {

//    chRegSetThreadName(__FUNCTION__);
 //   (void)arg;

//    systime_t time;

//    int16_t speed = 0;
//    int16_t speed_correction = 0;

//    while(1){
//        time = chVTGetSystemTime();

        // do things
        //computes the speed to give to the motors
        //distance_cm is modified by the image processing thread

 //       chThdSleepUntilWindowed(time, time + MS2ST(10));
//    }
//}

//void init_start(void){
//	chThdCreateStatic(waInit, sizeof(waInit), NORMALPRIO, InitWorld, NULL);
//}
