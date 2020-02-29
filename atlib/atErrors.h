/*
kody bledow - dla wszystkich systemow: PC,PI,X3
*/

#ifndef ATERRORS_H
#define ATERRORS_H

//---------------------------------------
//poziomy raportowania
//---------------------------------------
#define AT_LOG_ERRORS       1   //raportuje tylko bledy
#define AT_LOG_WORK         2   //praca u klienta
#define AT_LOG_DEBUG        4   //działanie modułów programu
#define AT_LOG_CLASSMAIN    5   //raportuje dziadzialanie glownych metod w klasach
#define AT_LOG_CLASSDEV     6   //raportuje dzialanie metod w klasach podrzenych
#define AT_LOG_CLASSPRIVATE 7   //raportuje dzialanie metod prywatnych w klasach

//---------------------------------------//
//kody bledow                            //
//---------------------------------------//

//bledy ogolne----------------------------------------
#define at_Konfiguracja_ERROR_pole_not_found    0x0001
#define at_Konfiguracja_ERROR_pole_corupted     0x0002
#define at_Konfiguracja_ERROR_double_format     0x0003
#define at_Konfiguracja_ERROR_int_format        0x0004
#define at_Konfiguracja_ERROR_hexint_format     0x0005

//bledy generowane na poziomie PI---------------------
#define ATERROR_BCM2835_INIT					0x1001
#define ATERROR_BCM2835_BEGIN					0x1002

#define ATERROR_SPI_OPEN						0x1101
#define ATERROR_SPI_BEGIN						0x1102
#define ATERROR_SPI_DEV_OPEN					0x1103
#define ATERROR_SPI_DEV_BCM2835					0x1104
#define ATERROR_SPI_DEV_GO						0x1105
#define ATERROR_SPI_DEV_CNF						0x1106 //brak lpiku konfiguracji

#define ATERROR_ADS1256_ID						0x1110
#define ATERROR_ADS1256_WR_MODE					0x1111
#define ATERROR_ADS1256_RD_MODE					0x1112
#define ATERROR_ADS1256_WR_BITS					0x1113
#define ATERROR_ADS1256_RD_BITS					0x1114
#define ATERROR_ADS1256_WR_SPEED				0x1115
#define ATERROR_ADS1256_RD_SPEED				0x1116
#define ATERROR_ADS1256ADDRESSDEVICE			0x1117
#define ATERROR_ADS1256_CNF_T					0x1118
#define ATERROR_ADS1256_CNF_dT					0x1119
#define ATERROR_ADS1256_CNF_PT100				0x111a
#define ATERROR_ADS1256_CNF_KALIBRACJA          0x111a  //nie wykonano kalibracji dla ga i dr ustawionych w konfigu

#define ATERROR_I2C_INIT						0x1121
#define ATERROR_I2C_BEGIN						0x1122
#define ATERROR_I2C_DEV_OPEN					0x1123
#define ATERROR_I2C_DEV_BCM2835					0x1124
#define ATERROR_I2C_DEV_GOWRITE 				0x1125
#define ATERROR_I2C_DEV_GOREAD   				0x1126

#define ATERROR_X3MODBUS_CRC					0x1131
#define ATERROR_X3WAITING						0x1132
#define ATERROR_X3UNRECOGNIZED					0x1133   //komunikacja z kieunku X3
#define ATERROR_X3ADDRESSDEVICE					0x1134   //komunikacja do X3

#define ATERROR_SOCKET_ID_DEVICE                0x1140
#define ATERROR_SOCKET_CRC                      0x1141

#define ATERROR_SD_X3_SETWAVE                   0x1150
#define ATERROR_SD_GETDATA                      0x1151
#define ATERROR_SD_NOT_DOWN                     0x1152
#define ATERROR_SD_X3COMMUNICATION              0x1153
#define ATERROR_SD_X3_START_UP                  0x1154
#define ATERROR_SD_X3_WAIT_UP                   0x1155
#define ATERROR_SD_X3_START_DN                  0x1156
#define ATERROR_SD_X3_WAIT_DN                   0x1157
#define ATERROR_SD_X3_START_ZATRZYMAJ           0x1158
#define ATERROR_SD_X3_WAIT_ZATRZYMAJ            0x1159
#define ATERROR_SD_X3_INI                       0x115a
#define ATERROR_SD_X3_ZASILANIE                 0x115b

// bledy generowne na poziomie X3---------------------
#define AT_ERROR_X3_MODBUS_CRC					0x2000
#define AT_ERROR_X3_DEVICEADDR					0x2001
#endif // ATERRORS_H
