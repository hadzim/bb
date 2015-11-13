#include <BB/Configuration.h>
#include <cstdlib>
#include <sstream>
#include "TBS/Log.h"
#include "Poco/NumberParser.h"
#include "exploreBB/gpio/PWM.h"
#include <iostream>

int main(int argc, char** argv) {

	float value = Poco::NumberParser::parse(argv[1]);

	exploringBB::PWM pwm("pwm_test_P9_42.10");

	std::cout << "file: " << PWM_PATH << "pwm_test_P9_42.10" << "/" << std::endl;

    pwm.setPeriod(100000);         					// Set the period in ns
    pwm.setDutyCycle(value);       					// Set the duty cycle as a percentage
    pwm.setPolarity(exploringBB::PWM::ACTIVE_LOW);  // using active low PWM
    pwm.run();                    					// start the PWM output
	return 0;
}
