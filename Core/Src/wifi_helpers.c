/*
 * wifi_helpers.c
 *
 *  Created on: Mar 6, 2021
 *      Author: devin
 */

///Note: All commands to the eS-WIFI module must be post-padded with 0x0A (Line Feed)
//to an even number of bytes. All data from eS-WIFI will be post-padded with 0x15(NAK)
//to an even number of bytes.

#include "wifi_helpers.h"
#include "main.h"

SPI_HandleTypeDef wifi_spi;

void ConnectWifi(SPI_HandleTypeDef *spidef)
{
	wifi_spi = *spidef;
}

//WiFi SPI ChipDeselect
void Wifi_ChipDeselect(void)
{
	HAL_GPIO_WritePin(WIFI_SPI_CSN_GPIO_Port, WIFI_SPI_CSN_Pin, GPIO_PIN_SET);
}

//WiFi SPI ChipSelect
void Wifi_ChipSelect(void)
{
	HAL_GPIO_WritePin(WIFI_SPI_CSN_GPIO_Port, WIFI_SPI_CSN_Pin, GPIO_PIN_RESET);
}

uint8_t GetCmdDataReady()
{
	return (uint8_t)HAL_GPIO_ReadPin(WIFI_DRDY_GPIO_Port, WIFI_DRDY_Pin);
}

bool IsCommandPhaseReady()
{
	if(GetCmdDataReady() != 0)
	{
		return true;
	}
	return false;
}

bool IsDataPhaseReady()
{
	if(GetCmdDataReady() != 0)
	{
		return true;
	}
	return false;
}


void GetVersion()
{
	//Send I?<cr> three bytes plus one 0x0A
	int length = 4;
	uint16_t wb[length];

	wb[0] = '?';
//	wb[0] += 0x8000;
	wb[1] = 'I';
	wb[2] = 0x0A;
	wb[3] = 0x0D;
	uint16_t rb[length];

	Wifi_ChipSelect();
	HAL_SPI_TransmitReceive(&wifi_spi, &wb, &rb, length, HAL_MAX_DELAY);
	Wifi_ChipDeselect();

//	return rb[length -1];
}




void SendHelpCommand()
{
	uint8_t checkReady;
	uint8_t wb[2];
	wb[0] = '?';
	wb[1] = '\n';
	uint8_t rb[2];
	Wifi_ChipSelect();

	HAL_SPI_TransmitReceive(&wifi_spi, &wb, &rb, 1, HAL_MAX_DELAY);
	checkReady = (uint8_t)HAL_GPIO_ReadPin(WIFI_DRDY_GPIO_Port, WIFI_DRDY_Pin);
	printf("checkReady == [%d], rb = %0x%0x\r\n", checkReady, rb[1], rb[0]);

	Wifi_ChipDeselect();
}

void GetHelpCommand()
{
	uint8_t checkReady;
	uint8_t wb[] = {0x0A, 0x0A};
	uint8_t rb[2];


	do
	{
		checkReady = (uint8_t)HAL_GPIO_ReadPin(WIFI_DRDY_GPIO_Port, WIFI_DRDY_Pin);
		printf("checkReady == [%d]\r\n", checkReady);
	}
	while(checkReady == 0);

	Wifi_ChipSelect();
	do
	{
		HAL_SPI_TransmitReceive(&wifi_spi, &wb, &rb, 1, HAL_MAX_DELAY);
		checkReady = (uint8_t)HAL_GPIO_ReadPin(WIFI_DRDY_GPIO_Port, WIFI_DRDY_Pin);
		printf("checkReady == [%d], rb = %0x%0x\r\n", checkReady, rb[1], rb[0]);
	}
	while(checkReady != 0);
	Wifi_ChipDeselect();
}


void WifiStartup()
{
	int length = 1;
//	uint8_t wb[] = {0x0a, 0x0a};
//	uint8_t rb[2];
	uint16_t wb[] = {0x0a0a};
	uint16_t rb[1];
	Wifi_ChipSelect();

	while(IsDataPhaseReady())
	{
		HAL_SPI_TransmitReceive(&wifi_spi, &wb, &rb, length, HAL_MAX_DELAY);
		printf("in first data phase, rb = 0x%0x, 0x%0x\r\n", rb[1], rb[0]);
	}
//	do
//	{
//		HAL_SPI_TransmitReceive(&wifi_spi, &wb, &rb, 1, HAL_MAX_DELAY);
//		checkReady = (uint8_t)HAL_GPIO_ReadPin(WIFI_DRDY_GPIO_Port, WIFI_DRDY_Pin);
//		printf("checkReady == [%d], rb = %0x\r\n", checkReady, rb[0]);
//	}
//	while(checkReady != 0);

//	int length = 4;
//	uint8_t wb2[length];
//	uint8_t rb2[length];
//
//	printf("ChipDeselect == [%d]\r\n", checkReady);
//
//	HAL_SPI_TransmitReceive(&wifi_spi, &wb2, &rb2, length, HAL_MAX_DELAY);
//
//	for(int i = 0; i < 4; i++)
//	{
//		printf("i = %d, value = %0x\r\n", i, rb2[i]);
//	}

	Wifi_ChipDeselect();
}



void GetWifiConnectionStatus()
{
	uint8_t checkReady;
	int length = 4;
	uint8_t wb[] = {'?', 'I', 0x0A, '\r'};//, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A};
	uint8_t rb[length];

	//Wait for chip to be ready for command
	do
	{
		checkReady = (uint8_t)HAL_GPIO_ReadPin(WIFI_DRDY_GPIO_Port, WIFI_DRDY_Pin);
		printf("ChipDeselect == [%d]\r\n", checkReady);
	}
	while(checkReady == 0);

	//Lower NSS to start command
	Wifi_ChipSelect();

	//Transmit command rb should be consistently 0x15;
	HAL_SPI_TransmitReceive(&wifi_spi, &wb, &rb, length, HAL_MAX_DELAY);
	//
	checkReady = (uint8_t)HAL_GPIO_ReadPin(WIFI_DRDY_GPIO_Port, WIFI_DRDY_Pin);
	printf("sending command checkReady should be high checkReady == [%d]\r\n", checkReady);

	printf("read buffer should be all 0x15\r\n");
	for(int i = 0; i < length; i++)
	{
		printf("i = %d, value = %0x\r\n", i, rb[i]);
	}

	Wifi_ChipDeselect();
	printf("Chip Deselect\r\n\n");
	//Wait for CMD/DATARDY to be high again
	do
	{
		checkReady = (uint8_t)HAL_GPIO_ReadPin(WIFI_DRDY_GPIO_Port, WIFI_DRDY_Pin);
		printf("Wait for CMD/DATARDY to be high again [%d]\r\n", checkReady);
	}
	while(checkReady == 0);



//	length = 2;
//	uint8_t wb2[] = {0x0A, 0x0A};
//	uint8_t rb2[length];
	length = 1;
	uint16_t wb2[] = { 0x0a0a };
	uint16_t rb2[length];
	checkReady = (uint8_t)HAL_GPIO_ReadPin(WIFI_DRDY_GPIO_Port, WIFI_DRDY_Pin);

	printf("Starting read data phase, checkReady = [%d]\r\n", checkReady);
	Wifi_ChipSelect();

	int i = 0;
	while(checkReady == 1)
	{
		HAL_SPI_TransmitReceive(&wifi_spi, &wb2, &rb2, length, HAL_MAX_DELAY);
		checkReady = (uint8_t)HAL_GPIO_ReadPin(WIFI_DRDY_GPIO_Port, WIFI_DRDY_Pin);

		printf("i = %d, value = %0x, %0x\r\n", i, rb2[1], rb2[0]);
		i++;

	}


	Wifi_ChipDeselect();
	printf("ChipDeselect == [%d]\r\n", checkReady);
}

void SetSSID()
{
	uint8_t checkReady;
	int length = 2;
//	uint16_t wb[] = {'T', 'M', '\r', '\n'};

	char tmp[] = "MT\r\n";
	uint16_t wb[length];
	memset(wb, 0, length);

	for(int i = 0; i < length; i++)
	{
		uint16_t lo = tmp[i*2];
		uint16_t hi = tmp[i*2+1] << 8;
		wb[i] = lo | hi;

		printf("wb[i] = %0x\r\n", wb[i]);
	}
	uint16_t rb[length];

	//Wait for chip to be ready for command
	printf("waiting for chip to be ready for first command\r\n");
	while(!IsCommandPhaseReady())
	{

	}

	printf("ready for first command\r\n");

//	//Wait for chip to be ready for command
//	do
//	{
//		checkReady = (uint8_t)HAL_GPIO_ReadPin(WIFI_DRDY_GPIO_Port, WIFI_DRDY_Pin);
//		printf("ChipDeselect == [%d]\r\n", checkReady);
//	}
//	while(checkReady == 0);

	//Lower NSS to start command
	Wifi_ChipSelect();

	//Transmit command rb should be consistently 0x15;
	HAL_SPI_TransmitReceive(&wifi_spi, &wb, &rb, length, HAL_MAX_DELAY);
	//
	checkReady = (uint8_t)HAL_GPIO_ReadPin(WIFI_DRDY_GPIO_Port, WIFI_DRDY_Pin);
	printf("sending command checkReady should be high checkReady == [%d]\r\n", checkReady);

	printf("read buffer should be all 0x15\r\n");
	for(int i = 0; i < length; i++)
	{
		printf("i = %d, value = %0x\r\n", i, rb[i]);
	}
	checkReady = GetCmdDataReady();
	printf("before chip deselect, data_ready = [%d]\r\n\n", checkReady);

	Wifi_ChipDeselect();
	checkReady = GetCmdDataReady();
	printf("post Chip Deselect = [%d]\r\n\n", checkReady);
	//Wait for CMD/DATARDY to be high again


	HAL_Delay(1);
	checkReady = GetCmdDataReady();
	printf("slight delay checkReady = [%d]\r\n\n", checkReady);

	while(!IsDataPhaseReady())
	{
		checkReady = GetCmdDataReady();
		printf("Wait for CMD/DATARDY to be high again [%d]\r\n", checkReady);

	}
//	do
//	{
//		checkReady = (uint8_t)HAL_GPIO_ReadPin(WIFI_DRDY_GPIO_Port, WIFI_DRDY_Pin);
//		printf("Wait for CMD/DATARDY to be high again [%d]\r\n", checkReady);
//	}
//	while(checkReady == 0);



//	length = 2;
//	uint8_t wb2[] = {0x0A, 0x0A};
//	uint8_t rb2[length];
	length = 1;
	uint16_t wb2[] = { 0x0A0A };
	uint16_t rb2[length];

	checkReady = GetCmdDataReady();
	printf("Starting read data phase, checkReady = [%d]\r\n", checkReady);
	Wifi_ChipSelect();

	int i = 0;
	while(IsDataPhaseReady())
	{
		HAL_SPI_TransmitReceive(&wifi_spi, &wb2, &rb2, length, HAL_MAX_DELAY);
		checkReady = GetCmdDataReady();

		printf("i = %d, value = %0x, %0x\r\n", i, rb2[0]);
		i++;

	}


	Wifi_ChipDeselect();
	printf("ChipDeselect == [%d]\r\n", checkReady);
}
