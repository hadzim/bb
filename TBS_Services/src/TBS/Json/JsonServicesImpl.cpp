#include "TBS/Services/Json/JsonServicesImpl.h"
/*
 * JsonServerCommChannelHolder.cpp
 *
 *  Created on: Oct 7, 2013
 *      Author: root
 */

namespace TBS {
	namespace Services {

		jsonrpc::HttpServerParams convert(const JsonServerChannel & ch) {
			jsonrpc::HttpServerParams p;
			p.port = ch.port();
			p.allowCrossDomain = ch.isCrossDomainAllowed();
			p.isHttps = ch.isHttps();
			if (ch.isHttps()) {
				p.privateKey = ch.getHttpsPrivateKey();
				p.certificate = ch.getHttpsCertificate();
			}
			p.isProtected = ch.isProtected();
			if (ch.isProtected()) {
				p.username = ch.getUserName();
				p.passwordMd5Hash = ch.getPasswordHash();
			}
			p.isDocFile = ch.hasDocumentation();
			if (ch.hasDocumentation()) {
				p.docFile = ch.getDocumentation();

			}
			return p;
		}

		jsonrpc::HttpClientParams convert(const JsonClientChannel & ch) {
			jsonrpc::HttpClientParams p;
			p.host = ch.hostName();
			p.port = ch.port();
			p.isHttps = ch.isHttps();

			p.isProtected = ch.isProtected();
			if (ch.isProtected()) {
				p.username = ch.getUserName();
				p.password = ch.getPassword();
			}
			return p;
		}

	} /* namespace Services */
} /* namespace TBS */
