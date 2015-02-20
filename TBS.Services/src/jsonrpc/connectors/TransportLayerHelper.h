/*
 * SerialHelpers.h
 *
 *  Created on: Nov 4, 2014
 *      Author: dron
 */

#ifndef SERIALHELPERS_H_
#define SERIALHELPERS_H_


#include <Poco/String.h>
#include <Poco/Crypto/CipherFactory.h>
#include <Poco/Crypto/Cipher.h>
#include <Poco/Crypto/CipherKey.h>

#include "json/json.h"
namespace TBS {
	namespace Services {


			class TransportLayerHelper {
				private:

				public:


					typedef Json::Value Message;
/*
					struct Message {
						std::string interface;
						std::string message;
					};
*/

					struct DataMessage {
							std::string interface;
							std::string message;
					};

					struct HandshakeMessage {
							std::string id;
					};

					struct SessionMessage {
							std::string id;
							std::string sessionKey;
					};

					struct EncryptedDataMessage {
							int counter;
							DataMessage data;
					};

					 Message read(std::string rawStream){
						return unwrapMessage(decode(rawStream));
					}

					 std::string write(const Message & m){
						return encode(wrapMessage(m));
					}


					 Message createDataMessage(std::string interface, std::string message){
						Message v;
						v["t"] = "dat";
						v["req"] = unwrapMessage(message);
						v["i"] = interface;
						return v;
					}

					 Message createHandshakeMessage(std::string id){
						Message v;
						v["t"] = "hnd";
						v["id"] = id;
						return v;
					}

					 Message createSessionMessage(std::string id, std::string sessionKey, std::string masterKey){
						Message v;
						v["t"] = "ses";
						v["id"] = id;
						v["ses"] = this->encrypt(sessionKey, masterKey);
						return v;
					}

					 Message createEncryptedDataMessage(std::string id, std::string interface, std::string message, std::string sessionKey, int counter){
						Message v;
						v["t"] = "edat";
						v["id"] = id;
						Message encrypted;
						encrypted["cnt"] = counter;
						encrypted["data"] = createDataMessage(interface, message);
						v["data"] = encyptMsg(encrypted, sessionKey);
						return v;
					}

					 bool isHandshake(const Message & m){
						return m["t"].asString() == "hnd";
					}
					bool isSession(const Message & m){
						return m["t"].asString() == "ses";
					}
					bool isData(const Message & m){
						return m["t"].asString() == "dat";
					}
					bool isEncryptedData(const Message & m){
						return m["t"].asString() == "edat";
					}

					std::string idFromMessage(const Message & m){
						return m["id"].asString();
					}

					DataMessage parseDataMessage(const Message & m){
						DataMessage dm;
						dm.interface = m["i"].asString();
						dm.message = wrapMessage(m["req"]);
						return dm;
					}

					HandshakeMessage parseHandshakeMessage(const Message & m){
						HandshakeMessage hm;
						hm.id = idFromMessage(m);
						return hm;
					}

					SessionMessage parseSessionMessage(const Message & m, const std::string masterKey){
						SessionMessage sm;
						sm.id = idFromMessage(m);
						sm.sessionKey =   decrypt(m["ses"].asString(), masterKey);
						return sm;
					}

					EncryptedDataMessage parseEncryptedDataMessage(const Message & m, std::string sessionKey){

						EncryptedDataMessage dm;

						Message encrypted = decryptMsg(m["data"].asString(), sessionKey);
						dm.counter = encrypted["cnt"].asInt();
						dm.data = parseDataMessage(encrypted["data"]);
						return dm;
					}


				private:

					 std::string encyptMsg(Message msg, std::string key){
						Poco::Crypto::CipherFactory & factory = Poco::Crypto::CipherFactory::defaultFactory();
						Poco::Crypto::Cipher* pCipher = factory.createCipher(Poco::Crypto::CipherKey("aes-256-cbc", key));
						return pCipher->encryptString(wrapMessage(msg), Poco::Crypto::Cipher::ENC_BASE64_NO_LF);
					}
					 Message decryptMsg(std::string cryptedText, std::string key){
						Poco::Crypto::CipherFactory & factory = Poco::Crypto::CipherFactory::defaultFactory();
						Poco::Crypto::Cipher* pCipher = factory.createCipher(Poco::Crypto::CipherKey("aes-256-cbc", key));
						return unwrapMessage(pCipher->decryptString(cryptedText, Poco::Crypto::Cipher::ENC_BASE64_NO_LF));
					}

					 std::string encrypt(std::string plainText, std::string key){
						Poco::Crypto::CipherFactory & factory = Poco::Crypto::CipherFactory::defaultFactory();
						Poco::Crypto::Cipher* pCipher = factory.createCipher(Poco::Crypto::CipherKey("aes-256-cbc", key));
						return pCipher->encryptString(plainText, Poco::Crypto::Cipher::ENC_BASE64_NO_LF);
					}
					 std::string decrypt(std::string cryptedText, std::string key){
						Poco::Crypto::CipherFactory & factory = Poco::Crypto::CipherFactory::defaultFactory();
						Poco::Crypto::Cipher* pCipher = factory.createCipher(Poco::Crypto::CipherKey("aes-256-cbc", key));
						return pCipher->decryptString(cryptedText, Poco::Crypto::Cipher::ENC_BASE64_NO_LF);
					}

					 std::string registerRequest(std::string id, std::string masterKey){
						Json::FastWriter w;
						Json::Value v, str;
						v["register"] = encyptMsg(id, masterKey);
						return w.write(v);
					}



					 std::string wrapMessage(const Message & m){
						Json::FastWriter w;
						return w.write(m);
					}

					 Message unwrapMessage(std::string msgString){
						Message m;
						Json::Reader r;
						r.parse(msgString, m);
						return m;
					}

					 std::string encode(std::string raw) {
						return Poco::replace(raw, "\n", "\\n") + "\n";
					}
					 std::string decode(std::string encoded) {
						if (!encoded.empty()) {
							//strip white characters
							std::string s = Poco::trim(encoded);
							return Poco::replace(s, "\\n", "\n");
						}
						return "";
					}
			};





	}  // namespace Services

}

#endif /* SERIALHELPERS_H_ */
