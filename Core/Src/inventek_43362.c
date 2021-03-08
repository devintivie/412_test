/*
 * inventek_43362.c
 *
 *  Created on: Mar 6, 2021
 *      Author: devin
 */
///Note: All commands to the eS-WIFI module must be post-padded with 0x0A (Line Feed)
//to an even number of bytes. All data from eS-WIFI will be post-padded with 0x15(NAK)
//to an even number of bytes.

/*
 * SPI Slave Interface:
 * Clock Rate : 	20 MHz max
 * Width:	16-bit
 * Mode 0
 * Endian: little
 *
 */

#include "inventek_43362.h"
