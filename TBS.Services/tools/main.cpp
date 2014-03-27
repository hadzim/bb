/**
 * @file main.cpp
 * @date 03.01.2013
 * @author Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @brief This application generates stubs for all specified methods
 * and notifications of a given file
 */

#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <cstring>

#include "Generator/definitions.h"
#include "Generator/InterfaceGenerator.h"
#include "Generator/Json/JsonClientGenerator.h"
#include "Generator/Json/JsonServerGenerator.h"
#include "Generator/Json/JsonServiceGenerator.h"
#include "Generator/DBus/DBusClientGenerator.h"
#include "Generator/DBus/DBusServerGenerator.h"
#include "Generator/DBus/DBusServiceGenerator.h"

#include "Generator/Doc/DocGenerator.h"

#include <set>
#include <Poco/Util/Application.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/StringTokenizer.h>
#include <Poco/String.h>
//#include "Generator/dotNET/dotNETInterfaceGenerator.h"

#include "Generator/Json/JsonJavascriptClientGenerator.h"
#include "Generator/dotNET/dotNETInterfaceGenerator.h"
#include "Generator/dotNET/dotNETClientGenerator.h"
#include "Generator/dotNET/dotNETServiceGenerator.h"


using namespace std;

class GeneratorApp: public Poco::Util::Application {
	public:

		GeneratorApp() {
			setUnixOptions(true);
			help = false;
		}

		void defineOptions(Poco::Util::OptionSet& options) {
			Application::defineOptions(options);

			options.addOption(
					Poco::Util::Option("help", "h", "display help information").required(false).repeatable(false).argument("name=value").callback(
							Poco::Util::OptionCallback<GeneratorApp>(this, &GeneratorApp::handleHelp)));
			options.addOption(
					Poco::Util::Option("def", "d", "path to xml file with definition").required(true).repeatable(false).argument("name=value").callback(
							Poco::Util::OptionCallback<GeneratorApp>(this, &GeneratorApp::handleDefinition)));
			options.addOption(
					Poco::Util::Option("name", "n", "name of generated interface").required(true).repeatable(false).argument("name=value").callback(
							Poco::Util::OptionCallback<GeneratorApp>(this, &GeneratorApp::handleName)));
			options.addOption(
					Poco::Util::Option("relative", "r", "relative path to interface").required(false).repeatable(false).argument("name=value").callback(
							Poco::Util::OptionCallback<GeneratorApp>(this, &GeneratorApp::handleRelative)));
			options.addOption(
					Poco::Util::Option("include", "i", "path where header files should be generated").required(false).repeatable(false).argument("name=value").callback(
							Poco::Util::OptionCallback<GeneratorApp>(this, &GeneratorApp::handleInclude)));
			options.addOption(
					Poco::Util::Option("src", "s", "path where src files should be generated").required(false).repeatable(false).argument("name=value").callback(
							Poco::Util::OptionCallback<GeneratorApp>(this, &GeneratorApp::handleSrc)));
			options.addOption(
								Poco::Util::Option("doc", "dc", "path where doc files should be generated").required(false).repeatable(false).argument("name=value").callback(
										Poco::Util::OptionCallback<GeneratorApp>(this, &GeneratorApp::handleDoc)));
			options.addOption(
					Poco::Util::Option("generate", "g", "what to be generated (Json, DBus, dotNET, doc)").required(false).repeatable(false).argument("name=value").callback(
							Poco::Util::OptionCallback<GeneratorApp>(this, &GeneratorApp::handleGenerate)));
			options.addOption(
					Poco::Util::Option("javascript", "js", "path for javascript").required(false).repeatable(false).argument("name=value").callback(
							Poco::Util::OptionCallback<GeneratorApp>(this, &GeneratorApp::handleJs)));
		}

		void displayHelp() {
			help = true;
			Poco::Util::HelpFormatter helpFormatter(options());
			helpFormatter.setCommand(commandName());
			helpFormatter.setUsage("OPTIONS");
			helpFormatter.setHeader("RPC generator.");
			helpFormatter.format(std::cout);
		}
		void handleHelp(const std::string& name, const std::string& value) {
			displayHelp();
			stopOptionsProcessing();
		}
		void handleDefinition(const std::string& name, const std::string& value) {
			std::cout << "definition: " << name << " val " << value << std::endl;
			definition = value;
		}
		void handleName(const std::string& name, const std::string& value) {
			info.name = value;
		}
		void handleRelative(const std::string& name, const std::string& value) {
			info.relative = value;
		}
		void handleInclude(const std::string& name, const std::string& value) {
			info.includePath = value;
		}
		void handleSrc(const std::string& name, const std::string& value) {
			info.srcPath = value;
		}
		void handleDoc(const std::string& name, const std::string& value) {
			info.docPath = value;
		}
		void handleJs(const std::string& name, const std::string& value) {
			info.jsPath = value;
		}
		void handleGenerate(const std::string& name, const std::string& value) {
			Poco::StringTokenizer st(value, ",");
			for (Poco::StringTokenizer::Iterator i = st.begin(); i != st.end(); i++) {
				std::string data = Poco::trim(*i);
				std::transform(data.begin(), data.end(), data.begin(), ::tolower);
				generate.insert(data);
			}
		}
		int main(const std::vector<std::string>& args) {
			if (help) {
				return EXIT_OK;
			}

			std::cout << "Generating from " << definition << std::endl;

			Poco::File f(definition);
			if (!f.exists()) {
				throw Poco::Exception("Input file does not exist");
			}

			InterfaceGenerator ifgen(definition, info);


			if (generate.find("dotnet") != generate.end()) {
				dotNETInterfaceGenerator dbs(definition, info);
				dotNETClientGenerator dcl(definition, info);
				dotNETServiceGenerator dsv(definition, info);
				//DBusServerGenerator dbs(definition, info);
				//DBusServiceGenerator dbsvc(definition, info);
			}
			if (generate.find("dbus") != generate.end()) {
				DBusClientGenerator dbc(definition, info);
				DBusServerGenerator dbs(definition, info);
				DBusServiceGenerator dbsvc(definition, info);
			}
			if (generate.find("json") != generate.end() && generate.find("jsonp") == generate.end()) {
				std::cout << "JSON NORMAL" << std::endl;
				JsonClientGenerator dbc(definition, info);
				JsonServerGenerator dbs(definition, info);
				JsonServiceGenerator dbsvc(definition, info);
				JsonJavascriptClientGenerator javascript(definition, info);

			}

			if (generate.find("jsonp") != generate.end()) {
				std::cout << "JSONPPPPPPP" << std::endl;
				JsonClientGenerator dbc(definition, info);
				JsonServerGenerator dbs(definition, info);
				JsonServiceGenerator dbsvc(definition, info, true);
				JsonJavascriptClientGenerator javascript(definition, info, true);
			}

			if (generate.find("doc") != generate.end()) {
				DocGenerator doc(definition, info);
			}
			return EXIT_OK;
		}
	private:
		bool help;
		Info info;
		std::string definition;
		std::set<std::string> generate;

};

int main(int argc, char** argv)			\
	{										\
		Poco::AutoPtr<GeneratorApp> pApp = new GeneratorApp;	\
		try									\
		{									\
			pApp->init(argc, argv);			\
		}									\
		catch (Poco::Exception& exc)		\
		{									\
			pApp->logger().log(exc);		\
			return Poco::Util::Application::EXIT_CONFIG;\
		}									\
		return pApp->run();					\
	}

