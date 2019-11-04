# SEMB1451/1452 software library (rev.0.10)

This is a C library for SEMB1451/1452 RZ/A2M Eva Lite board
included with e2studio environment setting files and .scfg file
for Smart Configurator provided by Renesas Electronics Corp.

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

## License
[Apache Licence Ver.2](http://www.apache.org/licenses/)
except included source codes created by the another one.
