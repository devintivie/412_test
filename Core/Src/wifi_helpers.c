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
#include "inventek_43362.h"

void Wifi_Init()
{
	WifiStartup();
	if(StartJoinNetwork("DevinsHotWifi", "RickAndMorty123") == ES_WIFI_STATUS_OK)
	{
		SetNetworkStatus(WL_CONNECTED);
		//	StartMQTT();

		StartTCP(80);
	}
	else
	{
		NetworkDisconnect();
	}

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




//void PollForData()
//{
//	GetStatus();
//	GetData();
//	SendResponse();
//	HAL_Delay(500);
//
//	PollForData();
//}


//ES_WIFI_Status_t WaitForSocketConnection(int socket_id)
//{
//	while(sockState[socket_id] != SOCKET_CONNECTED)
//	{
//		CheckSocketConnection(socket_id);
//	}
//}


void ProcessSocket(int socket_id)
{
//	if(sockState[socket_id] == SOCKET_CONNECTED)
//	{
//	printf("socket %d has state %d", socket_id, sockState[socket_id]);
//	ES_WIFI_Status_t state = CheckSocketConnection(socket_id);
	SetSocket(socket_id);
	ShowSettings(socket_id);
//	printf("socket %d, settings = %s", socket_id, spi_response);   //"socket %d state = %d\r\n", socket_id, state);
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
		SetKeepAlive(1, 3000);
		SetSocketState(sockNum, SOCKET_BUSY);
		SetSocket(sockNum);
		SetProtocol(ES_WIFI_TCP_CONNECTION);
		SetPort(port);
		StartTCPComm(sockNum);
//		PollForData();
		//	SetSocketToReopen();
	}

}
