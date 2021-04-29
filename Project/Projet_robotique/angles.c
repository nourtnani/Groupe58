#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>
#include <chprintf.h>

#include <walls.h>
#include <angles.h>
#include <sensors/proximity.h>
#include <leds.h>
#include <msgbus/messagebus.h>
#include <motors.h>




//https://stackoverflow.com/questions/11498169/dealing-with-angle-wrap-in-c-code
double constrainAngle(double x)
{
    x = fmod(x + 180,360);
    if (x < 0)
        x += 360;
    return x - 180;
}

int angle_to_step(int angle)
{
    return (1290/360)*constrainAngle(angle);
}


void rotate_to_angle(int angle)
{
    right_motor_set_pos(5000);
    int steps = angle_to_step(angle);
    if (steps<0)
    {
    	//   turn right
        //   53mm --> 166,5 de perimetre --> 4 tours complets de roue
    	//	41 mm --> 128 de perim
    	// 1 rot = 1000 steps --> tour complet = 1290 steps
    	// pour faire un tour complet il faut 1290 step sur un gauche et -1290 sur droite
        right_motor_set_speed(-100);
        left_motor_set_speed(+100);
        while (right_motor_get_pos()>(5000+steps))
        {
            chThdSleep(10);
            chprintf((BaseSequentialStream *)&SD3, "compteur moteur: %d\n", right_motor_get_pos());

        }
        right_motor_set_speed(0);
        left_motor_set_speed(0);
    }
    else
    {
        //        turn left
        // 1 rot = 1000 steps --> tour complet = 4000 steps
        // pour faire un tour complet il faut 4000 step sur un droite et -4000 sur gauche
        right_motor_set_speed(+100);
        left_motor_set_speed(-100);
        while (right_motor_get_pos()<(5000+steps))
        {
            chThdSleep(10);
            chprintf((BaseSequentialStream *)&SD3, "compteur moteur: %d\n", right_motor_get_pos());
        }
        right_motor_set_speed(0);
        left_motor_set_speed(0);
    }
}

void rotate_to_sensor(int sensor)
{
    switch (sensor)
    {
        case SENSOR_IR1:
        	set_led (LED1,1);
        	rotate_to_angle(A_SENSOR_IR1);
            break;
        case SENSOR_IR2:
        	set_rgb_led (LED2,100,0,0);
            rotate_to_angle(A_SENSOR_IR2);
            break;
        case SENSOR_IR3:
        	set_led (LED3,1);
            rotate_to_angle(A_SENSOR_IR3);
            break;
        case SENSOR_IR4:
        	set_rgb_led (LED4,100,0,0);
            rotate_to_angle(A_SENSOR_IR4);
            break;
        case SENSOR_IR5:
        	set_led (LED5,1);
            rotate_to_angle(A_SENSOR_IR5);
            break;
        case SENSOR_IR6:
        	set_rgb_led (LED6,100,0,0);
            rotate_to_angle(A_SENSOR_IR6);
            break;
        case SENSOR_IR7:
        	set_led (LED7,1);
            rotate_to_angle(A_SENSOR_IR7);
            break;
        case SENSOR_IR8:
        	set_rgb_led (LED8,100,0,0);
            rotate_to_angle(A_SENSOR_IR8);
            break;
        default:
            break;
    }
}

void init(void)
{


}


