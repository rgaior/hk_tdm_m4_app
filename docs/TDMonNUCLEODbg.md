# Debug TDM using NUCLEO F446RE board

To debug the TDM application, the `NUCLEO F446RE` board can be adapted. Actually, except for the LED which is driven by pin PA_5 (when SB21 and SB42 are shorted), the code for the TDM is compatible with the NUCLEO board.

The executable file can be uploaded as it is and the signals checked on the expansion connectors.

## Testing Analog inputs

Two analog inputs are foreseen on the TDM, on pins PC_0 and PC_1.

On the NUCLEO pins PC_0/1 can be brought to connectors CN7 and CN8 to applying jumpers SB51/6. The following table shows the correspondance of these signals on the connectors:

| Pin  | Sch name | Conn name | ARDUINO | MORPHO |
|:----:|:--------:|:---------:|:-------:|:------:|
| PC_0 |    PC0   |    A5     | CN7.38  | CN8.6  |
| PC_1 |    PC1   |    A4     | CN7.36  | CN8.5  |

Check the expansion connectors in the following figure, both as schematic version and board view.

![NUCLEO_F446RE_CONN](NUCLEO_Connector.png "NUCLEO F446RE Expansion Connectors")

![NUCLEO_F446RE_CONN](NUCLEO_Con.png "NUCLEO F446RE Expansion Connectors")

## Using Serial Ports

The main serial port on the TDM board is UART1 on pins PA_9/10, dedicated to the communication with the *uScale+ Zynq FPGA*. On the NUCLEO_F446RE these pins are mapped as in the following table:

| Pin  | Sch name | Conn name | ARDUINO | MORPHO |
|:----:|:--------:|:---------:|:-------:|:------:|
| PA_9 |   PA9    |     D8    | CN5.1   | CN10.21|
| PA_10|   PC1    |     D2    | CN9.3   | CN10.33|

Since the mbed-os standard serial port uses by default the pins PA_2/3, named STDIO_UART_TX/_RX, the file `PeripheralPins.c` must be edited appropriately if the same pins will be used for something else. In the case of TDM board, PA_2 and PA_3 are used as timer and fan control. To avoid conflicts, comment and uncomment the lines in the file to select the configuration of these pins in a compatible way.

Since the STDIO_UART_TX/RX labels are mandatory to compile under mbed-os, I assigned pins PA_9/10.

The default UART debug pins can be changed according to this document [Choose UART debug pins](https://os.mbed.com/teams/ST/wiki/STDIO).

https://os.mbed.com/teams/ST/wiki/steps-to-create-a-new-STM32-platform
