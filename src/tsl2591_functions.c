/*
Author: Gabriel Rodgers
Date: 12/30/2024
Purpose: Implement functions to operate on raw tsl2591 data
*/

//include files
#include <stdint.h>
#include <stdlib.h>

#include "tsl2591_functions.h"
#include "i2c.h"

//var definitions
#define TSL2591_CTRL 0xA1
#define TSL2591_LUX_DF 408.0F
#define TSL2591_LUX_C_CH0 1.64F 
#define TSL2591_LUX_C_CH1 0.59F

//set the gain and integration time
void tsl2591_write_settings(again_t AGAIN, atime_t ATIME) {
	uint32_t data = (TSL2591_CTRL << 8) | (AGAIN) | (ATIME);
	i2c2_write(2, &data);
}

//return the light data in units of LUX
uint32_t rawdata_to_lux(uint32_t raw_data, again_t AGAIN, atime_t ATIME) {
	//raw_data is big-endian: high 2 bytes are CH1, low 2 bytes are CH0
	uint16_t ch0 = raw_data & 0xFFFF;
	uint16_t ch1 = (raw_data >> 16) & 0xFFFF;

	float again;
	float atime;

	//gain determination
	switch (AGAIN) {
		case again_low:
			again = 1.0F;
			break;
		case again_medium:
			again = 25.0F;
			break;
		case again_high:
			again = 428.0F;
			break;
		case again_maximum:
			again = 9876.0F;
			break;
		default:
			again = 1.0F;
			break;
	}

	//integration time determination
	switch (ATIME) {
		case atime_100ms:
			atime = 100.0F;
			break;
		case atime_200ms:
			atime = 200.0F;
			break;
		case atime_300ms:
			atime = 300.0F;
			break;
		case atime_400ms:
			atime = 400.0F;
			break;
		case atime_500ms:
			atime = 500.0F;
			break;
		case atime_600ms:
			atime = 600.0F;
			break;
		default:
			atime = 100.0F;
			break;
	}

	float cpl = (atime * again) / TSL2591_LUX_DF;

	//lux calculations: alt func 1 and 2 (commented out)
	float lux_data = (((float)ch0 - (float)ch1)) * (1.0F - ((float)ch1 / (float)ch0)) / cpl;

	//following if statements and constant multiplication is calibration using my 
	//iPhone's Photone lux meter app: (for Low gain only!!!)
	if (AGAIN == again_low) {
		if (lux_data > 650.0F) return lux_data*1.6F;
		else if (lux_data < 300.0F) return lux_data*0.8F;
		else return lux_data;
	}
	return ((uint32_t) lux_data);
}
