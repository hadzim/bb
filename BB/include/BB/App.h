/*
 * App.h
 *
 *  Created on: 20.3.2014
 *      Author: JV
 */

#ifndef APP_H_
#define APP_H_

#include "BB/SensorApp.h"
#include "BB/ForwardApp.h"

#define BB_MAIN(APP) \
int main(int argc, char** argv) {\
	APP app;\
	return app.run(argc, argv);\
}

#define SENSOR_BB_MAIN(name, FACTORY) \
int main(int argc, char** argv) {\
	BB::SensorApp app(name, new FACTORY());\
	return app.run(argc, argv);\
}

#define FWD_BB_MAIN(name, FACTORY) \
int main(int argc, char** argv) {\
	BB::ForwarderApp app(name, new FACTORY());\
	return app.run(argc, argv);\
}

#define FWD_BB_MAIN2(name, FACTORY1, FACTORY2) \
int main(int argc, char** argv) {\
	BB::ForwarderApp app(name, new FACTORY1(), new FACTORY2());\
	return app.run(argc, argv);\
}


#endif /* APP_H_ */
