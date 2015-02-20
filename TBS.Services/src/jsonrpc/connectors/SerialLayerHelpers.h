/*
 * SerialLayerHelpers.h
 *
 *  Created on: Nov 6, 2014
 *      Author: dron
 */

#ifndef SERIALLAYERHELPERS_H_
#define SERIALLAYERHELPERS_H_

#include <TBS/Services/Serial/SerialServices.h>
#include "TBS/Log.h"
#include "TransportLayer.h"

namespace TBS {
	namespace Services {
		class SerialTransportLayer: public ITransportLayer {
			public:
				SerialTransportLayer(const SerialLayerParams & params) :
						params(params), serial(params.getPort(), params.getBaudrate(), serial::Timeout::simpleTimeout(params.getTimeoutMs())) {
					LTRACE("json") << "create SERIAL LAYER: " << params.getPort() << " br" << params.getBaudrate() << LE;

				}

				virtual void write(const std::string & data) {
					serial.write(data);
				}

				virtual std::string read() {
					int sleepTime = 5;
					for (int i = 0; i < params.getSerialLayer().getTimeoutMs(); i += sleepTime) {
						if (serial.available() > 0) {
							LTRACE("json") << "data ready: " << serial.available() << LE;
							return serial.readline();
						} else {
							//LTRACE("json") << "nothing" << LE;
						}
						Poco::Thread::sleep(sleepTime);
					}
					LTRACE("json") << "no data - timeout" << LE;
					return "";
				}

			private:
				SerialParams params;
				serial::Serial serial;

		};
	}
}

#endif /* SERIALLAYERHELPERS_H_ */
