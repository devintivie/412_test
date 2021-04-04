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
	StartMQTT();

	StartTCP(80);
}

int GetSPIResponse(char *rbuf)
{
//	uint8_t checkReady = GetCmdDataReady();
	while(!IsDataPhaseReady())
	{
//		checkReady = GetCmdDataReady();
	}
	int length = 1;
	uint16_t wb2[] = { 0x0A0A };
	uint16_t rb2[length];

	Wifi_ChipSelect();

	int i = 0;
	int index = 0;
	while(IsDataPhaseReady())
	{
		HAL_Delay(20);
		HAL_SPI_TransmitReceive(&wifi_spi, &wb2, &rb2, length, HAL_MAX_DELAY);

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

	}
	uint16_t rb[send_length];

	//Wait for chip to be ready for command
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


void WifiStartup()
{
	int length = 1;
	uint16_t wb[] = {0x0a0a};
	uint16_t rb[1];
	Wifi_ChipSelect();

	while(IsDataPhaseReady())
	{
		HAL_SPI_TransmitReceive(&wifi_spi, &wb, &rb, length, HAL_MAX_DELAY);
		printf("in first data phase, rb = 0x%0x, 0x%0x\r\n", rb[1], rb[0]);
	}
	Wifi_ChipDeselect();
}


void SetProductName(char *name)
{
	sprintf((char *)full_command, "%s=%s%s", AT_SYS_SET_PRODUCT_NAME, name, CMD_SUFFIX);
	SendSPICommand();
}

void SetAPSSID(char *ssid)
{
	sprintf((char *)full_command, "%s=%s%s", AT_NET_SET_SSID, ssid, CMD_SUFFIX);
	SendSPICommand();
}

void Disconnect()
{
	sprintf((char *)full_command, "%s%s", AT_NET_DISCONNECT, CMD_SUFFIX);
	SendSPICommand();
}

void SetAPPassword(char *pass)
{
	sprintf((char *)full_command, "%s=%s%s", AT_NET_SET_PASSPHRASE, pass, CMD_SUFFIX);
	SendSPICommand();
}

void SetAPSecurityMode(ES_WIFI_SecurityType_t mode)
{
	sprintf((char *)full_command, "%s=%d%s", AT_NET_SET_SECURITY_TYPE, mode, CMD_SUFFIX);
	SendSPICommand();
}

void SetDHCPSetting(ES_WIFI_IPType_t dhcp)
{
	sprintf((char *)full_command, "%s=%d%s", AT_NET_SET_DHCP, dhcp, CMD_SUFFIX);
	SendSPICommand();
}



ES_WIFI_Status_t JoinNetwork()
{
	sprintf((char *)full_command, "%s%s", AT_NET_JOIN, CMD_SUFFIX);
	SendSPICommand();
}

void SetSocket(int socket)
{
	sprintf((char *)full_command, "%s=%d%s", AT_TR_SET_SOCKET, socket, CMD_SUFFIX);
	SendSPICommand();
}

void SetProtocol(ES_WIFI_ConnType_t protocol)
{

	sprintf((char *)full_command, "%s=%d%s", AT_TR_SET_PROTOCOL, protocol, CMD_SUFFIX);
	SendSPICommand();
}

void SetPort(int port)
{
	sprintf((char *)full_command, "%s=%d%s", AT_TR_SET_LOCAL_PORT_NUMBER, port, CMD_SUFFIX);
	SendSPICommand();
}

void StartTCPComm(int socket_id)
{
	SetSocket(socket_id);
	sprintf((char *)full_command, "%s=11%s", AT_TR_SERVER, CMD_SUFFIX);
	SendSPICommand();

//	WaitForSocketConnection(socket_id);
//	CheckSocketConnection(socket_id);
}

void ResetOnClose()
{
	sprintf((char *)full_command, "%s=11%s", AT_TR_SERVER, CMD_SUFFIX);
	SendSPICommand();
}


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
	sprintf((char *)full_command, AT_READ_DATA);
	SendSPICommand();

//	if(strstr(spi_response, "help?"))
//	{
		SendResponse("yes please");
//		SendResponse("HTTP/1.1 200 OK\n");
//		SendResponse("Content-Length: 222\n");
//	  SendResponse("Connection: Close\n");
//	  SendResponse("Content-Type: text/html; charset=iso-8859-1\n");
//	  SendResponse("\n");
//
//	  // the content of the HTTP response follows the header:
//	  SendResponse("<html><head>\n<title>WiFi Web Server</title>\n</head>");
//	  SendResponse("<body>\n<h1>ST Web Server</h1>\n");
//	  SendResponse("Click <a href=\"/H\">here</a> turn the LED on LED_BUILTIN on<br>\n");
//	  SendResponse("Click <a href=\"/L\">here</a> turn the LED on LED_BUILTIN off<br>");
//
//	  // The HTTP response ends with another blank line:
//	  SendResponse("</body></html>\n");
//
//	}
//	if(strstr(spi_response, AT_OK_STRING))
//	{
//		return;
//	}
//	else if(strstr(spi_response, "-1"))
//	{
//		SetSocket(socket_id);
//		sprintf((char *)full_command, "%s=10%s", AT_TR_SERVER, CMD_SUFFIX);
//		SendSPICommand();
////		sockState[socket_id] = SOCKET_FREE;
//	}
//	else
//	{
//		SendResponse("yes please");
//	}


}

void SendResponse(char *msg)
{
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

void ShowSettings(int socket_id)
{
	sprintf((char *)full_command, "%s%s", AT_TR_SHOW_SETTNGS, CMD_SUFFIX);
	SendSPICommand();
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
//	printf("socket %d has state %d", socket_id, sockState[socket_id]);
//	ES_WIFI_Status_t state = CheckSocketConnection(socket_id);
	SetSocket(socket_id);
	ShowSettings(socket_id);
	printf("socket %d, settings = %s", socket_id, spi_response);   //"socket %d state = %d\r\n", socket_id, state);
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

void SetPublishTopic(char *topic)
{
	sprintf((char *)full_command, "%s=0,%s%s", AT_TR_MQTT_TTRIBUTES, topic, CMD_SUFFIX);
	SendSPICommand();
}

void SetSubscribeTopic(char *topic)
{
	sprintf((char *)full_command, "%s=1,%s%s", AT_TR_MQTT_TTRIBUTES, topic, CMD_SUFFIX);
	SendSPICommand();
}

void SetSecurity(int security)
{
	sprintf((char *)full_command, "%s=2,%d%s", AT_TR_MQTT_TTRIBUTES, security, CMD_SUFFIX);
	SendSPICommand();
}

void SetUsername(char *username)
{
	sprintf((char *)full_command, "%s=3,%s%s", AT_TR_MQTT_TTRIBUTES, username, CMD_SUFFIX);
	SendSPICommand();
}

void SetPassword(char *pw)
{
	sprintf((char *)full_command, "%s=4,%s%s", AT_TR_MQTT_TTRIBUTES, pw, CMD_SUFFIX);
	SendSPICommand();
}

void SetClientID(char *clientID)
{
	sprintf((char *)full_command, "%s=5,%s%s", AT_TR_MQTT_TTRIBUTES, clientID, CMD_SUFFIX);
	SendSPICommand();
}

void SetHostAddress(char *address)
{
	sprintf((char *)full_command, "%s=%s%s", AT_TR_SET_REMOTE_HOST_IP_ADDR, address, CMD_SUFFIX);
	SendSPICommand();
}

void StartConnection()
{
	sprintf((char *)full_command, "%s=1%s", AT_TR_CLIENT, CMD_SUFFIX);
	SendSPICommand();
}

void StartMQTT()
{
	int sockNum = 0;
	SetSocket(sockNum);
	SetProtocol(ES_WIFI_MQTT_CONNECTION);
	SetPublishTopic("master/control/wifi");
	SetSubscribeTopic("rasp/control/all");
	SetSecurity(ES_WIFI_MQTT_SEC_USER_PASS);
	SetUsername("devin");
	SetPassword("Ikorgil19");
	SetClientID("wifi board");
	SetHostAddress("192.168.68.109");
	SetPort(5672);
	StartConnection();
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
