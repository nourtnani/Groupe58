
#include <walls.h>
#include <proximity.h>
#include <leds.h>

void verif_prox ()
{
	proximity_start ();
	unsigned int sensor = 3 ;
	int prox_s3 = get_calibrated_prox(sensor);

	if (prox_s3 )
	{
		set_rgb_led (LED2, 1, 0,0);
	}
}


