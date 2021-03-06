/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    clientconnector.cpp
 * @date    05.03.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "jsonrpc/clientconnector.h"
#include <cstdlib>

using namespace std;

namespace jsonrpc {
	AbstractClientConnector::~AbstractClientConnector() {

	}

	string AbstractClientConnector::SendMessage(const string &message) {
		string result;
		this->SendMessage(message, result);
		return result;
	}
}
