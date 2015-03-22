# esp8266_irlib


make


To upload to the module, configure the following pins:
Pin 	Level 	Description
CH_PD 	High 	Enables the chip
GPIO0 	Low 	Selects UART download boot mode
GPIO2 	High 	Selects UART download boot mode
GPIO15 	Low 	If availble. Selects UART download boot mode


make ESPPORT=/dev/ttyUSB0 flash
