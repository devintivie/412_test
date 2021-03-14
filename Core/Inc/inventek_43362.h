/*
 * inventek_43362.h
 *
 *  Created on: Mar 6, 2021
 *      Author: devin
 */

#ifndef INC_INVENTEK_43362_H_
#define INC_INVENTEK_43362_H_

/* Exported macro-------------------------------------------------------------*/

#define AT_OK_STRING                     "\r\nOK\r\n> "
#define AT_OK_STRING_LEN                 (sizeof(AT_OK_STRING) - 1)
#define AT_ERROR_STRING                  "\r\nERROR"
#define AT_ERROR_STRING_LEN              40

/* List of AT Command                                                        */
/* -------------------------access point command---------------------------- */
#define AT_AP_ACTIVATE                   "A0"
#define AT_AP_SET_SECURITY_MODE          "A1"
#define AT_AP_SET_SECURITY_KEY           "A2"
#define AT_AP_GET_DHCP_CACHE_ADDR        "AA"
#define AT_AP_SET_CHANNEL                "AC"
#define AT_AP_ACTIVATE_DIRECT_MODE       "AD"
#define AT_AP_EXIT_DIRECT_MODE           "AE"
#define AT_AP_SET_LEASE_TIME             "AL"
#define AT_AP_GET_RSSI                   "AR"
#define AT_AP_SET_SSID                   "AS"
#define AT_AP_SET_MAX_CLIENT             "AT"
#define AT_AP_SHOW_SETTING               "A?"

/* ---------------------------spi command----------------------------------- */
#define AT_SPI_SET_MODE                  "B2"
#define AT_SPI_SET_READ_PIN              "B3"

#define AT_SHOW_COMM_INTERFACE           "B?"

/* ---------------------------network command-------------------------------- */
#define AT_NET_JOIN                      "C0"
#define AT_NET_SET_SSID                  "C1"
#define AT_NET_SET_PASSPHRASE            "C2"
#define AT_NET_SET_SECURITY_TYPE         "C3"
#define AT_NET_SET_DHCP                  "C4"
#define AT_NET_SET_IP_VER                "C5"
#define AT_NET_SET_IP_ADDR               "C6"
#define AT_NET_SET_IP_MASK               "C7"
#define AT_NET_SET_GATEWAY               "C8"
#define AT_NET_SET_FIRST_DNS             "C9"
#define AT_NET_SET_SECOND_DNS            "CA"
#define AT_NET_SET_JOIN_RETRY_CNT        "CB"
#define AT_NET_AUTO_CONNECT              "CC"
#define AT_NET_DISCONNECT                "CD"
#define AT_NET_SET_AUTH_TYPE             "CE"
#define AT_NET_PACKET_FILTER             "CF"
#define AT_NET_IGMP_GROUP                "CJ"
#define AT_NET_MAC_ALLOW_LIST            "CM"
#define AT_NET_SET_COUNTRY_CODE          "CN"
#define AT_NET_GET_RSSI                  "CR"
#define AT_NET_GET_CONNECTION_STATUS     "CS"
#define AT_NET_CONNECT_WPS               "CW"
#define AT_NET_SHOW_SETTINGS             "C?"

/* ---------------------------dns command----------------------------------- */
#define AT_DNS_LOOKUP                    "D0"
#define AT_ENABLE_mDNS                   "D1"
#define AT_ENABLE_mDNS_SERVICE           "D2"

#define AT_KEEP_ALIVE                    "E2"

/* ---------------------------scan command---------------------------------- */
#define AT_SCAN                          "F0"
#define AT_SCAN_REPEAT_CNT               "F1"
#define AT_SCAN_SET_DELAY_MS             "F2"
#define AT_SCAN_SET_CHANNEL              "F3"
#define AT_SCAN_SET_BBSID                "F4"
#define AT_SCAN_SET_SSID                 "F5"
#define AT_SCAN_SHOW_SETTINGS            "F?"

/* ---------------------------GPIO command---------------------------------- */
#define AT_GPIO_READ_MODE                "G2"
#define AT_GPIO_WRITE                    "G3"
#define AT_GPIO_SETUP                    "G4"
#define AT_GPIO_SHOW_SETTINGS            "G?"

#define AT_API_SHOW_SETTINGS             "I?"

/* ---------------------------message command------------------------------- */
#define AT_MESSAGE_TEST_SERIAL_FLASH     "MF"
#define AT_MESSAGE_READ                  "MR"
#define AT_MESSAGE_DELETE                "MS"
#define AT_MESSAGE_SET                   "MT"

/* --------------------------transport command------------------------------ */
#define AT_TR_SET_SOCKET                 "P0"
#define AT_TR_SET_PROTOCOL               "P1"
#define AT_TR_SET_LOCAL_PORT_NUMBER      "P2"
#define AT_TR_SET_REMOTE_HOST_IP_ADDR    "P3"
#define AT_TR_SET_REMOTE_PORT_NUMBER     "P4"
#define AT_TR_SERVER                     "P5"
#define AT_TR_CLIENT                     "P6"
#define AT_TR_REQUEST_TCP_LOOP           "P7"
#define AT_TR_SET_LISTEN_BACKLOGS        "P8"
#define AT_TR_SET_CERTIFICATION_VERIF    "P9"
#define AT_TR_SET_CCA                    "PA"
#define AT_TR_SET_CCA_RESULTS            "PB"
#define AT_TR_SECURITY_CERTIFICATES      "PC"
#define AT_TR_SECURITY_KEYS              "PD"
#define AT_TR_TCP_KEEP_ALIVE             "PK"
#define AT_TR_PBM_READ_MODE              "PR"
#define AT_TR_ETHERNET_PACKET_STAT       "PS"
#define AT_TR_SEQUENCING                 "PT"
#define AT_TR_PBM_WRITE_PACKET           "PW"
#define AT_TR_SET_TCP_MESSAGE_TIMEOUT    "PY"
#define AT_TR_SHOW_SETTNGS               "P?"

/* --------------------------read command----------------------------------- */
#define AT_READ_DATA                     "R0=\r"
#define AT_READ_SET_PACKET_SIZE          "R1"
#define AT_READ_SET_TIMEOUT_MS           "R2"
#define AT_READ_RECEIVE_MODE             "R3"
#define AT_READ_SHOW_SETTINGS            "R?"

/* --------------------------write command---------------------------------- */
#define AT_WRITE_DATA                    "S0"
#define AT_WRITE_SET_PACKET_SIZE         "S1"
#define AT_WRITE_SET_TIMEOUT             "S2"
#define AT_WRITE_DATA_WITH_LENGTH        "S3"
#define AT_WRITE_SHOW_SETTINGS           "S?"

/* --------------------------ping command----------------------------------- */
#define AT_PING_TARGET_ADDR              "T0"
#define AT_PING_SET_TARGET_ADDR          "T1"
#define AT_PING_SET_REPEAT_COUNT         "T2"
#define AT_PING_SET_DELAY_MS             "T3"
#define AT_PING_SHOW_SETTINGS            "T?"

/* --------------------------uart command----------------------------------- */
#define AT_UART_ENABLE                   "U0"
#define AT_UART_SET_BAUDRATE             "U2"
#define AT_UART_SHOW_SETTINGS            "U?"

#define AT_WLAN_GPIO                     "WL"

/* --------------------------system command--------------------------------- */
#define AT_SYS_SET_RESET_FACTORY         "Z0"
#define AT_SYS_SAVE_SETTINGS             "Z1"
#define AT_SYS_CLEAR_SETTINGS            "Z2"
#define AT_SYS_SET_FACTORY               "Z3"
#define AT_SYS_SET_MAC_ADDR              "Z4"
#define AT_SYS_GET_MAC_ADDR              "Z5"
#define AT_SEYS_SET_AP_IP_ADDR           "Z6"
#define AT_SYS_SET_WPS_PIN               "Z7"
#define AT_SYS_GET_WPS_PIN               "Z8"
#define AT_SYS_SET_USB                   "Z9"
#define AT_SYS_CLEAR_FACTOR_LOCK         "ZC"
#define AT_SYS_FLASH_DUMP                "ZD"
#define AT_SYS_SET_FACTORY_LOCK          "ZF"
#define AT_SYS_SET_PRODUCT_NAME          "ZN"
#define AT_SYS_OTA_GET_URL               "ZO"
#define AT_SYS_PWR_MNGT                  "ZP"
#define AT_SYS_RESET_MODULE              "ZR"
#define AT_SYS_FIRMWARE_UPGRADE          "ZU"
#define AT_SYS_SET_OTA_METHOD            "ZV"
#define AT_SYS_SHOW_SETTINGS             "Z?"

#define SUFFIX_CMD                       "\r"

#endif /* INC_INVENTEK_43362_H_ */
