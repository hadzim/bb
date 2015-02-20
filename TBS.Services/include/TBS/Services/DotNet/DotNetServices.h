/*
 * JsonServices.h
 *
 *  Created on: Oct 7, 2013
 *      Author: root
 */

#ifndef DotNetJSONSERVICESa_H_
#define DotNetJSONSERVICESa_H_

#include <Poco/Timespan.h>
#include <string>


namespace TBS {
	namespace Services {


		struct DotNetClientParams {
					std::string host;
					int port;
					std::string query;
					Poco::Timespan timeout;
		};
	}
}
#endif /* JSONSERVICES_H_ */
