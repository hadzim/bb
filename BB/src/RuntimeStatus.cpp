/*
 * RuntimeStatus.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: dron
 */

#include <BB/RuntimeStatus.h>
#include <sstream>

#include "Poco/Exception.h"

#include "BB/Sensor/SensorData.h"
namespace BB {



	RuntimeStatus::NamesIndexes::NamesIndexes(){
			this->readableEnum[IndexAlarm] = "Alarm";
			this->readableEnum[IndexPeople] = "People";
			this->readableEnum[IndexElectricity] = "Electricity";
			this->readableEnum[IndexDayPart] = "DayPart";
		}


	RuntimeStatus::NamesAlarm::NamesAlarm(){
		this->readableEnum[AlarmArmed] = "Armed";
		this->readableEnum[AlarmDisarmed] = "Disarmed";
	}

	RuntimeStatus::NamesPeople::NamesPeople(){
		this->readableEnum[PeopleAway] = "People Away";
		this->readableEnum[PeopleComing] = "People Coming";
		this->readableEnum[PeopleHome] = "People Home";
	}



	RuntimeStatus::NamesElectricity::NamesElectricity(){
		this->readableEnum[ElectricityLow] = "Electricity Low";
		this->readableEnum[ElectricityHigh] = "Electricity High";
	}

	RuntimeStatus::NamesDayPart::NamesDayPart(){
		this->readableEnum[DayPartDay] = "Day";
		this->readableEnum[DayPartNight] = "Night";
	}

	RuntimeStatus::NamesIndexes RuntimeStatus::namesIndexes;
	RuntimeStatus::NamesPeople RuntimeStatus::namesPeople;
	RuntimeStatus::NamesElectricity RuntimeStatus::namesElectricity;
	RuntimeStatus::NamesAlarm RuntimeStatus::namesAlarm;
	RuntimeStatus::NamesDayPart RuntimeStatus::namesDayPart;

	static RuntimeStatus::Index2Enum initIE(){
		RuntimeStatus::Index2Enum ie;
		ie[RuntimeStatus::IndexAlarm] = RuntimeStatus::NamesAlarm();
		ie[RuntimeStatus::IndexPeople] = RuntimeStatus::NamesPeople();
		ie[RuntimeStatus::IndexElectricity] = RuntimeStatus::NamesElectricity();
		ie[RuntimeStatus::IndexDayPart] = RuntimeStatus::NamesDayPart();
		return ie;
	}

	RuntimeStatus::Index2Enum RuntimeStatus::index2enum = initIE();

	RuntimeStatus::RuntimeStatus() : date(SensorData::localNow()) {

	}

	RuntimeStatus::RuntimeStatus(const Poco::DateTime & dt) : date(dt){

	}

	RuntimeStatus::StatusMap::mapped_type RuntimeStatus::getStatus(Indexes index){
		RuntimeStatus::StatusMap::iterator i = this->status.find(index);
		if (i == this->status.end()){
			return 0;
		}
		return i->second;
	}
	void RuntimeStatus::setStatus(Indexes index, StatusMap::mapped_type value){
		this->status[index] = value;
	}

	std::string RuntimeStatus::toLongString(const RuntimeStatus & rs){
		std::stringstream s;
		s << "(";
		for (RuntimeStatus::StatusMap::const_iterator it = rs.status.begin(); it != rs.status.end(); it++){
			s << readableIndex(it->first) << ":";
			s << readableValue(it->first,it->second) << ",";
		}
		s << ")";
		return s.str();
	}

	std::string RuntimeStatus::toShortString(const RuntimeStatus & rs){
		std::stringstream s;
		for (RuntimeStatus::StatusMap::const_iterator it = rs.status.begin(); it != rs.status.end(); it++){
			s << (char)it->first;
			s << (int)it->second;
		}
		return s.str();
	}

	RuntimeStatus RuntimeStatus::fromShortString(const std::string & rs){
		return fromShortString(rs, SensorData::localNow());
	}

	RuntimeStatus RuntimeStatus::fromShortString(const std::string & rs, const Poco::DateTime & dt){
		RuntimeStatus status(dt);
		for (std::size_t i = 0; i < rs.size(); i++){
			char current = rs.at(i);
			if (namesIndexes.has(current)){

				char value = rs.at(++i);
				status.setStatus((Indexes)current, (int)(value - '0'));
			}
		}
		return status;
	}

	void RuntimeStatus::update(const RuntimeStatus & rs){
		for (RuntimeStatus::StatusMap::const_iterator i = rs.status.begin(); i != rs.status.end(); i++){
			this->status[i->first] = i->second;
		}
	}

	std::string RuntimeStatus::readableIndex(Indexes index){
		return namesIndexes.toString(index);
	}
	std::string RuntimeStatus::readableValue(Indexes index, StatusMap::mapped_type value){
		if (index2enum.find(index) == index2enum.end()){
			throw Poco::Exception("Unknown index");
		}
		return index2enum.at(index).toString(value);
	}

	Poco::DateTime RuntimeStatus::getDate() const {
		return this->date;
	}
	std::string RuntimeStatus::getDateAsString() const {
		return SensorData::date2string(date);
	}


} /* namespace BB */

std::ostream & operator <<(std::ostream & o, const BB::RuntimeStatus & status){
	o << BB::RuntimeStatus::toLongString(status);
	return o;
}

