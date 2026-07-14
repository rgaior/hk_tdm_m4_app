//
//  COPacketCmdHkTdm.cpp
//
//  Created by Faber on 13 Jul 2022
//  Copyright © 2018 Faber. All rights reserved.
//

/*
 The peripherals instantiated on the M4 on the HK_TDM_V1 board are:
    - USART1 (PA9, PA10)
    - USART2 ()
    - USART3 ()
    - I2C1 (PB6, PB7)
    - SWDIO (PA13, PA14)
    - TIMER

 GPIOs
    - GPIO @ J100 (PC6, PB13, PB14, PB15)

 The tasks performed by M4 on the HK_TDM_V1 board are:
    - FAN control
    - Set Board ID
    - Control Power ON
    - Managing peripherals on I2C bus:
 
 List of device on I2C bus:
    - TMP112 (0x...)
 */

#include "COPacketCmdHkTdm.h"


const uint16_t   HkTdmCmdSize = 11;
_COPacketCmdType HkTdmCommands[HkTdmCmdSize] = {
    {"temp", HKTDM_READ_TEMPERATURE,      "$temp#        - Read TP112 Temperature"},

    {"gcid", HKTDM_GET_CARD_ID,           "$gcid#        - Get card ID"},

    {"gbv0", HKTDM_GET_BRD_V0,            "$gbv0#        - Read Board V0"},
    {"gbv1", HKTDM_GET_BRD_V1,            "$gbv1#        - Read Board V1"},
   //  {"lpwm", HKTDM_SET_LED_PWM,           "$lpwm val#    - Set LED PWM [0, 1.]"},

    {"pwmd", HKTDM_SET_PWM_DCYCLE,        "$pwmd id dcy# - id sets fan [0,3], dcy sets duty cycle [0, 1.]\r\n                0:AF,1:BF,2:AR,3:BR"},
    {"pwmp", HKTDM_SET_PWM_PERIOD,        "$pwmp id per# - id sets fan [0,3], per sets period in ms [0..65535]\r\n                0:AF,1:BF,2:AR,3:BR"},

    {"guid", HKTDM_GET_UNIQUE_ID,         "$guid#        - Get uc Unique ID"},
 
    {"spwr", HKTDM_SET_POWER,      "--RS485 specific $spwr cid 0/1#  - cid is the Card ID, 0:Off 1:On Switch on and off the power for the SOC"},
    {"gpwr", HKTDM_GET_POWER,      "--RS485 specific $gpwr cid #  - cid is the Card ID, returns Switch of the power for the SOC state"},

    {"help", HKTDM_HELP,                  "$help#        - Print this menu"},
    {"erro", HKTDM_ERRO,                  ""}

};

_COPacketCmdList HkTdmCmdList = {
    HkTdmCmdSize,
    HkTdmCommands
};
