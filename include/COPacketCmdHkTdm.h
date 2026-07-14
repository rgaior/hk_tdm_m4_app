//
//  COPacketCmdHkTdm.h
//
//  Created by Faber on 13 Jul 2022
//  Copyright © 2018 Faber. All rights reserved.
//
#ifndef COPacketCmdHkTdm_h
#define COPacketCmdHkTdm_h

#include <stdint.h>
#include "common/protocols/COPacket/COPacket.hpp"

// This enum defines the numeric values of command codes
enum HKTDM_CMD_LIST {
    HKTDM_READ_TEMPERATURE = 0,

    HKTDM_GET_CARD_ID,

    HKTDM_GET_BRD_V0,
    HKTDM_GET_BRD_V1,
    // HKTDM_SET_LED_PWM,
    
    HKTDM_SET_PWM_DCYCLE,
    HKTDM_SET_PWM_PERIOD,

    HKTDM_GET_UNIQUE_ID,
    // HKTDM_STAT,
    HKTDM_SET_POWER,
    HKTDM_GET_POWER,
    HKTDM_HELP,
    HKTDM_ERRO,
};

#endif /* COPacketCmdHkTdm_h */
