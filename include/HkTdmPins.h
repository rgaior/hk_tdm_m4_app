
#ifndef HK_TDM_PINS_
#define HK_TDM_PINS_

// TDM Specific peripherals

// CARD ID GPIO
constexpr PinName CARD_ID_0 = PB_12;
constexpr PinName CARD_ID_1 = PB_13;
constexpr PinName CARD_ID_2 = PB_14;
constexpr PinName CARD_ID_3 = PB_15;

// GPIO
constexpr PinName UC_LED = PC_6;
constexpr PinName MPOW_ON = PC_7;

// I2C bus
constexpr PinName I2C1_SCL = PB_6;
constexpr PinName I2C1_SDA = PB_7;

// USART1 connects to the Zynq
constexpr PinName USART1_TX = PA_9;
constexpr PinName USART1_RX = PA_10;

// USART3 implements RS485 Serial
constexpr PinName RS485_DIR = PB_0;
constexpr PinName USART3_TX = PB_10;
constexpr PinName USART3_RX = PC_5;

// UART5
constexpr PinName UART5_TX = PC_12;
constexpr PinName UART5_RX = PD_2;

// PWM pins for front and rear fans
constexpr PinName TIM3_CH1 = PA_6;
constexpr PinName TIM3_CH2 = PA_7_ALT0;
constexpr PinName TIM5_CH2 = PA_1;
constexpr PinName TIM5_CH3 = PA_2;
constexpr PinName FAN_SPD_R0 = PA_5;
constexpr PinName FAN_SPD_R1 = PC_3;
constexpr PinName FAN_SPD_F0 = PA_3;
constexpr PinName FAN_SPD_F1 = PC_4;

// V monitor
constexpr PinName V_MON_0 = PC_0;
constexpr PinName V_MON_1 = PC_1;

#endif

