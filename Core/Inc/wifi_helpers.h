/*
 * wifi_helpers.h
 *
 *  Created on: Mar 6, 2021
 *      Author: devin
 */

#ifndef INC_WIFI_HELPERS_H_
#define INC_WIFI_HELPERS_H_

#include "stm32f4xx_hal.h"
#include "inventek_43362.h"
#include <stdbool.h>




void Wifi_Init();
ES_WIFI_Status_t StartJoinNetwork(char *ssid, char *pw);
void StartTCP(int port);
void StartMQTT();

void SendHelpCommand();
void GetHelpCommand();
ES_WIFI_Status_t WaitForSocketConnection(int);
void ProcessSocket(int socket_id);





#endif /* INC_WIFI_HELPERS_H_ */
