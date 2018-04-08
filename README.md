# B-Sides Iowa 2018 Badge

This is the repo serves as the project development page for the B-Sides Iowa badge

## Background 

* This badge serves as B-Sides Iowa's first electronic badge!
* Consequently primary design goals included low manufacturing costs and friendly user customization.  We wanted to create an electronic badge, but didn't want to risk blowing the bank for a small non-profit conference.
* Boards were manufactured with Seeed Fusion PCB
* Other cons are welcome to use this badge as a starting point for their own badges

## Badge Specs

* ATtiny85 Microcontroller, Internal Clock 
* 12x Charlieplexed LEDs
* 8K Flash
* Momentary Reset
* Programmable Header
* Powered via CR2032 coin cell

## Badge Functions:

* LED patterns
* Personalization
* GPIOs/SPI

## Third Party Tools

The following packages were used to create this badge
* Autodesk Eagle https://www.autodesk.com/products/eagle
* Arduino IDE https://www.arduino.cc/en/Main/Software
* Sparkfun AVR Programmer https://www.sparkfun.com/products/9825

You are certainly not limited to these packages.  Any AVR compiler/programmer will do, and KiCAD is also a top notch open source alternative to Eagle. 

### Prerequisites

Be sure to configure your compiler to use the ATtiny85.  Packages are available at
https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json

### Uploading firmware to badge

1. Either use an AVR programmer with POGO pins or solder a 2x3 header to the board
2. Connect the AVR programmer to the header, ensuring to match the orientation for pin 1.  (If you are using a POGO adapter at the conference, just flip the board upside down such that the microcontroller is showing.  Orient the adapter such that the ribbon cable is directly above the microcontroller.
3. Upload the code!

## Authors / Contributors

* **G Rice**

## Acknowledgments

* Tip of of the hat to D. Darko for the Tri-State Pulse Width Modulator
* Thanks to the Seeed team for the great CAM service and inspection
* Special thanks to the B-Sides sponsors for making this badge a reality
