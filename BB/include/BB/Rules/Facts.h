/*
 * Facts.h
 *
 *  Created on: Nov 7, 2015
 *      Author: dron
 */

#ifndef FACTS_H_
#define FACTS_H_
#include <BB/Rules/IFact.h>
#include <map>


namespace BB {

typedef std::map <std::string, IFact::Ptr> Facts;

} /* namespace BB */


#endif /* FACTS_H_ */
