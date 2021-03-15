/*
 * tcp_server.h
 *
 *  Created on: Mar 14, 2021
 *      Author: devin
 */

#ifndef INC_TCP_SERVER_H_
#define INC_TCP_SERVER_H_

#include <stdio.h>
#include <stdbool.h>

#include "wifi_helpers.h"
#include "inventek_43362.h"

void ProcessSocket(int socket_id);

#endif /* INC_TCP_SERVER_H_ */
