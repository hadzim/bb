/*
 * JsonServices.h
 *
 *  Created on: Oct 7, 2013
 *      Author: root
 */

#ifndef JSONSERVICES_H_
#define JSONSERVICES_H_

#include <string>


namespace TBS {
	namespace Services {

		std::string computeMD5hash(std::string plainPassword);

		class JsonClientChannel {
			public:
				JsonClientChannel(std::string hostName, int port);
				std::string hostName() const;
				int port() const;

				void setHttps(bool https = true);
				void setProtected(const std::string & userName, const std::string & password);

				bool isHttps() const;

				bool isProtected() const;
				std::string getUserName() const;
				std::string getPassword() const;
			private:
				std::string hostName_;
				int port_;

				bool isHttps_;

				bool isProtected_;
				std::string userName_;
				std::string password_;
		};

		class JsonServerChannel {
			public:
				JsonServerChannel(int port);
				int port() const;

				void allowCrossDomain(bool cd);
				bool isCrossDomainAllowed() const;

				void setHttps(const std::string & privateKey, const std::string & certificate);
				void setProtected(const std::string & userName, const std::string & passwordHash);

				void setDocumentation(std::string doc);

				bool isHttps() const;
				std::string getHttpsPrivateKey() const;
				std::string getHttpsCertificate() const;

				bool isProtected() const;
				std::string getUserName() const;
				std::string getPasswordHash() const;

				bool hasDocumentation() const;
				std::string getDocumentation() const;

			private:
				int port_;

				bool allowCrossDomain_;

				bool isHttps_;
				std::string httpsPrivateKey_;
				std::string httpsCertificate_;

				bool isProtected_;
				std::string userName_;
				std::string passwordMD5Hash_;

				bool hasDocumentation_;
				std::string documentation;

		};
/*
		class MyClient {
			public:
				typedef Poco::SharedPtr<MyClient> Ptr;
				MyClient(const JsonClientChannel &);

			public:
				TBS::TSeries::Services::HWIO::IServices & Services();

			private:
				JsonClientChannel ch;
				TBS::TSeries::Services::HWIO::IServices::Ptr services_;

		};

		class MyServer {
			public:
				typedef Poco::SharedPtr<MyServer> Ptr;
				MyServer(int port);

			public:
				IServer::Ptr createServices(TBS::TSeries::Services::HWIO::IServices::Ptr interface);

			private:

				int port;
				ICommChannelHolder::Ptr channel;
		};
*/
	}
}
#endif /* JSONSERVICES_H_ */
