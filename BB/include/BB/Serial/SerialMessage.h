/*
 * SerialMessage.h
 *
 *  Created on: 18.5.2014
 *      Author: JV
 */

#ifndef SERIALMESSAGE_H_
#define SERIALMESSAGE_H_
#include <vector>
#include <string>
#include "BB/Sensor/SensorData.h"

namespace BB {


	/*
	 * byte sequence
	 * 0 - start byte
	 * 1 - NodeID
	 * 2 - NodeStatus
	 * 3 - NodeType
	 * 4 - MessageLength
	 * 5 - n Message
	 */


	class SerialMessage {
		public:



			typedef int NodeID;
			typedef unsigned char Length;
			enum NodeStatus {
				NodeOk = 'O',
				NodeError = 'E'
			};
			enum NodeType {
				NodeTemperature = 'T',
				NodeMotion = 'M'
			};

			enum DataType {
				DataDouble = 'D'
			};

			NodeID 		nodeID;
			NodeStatus 	nodeStatus;
			NodeType 	nodeType;
			DataType 	dataType;
			Length		length;
			std::string message;



			SerialMessage(std::vector <unsigned char> & bytes);
			virtual ~SerialMessage();

			SensorData createSensorData();
		private:


	};

} /* namespace BB */

std::ostream & operator<<(std::ostream & stream, const BB::SerialMessage::NodeType & s);
std::ostream & operator<<(std::ostream & stream, const BB::SerialMessage::NodeStatus & s);
std::ostream & operator<<(std::ostream & stream, const BB::SerialMessage & s);


#endif /* SERIALMESSAGE_H_ */
