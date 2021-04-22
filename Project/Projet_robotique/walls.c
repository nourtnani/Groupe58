
#include <walls.h>
#include <sensors/proximity.h>
#include <leds.h>

#define SENSOR_IR3  	3
#define LIM_OBSTACLE 	10



void verif_prox (void)
{
	//proximity_start ();

	if (get_prox(SENSOR_IR3) < LIM_OBSTACLE )
	{
		set_front_led(1);
	}
}



