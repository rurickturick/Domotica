/******
Demo for ssd1306 i2c driver for  Raspberry Pi 
******/

#include "ssd1306_i2c.h"
#include <stdio.h>

void main() {

	ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);

	ssd1306_display(); //Adafruit logo is visible
	ssd1306_clearDisplay();
	delay(5000);

	char* text = "Hola Mundo...\n y sino PUA";
	ssd1306_drawString(text);
	ssd1306_display();
	delay(5000);
	ssd1306_clearDisplay();
	char* text2 = "";
	ssd1306_drawString(text2);
	ssd1306_display();
	delay(5000);
	ssd1306_clearDisplay();
	ssd1306_command(SSD1306_DISPLAYOFF);
	return;
}
