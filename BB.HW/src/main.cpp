#include "Node/HWNode.h"
#include "BB/Node/INode.h"
#include "BB/App2.h"
namespace BB {

class ScreenFactory: public INodeFactory {

	public:

		UI::Ptr ui;
		INode::Ptr screenNode;

		ScreenFactory(){
			ui = new UI();
			screenNode = new HWNode(ui);
		}

		virtual int getCheckingPeriodInMs() {
			return 60000;
		}
		virtual INode::PtrList getNodes() {
			INode::PtrList nodes;

			{
				nodes.push_back(screenNode);
			}

			return nodes;
		}
};

} /* namespace BB */

NODE_BB_MAIN("Screen", BB::ScreenFactory)

