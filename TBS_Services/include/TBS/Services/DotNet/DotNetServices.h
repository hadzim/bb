/*
 * JsonServices.h
 *
 *  Created on: Oct 7, 2013
 *      Author: root
 */

#ifndef DotNetJSONSERVICESa_H_
#define DotNetJSONSERVICESa_H_

#include <string>


namespace TBS {
	namespace Services {


		struct DotNetClientParams {
					std::string host;
					int port;
					std::string query;
		};
	}
}
#endif /* JSONSERVICES_H_ */
