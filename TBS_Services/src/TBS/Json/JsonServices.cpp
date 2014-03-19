#include "TBS/Services/Json/JsonServices.h"
#include "jsonrpc/connectors/httpinterfaceserver.h"
/*
 * JsonServices.cpp
 *
 *  Created on: Oct 7, 2013
 *      Author: root
 */

namespace TBS {
	namespace Services {

		std::string computeMD5hash(std::string plainPassword) {
			return jsonrpc::md5Hash(plainPassword);
		}

		JsonClientChannel::JsonClientChannel(std::string hostName, int port) :
				hostName_(hostName), port_(port), isHttps_(false), isProtected_(false) {

		}

		std::string JsonClientChannel::hostName() const {
			return hostName_;
		}

		int JsonClientChannel::port() const {
			return port_;
		}

		void JsonClientChannel::setHttps(bool https) {
			this->isHttps_ = https;
		}
		void JsonClientChannel::setProtected(const std::string & userName, const std::string & password) {
			this->isProtected_ = true;
			this->userName_ = userName;
			this->password_ = password;
		}
		bool JsonClientChannel::isHttps() const {
			return isHttps_;
		}

		bool JsonClientChannel::isProtected() const {
			return this->isProtected_;
		}
		std::string JsonClientChannel::getUserName() const {
			return this->userName_;
		}
		std::string JsonClientChannel::getPassword() const {
			return this->password_;
		}

		JsonServerChannel::JsonServerChannel(int port) :
				port_(port), allowCrossDomain_(false), isHttps_(false), isProtected_(false), hasDocumentation_(false) {
		}

		void JsonServerChannel::allowCrossDomain(bool cd) {
			this->allowCrossDomain_ = cd;
		}
		bool JsonServerChannel::isCrossDomainAllowed() const {
			return this->allowCrossDomain_;
		}

		int JsonServerChannel::port() const {
			return port_;
		}

		void JsonServerChannel::setHttps(const std::string & privateKey, const std::string & certificate) {
			this->isHttps_ = true;
			this->httpsPrivateKey_ = privateKey;
			this->httpsCertificate_ = certificate;
		}
		void JsonServerChannel::setProtected(const std::string & userName, const std::string & passwordHash) {
			this->isProtected_ = true;
			this->userName_ = userName;
			this->passwordMD5Hash_ = passwordHash;
		}
		bool JsonServerChannel::isHttps() const {
			return isHttps_;
		}
		std::string JsonServerChannel::getHttpsPrivateKey() const {
			return this->httpsPrivateKey_;
		}
		std::string JsonServerChannel::getHttpsCertificate() const {
			return this->httpsCertificate_;
		}

		bool JsonServerChannel::isProtected() const {
			return this->isProtected_;
		}
		std::string JsonServerChannel::getUserName() const {
			return this->userName_;
		}
		std::string JsonServerChannel::getPasswordHash() const {
			return this->passwordMD5Hash_;
		}

		void JsonServerChannel::setDocumentation(std::string doc) {
			this->hasDocumentation_ = true;
			this->documentation = doc;
		}
		bool JsonServerChannel::hasDocumentation() const {
			return hasDocumentation_;
		}
		std::string JsonServerChannel::getDocumentation() const {
			return documentation;
		}

	}
}
