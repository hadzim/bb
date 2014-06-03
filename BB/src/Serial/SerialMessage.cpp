/*
 * SerialMessage.cpp
 *
 *  Created on: 18.5.2014
 *      Author: JV
 */

#include "Poco/Exception.h"
#include <iostream>
#include "BB/Serial/SerialMessage.h"
#include <Poco/NumberParser.h>
#include <stdio.h>
#include <cstring>

//INDEXES

#define INDEX_NODEID 1
#define INDEX_NODESTATUS 2
#define INDEX_NODETYPE 3
#define INDEX_DATATYPE 4
#define INDEX_LENGTH 5
#define INDEX_MESSAGE0 6

//VALUES

#define NODETYPE_TEMPERATURE 1

#define DATATYPE_DOUBLE 1

#define NODESTATUS_OK 1
#define NODESTATUS_ERROR 2

namespace BB {

	static SerialMessage::NodeStatus convertStatus(unsigned char byte) {

		switch (byte) {
			case SerialMessage::NodeOk:
				return SerialMessage::NodeOk;
				break;
			case SerialMessage::NodeError:
				return SerialMessage::NodeError;
				break;
		}
		throw Poco::Exception("Unknown status byte");
	}

	static SerialMessage::NodeType convertType(unsigned char byte) {

		switch (byte) {
			case SerialMessage::NodeTemperature:
				return SerialMessage::NodeTemperature;
				break;
		}
		throw Poco::Exception("Unknown type byte");
	}

	static SerialMessage::DataType convertDataType(unsigned char byte) {

			switch (byte) {
				case SerialMessage::DataDouble:
					return SerialMessage::DataDouble;
					break;
			}
			throw Poco::Exception("Unknown data byte");
		}

	SerialMessage::SerialMessage(std::vector<unsigned char> & bytes) {
		if (bytes.size() < INDEX_LENGTH) {
			throw Poco::Exception("Message too short");
		}
		if (bytes.at(0) != 0) {
			throw Poco::Exception("First byte should be 0x00");
		}

		nodeID = bytes.at(INDEX_NODEID);
		nodeStatus = convertStatus(bytes.at(INDEX_NODESTATUS));
		nodeType = convertType(bytes.at(INDEX_NODETYPE));
		dataType = convertDataType(bytes.at(INDEX_DATATYPE));

		length = bytes.at(INDEX_LENGTH);
		for (unsigned char i = 0; i < length; i++) {
			message.push_back((char) bytes.at(INDEX_MESSAGE0 + i));
		}
	}

	SerialMessage::~SerialMessage() {

	}

	static std::string gettype(const BB::SerialMessage & s) {
		switch (s.nodeType) {
			case BB::SerialMessage::NodeTemperature:
				return SensorData::Temperature;
				break;
		}
		throw Poco::Exception("Non existing value");
	}

	static std::string getunit(const BB::SerialMessage & s) {
			switch (s.nodeType) {
				case BB::SerialMessage::NodeTemperature:
					return SensorData::UnitTemperature;
					break;
			}
			throw Poco::Exception("Non existing value");
		}


	static double getval(const BB::SerialMessage & s) {
			switch (s.dataType) {
				case BB::SerialMessage::DataDouble:
					return Poco::NumberParser::parseFloat(s.message);
					break;
			}
			throw Poco::Exception("Non parsable value");
		}

	SensorData SerialMessage::createSensorData(){

		std::string type = gettype(*this);
		std::string unit = getunit(*this);
		char buffer[15];
		sprintf(buffer, "%s%04d", "Serial", this->nodeID);
		std::string name(buffer, strlen(buffer));
		double val = getval(*this);
		return SensorData(
						type,
						name,
						name,
						unit,
						SensorData::localNow(),
						SensorData::Sensor_Ok,
						val,
						message);
	}

} /* namespace BB */

std::ostream & operator<<(std::ostream & stream, const BB::SerialMessage::NodeType & s) {
	switch (s) {
		case BB::SerialMessage::NodeTemperature:
			stream << "Temperature";
			break;
	}
	return stream;
}
std::ostream & operator<<(std::ostream & stream, const BB::SerialMessage::DataType & s) {
	switch (s) {
		case BB::SerialMessage::DataDouble:
			stream << "Double";
			break;
	}
	return stream;
}
std::ostream & operator<<(std::ostream & stream, const BB::SerialMessage::NodeStatus & s) {
	switch (s) {
		case BB::SerialMessage::NodeOk:
			stream << "Ok";
			break;
		case BB::SerialMessage::NodeError:
			stream << "Error";
			break;
	}
	return stream;
}

std::ostream & operator<<(std::ostream & stream, const BB::SerialMessage & s) {
	stream << "NodeID " << (int) s.nodeID << " status: " << s.nodeStatus << " type: " << s.nodeType << " datatype: " << s.dataType << " len: "<< (int)s.length << " msg: "
			<< s.message ;
	return stream;
}
