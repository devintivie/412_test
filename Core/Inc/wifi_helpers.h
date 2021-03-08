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

void ConnectWifi(SPI_HandleTypeDef *spidef);
void WifiStartup();
void GetWifiConnectionStatus();
void GetVersion();
void SetSSID();

void SendHelpCommand();

void GetHelpCommand();



#endif /* INC_WIFI_HELPERS_H_ */
