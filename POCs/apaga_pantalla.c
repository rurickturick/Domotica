/******
Demo for ssd1306 i2c driver for  Raspberry Pi 
******/

#include "ssd1306_i2c.h"
#include <stdio.h>
void main() {

	ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
	ssd1306_clearDisplay();
	char* text = "";
	ssd1306_drawString(text);
	ssd1306_display();
	return ;
}
