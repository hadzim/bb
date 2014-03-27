/*
 * App.h
 *
 *  Created on: 20.3.2014
 *      Author: JV
 */

#ifndef APP_H_
#define APP_H_

#include "BB/SensorApp.h"

#define BB_MAIN(APP) \
int main(int argc, char** argv) {\
	APP app;\
	return app.run(argc, argv);\
}

#define SENSOR_BB_MAIN(FACTORY) \
int main(int argc, char** argv) {\
	BB::SensorApp app(new FACTORY());\
	return app.run(argc, argv);\
}

#endif /* APP_H_ */
