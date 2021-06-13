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
#include <angles.h>
#include <sensors/proximity.h>
#include <sensors/imu.h>
#include <msgbus/messagebus.h>
#include <audio/play_melody.h>
#include <audio/microphone.h>
#include <audio/audio_thread.h>
#include <sensors/VL53L0X/VL53L0X.h>
#include <chmtx.h>
#include <chprintf.h>
#include <i2c_bus.h>
#include "path_check.h"
#include "path_exit.h"



messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);


int main(void)
{
    halInit();
    chSysInit();
    // inits the Inter Process Communication bus.
    messagebus_init(&bus, &bus_lock, &bus_condvar);
    // inits the I2C communication
    i2c_start();
    //inits the captors
    mpu_init();
	VL53L0X_start();
    proximity_start();
    imu_start();
    // starts the calibration of the sensors
    calibrate_ir();
    calibrate_gyro();
    calibrate_acc();
    // starts the buffer and the melody thread
    dac_start();
    playMelodyStart();
    // inits the motor
    motors_init();
    // starts the labyrinth resolution threads
    labyrinth_start();
    move_right_start();
    fall_monitoring_start();

    return 0;
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
