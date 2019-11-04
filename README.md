# SEMB1451/1452 software library (rev.0.10)

This is a C library for SEMB1451/1452 RZ/A2M Eva Lite board
included with e2studio environment setting files and .scfg file
for Smart Configurator provided by Renesas Electronics Corp.
  
This library is based on software package version 5 provided by Renesas.
So you can easily modify and add this codes with e2studio on Windows PC.
  
Let's enjoy it !

## Information for target board

SEMB-1451/1452 is provided by Shimafuji in Japan.
  
JPN : http://www.shimafuji.co.jp/products/1767
  
ENG : http://www.shimafuji.co.jp/en/products/1522

## Installation

Import this library into e2studio.

```e2studio
[New Project] -> [Import] ~/demos/renesas/rza2m-ebk/e2studio
```

## Usage

Build the project on e2studio/Windows PC.
If there is no error, you can get the hex code for SEMB1451.
After that, please wirte the firmware to RZ/A2M with e2studio debug mode.

If you use the console as debug or something to do,
because SCIFA4 is defined as the console of RZ/A2M,
you should connect the USB-Serial converter onto SCIFA4 connector.
Please set Vertual COM port as terminal with Data 8bit, Non-Parity, Stop 1bit, and 115,200bps.

## Contributing

You can modify the peripheral function with Smart Configulator.
If you will do it, please check the user's manual for smart configulator.

## Current Support Peripherals

|Function Name    | Peripheral Name | Status | notes |
|:----------------|:----------------|:------:|:------|
|Serial Serbo I/F |SCIFA2-4         | ready  | SCIFA4 is normally using as console     |
|RSS485 I/F       |SCIFA0,1         | ready  | automatically direction changing(RX,TX) | 
|ADC I/F          |ADC4-7           | ready  | sample ADC4,5:Joystick, ADC6:distance   |
|Battery Monitor  |ADC2             | ready  | |
|RSPI I/F         |RSPI0,1          | ready  | spi master |
|Ethernet         |Ethernet0        | N.A.   | underdevelopment |
|USB Host         |USB0             | N.A.   | underdevelopment |
|USB Function     |USB1             | N.A.   | underdevelopment |
|micro SD slot    |SDHI0            | N.A.   | underdevelopment |
|CAN(FD) I/F      |CAN(FD)          | N.A.   | underdevelopment |

## License
[Apache Licence Ver.2](http://www.apache.org/licenses/)
except included source codes created by the another one.
