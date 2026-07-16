//
// hk_tdm_v1_main.cpp
//
//  Created by Faber on 13 Jul 2022
//  Copyright © 2018 Faber. All rights reserved.
//
// This program can run on the Nucleo board as it is!
//

#include <mbed.h>
#include <string>
#include <PwmOut.h>

#include <common/error_codes.h>
#include <common/include/include_types.h>
#include <common/utils/gcc_compilemacros.h>
#include <common/protocols/COPacket/COPacket.hpp>       // <--- The new protocols folder is in redlibs "ProtocolFolder" branch

#include "COPacketCmdHkTdm.h"
#include "HkTdmErrors.h"
#include "HkTdmCtrl.h"

#define DEBUG_LEVEL (0)

#define PKT_SIZE MAX_PACKET_SIZE
char packet[PKT_SIZE];
char outPacket[PKT_SIZE];

// Serial port instantiation
#ifdef DEBUG_ON_NUCLEO_F446RE
//Serial serial(PA_2, PA_3, 115200);
#else
UnbufferedSerial serial(PA_9, PA_10, 115200);
#endif

char outbuf[MAX_PACKET_SIZE * 2];

// DEFINE LIST OF COMMANDS
extern _COPacketCmdList HkTdmCmdList;
CCOPacket pkt(COPKT_DEFAULT_START, COPKT_DEFAULT_STOP, COPKT_DEFAULT_SEP);
//CCOPacket pkt; <----- this works too: special chars are defaults!!!

// IO device
typedef enum {
    SERIAL_IO_DEVICE = 0,
    USB_IO_DEVICE                  
} IOPrintDevice_type;

HKTDM_Error_type errCode;
COPacketResponse_type pktError;

HKTDM_Error_type getPacketFrom(char _gcmd[], IOPrintDevice_type);
HKTDM_Error_type executePacket(char _gcmd[], IOPrintDevice_type);

// Return false in case of error
bool disMenu(IOPrintDevice_type _IO);

// Instantiate HK-TDM control class.
HkTdmCtrlr hk_tdm;

extern DigitalOut led;


//romain
HKTDM_Error_type tmp_errCode;
char packet_onoff[PKT_SIZE];
char outPacket_onoff[PKT_SIZE];
UnbufferedSerial serial_onoff(PB_10, PC_5, 115200);
extern _COPacketCmdList HkTdmCmdListOnOff;
CCOPacket pkt_onoff(COPKT_DEFAULT_START, COPKT_DEFAULT_STOP, COPKT_DEFAULT_SEP);
HKTDM_Error_type getPacketFromOnOff(char _gcmd[], IOPrintDevice_type);
HKTDM_Error_type executePacketOnOff(char _gcmd[], IOPrintDevice_type);

DigitalOut DriverEnable(PB_0);
//romain


int main(void) {
  // switch on the SOM at power up
  tmp_errCode = hk_tdm.SetMPOW(1);
  
  // Display menu
  if (DEBUG_LEVEL){
    if (!disMenu(SERIAL_IO_DEVICE)){
      pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)HKTDM_ERR_MENU_DISPLAY);
    }else{
      pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_HELP].CmdString);
        }
  }

    // Check if hk_tdm class is ok
    if (!hk_tdm.GetInitialized())
    {
        // HANDLE ERROR
        pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)HKTDM_ERR_BOARD_INIT);
        sprintf(outbuf, "%s\r\n", outPacket);
        serial.write(outbuf, strlen(outbuf));
    }

    // Initialize Board
    errCode = hk_tdm.Init();
    if(errCode != HKTDM_ERR_NO_ERROR)
    {
        // HANDLE ERROR
        pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
        sprintf(outbuf, "%s\r\n", outPacket);
        serial.write(outbuf, strlen(outbuf));
    }

    // START DEBUG //
    if (DEBUG_LEVEL>0){
        float v0, v1;
        hk_tdm.GetBoardVin0(&v0);
        hk_tdm.GetBoardVin1(&v1);
        sprintf(outbuf, "%s", "TDM Debug #1\r\n");
        serial.write(outbuf, strlen(outbuf));
        sprintf(outbuf, "%f %f\r\n", v0, v1);
        serial.write(outbuf, strlen(outbuf));
    }

    // Infinite loop waiting for command packets
    while (true) {
        // Read input char from serial port
        if(serial.readable()) {
            // Toggle led to signal serial activity
            led = !led;
            // Receive command packet
            errCode = getPacketFrom(packet, SERIAL_IO_DEVICE);
            // DEBUG: uncomment to print received packet
            if (DEBUG_LEVEL>0) {
               sprintf(outbuf, "%s\r\n", packet);
               serial.write(outbuf, strlen(outbuf));
            }

            // Parse and execute command, and output response or error
            if(errCode == HKTDM_ERR_NO_ERROR){
                executePacket(packet, SERIAL_IO_DEVICE);
                sprintf(outbuf, "%s\r\n", outPacket);
                serial.write(outbuf, strlen(outbuf));
            }else{
                pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
                sprintf(outbuf, "%s\r\n", outPacket);
                serial.write(outbuf, strlen(outbuf));
            }
        }

	//romain immplementation of the RS485 link
	if (serial_onoff.readable()){
  	  errCode = getPacketFromOnOff(packet_onoff, SERIAL_IO_DEVICE);
	  pktError = pkt_onoff.LoadString(packet_onoff);
	  int16_t cmdIdx = pkt_onoff.GetNextFiedlAsCOMMAND(HkTdmCmdList);
	  uint16_t card_id_target = 0;  
	  uint16_t card_id = 0;  
	  tmp_errCode = hk_tdm.GetUniqueID(&card_id);
	  pkt_onoff.GetNextFieldAsUINT16(card_id_target);
	  // check if the board ID match the one in the message
	  if (card_id == card_id_target){
	    sprintf(outbuf, "%s", "RIGHT TDM \r\n");
	    serial.write(outbuf, strlen(outbuf));
	    // set the board in emitter mode
	    //ReceiverEnable = 1;
	    DriverEnable = 1;
	    if(errCode == HKTDM_ERR_NO_ERROR){
	      executePacketOnOff(packet_onoff, SERIAL_IO_DEVICE);
	      sprintf(outbuf, "%s\r\n", outPacket_onoff);
	      serial_onoff.write(outbuf, strlen(outbuf));
	      //	      serial_onoff.printf("%s\r\n", outPacket_onoff);
	    }
	  }	  
	  // if the board ID doesn't match one has to make sure to remain quiet on the line.
	  else{
	    sprintf(outbuf, "%s", "WRONG TDM \r\n");
	    serial.write(outbuf, strlen(outbuf));
	    DriverEnable = 0;
	  }
	  DriverEnable = 0;
	}
	// --romain
    }
}

// Print help menu
bool disMenu(IOPrintDevice_type _IO){
    char menu_str[MENU_STRING_LENGTH] = "\r\n";

    // Call a method that creates a string containing the menu
    if (pkt.createCmdMenu(HkTdmCmdList, menu_str) != COPACKET_NOERR) return false;

    // Add Build date to help menu
    if ((strlen(menu_str) + 32) > MENU_STRING_LENGTH) return false;
    sprintf(menu_str + strlen(menu_str), "Build: %d-%d-%d %d:%02d:%02d\r\n", BUILD_DAY_NUM, BUILD_MONTH_NUM, BUILD_YEAR_NUM, BUILD_HOUR_NUM, BUILD_MIN_NUM, BUILD_SEC_NUM);

    // Print to selected IO device
    if(_IO == SERIAL_IO_DEVICE){
        serial.write(menu_str, strlen(menu_str));
    }
    return true;
}

// Receive input packet
HKTDM_Error_type getPacketFrom(char _gcmd[], IOPrintDevice_type _IO){
    char    pkt_char = ' ';
    uint16_t index = 0;

    if(_IO == SERIAL_IO_DEVICE) 
       serial.read(&pkt_char, 1);

    if (pkt_char == pkt.GetStartChar()){
        index = 0;  // reset
        _gcmd[index++] = pkt_char;
        do
        {               
            if(_IO == SERIAL_IO_DEVICE) 
               serial.read(&pkt_char, 1);

            if (index < PKT_SIZE) _gcmd[index++] = pkt_char;  // put it into the value array and increment the index
        } while (pkt_char != pkt.GetStopChar());    // loop until the '#' character
    }else{
        return HKTDM_ERR_COMMUNICATION;
    }
    return HKTDM_ERR_NO_ERROR;
}

HKTDM_Error_type executePacket(char _gcmd[], IOPrintDevice_type _IO){
    uint8_t arg = 0;
    uint16_t arg16 = 0;
    float   brd_mon_val;
    uint8_t pwm_id;
    uint16_t pwm_period;

    // Parse Command string
    //sprintf(outbuf, "\r\nPacket \"%s\"\r\n", _gcmd);
    //serial.write(outbuf, strlen(outbuf));
    pktError = pkt.LoadString(_gcmd);
    //sprintf(outbuf, "\r\nPacket \"%s\" OK. Field number = %u.\r\n", _gcmd, pkt.GetFieldNumber());
    //serial.write(outbuf, strlen(outbuf));
    
    if(pktError != COPACKET_NOERR) {
        errCode = HKTDM_ERR_PACKET_ERROR;
        pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
        return errCode;
    }
    
    // // Get Command field from packet and switch on it
    int16_t cmdIdx = pkt.GetNextFiedlAsCOMMAND(HkTdmCmdList);
    switch(cmdIdx){
      // Read card ID
    case HKTDM_GET_CARD_ID:
      //	  uint8_t ID = 0;  
      errCode = hk_tdm.GetUniqueID(&arg16);
      if(errCode != HKTDM_ERR_NO_ERROR) {
	pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
      }else{
	pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[cmdIdx].CmdString, (uint32_t)arg16);
      }
      break;

 
      // Read temperature with TMP112
    case HKTDM_READ_TEMPERATURE:
      float   temp;

            errCode = hk_tdm.ReadTemp(temp);
            if(errCode != HKTDM_ERR_NO_ERROR) {
                pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
                //pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)msg);
            }else{
                pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[cmdIdx].CmdString, temp);
            }
        break;

        // Set PWM duty cycle
        case HKTDM_SET_PWM_DCYCLE:
            // Get Number of PWM to modify
            pktError = pkt.GetNextFieldAsUINT8(pwm_id);
            if(pktError != COPACKET_NOERR || pwm_id > 4) {
                errCode = HKTDM_ERR_PACKET_ERROR;
                pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
                break;
            }

            // Get new pwm duty cycle value
            pktError = pkt.GetNextFieldAsFLOAT(brd_mon_val);
            if(pktError != COPACKET_NOERR || brd_mon_val > 1.) {
                errCode = HKTDM_ERR_PACKET_ERROR;
                pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
                break;
            }

            errCode = HKTDM_ERR_NO_ERROR;
            switch(pwm_id){
                case 0:
                    hk_tdm.SetPwmDutyCycle_JFAF(brd_mon_val);
                break;
                case 1:
                    hk_tdm.SetPwmDutyCycle_JFBF(brd_mon_val);
                break;
                case 2:
                    hk_tdm.SetPwmDutyCycle_JFAR(brd_mon_val);
                break;
                case 3:
                    hk_tdm.SetPwmDutyCycle_JFBR(brd_mon_val);
                break;
                default:
                    errCode = HKTDM_ERR_PARAM_ERROR;
                break;
            }

            if(errCode!=HKTDM_ERR_NO_ERROR) {
                pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
            }else{
                pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[cmdIdx].CmdString, (uint32_t)pwm_id, brd_mon_val);
            }
        break;

        // Set PWM period in ms
        case HKTDM_SET_PWM_PERIOD:
            // Get Number of PWM to modify
            pktError = pkt.GetNextFieldAsUINT8(pwm_id);
            if(pktError != COPACKET_NOERR || pwm_id > 4) {
                errCode = HKTDM_ERR_PACKET_ERROR;
                pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
                break;
            }

            // Get new pwm period
            pktError = pkt.GetNextFieldAsUINT16(pwm_period);
            if(pktError != COPACKET_NOERR) {
                errCode = HKTDM_ERR_PACKET_ERROR;
                pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
                break;
            }

            errCode = HKTDM_ERR_NO_ERROR;
            switch(pwm_id){
                case 0:
                    hk_tdm.SetPwmPeriod_JFAF((int)pwm_period);
                break;
                case 1:
                    hk_tdm.SetPwmPeriod_JFBF((int)pwm_period);
                break;
                case 2:
                    hk_tdm.SetPwmPeriod_JFAR((int)pwm_period);
                break;
                case 3:
                    hk_tdm.SetPwmPeriod_JFBR((int)pwm_period);
                break;
                default:
                    errCode = HKTDM_ERR_PARAM_ERROR;
                break;
            }

            if(errCode!=HKTDM_ERR_NO_ERROR) {
                pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
            }else{
                pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[cmdIdx].CmdString, (uint32_t)pwm_id, (uint32_t)pwm_period);
            }
        break;

        // Read Board V0 voltage
        case HKTDM_GET_BRD_V0:
            errCode = hk_tdm.GetBoardVin0(&brd_mon_val);
            if(errCode != HKTDM_ERR_NO_ERROR) {
                pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
            }else{
                pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[cmdIdx].CmdString, brd_mon_val);
            }
        break;

        // Read Board V0 voltage
        case HKTDM_GET_BRD_V1:
            errCode = hk_tdm.GetBoardVin1(&brd_mon_val);
            if(errCode != HKTDM_ERR_NO_ERROR) {
                pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
            }else{
                pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[cmdIdx].CmdString, brd_mon_val);
            }
        break;

        case HKTDM_HELP:
            if (!disMenu(_IO)){
                pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)HKTDM_ERR_MENU_DISPLAY);
            }else{
                pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[cmdIdx].CmdString);
            }
        break;

        // case HKTDM_STAT:
        //     // uint8_t in_status, pll_status;
        //     char stat_val[20];
        //     // Get command argument
        //     pktError = pkt.GetNextFieldAsUINT8(arg);
        //     if(pktError != COPACKET_NOERR) {
        //         errCode = HKTDM_ERR_PACKET_ERROR;
        //         pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
        //         break;
        //     }

        //     // 0  -> Show status register
        //     // 1  -> Show PLL status
        //     // Default=0  -> Show status register
        //     switch(arg){
        //         case 0:
        //             sprintf(stat_val, " 0x%06lX", hk_tdm.GetCalmuxStatusReg());
        //         break;
        //         case 1:
        //             sprintf(stat_val, " 0x%02X-0x%02X", 0, 0);
        //         break;
        //         default:
        //             sprintf(stat_val, " 0x%08lX", hk_tdm.GetStatusReg());
        //         break;
        //     }
        //     errCode = HKTDM_ERR_NO_ERROR;
        //     pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[cmdIdx].CmdString, (uint32_t)arg, stat_val);
        // break;

/*        // Set Led pwm 
        case HKTDM_SET_LED_PWM:
            pktError = pkt.GetNextFieldAsFLOAT(brd_mon_val);
            if(pktError != COPACKET_NOERR || brd_mon_val > 1.) {
                errCode = HKTDM_ERR_PACKET_ERROR;
                pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
                break;
            }
            // hk_tdm.SetPwmDutyCycle(&led, brd_mon_val);
            if(errCode!=HKTDM_ERR_NO_ERROR) {
                pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
                break;
            }else{
                pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[cmdIdx].CmdString, brd_mon_val);
            }
        break;
*/
        case HKTDM_ERRO:
            errCode = HKTDM_ERR_NO_ERROR;   
            pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[cmdIdx].CmdString);
        break;

        default:
            errCode = HKTDM_ERR_COMMAND_NOT_FOUND;   
            pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
        break;
    }
    return errCode;
}




// Receive input packet
HKTDM_Error_type getPacketFromOnOff(char _gcmd[], IOPrintDevice_type _IO){
  char    pkt_char = ' ';
  uint16_t index = 0;
  
  if(_IO == SERIAL_IO_DEVICE)
       serial_onoff.read(&pkt_char, 1);
  
  if (pkt_char == pkt_onoff.GetStartChar()){
    index = 0;  // reset
    _gcmd[index++] = pkt_char;
    do
      {               
	if(_IO == SERIAL_IO_DEVICE)
	  serial_onoff.read(&pkt_char, 1);
	if (index < PKT_SIZE) _gcmd[index++] = pkt_char;// put it into the value array and increment the index
      } while (pkt_char != pkt_onoff.GetStopChar());    // loop until the '#' character
  }else{
    return HKTDM_ERR_COMMUNICATION;
  }
  return HKTDM_ERR_NO_ERROR;
}


HKTDM_Error_type executePacketOnOff(char _gcmd[], IOPrintDevice_type _IO){
    uint8_t arg = 0;
    float   brd_mon_val;
    uint8_t pwm_id;
    uint16_t pwm_period;

    // Parse Command string
    // serial.printf("\r\nPacket \"%s\"\r\n", _gcmd);
    pktError = pkt_onoff.LoadString(_gcmd);
    //serial.printf("\r\nPacket \"%s\" OK. Field number = %u.\r\n", _gcmd, pkt.GetFieldNumber());
    //    serial_onoff.printf("commmand string %s \r\n", _gcmd);

    if(pktError != COPACKET_NOERR) { 
      errCode = HKTDM_ERR_PACKET_ERROR;
      pkt_onoff.CreatePacket(outPacket_onoff, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
      return errCode;
    }
    
    // Get Command field from packet and switch on it
    int16_t cmdIdx = pkt_onoff.GetNextFiedlAsCOMMAND(HkTdmCmdList);
    uint16_t card_id_target = 0;  
    uint16_t card_id = 0;
    uint8_t power_setting = 2;  
    uint8_t curr_MPOW = 2;
    switch(cmdIdx){
    case HKTDM_SET_POWER:
      errCode = hk_tdm.GetUniqueID(&card_id);
      pkt_onoff.GetNextFieldAsUINT16(card_id_target);
      pkt_onoff.GetNextFieldAsUINT8(power_setting);
      hk_tdm.SetMPOW(power_setting);      
      if(errCode != HKTDM_ERR_NO_ERROR) {
	pkt_onoff.CreatePacket(outPacket_onoff, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
      }else{
	pkt_onoff.CreatePacket(outPacket_onoff, HkTdmCmdList.CmdList[cmdIdx].CmdString, (uint32_t)card_id_target);
      }
      break;      

    case HKTDM_GET_POWER:
      errCode = hk_tdm.GetMPOW(&curr_MPOW);
      if(errCode != HKTDM_ERR_NO_ERROR) {
	pkt_onoff.CreatePacket(outPacket_onoff, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
      }else{
	pkt_onoff.CreatePacket(outPacket_onoff, HkTdmCmdList.CmdList[cmdIdx].CmdString, (uint32_t)curr_MPOW);
      }
      break;      

    // case HKTDM_HELP:
    //   if (!disMenu(_IO)){
    // 	pkt_onoff.CreatePacket(outPacket_onoff, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)HKTDM_ERR_MENU_DISPLAY);
    //   }else{
    // 	pkt_onoff.CreatePacket(outPacket_onoff, HkTdmCmdList.CmdList[cmdIdx].CmdString);
    //   }
    //   break;
       
    // case HKTDM_ERRO:
    //   errCode = HKTDM_ERR_NO_ERROR;   
    //   pkt_onoff.CreatePacket(outPacket_onoff, HkTdmCmdList.CmdList[cmdIdx].CmdString);
    //   break;
      
    default:
      errCode = HKTDM_ERR_COMMAND_NOT_FOUND;   
    //   pkt_onoff.CreatePacket(outPacket_onoff, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
      break;
    }
    return errCode;



}
