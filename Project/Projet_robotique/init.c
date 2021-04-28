#include "ch.h"
#include "hal.h"
#include <math.h>
#include <usbcfg.h>
#include <chprintf.h>

#include <walls.h>
#include <sensors/proximity.h>
#include <leds.h>
#include <msgbus/messagebus.h>

#define SENSOR_IR1 0
#define SENSOR_IR2 1
#define SENSOR_IR3 2
#define SENSOR_IR4 3
#define SENSOR_IR5 4
#define SENSOR_IR6 5
#define SENSOR_IR7 6
#define SENSOR_IR8 7

#define A_SENSOR_IR1 -20
#define A_SENSOR_IR2 -50
#define A_SENSOR_IR3 -90
#define A_SENSOR_IR4 -150
#define A_SENSOR_IR5 150
#define A_SENSOR_IR6 90
#define A_SENSOR_IR7 50
#define A_SENSOR_IR8 20


uint8_t init_prox(){
    uint8_t number = 0;
    uint8_t distance[7];
    for (int i=0; i<7; i++) {
        distance[i]=prox_values.delta[i];
    }
    int max=0;
    for (int i=1; i<sizeof(prox_values); i++){
        if (distance[i]>max){
            max=distance[i];
            number=i;
        }
    }

    return number;
}

//https://stackoverflow.com/questions/11498169/dealing-with-angle-wrap-in-c-code
double constrainAngle(double x){
    x = fmod(x + 180,360);
    if (x < 0)
        x += 360;
    return x - 180;
}

int angle_to_step(int angle){
    return (4000/360)*constrainAngle(angle);}


void rotate_to_angle(int angle){
    right_motor_set_pos(5000);
    int steps=angle_to_step(angle);
    if (steps<0) {
//        turn right
        //   53mm --> 166,5 de diamètre --> 4 tours complets de roue
    // 1 rot = 1000 steps --> tour complet = 4000 steps
    // pour faire un tour complet il faut 4000 step sur un gauche et -4000 sur droite
        right_motor_set_speed(-100);
        left_motor_set_speed(+100);
        while (right_motor_get_pos()>5000+steps) {
            asm nop; // chsleep bs
        }
        right_motor_set_speed(0);
        left_motor_set_speed(0);
    }else{
        //        turn left
        // 1 rot = 1000 steps --> tour complet = 4000 steps
        // pour faire un tour complet il faut 4000 step sur un droite et -4000 sur gauche
        right_motor_set_speed(+100);
        left_motor_set_speed(-100);
        while (right_motor_get_pos()<5000+steps) {
            asm nop; // chsleep bs
        }
        right_motor_set_speed(0);
        left_motor_set_speed(0);
        }


}

void rotate_to_sensor(int sensor){
    switch (sensor) {
        case SENSOR_IR1:
            rotate_to_angle(A_SENSOR_IR1);
            break;
        case SENSOR_IR2:
            rotate_to_angle(A_SENSOR_IR2);
            break;
        case SENSOR_IR3:
            rotate_to_angle(A_SENSOR_IR3);
            break;
        case SENSOR_IR4:
            rotate_to_angle(A_SENSOR_IR4);
            break;
        case SENSOR_IR5:
            rotate_to_angle(A_SENSOR_IR5);
            break;
        case SENSOR_IR6:
            rotate_to_angle(A_SENSOR_IR6);
            break;
        case SENSOR_IR7:
            rotate_to_angle(A_SENSOR_IR7);
            break;
        case SENSOR_IR8:
            rotate_to_angle(A_SENSOR_IR8);
            break;
        default:
            break;
    }
}

void init(){
    rotate_to_sensor(init_prox());
//    avancer jusqu'à l'obstacle
    rotate_to_sensor(SENSOR_IR3);


}


