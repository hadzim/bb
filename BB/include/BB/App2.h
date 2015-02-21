/*
 * App.h
 *
 *  Created on: 20.3.2014
 *      Author: JV
 */

#ifndef APP2_H_
#define APP2_H_

#include "BB/SensorApp2.h"
#include "BB/NodeApp.h"

#define SENSOR2_BB_MAIN(name, FACTORY) \
int main(int argc, char** argv) {\
	BB::SensorApp2 app(name, new FACTORY());\
	return app.run(argc, argv);\
}

#define NODE_BB_MAIN(name, FACTORY) \
int main(int argc, char** argv) {\
	BB::NodeApp app(name, new FACTORY());\
	return app.run(argc, argv);\
}

#endif /* APP_H_ */
