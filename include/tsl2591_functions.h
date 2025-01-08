/*
Author: Gabriel Rodgers
Date: 12/30/2024
*/

#ifndef TSL2591_FUNCTIONS_H
#define TSL2591_FUNCTIONS_H

//enumerations to avoid magic numbers
//Integration time
typedef enum {
	atime_100ms = 0x00,
	atime_200ms = 0x01,
	atime_300ms = 0x02,
	atime_400ms = 0x03,
	atime_500ms = 0x04,
	atime_600ms = 0x05,
} atime_t;

//ALS gain
typedef enum {
	again_low = 0x00,
	again_medium = 0x10,
	again_high = 0x20,
	again_maximum = 0x30,
} again_t;

void tsl2591_write_settings(again_t AGAIN, atime_t ATIME);
uint32_t rawdata_to_lux(const uint32_t raw_data, again_t AGAIN, atime_t ATIME);

#endif /* TSL2591_FUNCTIONS_H */
