/*
 * wifi.c
 *
 *  Created on: Feb 22, 2015
 *      Author: adam
 */

#include "wifi.h"
#include "string.h"
#include "stdio.h"
#include "config.h"
wifi_uart_t wifi_uart_params;
uint8_t rx_buffer[500]= {0};

/**
 * @brief Configure physical interface
 */
int8_t wifi_open()
{
    wifi_uart_params.handle = AS1_Init(NULL);

    /* Test UART interface */
    send_at_cmd("AT\r\n", 4);
    receive_at_cmd_response(rx_buffer, 10);
}

/**
 * @brief Initialization of ESP8266 WiFi module
 * @return
 */
int8_t esp8266_init()
{
	/* Set WiFi mode */
	send_at_cmd("AT+CWMODE=1\r\n", 13);
    receive_at_cmd_response(rx_buffer, 19);
}

/**
 * @brief Connect to the WiFi network
 */
int8_t wifi_network_connect()
{
	/* Connect to AP */
	// Create command
	uint8_t command_buffer[100] = {0};

	memcpy(command_buffer, "AT+CWJAP=\"", strlen("AT+CWJAP=\""));
	memcpy(&command_buffer[strlen("AT+CWJAP=\"")], ACCESS_POINT_NAME,
			strlen(ACCESS_POINT_NAME));
	memcpy(&command_buffer[strlen("AT+CWJAP=\"") + strlen(ACCESS_POINT_NAME)],
			"\",\"", 3);
	memcpy(&command_buffer[strlen("AT+CWJAP=\"") + strlen(ACCESS_POINT_NAME) + 3],
			ACCESS_POINT_PASSWORD, strlen(ACCESS_POINT_PASSWORD));
	memcpy(&command_buffer[strlen("AT+CWJAP=\"") + strlen(ACCESS_POINT_NAME) + 3 +
						   strlen(ACCESS_POINT_PASSWORD)], "\"\r\n",
			strlen("\"\r\n"));

	send_at_cmd(command_buffer,strlen(command_buffer));
    receive_at_cmd_response(rx_buffer, strlen(command_buffer) + 6);

//    int8_t at_cwjap[]= "AT+CWJAP?\r\n";
//    send_at_cmd(at_cwjap, strlen(at_cwjap));
//    receive_at_cmd_response(rx_buffer, 38);

//    /* Query device IP */
//	send_at_cmd("AT+CIFSR\r\n", 10);
//    receive_at_cmd_response(rx_buffer, 81);
}

/**
 * @brief Open TCP/IP socket connection
 */
int8_t wifi_socket_open()
{
	uint32_t var = 0;
	/* Talk to the ThigSpeak */
	uint8_t at_cipmux[] = "AT+CIPMUX=1\r\n";
	send_at_cmd(at_cipmux, strlen(at_cipmux));
	receive_at_cmd_response(rx_buffer, strlen(at_cipmux) + 6);

	uint8_t at_cipstart[]= "AT+CIPSTART=4,\"TCP\",\"184.106.153.149\",80\r\n";
	send_at_cmd(at_cipstart, strlen(at_cipstart));
	receive_at_cmd_response(rx_buffer, strlen(at_cipstart) + 17);
	for (var = 0; var < 10000; ++var) {}
}

int8_t wifi_socket_close()
{
	uint8_t at_cipclose[] = "AT+CIPCLOSE\r\n";
	send_at_cmd(at_cipclose, strlen(at_cipclose));
	receive_at_cmd_response(rx_buffer, 19);
}

int8_t wifi_send_data(uint8_t *data, uint32_t data_size)
{

#define GET_HEADER "GET /update?key="

	uint8_t data_buff[100] = "";

	memcpy(&data_buff[0], GET_HEADER, strlen(GET_HEADER));
	//Append API key
	memcpy(&data_buff[strlen(GET_HEADER)], iot_server_api_write_key,
			strlen(iot_server_api_write_key));
	data_buff[strlen(GET_HEADER)+strlen(iot_server_api_write_key)] = '&';
	//Append API field
	memcpy(&data_buff[strlen(GET_HEADER)+strlen(iot_server_api_write_key)+1],
			iot_server_temperature_field, strlen(iot_server_temperature_field));
	data_buff[strlen(GET_HEADER)+strlen(iot_server_api_write_key) + 1 +
			  strlen(iot_server_temperature_field)] = '=';
	//Append data to be send
	memcpy(&data_buff[strlen(GET_HEADER)+strlen(iot_server_api_write_key) + 1 +
					  strlen(iot_server_temperature_field)+1], data, data_size);
	memcpy(&data_buff[strlen(GET_HEADER)+strlen(iot_server_api_write_key) + 1 +
					  strlen(iot_server_temperature_field)+1+data_size], "\r\n", 2);

	//Send data
	uint8_t at_cipsend[20] = "AT+CIPSEND=4,";
	uint8_t cipsend_len[3]= {0};
	cipsend_len[0] = '0' + (strlen(data_buff)-1)/100;
	cipsend_len[1] = '0' + ((strlen(data_buff)-1) -
			(((strlen(data_buff)-1)/100)*100))/10;
	cipsend_len[2] = '0' + (strlen(data_buff)-1)%10;
	memcpy(&at_cipsend[13], cipsend_len, 3);
	memcpy(&at_cipsend[16], "\r\n", 2);

	send_at_cmd(at_cipsend, strlen(at_cipsend));
	receive_at_cmd_response(rx_buffer, 19);

	send_at_cmd(data_buff, strlen(data_buff));
	receive_at_cmd_response(rx_buffer, strlen(data_buff) +
			strlen("\r\nSEND OK\r\n") + 19);
}
int8_t send_at_cmd(uint8_t* buffer, uint32_t size)
{
    AS1_SendBlock(wifi_uart_params.handle, buffer, size);
//    while(!AS1_GetTxCompleteStatus(wifi_uart_params.handle));
}

int8_t receive_at_cmd_response(uint8_t* buffer, uint32_t size)
{
    AS1_ReceiveBlock(wifi_uart_params.handle, buffer, size);
	while(wifi_uart_params.is_received==FALSE);
	wifi_uart_params.is_received=FALSE;
}

