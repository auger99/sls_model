## Set up environment
1. Navigate to Documents\Arduino
2. Create a folder named "hardware" if one doesn't already exist
3. Unzip breadboard-1-6-x.zip and save "breadboard" folder in "hardware"
4. Restart Arduino IDE
5. In Arduino IDE, click on Tools > Board > breadboard-avr > "ATmega328 on a breadboard (8Mz internal clock)"


## Burn Bootloader
1. Plug in AVR Pocket Programmer
2. Click on Tools > Programmer > USBtinyISP
3. Click on Tools > "Burn Bootloader"


## Program Board
1. Plug in UART (or FTDI) connector
2. Hold down reset button. Click upload. Let go of reset button.

NeoPixel library: Adafruit NeoPixel 1.12.3