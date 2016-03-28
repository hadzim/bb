/*
 * main.cpp
 *
 *  Created on: Jan 31, 2013
 *      Author: root
 */


#include <BB/Node/NodeFactory.h>
#include "BB/App2.h"
#include "BB/Node/INode.h"
#include "BB/Node/Net/ForecastNode.h"

#include "TBS/Log.h"


namespace BB {
	namespace Forecast {


		class Factory: public DynamicNodeFactory {
			private:
				std::vector <INode::Ptr> forecastNodes;

			public:

				Factory() : DynamicNodeFactory("Forecast"){

					for (int i = 0; i < this->getCount(this->getName()); i++){
						std::stringstream s;
						s << "forecast@" << (i+1);
						INode::Ptr n = new ForecastNode(s.str());
						forecastNodes.push_back(n);
					}
					//pin P8_33 -> GPIO9
					//relayNode = new GPOutSwitchNode("relay@1", 9, false);
				}

				virtual int getCheckingPeriodInMs() {
					return 60000;
				}
				virtual INode::PtrList getNodes() {
					return forecastNodes;
				}
		};

	}

} /* namespace BB */

NODE_BB_MAIN("Forecast", BB::Forecast::Factory)



