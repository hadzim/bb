#include <BB/Node/INode.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>

namespace BB {

	POCO_IMPLEMENT_EXCEPTION(NoDataException, Poco::Exception,
			"No data available"
	)

}  // namespace BB

std::ostream & operator<<(std::ostream & o, const BB::INode::EventLogMessage & message){
	o << "time: " << Poco::DateTimeFormatter::format(message.time, Poco::DateTimeFormat::SORTABLE_FORMAT) << " level: " << message.level << " message: " << message.message << " datatype: " << message.dataType << " length: " << message.data.size();
	return o;
}
