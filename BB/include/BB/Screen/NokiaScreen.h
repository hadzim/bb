/*********************************************************************
 This is a library for our Monochrome Nokia 5110 LCD Displays

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/products/338

 These displays use SPI to communicate, 4 or 5 pins are required to
 interface

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 Written by Limor Fried/Ladyada  for Adafruit Industries.
 BSD license, check license.txt for more information
 All text above, and the splash screen must be included in any redistribution
 *********************************************************************/
#ifndef _NokiaScreen_H
#define _NokiaScreen_H

#include "exploreBB/bus/SPIDevice.h"
#include "exploreBB/gpio/GPIO.h"

#include "IScreen.h"

#include "exploreBB/gpio/PWM.h"

typedef volatile int PortReg;
typedef uint32_t PortMask;

#define BLACK 1
#define WHITE 0

#define LCDWIDTH 84
#define LCDHEIGHT 48

#define PCD8544_POWERDOWN 0x04
#define PCD8544_ENTRYMODE 0x02
#define PCD8544_EXTENDEDINSTRUCTION 0x01

#define PCD8544_DISPLAYBLANK 0x0
#define PCD8544_DISPLAYNORMAL 0x4
#define PCD8544_DISPLAYALLON 0x1
#define PCD8544_DISPLAYINVERTED 0x5

// H = 0
#define PCD8544_FUNCTIONSET 0x20
#define PCD8544_DISPLAYCONTROL 0x08
#define PCD8544_SETYADDR 0x40
#define PCD8544_SETXADDR 0x80

// H = 1
#define PCD8544_SETTEMP 0x04
#define PCD8544_SETBIAS 0x10
#define PCD8544_SETVOP 0x80

// Default to max SPI clock speed for PCD8544 of 4 mhz (16mhz / 4) for normal Arduinos.
// This can be modified to change the clock speed if necessary (like for supporting other hardware).
#define PCD8544_SPI_CLOCK_DIV SPI_CLOCK_DIV4

//DC - P9-14 = 50
//RST - P9-12 = 60
//SPIPORT 1
//SPI DEVICE 0

namespace BB {

	class NokiaScreen: public IScreen {
		public:
			NokiaScreen(int8_t DC, /*int8_t CS,*/int8_t RST, int spiport, int spiaddr, std::string pwmName);
			virtual ~NokiaScreen();

			void setContrast(uint8_t val);

			void clearDisplay(void);
			void display();

			void drawPixel(int16_t x, int16_t y, uint16_t color);
			uint8_t getPixel(int8_t x, int8_t y);

			void setMaxIntensity(int);
		private:

			void command(uint8_t c);
			void data(uint8_t c);

			int8_t _din, _sclk, _dc, _rst, _cs;
			volatile PortReg *mosiport, *clkport;
			PortMask mosipinmask, clkpinmask;

			void spiWrite(uint8_t c);
			bool isHardwareSPI();


		private:
			void init();
			void begin(uint8_t contrast = 40, uint8_t bias = 0x04);

			exploringBB::GPIO dcPin;
			exploringBB::GPIO rstPin;
			exploringBB::SPIDevice * busDevice;

			exploringBB::PWM intensity;

			int spiport;
			int spiaddr;
	};

}

#endif
