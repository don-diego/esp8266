/*
 * si7005.h
 *
 *  Created on: 14 gru 2014
 *      Author: adam
 */

#ifndef SI7005_H_
#define SI7005_H_

#include "stdint.h"

#include "PE_Types.h"
#include "PE_LDD.h"

typedef enum {
	SI7005_OK = 0,
	SI7005_ERROR
} si7005_status_code_e;

si7005_status_code_e si7005_open(LDD_TDeviceData **si7005_device_data_p);
uint8_t read_rH(uint8_t *rH);
uint8_t read_temperature(uint8_t *temperature);

#endif /* SI7005_H_ */
