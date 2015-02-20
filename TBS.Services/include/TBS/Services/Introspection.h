/*
 * Introspection.h
 *
 *  Created on: Oct 31, 2013
 *      Author: root
 */

#ifndef INTROSPECTION_H_
#define INTROSPECTION_H_
#include <vector>
#include <map>
#include <string>
namespace TBS {
	namespace Services {
		namespace Introspection {

			typedef std::vector<std::string> Namespace;


			struct Argument {
					typedef std::vector<Argument> List;
					std::string name;
					std::string type;
					std::string comment;
					std::string alias;

					Argument(std::string name, std::string type, std::string comment = "") : name(name), type(type), comment(comment){

					}
			};

			struct Struct {
				typedef std::map <std::string, Struct> Map;
				typedef std::vector <std::string> Indexes;
				std::string name;
				Argument::List members;
			};

			struct Method {
					typedef std::vector<Method> List;
					std::string name;
					Argument::List inArguments;
					Argument::List outArguments;
					std::string comment;

					Method(std::string name, std::string comment = "") : name(name), comment(comment){

					}

					bool isVoid(){
						return outArguments.empty();
					}
					bool isReturn(){
						return outArguments.size() == 1;
					}

			};

			struct Signal {
					typedef std::vector<Signal> List;
					std::string name;
					Argument::List arguments;
					std::string comment;

					Signal(std::string name, std::string comment = "") : name(name), comment(comment){

					}
			};

			struct Class {
					typedef std::vector<Class> List;
					std::string fullName;
					std::string name;
					Namespace namesp;
					Method::List methods;
					Signal::List signals;
					std::string comment;

					Class(std::string fullName, std::string name, Namespace namesp, std::string comment = "") : fullName(fullName), name(name), namesp(namesp), comment(comment){

					}
			};

			struct Interface {
					//std::string name;
					Namespace namesp;
					std::string comment;
					Struct::Map structs;
					Struct::Indexes structsIndexes;
					Class::List classes;

					Interface(/*std::string name,*/ Namespace namesp, std::string comment = "") : /*name(name),*/ namesp(namesp), comment(comment){

					}
			};

		}
	}
}

#endif /* INTROSPECTION_H_ */
