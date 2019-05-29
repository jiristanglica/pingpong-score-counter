# Pingpong Score Counter

Pingpong score counter for our office pingis table.

## Architecture
Basic architecture consist of two Arduino Nano V3s that communicate wirelessly. There is a _buttons module_ and a _display module_. All parts are mounted in 3D printed enclosures. The .stl files for 3D printed parts are inside the `./assembly` folder in this repository.

### Buttons module
Buttons are mounted on each side of the table. Each button is connected into an Arduino Nano V3, which is mounted under the table.

**Components list**:
- 1x [Arduino Nano V3](https://store.arduino.cc/arduino-nano)
- 1x [NRF24L01+ module](https://www.elektroniikkaosat.com/c-45/p-464288850/NRF24L01-lahetin-vastaanotin.html)
- 1x [NRF24L01+ 5V adapter](https://www.elektroniikkaosat.com/c-45/p-529542940/NRF24L01-adapteri.html)
- 2x [Arcade Push Button](https://www.partco.fi/en/electromechanics/switches/arcade-switches/19574-kyt-arc33-sin.html)
- 2x 10kΩ resistor

Fritzing diagram showing the wiring:
![Buttons module schema](https://raw.githubusercontent.com/jiristanglica/pingpong-score-counter/master/assembly/schema_buttons_module.png)

### Display module
Display is in a separate enclosure which can be placed on any suitable place within the reach of the 2.4GHz radio communication (and eyes of the players).

**Components list**:
- 1x [Arduino Nano V3](https://store.arduino.cc/arduino-nano)
- 1x [NRF24L01+ module](https://www.elektroniikkaosat.com/c-45/p-464288850/NRF24L01-lahetin-vastaanotin.html)
- 1x [NRF24L01+ 5V adapter](https://www.elektroniikkaosat.com/c-45/p-529542940/NRF24L01-adapteri.html)
- 1x [MAX7219 8x8x4 LED Matrix Module](https://www.partco.fi/en/arduino/arduino-playground/19683-max7219-mod-8x8x4.html)
- 1x [LK Buzzer](https://www.partco.fi/en/diy-kits/linkerkit/19405-lk-buzzer.html)

Fritzing diagram showing the wiring:
![Buttons module schema](https://raw.githubusercontent.com/jiristanglica/pingpong-score-counter/master/assembly/schema_display_module.png)