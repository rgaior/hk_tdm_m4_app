//
// HkTdmCtrl.h
//
//  Created by Faber on 13 Jul 2022
//  Copyright © 2018 Faber. All rights reserved.
//

#ifndef HKTDMCTRL_H
#define HKTDMCTRL_H

#if defined(__BIG_ENDIAN__)
#elif defined(__LITTLE_ENDIAN__)
#else
#error "UNDEFINED ENDIANNESS:  __BIG_ENDIAN__ or __LITTLE_ENDIAN__ must be defined"
#endif

#include <mbed.h>
#include <PinNames.h>
#include <PwmOut.h>
#include "HkTdmErrors.h"
#include <common/error_codes.h>
#include <common/include/include_types.h>
#include <common/include/include_IICController.h>
#include "libs/system/SysTime.h"

// using namespace LIBS::PERIPHERALS;
// error namespace definition
using namespace COMMON::DEFINES::ERRORS;
// typedef ERRORS::ERRCode_type ERRCode_type;
typedef ERRCode_type ERRCode_type;

typedef struct HkTdmStatus {
public:
	union  {
		struct {
			#if defined(__BIG_ENDIAN__)
				/* ---------------------------------------- */
				/* BIT 31 (MSb) */  unsigned BIT31	        : 1; // unused
				/* BIT 30       */  unsigned BIT30          : 1; // unused
				/* BIT 29       */  unsigned BIT29          : 1; // unused
				/* BIT 28       */  unsigned BIT28          : 1; // unused
				/* BIT 27       */  unsigned BIT27          : 1; // unused
				/* BIT 26       */  unsigned BIT26          : 1; // unused
				/* BIT 25       */  unsigned BIT25          : 1; // unused
				/* BIT 24       */  unsigned BIT24          : 1; // unused
				/* BIT 23       */  unsigned CALMUX23          : 1; // CALMUX
				/* BIT 22       */  unsigned CALMUX22          : 1; // CALMUX
				/* BIT 21       */  unsigned CALMUX21          : 1; // CALMUX
				/* BIT 20       */  unsigned CALMUX20          : 1; // CALMUX
				/* BIT 19       */  unsigned CALMUX19          : 1; // CALMUX
				/* BIT 18       */  unsigned CALMUX18          : 1; // CALMUX
				/* BIT 17       */  unsigned CALMUX17          : 1; // CALMUX
				/* BIT 16       */  unsigned CALMUX16          : 1; // CALMUX
				/* BIT 15       */  unsigned CALMUX15          : 1; // CALMUX
				/* BIT 14       */  unsigned CALMUX14          : 1; // CALMUX
				/* BIT 13       */  unsigned CALMUX13          : 1; // CALMUX
				/* BIT 12       */  unsigned CALMUX12          : 1; // CALMUX
				/* BIT 11       */  unsigned CALMUX11          : 1; // CALMUX
				/* BIT 10       */  unsigned CALMUX10          : 1; // CALMUX
				/* BIT 9        */  unsigned CALMUX9           : 1; // CALMUX
				/* BIT 8        */  unsigned CALMUX8           : 1; // CALMUX
				/* BIT 7        */  unsigned CALMUX7           : 1; // CALMUX
				/* BIT 6        */  unsigned CALMUX6           : 1; // CALMUX
				/* BIT 5        */  unsigned CALMUX5           : 1; // CALMUX
				/* BIT 4        */  unsigned CALMUX4           : 1; // CALMUX
				/* BIT 3        */  unsigned CALMUX3           : 1; // CALMUX
				/* BIT 2        */  unsigned CALMUX2           : 1; // CALMUX
				/* BIT 1        */  unsigned CALMUX1           : 1; // CALMUX
				/* BIT 0        */  unsigned CALMUX0	       : 1; // CALMUX
				/* ----------------------------------------------------- */
			#elif defined(__LITTLE_ENDIAN__)
				/* ----------------------------------------------------- */
				/* BIT 0        */  unsigned CALMUX0		    : 1; // CALMUX
				/* BIT 1        */  unsigned CALMUX1		    : 1; // CALMUX
				/* BIT 2        */  unsigned CALMUX2		    : 1; // CALMUX
				/* BIT 3        */  unsigned CALMUX3		    : 1; // CALMUX
				/* BIT 4        */  unsigned CALMUX4		    : 1; // CALMUX
				/* BIT 5        */  unsigned CALMUX5		    : 1; // CALMUX
				/* BIT 6        */  unsigned CALMUX6		    : 1; // CALMUX
				/* BIT 7        */  unsigned CALMUX7		    : 1; // CALMUX
				/* BIT 8        */  unsigned CALMUX8		    : 1; // CALMUX
				/* BIT 9        */  unsigned CALMUX9		    : 1; // CALMUX
				/* BIT 10       */  unsigned CALMUX10		    : 1; // CALMUX
				/* BIT 11       */  unsigned CALMUX11		    : 1; // CALMUX
				/* BIT 12       */  unsigned CALMUX12		    : 1; // CALMUX
				/* BIT 13       */  unsigned CALMUX13		    : 1; // CALMUX
				/* BIT 14       */  unsigned CALMUX14		    : 1; // CALMUX
				/* BIT 15       */  unsigned CALMUX15		    : 1; // CALMUX
				/* BIT 16       */  unsigned CALMUX16		    : 1; // CALMUX
				/* BIT 17       */  unsigned CALMUX17		    : 1; // CALMUX
				/* BIT 18       */  unsigned CALMUX18		    : 1; // CALMUX
				/* BIT 19       */  unsigned CALMUX19		    : 1; // CALMUX
				/* BIT 20       */  unsigned CALMUX20		    : 1; // CALMUX
				/* BIT 21       */  unsigned CALMUX21		    : 1; // CALMUX
				/* BIT 22       */  unsigned CALMUX22		    : 1; // CALMUX
				/* BIT 23       */  unsigned CALMUX23		    : 1; // CALMUX
				/* BIT 24       */  unsigned BIT24          : 1; // unused
				/* BIT 25       */  unsigned BIT25          : 1; // unused
				/* BIT 26       */  unsigned BIT26          : 1; // unused
				/* BIT 27       */  unsigned BIT27          : 1; // unused
				/* BIT 28       */  unsigned BIT28  	    : 1; // unused
				/* BIT 29       */  unsigned BIT29  	    : 1; // unused
				/* BIT 30       */  unsigned BIT30  	    : 1; // unused
				/* BIT 31 (MSb) */  unsigned BIT31  	    : 1; // unused
				/* ----------------------------------------------------- */			
			#endif
		};
		struct {
			#if defined(__BIG_ENDIAN__)
				/* ---------------------------------------- */
				/* BIT 24-31    */  unsigned BITUNUSED      :  8; // unused
				/* BIT  0-23    */  unsigned CALMUX         : 24; // CALMUX
				/* ----------------------------------------------------- */
			#elif defined(__LITTLE_ENDIAN__)
				/* ----------------------------------------------------- */
				/* BIT  0-23    */  unsigned CALMUX         : 24; // CALMUX
				/* BIT 24-31    */  unsigned BITUNUSED      :  8; // unused
				/* ----------------------------------------------------- */			
			#endif
		};
		struct {
			uint32_t reg;
		};
	};
} HkTdmStatus_t;

class HkTdmCtrlr {

public:
	HkTdmCtrlr();

	HKTDM_Error_type Init(int _period_in_ms=100, float _dutyCycle=.95);
	HKTDM_Error_type ReadTemp(float& _temp) { _temp=0; return HKTDM_ERR_NO_ERROR; }
	HKTDM_Error_type GetCardID(uint8_t* _cardID);

	HKTDM_Error_type GetBoardVin0(float* _adc_mon);
	HKTDM_Error_type GetBoardVin1(float* _adc_mon);

	bool			 GetInitialized()     { return initialized; };
	// uint32_t		 GetCalmuxStatusReg() { return status.CALMUX; };
	// uint32_t		 GetStatusReg()       { return (uint32_t)status.reg; };

	// PWM methods
	void			 SetPwmPeriod_JFAF(int _period_in_ms);
	void			 SetPwmPeriod_JFBF(int _period_in_ms);
	void			 SetPwmPeriod_JFAR(int _period_in_ms);
	void			 SetPwmPeriod_JFBR(int _period_in_ms);
	void			 SetPwmDutyCycle_JFAF(float _dutyCycle);
	void			 SetPwmDutyCycle_JFBF(float _dutyCycle);
	void			 SetPwmDutyCycle_JFAR(float _dutyCycle);
	void			 SetPwmDutyCycle_JFBR(float _dutyCycle);

// Private methods
private:
	void			 SetPwmPeriod_ms(PwmOut* _lpPwm, int _period_in_ms){ _lpPwm->period_ms(_period_in_ms); };
	void			 SetPwmDutyCycle(PwmOut* _lpPwm, float _dutyCycle) { _lpPwm->write(_dutyCycle); };

	// void SetCalmuxStatusBit(uint8_t _status_bit_pos) {
	// 	status.CALMUX |= (1UL << _status_bit_pos);
	// };
	// void ClearCalmuxStatusBit(uint8_t _status_bit_pos) {
	// 	status.CALMUX &= ~(1UL << _status_bit_pos);
	// };
	// void ClearStatusReg() { status.reg = 0;} ;

// Private variables
private:
	bool			initialized;
	// HkTdmStatus_t	status;
};

#endif /* HKTDMCTRL_H */
