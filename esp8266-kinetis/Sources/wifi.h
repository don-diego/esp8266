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

// ESP8266 AT commands
#define AT "AT"
#define AT_CWMODE	"AT+CWMODE"
#define AT_CWJAP	"AT+CWJAP"
#define AT_CIFSR	"AT+CIFSR"
#define AT_CIPMUX	"AT+CIPMUX"
#define AT_CIPSTART	"AT+CIPSTART"
#define AT_CIPCLOSE	"AT+CIPCLOSE"
#define AT_CIPSEND	"AT+CIPSEND"


typedef struct{
    LDD_TDeviceData *handle;
    bool is_send;
    bool is_received;
}wifi_uart_t;

int8_t wifi_open();
int8_t esp8266_init();
int8_t wifi_network_connect();
int8_t send_at_cmd(uint8_t* buffer, uint32_t size);
int8_t receive_at_cmd_response(uint8_t* buffer, uint32_t size);
int8_t wifi_socket_open();
int8_t wifi_socket_close();
int8_t wifi_send_data(uint8_t *data, uint32_t data_size, uint8_t *data2, uint32_t data2_size, uint8_t *data3, uint32_t data3_size);

#endif /* WIFI_H_ */
