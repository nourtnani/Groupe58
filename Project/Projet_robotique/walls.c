
#include <walls.h>
#include <sensors/proximity.h>
#include <leds.h>
#include <usbcfg.h>
#include <chprintf.h>
#include <msgbus/messagebus.h>




proximity_msg_t prox_values;

void verif_prox (void)
{

	// Read proximity sensors.
	chprintf((BaseSequentialStream *)&SD3, "PROXIMITY\r\n");
	chprintf((BaseSequentialStream *)&SD3, "%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d\r\n\n", prox_values.delta[0], prox_values.delta[1], prox_values.delta[2], prox_values.delta[3], prox_values.delta[4], prox_values.delta[5], prox_values.delta[6], prox_values.delta[7]);
	chprintf((BaseSequentialStream *)&SD3, "AMBIENT\r\n");
	chprintf((BaseSequentialStream *)&SD3, "%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d\r\n\n", prox_values.ambient[0], prox_values.ambient[1], prox_values.ambient[2], prox_values.ambient[3], prox_values.ambient[4], prox_values.ambient[5], prox_values.ambient[6], prox_values.ambient[7]);

	//chprintf((BaseSequentialStream *) &SD3, "%d;",get_prox(SENSOR_IR3));
	if (prox_values.delta[SENSOR_IR3] < LIM_OBSTACLE )
	{
		set_front_led(1);
	}
}



