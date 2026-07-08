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
// Serial serial(UART5_TX, UART5_RX, 115200);
// Serial serial(USART1_TX, USART1_RX, 115200);
//Serial serial(PA_9, PA_10, 115200);
BufferedSerial serial(PA_9, PA_10, 115200);
#endif

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
// extern PwmOut jf_af;
// extern PwmOut jf_ar;
// extern PwmOut jf_bf;
// extern PwmOut jf_br;
// extern AnalogIn v_mon0;
// extern AnalogIn v_mon1;
// extern Serial serial3;
// extern Serial serial5;

// main()
int main() {
    // char c='@';
    // uint16_t i=0;

    // do{
    //     serial.printf("TDM Debug #%d\r\n", i++);
    //     // wait_ms(500);
    //     wait(1.);
    //     tstled = !tstled;

    //     // Check data on serial and update c char
    //     if (serial.readable()){
    //         c = serial.getc();
    //         // Transmit c char on serial
    //         // serial.putc(c);
    //         serial.printf("char: %c\r\n", c);
    //     }
    // // }while (c != '#');
    // }while(1);

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
        //serial.printf("%s\r\n", outPacket);
    }

    // Initialize Board
    errCode = hk_tdm.Init();
    if(errCode != HKTDM_ERR_NO_ERROR)
    {
        // HANDLE ERROR
        pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
        //serial.printf("%s\r\n", outPacket);
    }

    // START DEBUG //
    if (DEBUG_LEVEL>0){
        float v0, v1;
        hk_tdm.GetBoardVin0(&v0);
        hk_tdm.GetBoardVin1(&v1);
        //serial.printf("TDM Debug #1\r\n");
        //serial.printf("%f %f\r\n", v0, v1);
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
            //if (DEBUG_LEVEL>0) serial.printf("%s\r\n", packet);

            // Parse and execute command, and output response or error
            if(errCode == HKTDM_ERR_NO_ERROR){
                executePacket(packet, SERIAL_IO_DEVICE);
                //serial.printf("%s\r\n", outPacket);
            }else{
                pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
                //serial.printf("%s\r\n", outPacket);
            }
        }
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
        //serial.printf(menu_str);
    }
    return true;
}

// Receive input packet
HKTDM_Error_type getPacketFrom(char _gcmd[], IOPrintDevice_type _IO){
    char    pkt_char = ' ';
    uint16_t index = 0;

    //if(_IO == SERIAL_IO_DEVICE) pkt_char = serial.getc();

    if (pkt_char == pkt.GetStartChar()){
        index = 0;  // reset
        _gcmd[index++] = pkt_char;
        do
        {               
            //if(_IO == SERIAL_IO_DEVICE) pkt_char = serial.getc();
            if (index < PKT_SIZE) _gcmd[index++] = pkt_char;  // put it into the value array and increment the index
        } while (pkt_char != pkt.GetStopChar());    // loop until the '#' character
    }else{
        return HKTDM_ERR_COMMUNICATION;
    }
    return HKTDM_ERR_NO_ERROR;
}

HKTDM_Error_type executePacket(char _gcmd[], IOPrintDevice_type _IO){
    uint8_t arg = 0;
    float   brd_mon_val;
    uint8_t pwm_id;
    uint16_t pwm_period;

    // Parse Command string
    //serial.printf("\r\nPacket \"%s\"\r\n", _gcmd);
    pktError = pkt.LoadString(_gcmd);
    //serial.printf("\r\nPacket \"%s\" OK. Field number = %u.\r\n", _gcmd, pkt.GetFieldNumber());
    
    if(pktError != COPACKET_NOERR) {
        errCode = HKTDM_ERR_PACKET_ERROR;
        pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
        return errCode;
    }
    
    // Get Command field from packet and switch on it
    int16_t cmdIdx = pkt.GetNextFiedlAsCOMMAND(HkTdmCmdList);
    switch(cmdIdx){
        // Read card ID
        case HKTDM_GET_CARD_ID:
            errCode = hk_tdm.GetCardID(&arg);
            if(errCode != HKTDM_ERR_NO_ERROR) {
                pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[HKTDM_ERRO].CmdString, (uint32_t)errCode);
            }else{
                pkt.CreatePacket(outPacket, HkTdmCmdList.CmdList[cmdIdx].CmdString, (uint32_t)arg);
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
