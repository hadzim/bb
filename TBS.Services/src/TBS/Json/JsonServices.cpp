#include "TBS/Services/Json/JsonServices.h"
#include "jsonrpc/connectors/helpers.h"
#include <fstream>
#include <iostream>
#include "Poco/File.h"
#include <Poco/StreamCopier.h>
#include <iosfwd>
#include "Poco/Net/HTTPServerResponse.h"
#include "TBS/Log.h"
#include "Poco/DigestEngine.h"
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
			//return jsonrpc::md5Hash(plainPassword);
		}

		JsonClientParams::JsonClientParams(std::string hostName, int port, JsonProtocol protocol_) :
				hostName_(hostName), port_(port), protocol_(protocol_), isHttps_(false), isProtected_(false) {

		}

		std::string JsonClientParams::hostName() const {
			return hostName_;
		}

		int JsonClientParams::port() const {
			return port_;
		}

		JsonClientParams::JsonProtocol JsonClientParams::protocol() const{
			return protocol_;
		}

		void JsonClientParams::setHttps(bool https) {
			this->isHttps_ = https;
		}
		void JsonClientParams::setProtected(const std::string & userName, const std::string & password) {
			this->isProtected_ = true;
			this->userName_ = userName;
			this->password_ = password;
		}
		bool JsonClientParams::isHttps() const {
			return isHttps_;
		}

		bool JsonClientParams::isProtected() const {
			return this->isProtected_;
		}
		std::string JsonClientParams::getUserName() const {
			return this->userName_;
		}
		std::string JsonClientParams::getPassword() const {
			return this->password_;
		}

		JsonServerParams::JsonServerParams(int port) :
				port_(port), allowCrossDomain_(false), isHttps_(false), isProtected_(false), passwordAlgorithm(PlainPassword) {
		}


		RequestHandler::~RequestHandler(){

		}

		void JsonServerParams::allowCrossDomain(bool cd) {
			this->allowCrossDomain_ = cd;
		}
		bool JsonServerParams::isCrossDomainAllowed() const {
			return this->allowCrossDomain_;
		}

		int JsonServerParams::port() const {
			return port_;
		}

		void JsonServerParams::setHttps(const std::string & privateKey, const std::string & certificate) {
			this->isHttps_ = true;
			this->httpsPrivateKey_ = privateKey;
			this->httpsCertificate_ = certificate;
		}
		void JsonServerParams::setProtected(const std::string & userName, const std::string & passwordHash, PasswordAlgorithm passwordAlgo) {
			this->isProtected_ = true;
			this->userName_ = userName;
			this->passwordHash_ = passwordHash;
			this->passwordAlgorithm = passwordAlgo;
		}
		bool JsonServerParams::isHttps() const {
			return isHttps_;
		}
		std::string JsonServerParams::getHttpsPrivateKey() const {
			return this->httpsPrivateKey_;
		}
		std::string JsonServerParams::getHttpsCertificate() const {
			return this->httpsCertificate_;
		}

		bool JsonServerParams::isProtected() const {
			return this->isProtected_;
		}
		std::string JsonServerParams::getUserName() const {
			return this->userName_;
		}


		bool JsonServerParams::isPasswordValid(std::string password){
			if (this->passwordAlgorithm == PlainPassword){
				return this->passwordHash_ == password;
			}
			if (this->passwordAlgorithm == MD5Password){
				return computeMD5hash(password) == this->passwordHash_;
			}
			return false;
		}
/*
		std::string JsonServerParams::getPasswordHash() const {
			return this->passwordHash_;
		}

		JsonServerParams::PasswordAlgorithm JsonServerParams::getPasswordAlgorithm() const {
			return this->passwordAlgorithm;
		}
*/

		void JsonServerParams::addSpecialRequestHandler(RequestHandler::Ptr rh){
			this->specialHandlers.push_back(rh);
		}

		RequestHandler::PtrList & JsonServerParams::getRequestHandlers(){
			return this->specialHandlers;
		}

		FileStreamRequestHandler::FileStreamRequestHandler(std::string url, std::string filePath,std::string contentType)
			: url(url), fname(filePath), contentType(contentType){

		}

		FileStreamRequestHandler::~FileStreamRequestHandler(){

		}

		bool FileStreamRequestHandler::canHandle(std::string query){
			return this->url == query;
		}
		void FileStreamRequestHandler::handle(Poco::Net::HTTPServerRequest & request, Poco::Net::HTTPServerResponse & response){
			Poco::File f(fname);
			if (!f.exists()){
				throw Poco::Exception("File " + fname + " does not exists");
			}
			std::cout << "docu real " << fname << std::endl;
			response.setContentType(this->contentType);
			response.setChunkedTransferEncoding(true);
			std::ostream & o = response.send();
			std::ifstream docf(fname.c_str());
			Poco::StreamCopier::copyStream(docf, o);
			return;
		}

	}
}
