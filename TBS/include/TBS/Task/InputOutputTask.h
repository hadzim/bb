/*
 * InputOutputTask.h
 *
 *  Created on: Feb 2, 2012
 *      Author: root
 */

#ifndef TBSNewINPUTOUTPUTTASK_H_
#define TBSNewINPUTOUTPUTTASK_H_
#include "Task.h"
#include "TBS/Nullable.h"

namespace TBS {

	namespace Task {
		typedef void * NullType;

		template<class InputDataType>
		class InputTask: public Task {
			public:
				typedef InputDataType Input;

				InputTask(std::string name, INotificationWorker::Ptr nw) :
						Task(name, nw) {
				}

				void setInput(const Input & input) {
					if (this->isActive()) {
						throw Poco::Exception("Not able to set input in progress - " + this->getName());
					}
					this->input.set(input);
				}

				void clearInput() {
					if (this->isActive()) {
						throw Poco::Exception("Not able to clear input in progress - " + this->getName());
					}
					this->input.reset();
				}

				void clear() {
					this->clearInput();
				}

			protected:
				Input & getInput() {
					return this->input.getReference();
				}

			private:
				Nullable<Input> input;
		};

		template<class OutputDataType>
		class OutputTask: public Task {
			public:
				typedef OutputDataType Output;

				OutputTask(std::string name, INotificationWorker::Ptr nw) :
						Task(name, nw) {
				}

				Output & getOutput() {
					if (this->isActive()) {
						throw Poco::Exception("Not able to read output in progress - " + this->getName());
					}
					return this->output.getReference();
				}

				void clearOutput() {
					if (!this->isActive()) {
						throw Poco::Exception("Not able to clear output not in progress - " + this->getName());
					}
					this->output.reset();
				}
				void clear() {
					this->clearOutput();
				}

			protected:

				void setOutput(const Output & value) {
					if (!this->isActive()) {
						throw Poco::Exception("Not able to set output not in progress - " + this->getName());
					}
					this->output.set(value);
				}

			private:

				Nullable<Output> output;
		};

		template<class InputDataType, class OutputDataType>
		class InputOutputTask: public Task {
			public:
				InputOutputTask(std::string name, INotificationWorker::Ptr nw) :
						Task(name, nw) {

				}

			public:
				typedef OutputDataType Output;
				typedef InputDataType Input;

				void setInput(const Input & input) {
					if (this->isActive()) {
						throw Poco::Exception("Not able to set input in progress - " + this->getName());
					}
					this->input.set(input);
				}

				Output & getOutput() {
					if (this->isActive()) {
						throw Poco::Exception("Not able to read output in progress - " + this->getName());
					}
					return this->output.getReference();
				}

				void clearInput() {
					if (this->isActive()) {
						throw Poco::Exception("Not able to clear input in progress - " + this->getName());
					}
					this->input.reset();
				}
				void clearOutput() {
					//if (!this->isActive()) {
					//	throw Poco::Exception("Not able to clear output not in progress - " + this->getName());
					//}
					this->output.reset();
				}
				void clear() {
					this->clearInput();
					this->clearOutput();
				}
			protected:
				Input & getInput() {
					return this->input.getReference();
				}
				void setOutput(const Output & value) {
					if (!this->isActive()) {
						throw Poco::Exception("Not able to set output not in progress - " + this->getName());
					}
					this->output.set(value);
				}

			private:
				Nullable<Input> input;
				Nullable<Output> output;

		};

	} /* namespace Task */
} /* namespace TBS */
#endif /* INPUTOUTPUTTASK_H_ */
