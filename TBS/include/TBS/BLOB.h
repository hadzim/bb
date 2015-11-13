/*
 * BLOB.h
 *
 *  Created on: Apr 2, 2015
 *      Author: dron
 */

#ifndef TBSBLOB_H_
#define TBSBLOB_H_


#include <Poco/Version.h>


		#if POCO_VERSION >= 0x01060000

				#include <Poco/Data/LOB.h> //BLOB.h>
				#include <Poco/Data/LOBStream.h>


				#define USING_POCO_DATA using namespace Poco::Data::Keywords; using namespace Poco::Data;

				#define POCO16 true

		#else

				#include <Poco/Data/BLOB.h>
				#include <Poco/Data/BLOBStream.h>

				namespace Poco{
					namespace Data {
						typedef BLOB CLOB;
						typedef BLOBOutputStream CLOBOutputStream;
						typedef BLOBInputStream CLOBInputStream;
					}
				}

				#define USING_POCO_DATA using namespace Poco::Data;

				#define POCO16 false

		#endif

#endif /* BLOB_H_ */
