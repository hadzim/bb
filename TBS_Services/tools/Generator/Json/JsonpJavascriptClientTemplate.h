#ifndef CLIENTTEMPLATE_CLIENTjsonp_H_
#define CLIENTTEMPLATE_CLIENTjsonp_H_

#define JSONP_TEMPLATE_CLIENT_METHOD "\
        this.<methodName> = function(<inParams>){\n\
			return $.ajax({\n\
    			url: settings.url,\n\
    			jsonp: \"callback\",\n\
    			dataType: \"jsonp\",\n\
    			data: {\n\
    				method: \"<methodName>\"<inParamsAssign>\n\
				}\n\
			});\n\
		};\n\
"

#define JSONP_TEMPLATE_CLASS "\
function <className>(params){\n\
  var settings = {\n\
      url: params.url + \"<interfaceName>\",\n\
  };\n\
  \n\
  \n //methods \n\
  <methods>\n\
}\n\
\n"


#endif
