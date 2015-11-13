/*
 * ISensor.h
 *
 *  Created on: Feb 1, 2013
 *      Author: root
 */

#ifndef BASICSENSORNODE_H_
#define BASICSENSORNODE_H_
#include "Poco/SharedPtr.h"
#include <vector>
#include <string>

#include "BB/Node/INode.h"

namespace BB {


	class BasicNode : public INode {
		public:
			typedef Poco::SharedPtr <INode> Ptr;
			typedef std::vector<Ptr> PtrList;

			BasicNode(Node::Info info, int period) : info(info), period(period){
				for (auto & s : info.getSettings()){
					this->settings.insert(std::make_pair(s.first, s.second.defaultValue));
				}
			}
			virtual ~BasicNode(){}

			virtual Node::Info getInfo(){
				return info;
			}
			virtual int getCheckingPeriodInMs(){
				return period;
			}

			bool updateSetting(SettingsValues newValues){
				bool changed = true;
				for (auto & s : newValues){
					if (settings.find(s.first) != settings.end()){
						if (this->settings.at(s.first) != s.second){
							this->settings.at(s.first) = s.second;
							changed = true;
						}
					}

				}
				if (changed){
					SettingsChanged.notify(this, settings);
				}
				return changed;
			}
		public:
			SettingsValues getSettings(){
				return this->settings;
			}

		private:
			Node::Info info;
			SettingsValues settings;
			int period;
	};

} /* namespace TBS */
#endif /* ISENSOR_H_ */
