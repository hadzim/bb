/*
 * RuntimeStatus.h
 *
 *  Created on: Oct 2, 2014
 *      Author: dron
 */

#ifndef RUNTIMESTATUS_H_
#define RUNTIMESTATUS_H_

#include "TBS/ReadableEnum.h"
#include <iosfwd>
#include <map>
#include <string>

#include "Poco/DateTime.h"

namespace BB {

	class RuntimeStatus {
		public:

			typedef std::map <int, RuntimeStatus> Map;

			enum Indexes {
				IndexAlarm = 'A',
				IndexPeople = 'P',
				IndexElectricity = 'E',
				IndexDayPart = 'D',
			};

			enum Alarm {
				AlarmArmed = 1,
				AlarmDisarmed = 2,
			};

			enum People {
				PeopleAway = 1,
				PeopleComing = 2,
				PeopleHome = 3,
			};

			enum Electricity {
				ElectricityLow = 1,
				ElectricityHigh = 2,
			};

			enum DayPart {
				DayPartDay = 1,
				DayPartNight = 2,
			};


			struct NamesAlarm: public TBS::ReadableEnum {
					NamesAlarm();
			};

			struct NamesIndexes: public TBS::ReadableEnum {
					NamesIndexes();
			};
			struct NamesPeople: public TBS::ReadableEnum {
					NamesPeople();
			};

			struct NamesElectricity: public TBS::ReadableEnum {
					NamesElectricity();
			};

			struct NamesDayPart: public TBS::ReadableEnum {
					NamesDayPart();
			};


			static NamesIndexes namesIndexes;
			static NamesAlarm namesAlarm;
			static NamesPeople namesPeople;
			static NamesElectricity namesElectricity;
			static NamesDayPart namesDayPart;

			typedef std::map <Indexes, TBS::ReadableEnum> Index2Enum;
			static Index2Enum index2enum;

			RuntimeStatus();
			RuntimeStatus(const Poco::DateTime & dt);

			static std::string toLongString(const RuntimeStatus & rs);
			static std::string toShortString(const RuntimeStatus & rs);
			static RuntimeStatus fromShortString(const std::string & rs);
			static RuntimeStatus fromShortString(const std::string & rs, const Poco::DateTime & dt);

			typedef std::map <Indexes, int> StatusMap;

			StatusMap::mapped_type getStatus(Indexes index);
			void setStatus(Indexes index, StatusMap::mapped_type);

			void update(const RuntimeStatus & rs);

			static std::string readableIndex(Indexes index);
			static std::string readableValue(Indexes index, StatusMap::mapped_type value);

			typedef StatusMap::iterator iterator;
			typedef StatusMap::const_iterator const_iterator;

			iterator begin(){
				return status.begin();
			}
			const_iterator begin() const {
				return status.begin();
			}

			iterator end(){
				return status.end();
			}
			const_iterator end() const {
				return status.end();
			}

			Poco::DateTime getDate() const;
			std::string getDateAsString() const;


		private:
			StatusMap status;
			Poco::DateTime date;


	};

} /* namespace BB */

std::ostream & operator <<(std::ostream & o, const BB::RuntimeStatus & status);

#endif /* RUNTIMESTATUS_H_ */
