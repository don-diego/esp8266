/*
 * wifi.h
 *
 *  Created on: Feb 22, 2015
 *      Author: adam
 */

#ifndef WIFI_H_
#define WIFI_H_

#include "PE_Types.h"
#include "Events.h"
#include "AS1.h"

typedef struct{
    LDD_TDeviceData *handle;
    bool is_send;
    bool is_received;
}wifi_uart_t;

int8_t wifi_open();
int8_t wifi_send(uint8_t* buffer, uint32_t size);
int8_t wifi_receive(uint8_t* buffer, uint32_t size);

#endif /* WIFI_H_ */
