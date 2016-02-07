/*
 * si7005.c
 *
 *  Created on: 14 gru 2014
 *      Author: adam
 */

#include "si7005.h"
#include "SI7005_I2C.h"
#include "Cpu.h"

#include "PE_Types.h"
#include "PE_LDD.h"

volatile bool si7005_data_sent_flag = FALSE;
volatile bool si7005_data_receive_flag = FALSE;

extern LDD_TDeviceData *si7005_device_data_p;

static si7005_status_code_e si7005_write_register(uint8_t* data_in_p, uint32_t data_in_size);
static si7005_status_code_e si7005_poll_rdy();
static si7005_status_code_e si7005_read_data(uint8_t *read_buffer, uint32_t read_length);

uint8_t read_rH(uint8_t *rH)
{
    uint16_t retcode = 0;
	uint8_t tx_buffer[] = {0x03, 0x01};
	uint8_t rx_buffer[2] = {0};
	uint16_t humidity = 0;

	/* Start measurement
	 * Set START (D0) in CONFIG to begin a new conversion */
	si7005_write_register((uint8_t *)&tx_buffer, sizeof(tx_buffer));

	/* Poll RDY (D0) in STATUS (register 0) until it is low (= 0) */
	si7005_poll_rdy();

	/*Read the upper and lower bytes of the RH value from DATAh and DATAl (registers 0x01 and 0x02),
	respectively*/
	si7005_read_data((uint8_t *)&rx_buffer, 2);

	/*Convert the RH value to %RH*/
	humidity |= ((rx_buffer[0]<<4) | (rx_buffer[1]>>4));
	*rH = (uint8_t)((humidity/16) - 24);
	/*Apply temperature compensation and/or linearization*/
	return retcode;
}

uint8_t read_temperature(uint8_t *temperature)
{
	uint16_t retcode = 0;
	uint8_t tx_buffer[] = {0x03, 0x11};
	uint8_t rx_buffer[2] = {0};
	uint16_t tempr = 0;

	/* Start measurement
	 * Set START (D0) in CONFIG to begin a new conversion */
	si7005_write_register((uint8_t *)&tx_buffer, sizeof(tx_buffer));

	/* Poll RDY (D0) in STATUS (register 0) until it is low (= 0) */
	si7005_poll_rdy();

	/*Read the upper and lower bytes of the RH value from DATAh and DATAl (registers 0x01 and 0x02),
	respectively*/
	si7005_read_data((uint8_t *)&rx_buffer, 2);

	/*Convert the read value to temperature in Celcius*/
	tempr |= ((rx_buffer[0]<<6) | (rx_buffer[1]>>2));
	*temperature = (uint8_t)((tempr/32) - 50);
	/*Apply temperature compensation and/or linearization*/
	return retcode;
}

si7005_status_code_e si7005_open(LDD_TDeviceData **si7005_device_p)
{
	*si7005_device_p = SI7005_I2C_Init(NULL);

	return SI7005_OK;
}

static si7005_status_code_e si7005_write_register(uint8_t *data_in_p, uint32_t data_in_size)
{
	SI7005_I2C_MasterSendBlock(si7005_device_data_p, data_in_p, 2, LDD_I2C_SEND_STOP);
	while (!si7005_data_sent_flag) {} /* Wait until data is sent */
	si7005_data_sent_flag = FALSE;

	return SI7005_OK;
}

static si7005_status_code_e si7005_poll_rdy()
{
	uint32_t i = 0;
	uint8_t tx_buffer[2] = {0};
	uint8_t rx_buffer[2] = {0};
	do {
		for (i = 0; i < 1000; i++);

		tx_buffer[0] = 0x00; // Address of STATUS register
		tx_buffer[1] = 0x00;
		SI7005_I2C_MasterSendBlock(si7005_device_data_p, &tx_buffer, 1, LDD_I2C_NO_SEND_STOP);
		while (!si7005_data_sent_flag) {} /* Wait until data is sent */
		si7005_data_sent_flag = FALSE;

		SI7005_I2C_MasterReceiveBlock(si7005_device_data_p, &rx_buffer, 1, LDD_I2C_SEND_STOP);
		while (!si7005_data_receive_flag) {} /* Wait until data is received */
		si7005_data_receive_flag = FALSE;
	}while (rx_buffer[0] & 0x01);

	return SI7005_OK;
}

static si7005_status_code_e si7005_read_data(uint8_t *read_buffer, uint32_t read_length)
{
	uint32_t i = 0;
	uint8_t tx_buffer[2] = {0};

	for (i = 0; i < 1000; i++);

	tx_buffer[0] = 0x01; // Address of data H register
	SI7005_I2C_MasterSendBlock(si7005_device_data_p, &tx_buffer, 1, LDD_I2C_NO_SEND_STOP);
	while (!si7005_data_sent_flag) {} /* Wait until data is sent */
	si7005_data_sent_flag = FALSE;

	SI7005_I2C_MasterReceiveBlock(si7005_device_data_p, read_buffer, 2, LDD_I2C_SEND_STOP);
	while (!si7005_data_receive_flag) {} /* Wait until data is received received */
	si7005_data_receive_flag = FALSE;

	return SI7005_OK;
}
