/*
 * NetworkPage.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: dron
 */

#include "NetworkPage.h"
#include <string>
#include <iostream>

#include <Poco/Net/IPAddress.h>
#include <Poco/Net/NetworkInterface.h>

namespace BB {

	NetworkPage::NetworkPage(IScreen::Ptr screen) :
			Page("network", screen) {

	}

	NetworkPage::~NetworkPage() {

	}

	void NetworkPage::show() {
		screen->setTextSize(1);

		std::cout << "show ip" << std::endl;
		screen->setTextColor(1);

		screen->setCursor(21, 2);
		screen->print("Network");

		Poco::Net::NetworkInterface::NetworkInterfaceList list = Poco::Net::NetworkInterface::list();
		int index = 0;

		for (auto itr = list.begin(); itr != list.end(); ++itr) {
			std::cout << ++index << " " << itr->name() << " " << itr->displayName() << " "
					<< (itr->supportsIPv4() ? "IPv4" : (itr->supportsIPv6() ? "IPv6" : "    ")) << " " << " " << itr->address().toString() << " "
					<< itr->broadcastAddress().toString();

			std::string addressInfo;
			addressInfo += std::string((Poco::Net::IPAddress::IPv4 == itr->address().family()) ? "IPv4" : "IPv6");
			addressInfo += itr->address().isBroadcast() ? " isBroadcast" : "";
			addressInfo += itr->address().isGlobalMC() ? " isGlobalMC" : "";
			addressInfo += itr->address().isIPv4Compatible() ? " isIPv4Compatible" : "";
			addressInfo += itr->address().isIPv4Mapped() ? " isIPv4Mapped" : "";
			addressInfo += itr->address().isLinkLocal() ? " isLinkLocal" : "";
			addressInfo += itr->address().isLinkLocalMC() ? " isLinkLocalMC" : "";
			addressInfo += itr->address().isLoopback() ? " isLoopback" : "";
			addressInfo += itr->address().isMulticast() ? " isMulticast" : "";
			addressInfo += itr->address().isNodeLocalMC() ? " isNodeLocalMC" : "";
			addressInfo += itr->address().isOrgLocalMC() ? " isOrgLocalMC" : "";
			addressInfo += itr->address().isSiteLocal() ? " isSiteLocal" : "";
			addressInfo += itr->address().isSiteLocalMC() ? " isSiteLocalMC" : "";
			addressInfo += itr->address().isUnicast() ? " isUnicast" : "";
			addressInfo += itr->address().isWellKnownMC() ? " isWellKnownMC" : "";
			addressInfo += itr->address().isWildcard() ? " isWildcard" : "";
			std::cout << addressInfo << std::endl;

			if (itr->address().isLoopback()){
				continue;
			}
			if (itr->supportsIPv6()){
				continue;
			}
			//screen->setCursor(2, 12);
			//screen->print("IP:");
			screen->setCursor(2, 13);
			screen->print(itr->address().toString());

			break;

		}

/*
		screen->setCursor(2, 12);
		screen->print("IP: .99.99");

		screen->setCursor(2, 22);
		screen->print("GW: .99.1");*/
	}

} /* namespace BB */
