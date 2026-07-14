//
// HkTdmCtrl.cpp
//
//  Created by Faber on 13 Jul 2022
//  Copyright © 2018 Faber. All rights reserved.
//

#include "HkTdmCtrl.h"
#include "HkTdmPins.h"

// Instantiate I2C peripheral
I2C             i2c(I2C1_SDA, I2C1_SCL);

// Instantiate I2C controller
CIICController	p_iicCtrl(&i2c);
// Instantiate SHT21 obj
// CSHT21      SHT21(p_iicCtrl);

// Board LED
DigitalOut led(UC_LED, 1);

// PWM-TIMER pins 
PwmOut jf_af(TIM3_CH1);        // JF_AF -> FAN_PWM<0> -> PA6      -> TIM3_CH1
PwmOut jf_ar(TIM3_CH2);        // JF_AR -> FAN_PWM<2> -> PA7_ALT0 -> TIM3_CH2
PwmOut jf_bf(TIM5_CH2);        // JF_BF -> FAN_PWM<1> -> PA1      -> TIM5_CH2
PwmOut jf_br(TIM5_CH3);        // JF_BR -> FAN_PWM<3> -> PA2      -> TIM5_CH3

AnalogIn v_mon0(V_MON_0);
AnalogIn v_mon1(V_MON_1);

// SOC Power 
DigitalOut MPOW(PC_7);

// Serial serial3(USART3_TX, USART3_RX, 9600);
// Serial serial5(UART5_TX, UART5_RX, 9600);

HkTdmCtrlr::HkTdmCtrlr()
{
    initialized = false;
    // ClearStatusReg();
    initialized = true;
}

HKTDM_Error_type HkTdmCtrlr::Init(int _period_in_ms, float _dutyCycle) {
    // Clear status register
    // ClearStatusReg();

    // Set I2C speed
    i2c.frequency(100000);

    // Set default PWM values to 100ms and 0.95
    SetPwmPeriod_JFAF(_period_in_ms);
    SetPwmPeriod_JFBF(_period_in_ms);
    SetPwmPeriod_JFAR(_period_in_ms);
    SetPwmPeriod_JFBR(_period_in_ms);
    SetPwmDutyCycle_JFAF(_dutyCycle);
    SetPwmDutyCycle_JFBF(_dutyCycle);
    SetPwmDutyCycle_JFAR(_dutyCycle);
    SetPwmDutyCycle_JFBR(_dutyCycle);
    return HKTDM_ERR_NO_ERROR;
}

// void HkTdmCtrlr::SetCardID()
// {
//    // get the Unique 96 bits chip ID by reading the register at address 0x1FFF7A10 as p/1334 https://www.st.com/resource/en/reference_manual/rm0390-stm32f446xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
//    // UID 540226882 and 540619073 are from eval board
//    // return 0 for unknown chip ID
//   unsigned long *uid = (unsigned long *)0x1FFF7A10;    
//   cardID = id_map[uid[2]];
// }


// HKTDM_Error_type HkTdmCtrlr::GetCardID(uint8_t *_cardID)
// {
//    *_cardID = cardID;
//    //   *_cardID = cardID;
//     return HKTDM_ERR_NO_ERROR;
// }

HKTDM_Error_type HkTdmCtrlr::GetUniqueID(uint32_t *_uniqueID)
{
  unsigned long *uid = (unsigned long *)0x1FFF7A10;    
  //  _uniqueID = (unsigned long *)0x1FFF7A10;    
  //  uint32_t* a = 12;
  *_uniqueID = uid[2];
  //  *_cardID = cardID;
   //   *_cardID = cardID;
    return HKTDM_ERR_NO_ERROR;
}

HKTDM_Error_type HkTdmCtrlr::SetMPOW(uint8_t setting)
{
  if (setting == 1){
    MPOW = 1 ;
    return HKTDM_ERR_NO_ERROR;
  }
  if (setting == 0){
    MPOW = 0 ;
    return HKTDM_ERR_NO_ERROR;
  }
  else {
    return HKTDM_ERR_NO_ERROR;
  }
}

HKTDM_Error_type HkTdmCtrlr::GetMPOW(uint8_t *_setting)
{
  *_setting = MPOW;
  return HKTDM_ERR_NO_ERROR;
}

// Get voltage V0
HKTDM_Error_type HkTdmCtrlr::GetBoardVin0(float* _adc_mon) {
    *_adc_mon = v_mon0.read();
    return HKTDM_ERR_NO_ERROR;
}

// Get voltage V1
HKTDM_Error_type HkTdmCtrlr::GetBoardVin1(float* _adc_mon) {
    *_adc_mon = v_mon1.read();
    return HKTDM_ERR_NO_ERROR;
}

void HkTdmCtrlr::SetPwmPeriod_JFAF(int _period_in_ms)
{
    SetPwmPeriod_ms(&jf_af, _period_in_ms);
}
void HkTdmCtrlr::SetPwmPeriod_JFBF(int _period_in_ms)
{
    SetPwmPeriod_ms(&jf_bf, _period_in_ms);
}
void HkTdmCtrlr::SetPwmPeriod_JFAR(int _period_in_ms)
{
    SetPwmPeriod_ms(&jf_ar, _period_in_ms);
}
void HkTdmCtrlr::SetPwmPeriod_JFBR(int _period_in_ms)
{
    SetPwmPeriod_ms(&jf_br, _period_in_ms);
}

void HkTdmCtrlr::SetPwmDutyCycle_JFAF(float _dutyCycle)
{
    SetPwmDutyCycle(&jf_af, _dutyCycle);
}
void HkTdmCtrlr::SetPwmDutyCycle_JFBF(float _dutyCycle)
{
    SetPwmDutyCycle(&jf_bf, _dutyCycle);
}
void HkTdmCtrlr::SetPwmDutyCycle_JFAR(float _dutyCycle)
{
    SetPwmDutyCycle(&jf_ar, _dutyCycle);
}
void HkTdmCtrlr::SetPwmDutyCycle_JFBR(float _dutyCycle)
{
    SetPwmDutyCycle(&jf_br, _dutyCycle);
}

// HKTDM_Error_type HkTdmCtrlr::SetPwmDutyCycle(PwmOut* _lpPwm, float _dutyCycle)
// {
//     if ((_dutyCycle < 0) || (_dutyCycle > 1.)) return HKTDM_ERR_PARAM_ERROR;
//     _lpPwm->write(_dutyCycle);
//     return HKTDM_ERR_NO_ERROR;
// }

// HKTDM_Error_type HkTdmCtrlr::SetPwmPeriod_ms(PwmOut* _lpPwm, int _period_in_ms)
// {
//     _lpPwm->period_ms(_period_in_ms);
//     return HKTDM_ERR_NO_ERROR;
// }

// /*
//  *                SET CALMUX pins
//  */
// HKTDM_Error_type HkTdmCtrlr::CalInputEnable(uint8_t _ch_num) {
//     if (_ch_num > (HKTDM_CHAN_NUM-1)) return HKTDM_ERR_CHANNEL_NUM;
//     // cm[_ch_num] = calmux_active_level ? 1 : 0;
//     SetCalmuxStatusBit(_ch_num);
//     return HKTDM_ERR_NO_ERROR;
// }

// HKTDM_Error_type HkTdmCtrlr::CalInputDisable(uint8_t _ch_num) {
//     if (_ch_num > (HKTDM_CHAN_NUM-1)) return HKTDM_ERR_CHANNEL_NUM;
//     // cm[_ch_num] = calmux_active_level ? 0 : 1;
//     ClearCalmuxStatusBit(_ch_num); 
//     return HKTDM_ERR_NO_ERROR;
// }

// HKTDM_Error_type HkTdmCtrlr::CalInputEnable_All() {
//     HKTDM_Error_type errCode;

//     for(uint8_t num = 0; num < HKTDM_CHAN_NUM; num++){
//          errCode = CalInputEnable(num);
//          if (errCode != HKTDM_ERR_NO_ERROR) return errCode;
//     }
//     return HKTDM_ERR_NO_ERROR;
// }

// HKTDM_Error_type HkTdmCtrlr::CalInputDisable_All() {
//     HKTDM_Error_type errCode;

//     for(uint8_t num = 0; num < HKTDM_CHAN_NUM; num++){
//          errCode = CalInputDisable(num);
//          if (errCode != HKTDM_ERR_NO_ERROR) return errCode;
//     }
//     return HKTDM_ERR_NO_ERROR;
// }

// Read TPS temperature
// HKTDM_Error_type HkTdmCtrlr::TempMon(float *Btemp, int& _msg){
//     return HKTDM_ERR_NO_ERROR; 
// }
