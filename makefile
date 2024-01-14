device:=$(shell ls /dev | grep ACM)

default:
	avr-gcc  -DF_CPU=16000000 -Os -mmcu=atmega328p Tetris.c -o Tetris
	avr-objcopy -O ihex -R .eeprom Tetris Tetris.hex
	avrdude -F -V -c arduino -p m328P -P /dev/$(device) -b 115200 -U flash:w:Tetris.hex
	rm Tetris Tetris.hex

