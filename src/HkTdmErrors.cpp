//
// HkTdmErrors.cpp
//
//  Created by Faber on 13 Jul 2022
//  Copyright © 2018 Faber. All rights reserved.
//
#include "HkTdmErrors.h"

// (LEAVE IN COMMON CODE FOR HOST SOFTWARE)
const char* getHkTdmErrorString(HKTDM_Error_type _err)
{
	char* strPtr;

	switch(_err) {
		case HKTDM_ERR_NO_ERROR:
			strPtr = (char*)"HKTDM_NOERR";
		break;
        case  HKTDM_ERR_COMMUNICATION:
			strPtr = (char*)"HKTDM_ERR_COMM";
		break;
        case  HKTDM_ERR_PACKET_OK:
			strPtr = (char*)"HKTDM_ERR_PACKET_OK";
		break;
		case  HKTDM_ERR_PACKET_ERROR:
			strPtr = (char*)"HKTDM_ERR_PACKET_ERROR";
		break;
        case  HKTDM_ERR_COMMAND_NOT_FOUND:
			strPtr = (char*)"HKTDM_ERR_CMD_NOT_FOUND";
		break;
		
		// case HKTDM_ERR_ADS1115_WRITECONFREG:
		// 	strPtr = (char*)"HKTDM_ERR_ADS1115_WRCONFREG";
		// break;
		// case HKTDM_ERR_ADS1115_BEGINCONV:
		// 	strPtr = (char*)"HKTDM_ERR_ADS1115_BEGCONV";
		// break;
        // case HKTDM_ERR_ADS1115_READCONV:
		// 	strPtr = (char*)"HKTDM_ERR_ADS1115_READ_CONV";
		// break;

		// case HKTDM_ERR_CHANNEL_NUM:
		// 	strPtr = (char*)"HKTDM_ERR_CH_NUM";
		// break;
		// case HKTDM_ERR_THR_VALUE:
		// 	strPtr = (char*)"HKTDM_ERR_THR_VALUE";
		// break;
		// case HKTDM_ERR_CALMUX:
		// 	strPtr = (char*)"HKTDM_ERR_CALMUX";
		// break;

        // case HKTDM_ERR_AD5665R_CHA:
		// 	strPtr = (char*)"HKTDM_ERR_AD5665R_CHA";
		// break;
        // case HKTDM_ERR_AD5665R_CHB:
		// 	strPtr = (char*)"HKTDM_ERR_AD5665R_CHB";
		// break;
        // case HKTDM_ERR_AD5665R_CHC:
		// 	strPtr = (char*)"HKTDM_ERR_AD5665R_CHC";
		// break;
        // case HKTDM_ERR_AD5665R_CHD:
		// 	strPtr = (char*)"HKTDM_ERR_AD5665R_CHD";
		// break;
        
        // case HKTDM_ERR_ADS1115_INIT:
		// 	strPtr = (char*)"HKTDM_ERR_ADS1115_INIT";
		// break;
        // case HKTDM_ERR_AD5665R_INIT:
		// 	strPtr = (char*)"HKTDM_ERR_AD5665R_INIT";
		// break;
        // case HKTDM_ERR_AD5665R_SWRST:
		// 	strPtr = (char*)"HKTDM_ERR_AD5665R_SOFTWARE_RESET";
		// break;
        // case HKTDM_ERR_AD5665R_DIS_LDAC:
		// 	strPtr = (char*)"HKTDM_ERR_AD5665R_DIS_LDAC";
		// break;
        // case HKTDM_ERR_AD5665R_SETREFON:
		// 	strPtr = (char*)"HKTDM_ERR_AD5665R_SET_REF_ON";
		// break;
		// case HKTDM_ERR_SHT21_READ:
		// 	strPtr = (char*)"HKTDM_ERR_SHT21_READ";
		// break;
		// case HKTDM_ERR_SHT21_MEASURE:
		// 	strPtr = (char*)"HKTDM_ERR_SHT21_MEASURE";
		// break;
		// case HKTDM_ERR_SI5345_PLLINIT:
		// 	strPtr = (char*)"HKTDM_ERR_SI5345_PLL_INIT";
		// break;
		case HKTDM_ERR_MENU_DISPLAY:
			strPtr = (char*)"HKTDM_ERR_MENU_DISPLAY";
		break;
		// case HKTDM_ERR_DS1820_INVALID:
		// 	strPtr = (char*)"HKTDM_ERR_DS1820_INVALID_CONV";
		// break;
		case HKTDM_ERR_BOARD_INIT:
			strPtr = (char*)"HKTDM_ERR_BOARD_INIT";
		break;
        default:
			strPtr = (char*)"WVBRD_ERR STRING UNDEFINED";
		break;
	}

	return strPtr;
}
