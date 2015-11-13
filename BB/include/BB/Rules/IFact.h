/*
 * IFact.h
 *
 *  Created on: Nov 7, 2015
 *      Author: dron
 */

#ifndef IFACT_H_
#define IFACT_H_
#include <Poco/SharedPtr.h>
#include <string>

namespace BB {

class IFact {
public:

	typedef Poco::SharedPtr <IFact> Ptr;

	IFact(std::string name);
	virtual ~IFact();

	const std::string name() const;
private:
	std::string name_;
};

} /* namespace BB */

#endif /* IFACT_H_ */
