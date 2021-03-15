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
#include <stdbool.h>
SPI_HandleTypeDef wifi_spi;
#define BUFFER_SIZE 1200
char spi_response[BUFFER_SIZE];
char full_command[1024];
char *NL = "\n";
int status = WL_IDLE_STATUS;
int sockState[2] = {SOCKET_FREE, SOCKET_FREE};

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

void Wifi_Init()
{
	WifiStartup();
	if(StartJoinNetwork("DevinsHotWifi", "RickAndMorty123") == ES_WIFI_STATUS_OK)
	{
		status = WL_CONNECTED;
	}
	else
	{
		Disconnect();
	}
	StartTCP(5025);
}

int GetSPIResponse(char *rbuf)
{
	uint8_t checkReady = GetCmdDataReady();
	while(!IsDataPhaseReady())
	{
		checkReady = GetCmdDataReady();
//		printf("Wait for CMD/DATARDY to be high again [%d]\r\n", checkReady);

	}
	int length = 1;
	uint16_t wb2[] = { 0x0A0A };
	uint16_t rb2[length];

//	checkReady = GetCmdDataReady();
//	printf("Starting read data phase, checkReady = [%d]\r\n", checkReady);
	Wifi_ChipSelect();

	int i = 0;
	int index = 0;
	while(IsDataPhaseReady())
	{
		HAL_Delay(20);
		HAL_SPI_TransmitReceive(&wifi_spi, &wb2, &rb2, length, HAL_MAX_DELAY);
		checkReady = GetCmdDataReady();

//		printf("i = %d, value = %0x\r\n", i, rb2[0]);
		char first = rb2[0] & 0xff;
		char second = rb2[0] >> 8;

		if(first != 0x15)
		{
			spi_response[index++] = first;
		}
		if(second != 0x15)
		{
			spi_response[index++] = (char)second;
		}

		i++;
	}

	printf("%s\r\n", spi_response);
	Wifi_ChipDeselect();
	return index;
}

ES_WIFI_Status_t ReceiveSPICommand(char *pdata)
{
	int recv_len = 0;
	recv_len = GetSPIResponse(pdata);

	if ((recv_len > 0) && (recv_len < BUFFER_SIZE))
	{
	    *(pdata + recv_len) = 0;
	    if (strstr((char *)pdata, AT_OK_STRING))
	    {
	    	return ES_WIFI_STATUS_OK;
	    }
	    else
	    {
	    	if (strstr((char *)pdata, AT_ERROR_STRING))
	    	{
	    		return ES_WIFI_STATUS_ERROR;
	    	}
	    	else
	    	{
	    		/* Some data still to get. Typically with AP list */
	    		if (recv_len == (int16_t)(BUFFER_SIZE - AT_OK_STRING_LEN - 2))
	    		{
	    			return ES_WIFI_STATUS_REQ_DATA_STAGE;
	    		}
	    	}
		}
	}
	return ES_WIFI_STATUS_IO_ERROR;
}

ES_WIFI_Status_t SendSPICommand()//char *command)
{
	uint8_t checkReady;
	printf("send spi command %s\r\n", full_command);
	int full_length = strlen(full_command);
	int send_length = strlen(full_command)/2;
	memset(spi_response, 0, BUFFER_SIZE);
	bool odd = false;
	if(full_length % 2 == 1)
	{
		send_length++;
		odd = true;
	}
	uint16_t wb[send_length];
	memset(wb, 0, send_length);

	for(int i = 0; i < send_length; i++)
	{
		uint16_t lo = full_command[i*2];
		uint16_t hi = 0;
		if(i == send_length - 1 && odd)
		{
			hi = 0x0A << 8;
		}
		else
		{
			hi = full_command[i*2+1] << 8;
		}
		wb[i] = lo | hi;

//		printf("wb[i] = %0x\r\n", wb[i]);
	}
	uint16_t rb[send_length];

	//Wait for chip to be ready for command
//	printf("waiting for chip to be ready to send spi command\r\n");
	while(!IsCommandPhaseReady())
	{

	}

	Wifi_ChipSelect();
	HAL_SPI_TransmitReceive(&wifi_spi, &wb, &rb, send_length, HAL_MAX_DELAY);
	Wifi_ChipDeselect();

	while(IsCommandPhaseReady())
	{

	}

	return ReceiveSPICommand(spi_response);


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
//	printf("checkReady == [%d], rb = %0x%0x\r\n", checkReady, rb[1], rb[0]);

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
//		printf("checkReady == [%d]\r\n", checkReady);
	}
	while(checkReady == 0);

	Wifi_ChipSelect();
	do
	{
		HAL_SPI_TransmitReceive(&wifi_spi, &wb, &rb, 1, HAL_MAX_DELAY);
		checkReady = (uint8_t)HAL_GPIO_ReadPin(WIFI_DRDY_GPIO_Port, WIFI_DRDY_Pin);
//		printf("checkReady == [%d], rb = %0x%0x\r\n", checkReady, rb[1], rb[0]);
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
//		printf("ChipDeselect == [%d]\r\n", checkReady);
	}
	while(checkReady == 0);

	//Lower NSS to start command
	Wifi_ChipSelect();

	//Transmit command rb should be consistently 0x15;
	HAL_SPI_TransmitReceive(&wifi_spi, &wb, &rb, length, HAL_MAX_DELAY);
	//
	checkReady = (uint8_t)HAL_GPIO_ReadPin(WIFI_DRDY_GPIO_Port, WIFI_DRDY_Pin);
//	printf("sending command checkReady should be high checkReady == [%d]\r\n", checkReady);

//	printf("read buffer should be all 0x15\r\n");
//	for(int i = 0; i < length; i++)
//	{
//		printf("i = %d, value = %0x\r\n", i, rb[i]);
//	}

	Wifi_ChipDeselect();
//	printf("Chip Deselect\r\n\n");
	//Wait for CMD/DATARDY to be high again
	do
	{
		checkReady = (uint8_t)HAL_GPIO_ReadPin(WIFI_DRDY_GPIO_Port, WIFI_DRDY_Pin);
//		printf("Wait for CMD/DATARDY to be high again [%d]\r\n", checkReady);
	}
	while(checkReady == 0);



//	length = 2;
//	uint8_t wb2[] = {0x0A, 0x0A};
//	uint8_t rb2[length];
	length = 1;
	uint16_t wb2[] = { 0x0a0a };
	uint16_t rb2[length];
	checkReady = (uint8_t)HAL_GPIO_ReadPin(WIFI_DRDY_GPIO_Port, WIFI_DRDY_Pin);

//	printf("Starting read data phase, checkReady = [%d]\r\n", checkReady);
	Wifi_ChipSelect();

	int i = 0;
	while(checkReady == 1)
	{
		HAL_SPI_TransmitReceive(&wifi_spi, &wb2, &rb2, length, HAL_MAX_DELAY);
		checkReady = (uint8_t)HAL_GPIO_ReadPin(WIFI_DRDY_GPIO_Port, WIFI_DRDY_Pin);

//		printf("i = %d, value = %0x, %0x\r\n", i, rb2[1], rb2[0]);
		i++;

	}


	Wifi_ChipDeselect();
//	printf("ChipDeselect == [%d]\r\n", checkReady);
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

//		printf("wb[i] = %0x\r\n", wb[i]);
	}
	uint16_t rb[length];

	//Wait for chip to be ready for command
//	printf("waiting for chip to be ready for first command\r\n");
	while(!IsCommandPhaseReady())
	{

	}

//	printf("ready for first command\r\n");

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
//	printf("sending command checkReady should be high checkReady == [%d]\r\n", checkReady);

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
	int index = 0;
	while(IsDataPhaseReady())
	{
		HAL_SPI_TransmitReceive(&wifi_spi, &wb2, &rb2, length, HAL_MAX_DELAY);
		checkReady = GetCmdDataReady();

		printf("i = %d, value = %0x\r\n", i, rb2[0]);
		char first = rb2[0] & 0xff;
		char second = rb2[0] >> 8;
		//		uint16_t first = (uint16_t)rb2[0];// && 0xff;
//		uint16_t second = (*rb2 && 0xff00) >> 8;//   tmp[i*2+1] << 8;

//		printf("wb2[i] = %0x\r\n", wb2[i]);
		if(first != 0x15)
		{
			spi_response[index++] = first;
		}
		if(second != 0x15)
		{

			spi_response[index++] = (char)second;
		}

		i++;
	}

	printf("%s\r\n", spi_response);

	Wifi_ChipDeselect();
	printf("ChipDeselect == [%d]\r\n", checkReady);
}

void SetProductName(char *name)
{
//	uint8_t checkReady;
//	char command[] = "ZN=";
//
//	strcpy(full_command, command);
//	strcat(full_command, name);
//	strcat(full_command, CR);
//
//	int totalLength = strlen(full_command);
////	printf("total length before = %d", totalLength);
//	if(totalLength % 2 == 1)
//	{
//		strcat(full_command, NL);
//	}
//	totalLength = strlen(full_command);
////	printf("total length after = %d", totalLength);
//
//	SendSPICommand(full_command);
}

void SetAPSSID(char *ssid)
{
//	uint8_t checkReady;
//	char command[] = "C1=";
//
//	strcpy(full_command, command);
//	strcat(full_command, ssid);
//	strcat(full_command, CR);

//	int totalLength = strlen(full_command);
//	printf("total length before = %d", totalLength);
//	if(totalLength % 2 == 1)
//	{
//		strcat(full_command, NL);
//	}
//	totalLength = strlen(full_command);
//	printf("total length after = %d", totalLength);
	sprintf((char *)full_command, "%s=%s%s", AT_NET_SET_SSID, ssid, CMD_SUFFIX);
	SendSPICommand();//full_command);
}

void Disconnect()
{
//	uint8_t checkReady;
//	char command[] = "CD";
//
//	strcpy(full_command, command);
//	strcat(full_command, CR);
//
//	int totalLength = strlen(full_command);
//	if(totalLength % 2 == 1)
//	{
//		strcat(full_command, NL);
//	}
//	totalLength = strlen(full_command);
//
	sprintf((char *)full_command, "%s%s", AT_NET_DISCONNECT, CMD_SUFFIX);
	SendSPICommand();//full_command);
}

void SetAPPassword(char *pass)
{
//	uint8_t checkReady;
//	char command[] = "C2=";
//
//	strcpy(full_command, command);
//	strcat(full_command, pass);
//	strcat(full_command, CR);
//
//	int totalLength = strlen(full_command);
////	printf("total length before = %d", totalLength);
//	if(totalLength % 2 == 1)
//	{
//		strcat(full_command, NL);
//	}
//	totalLength = strlen(full_command);
////	printf("total length after = %d", totalLength);
//
//	SendSPICommand(full_command);
	sprintf((char *)full_command, "%s=%s%s", AT_NET_SET_PASSPHRASE, pass, CMD_SUFFIX);
	SendSPICommand();//full_command);
}

void SetAPSecurityMode(ES_WIFI_SecurityType_t mode)
{
//	uint8_t checkReady;
//	char command[] = "C3=";
//
//	strcpy(full_command, command);
//	strcat(full_command, mode);
//	strcat(full_command, CR);
//
//	int totalLength = strlen(full_command);
////	printf("total length before = %d", totalLength);
//	if(totalLength % 2 == 1)
//	{
//		strcat(full_command, NL);
//	}
//	totalLength = strlen(full_command);
////	printf("total length after = %d", totalLength);
//
//	SendSPICommand(full_command);
	sprintf((char *)full_command, "%s=%d%s", AT_NET_SET_SECURITY_TYPE, mode, CMD_SUFFIX);
	SendSPICommand();
}

void SetDHCPSetting(ES_WIFI_IPType_t dhcp)
{
//	uint8_t checkReady;
//	char command[] = "C4=";
//
//	strcpy(full_command, command);
//	strcat(full_command, dhcp);
//	strcat(full_command, CR);
//
//	int totalLength = strlen(full_command);
////	printf("total length before = %d", totalLength);
//	if(totalLength % 2 == 1)
//	{
//		strcat(full_command, NL);
//	}
//	totalLength = strlen(full_command);
////	printf("total length after = %d", totalLength);
//
//	SendSPICommand(full_command);
	sprintf((char *)full_command, "%s=%d%s", AT_NET_SET_DHCP, dhcp, CMD_SUFFIX);
	SendSPICommand();
}



ES_WIFI_Status_t JoinNetwork()
{
//	uint8_t checkReady;
//	char command[] = "C0";
//
//	strcpy(full_command, command);
//	strcat(full_command, CR);
//
//	int totalLength = strlen(full_command);
////	printf("total length before = %d", totalLength);
//	if(totalLength % 2 == 1)
//	{
//		strcat(full_command, NL);
//	}
//	totalLength = strlen(full_command);
////	printf("total length after = %d", totalLength);
//
//	return SendSPICommand(full_command);
	sprintf((char *)full_command, "%s%s", AT_NET_JOIN, CMD_SUFFIX);
	SendSPICommand();
}

void SetSocket(int socket)
{
//	uint8_t checkReady;
//	char command[] = "P0=";
//
//	strcpy(full_command, command);
//	char str[1];
//	sprintf(str, "%d", socket);
//	strcat(full_command, str);
//	strcat(full_command, CR);
//
//	int totalLength = strlen(full_command);
////	printf("total length before = %d", totalLength);
//	if(totalLength % 2 == 1)
//	{
//		strcat(full_command, NL);
//	}
//	totalLength = strlen(full_command);
////	printf("total length after = %d", totalLength);
//
//	SendSPICommand(full_command);
	sprintf((char *)full_command, "%s=%d%s", AT_TR_SET_SOCKET, socket, CMD_SUFFIX);
	SendSPICommand();
}

void SetProtocol(ES_WIFI_ConnType_t protocol)
{
//	uint8_t checkReady;
//	char command[] = "P1=";
//
//	strcpy(full_command, command);
//	strcat(full_command, protocol);
//	strcat(full_command, CR);
//
//	int totalLength = strlen(full_command);
////	printf("total length before = %d", totalLength);
//	if(totalLength % 2 == 1)
//	{
//		strcat(full_command, NL);
//	}
//	totalLength = strlen(full_command);
////	printf("total length after = %d", totalLength);
//
//	SendSPICommand(full_command);
	sprintf((char *)full_command, "%s=%d%s", AT_TR_SET_PROTOCOL, protocol, CMD_SUFFIX);
	SendSPICommand();
}

void SetPort(int port)
{
//	uint8_t checkReady;
//	char command[] = "P2=";
//
//	strcpy(full_command, command);
//	char portString[5];
//	itoa(port, portString, 10);
//	strcat(full_command, portString);
//	strcat(full_command, CR);
//
//	int totalLength = strlen(full_command);
////	printf("total length before = %d", totalLength);
//	if(totalLength % 2 == 1)
//	{
//		strcat(full_command, NL);
//	}
//	totalLength = strlen(full_command);
////	printf("total length after = %d", totalLength);
//
//	SendSPICommand(full_command);
	sprintf((char *)full_command, "%s=%d%s", AT_TR_SET_LOCAL_PORT_NUMBER, port, CMD_SUFFIX);
	SendSPICommand();
}

void StartTCPComm(int socket_id)
{
//	uint8_t checkReady;
//	char command[] = "P5=1";
//
//	strcpy(full_command, command);
//	strcat(full_command, CR);
//
//	int totalLength = strlen(full_command);
////	printf("total length before = %d", totalLength);
//	if(totalLength % 2 == 1)
//	{
//		strcat(full_command, NL);
//	}
//	totalLength = strlen(full_command);
////	printf("total length after = %d", totalLength);
//
//	SendSPICommand(full_command);
	SetSocket(socket_id);
	sprintf((char *)full_command, "%s=1%s", AT_TR_SERVER, CMD_SUFFIX);
	SendSPICommand();

	WaitForSocketConnection(socket_id);
//	CheckSocketConnection(socket_id);
}

void ResetOnClose()
{
	sprintf((char *)full_command, "%s=11%s", AT_TR_SERVER, CMD_SUFFIX);
	SendSPICommand();
}

//void SetSocketToReopen()
//{
////	uint8_t checkReady;
////	char command[] = "P5=10";
////
////	strcpy(full_command, command);
////	strcat(full_command, CR);
////
////	int totalLength = strlen(full_command);
//////	printf("total length before = %d", totalLength);
////	if(totalLength % 2 == 1)
////	{
////		strcat(full_command, NL);
////	}
////	totalLength = strlen(full_command);
//////	printf("total length after = %d", totalLength);
////
////	SendSPICommand(full_command);
//	sprintf((char *)full_command, "%s=1%s", AT_TR_SERVER, CMD_SUFFIX);
//	SendSPICommand();
//}


ES_WIFI_Status_t StartJoinNetwork(char *ssid, char *pw)
{
	SetProductName("devin_test_simple");
	SetAPSSID(ssid);
	SetAPPassword(pw);
	SetAPSecurityMode(ES_WIFI_SEC_WPA2);
	SetDHCPSetting(ES_WIFI_DHCP);
	return JoinNetwork();
}

//void GetStatus()
//{
////	//	while(true)
////	//	{
////	//		GetSPIResponse();
////	//	}
////		printf("poll for data start\r\n");
////
////		uint8_t checkReady;
////		char command[] = "P?";
////
////		strcpy(full_command, command);
////		strcat(full_command, CR);
////
////		int totalLength = strlen(full_command);
////	//	printf("total length before = %d", totalLength);
////		if(totalLength % 2 == 1)
////		{
////			strcat(full_command, NL);
////		}
////		totalLength = strlen(full_command);
////	//	printf("total length after = %d", totalLength);
////
////		SendSPICommand(full_command);
////		printf("poll for data end\r\n");
////
////	//	while(true)
////	//	{
////	//		GetSPIResponse();
////	//	}
//
//}

void GetData(int socket_id)
{
	//	while(true)
//	//	{
//	//		GetSPIResponse();
//	//	}
//		printf("poll for data start\r\n");
//
//		uint8_t checkReady;
//		char command[] = "R0";
//
//		strcpy(full_command, command);
//		strcat(full_command, CR);
//
//		int totalLength = strlen(full_command);
//	//	printf("total length before = %d", totalLength);
//		if(totalLength % 2 == 1)
//		{
//			strcat(full_command, NL);
//		}
//		totalLength = strlen(full_command);
//	//	printf("total length after = %d", totalLength);
//
//		SendSPICommand(full_command);
//		printf("poll for data end\r\n");
//
//	//	while(true)
//	//	{
//	//		GetSPIResponse();
//	//	}

	sprintf((char *)full_command, AT_READ_DATA);
	SendSPICommand();

	if(strstr(spi_response, "-1"))
	{
		sprintf((char *)full_command, "%s=0%s", AT_TR_SERVER, CMD_SUFFIX);

		SendSPICommand();
		sockState[socket_id] = SOCKET_FREE;
	}
	else
	{
		SendResponse("yes please");

	}


}

void SendResponse(char *msg)
{
//	//	while(true)
//	//	{
//	//		GetSPIResponse();
//	//	}
//		printf("poll for data start\r\n");
//
//		uint8_t checkReady;
//		char command[] = "S0\r01256";
//
//		strcpy(full_command, command);
//		strcat(full_command, CR);
//
//		int totalLength = strlen(full_command);
//	//	printf("total length before = %d", totalLength);
//		if(totalLength % 2 == 1)
//		{
//			strcat(full_command, NL);
//		}
//		totalLength = strlen(full_command);
//	//	printf("total length after = %d", totalLength);
//
//		SendSPICommand(full_command);
//		printf("poll for data end\r\n");
//
//	//	while(true)
//	//	{
//	//		GetSPIResponse();
//	//	}
	int msgLen = strlen(msg);// + 3;
	printf("msg sent = %d\r\n", msgLen);
	sprintf((char *)full_command, "%s=%u%s", AT_WRITE_SET_PACKET_SIZE, msgLen, CMD_SUFFIX);
	SendSPICommand();

	sprintf((char *)full_command, "%s=%s%s", AT_WRITE_DATA, CMD_SUFFIX, msg);
	SendSPICommand();
}

//void PollForData()
//{
//	GetStatus();
//	GetData();
//	SendResponse();
//	HAL_Delay(500);
//
//	PollForData();
//}

int GetFreeSocket()
{
	for(int i = 0; i < 2; i++)
	{
		if (sockState[i] == SOCKET_FREE)
		{
		  return i;
		}
	}
	return -1;
}


ES_WIFI_Status_t CheckSocketConnection(int socket_id)
{
//	char *ptr;
	ES_WIFI_Status_t ret = ES_WIFI_STATUS_ERROR;

//	do
//	{
		sprintf((char *)full_command, "%s%s", AT_MESSAGE_READ, CMD_SUFFIX);

		if(SendSPICommand() == ES_WIFI_STATUS_OK)
		{

			if ((strstr((char *)spi_response , "[SOMA]")) && (strstr((char *)spi_response, "[EOMA]")))
			{
				if (strstr((char *)spi_response, "Accepted"))
				{
					printf("%s\r\n", spi_response);
	//				ptr = strtok((char *)spi_response + 2, " ");
	//				ptr = strtok(NULL, " ");
	//				ptr = strtok(NULL, " ");
	//				ptr = strtok(NULL, ":");
					ret = ES_WIFI_STATUS_OK;
					sockState[socket_id] = SOCKET_CONNECTED;
//					break;
				}
			}
		}
		else
		{
			ret = ES_WIFI_STATUS_ERROR;
		}
		HAL_Delay(1000);
//	}
//
//	while(1);


	return ret;
}

ES_WIFI_Status_t WaitForSocketConnection(int socket_id)
{
	while(sockState[socket_id] != SOCKET_CONNECTED)
	{
		CheckSocketConnection(socket_id);
	}
}


void ProcessSocket(int socket_id)
{
//	if(sockState[socket_id] == SOCKET_CONNECTED)
//	{
		GetData(socket_id);
//	}
//	else if(socket_id == SOCKET_FREE)
//	{
////		StartTCPComm(socket_id);
//	}
//	else
//	{
//		CheckSocketConnection(socket_id);
//	}
//	if(sockState[socket_id] == SOCKET_BUSY)
//	{
//		WaitForConnection(socket_id);
//	}
//	else
//	{

//	}
}


void StartTCP(int port)
{
	int sockNum = GetFreeSocket();
	if(sockNum != -1)
	{
		sockState[sockNum] = SOCKET_BUSY;
		SetSocket(sockNum);
		SetProtocol(ES_WIFI_TCP_CONNECTION);
		SetPort(port);
		StartTCPComm(sockNum);
//		PollForData();
		//	SetSocketToReopen();
	}

}
