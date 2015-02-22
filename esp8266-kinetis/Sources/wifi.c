/*
 * wifi.c
 *
 *  Created on: Feb 22, 2015
 *      Author: adam
 */

#include "wifi.h"

wifi_uart_t wifi_uart_params;

int8_t wifi_open()
{
    wifi_uart_params.handle = AS1_Init(NULL);
}

int8_t wifi_send(uint8_t* buffer, uint32_t size)
{
    AS1_SendBlock(wifi_uart_params.handle, "AT\r\n", 4);
    while(wifi_uart_params.is_send != TRUE);
}

int8_t wifi_receive(uint8_t* buffer, uint32_t size)
{
    AS1_ReceiveBlock(wifi_uart_params.handle, buffer, size);
}

