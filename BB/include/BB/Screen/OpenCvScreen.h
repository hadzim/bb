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
#ifndef _OpencvScreen_H
#define _OpencvScreen_H

#include "BB/Screen/IScreen.h"

#include "opencv2/core/core.hpp"

typedef volatile int PortReg;
typedef uint32_t PortMask;

#define BLACK 1
#define WHITE 0

#define OPENCVWIDTH 84
#define OPENCVHEIGHT 48

namespace BB {

	class OpenCvScreen: public IScreen {
		public:
			OpenCvScreen(std::string name);
			virtual ~OpenCvScreen();

			void clearDisplay();
			void display();

			void drawPixel(int16_t x, int16_t y, uint16_t color);

			void setMaxIntensity(int);

		private:

		private:
			std::string name;
			cv::Mat buffer;
			int intW;
			int intH;

			int maxIntensity;

	};
}
#endif
