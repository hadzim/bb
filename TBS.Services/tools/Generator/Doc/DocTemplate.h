#ifndef DOCTEMPLATE_CLIENT_H_
#define DOCTEMPLATE_CLIENT_H_


#define TEMPLATE_INARGUMENT "<dt>Arguments:</dt>"

#define TEMPLATE_OUTARGUMENT "<dt>Result:</dt>\n"

#define TEMPLATE_ARGUMENT "<dd><code><type> <name> <i class='comment'><comment></i></code></dd>\n"

#define TEMPLATE_METHOD "\
	<div class='well emptywell'>\n\
			  <h3 id='<methodName>'><methodName></h3>\n\
              <p class='comment'><methodComment></p>\n\
              <dl>\n\
                <dt>Signature:</dt>\n\
                <dd><code><returnType> <b><methodName></b>(<params>)</code></dd>\n\
                <inputParams>\n\
                <outputParams>\n\
              </dl>\n\
  </div>\n\
"

#define TEMPLATE_CLASS "\
<h2 id='<className>'><className></h2>\n\
            <p class='comment'><classComment></p>\n\
            <dl>\n\
                <dt>Service Address:</dt>\n\
                 <dd><code><address></code></dd>\n\
                 <dt>Functions:</dt>\n\
            </dl>\n\
            <div class='leftmargin'>\n\
            	<methods>\n\
          	</div>\n\
"


#define TEMPLATE_STUB "\
<!DOCTYPE html>\n\
<html>\n\
  <head>\n\
    <title><serviceName></title>\n\
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n\
    <style type='text/css'>\n\
    	<bootstrap1>\n\
    	<bootstrap2>\n\
    	<bootstrap3>\n\
	</style>\n\
	<style type='text/css'>\n\
body {  padding-top: 20px;  padding-bottom: 40px;}\n\
.leftmargin {  margin-left: 5px;}\n\
dd code {  display: block;  margin: 5px;  padding: 1px 10px;  color: black;}\n\
dd code i {  color: gray;}\n\
.comment {  font-style: italic;  color: gray;}\n\
.emptywell {  background-color: white;}\n\
	</style>\n\
  </head>\n\
  <body>\n\
  <div class='container-fluid'>\n\
      <div class='row-fluid'>\n\
        <div class='span3'>\n\
          <div class='well sidebar-nav'>\n\
            <ul class='nav nav-list'>\n\
                <li class='nav-header'>Services</li>\n\
                <serviceNavigation>\n\
            </ul>\n\
          </div>\n\
        </div>\n\
        <div class='span9'>\n\
          <div class='masthead'>\n\
            <h1><serviceName></h1>\n\
            <p class='comment'><serviceComment></p>\n\
          </div>\n\
          <hr />\n\
          	<classes>\n\
        </div>\n\
    </div>\n\
  </div>\n\
  </body>\n\
</html>\n\
"

#define TEMPLATE_BOOTSTRAP1 "\
		/*!\n\
		 * Bootstrap v2.3.2\n\
		 *\n\
		 * Copyright 2013 Twitter, Inc\n\
		 * Licensed under the Apache License v2.0\n\
		 * http://www.apache.org/licenses/LICENSE-2.0\n\
		 *\n\
		 * Designed and built with all the love in the world @twitter by @mdo and @fat.\n\
		 */\n\
		.clearfix{*zoom:1;}.clearfix:before,.clearfix:after{display:table;content:\"\";line-height:0;}\n\
		.clearfix:after{clear:both;}\n\
		.hide-text{font:0/0 a;color:transparent;text-shadow:none;background-color:transparent;border:0;}\n\
		.input-block-level{display:block;width:100%;min-height:30px;-webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box;}\n\
		article,aside,details,figcaption,figure,footer,header,hgroup,nav,section{display:block;}\n\
		audio,canvas,video{display:inline-block;*display:inline;*zoom:1;}\n\
		audio:not([controls]){display:none;}\n\
		html{font-size:100%;-webkit-text-size-adjust:100%;-ms-text-size-adjust:100%;}\n\
		a:focus{outline:thin dotted #333;outline:5px auto -webkit-focus-ring-color;outline-offset:-2px;}\n\
		a:hover,a:active{outline:0;}\n\
		sub,sup{position:relative;font-size:75%;line-height:0;vertical-align:baseline;}\n\
		sup{top:-0.5em;}\n\
		sub{bottom:-0.25em;}\n\
		img{max-width:100%;width:auto\\9;height:auto;vertical-align:middle;border:0;-ms-interpolation-mode:bicubic;}\n\
		#map_canvas img,.google-maps img{max-width:none;}\n\
		button,input,select,textarea{margin:0;font-size:100%;vertical-align:middle;}\n\
		button,input{*overflow:visible;line-height:normal;}\n\
		button::-moz-focus-inner,input::-moz-focus-inner{padding:0;border:0;}\n\
		button,html input[type=\"button\"],input[type=\"reset\"],input[type=\"submit\"]{-webkit-appearance:button;cursor:pointer;}\n\
		label,select,button,input[type=\"button\"],input[type=\"reset\"],input[type=\"submit\"],input[type=\"radio\"],input[type=\"checkbox\"]{cursor:pointer;}\n\
		input[type=\"search\"]{-webkit-box-sizing:content-box;-moz-box-sizing:content-box;box-sizing:content-box;-webkit-appearance:textfield;}\n\
		input[type=\"search\"]::-webkit-search-decoration,input[type=\"search\"]::-webkit-search-cancel-button{-webkit-appearance:none;}\n\
		textarea{overflow:auto;vertical-align:top;}\n\
		@media print{*{text-shadow:none !important;color:#000 !important;background:transparent !important;box-shadow:none !important;} a,a:visited{text-decoration:underline;} a[href]:after{content:\" (\" attr(href) \")\";} abbr[title]:after{content:\" (\" attr(title) \")\";} .ir a:after,a[href^=\"javascript:\"]:after,a[href^=\"#\"]:after{content:\"\";} pre,blockquote{border:1px solid #999;page-break-inside:avoid;} thead{display:table-header-group;} tr,img{page-break-inside:avoid;} img{max-width:100% !important;} @page {margin:0.5cm;}p,h2,h3{orphans:3;widows:3;} h2,h3{page-break-after:avoid;}}body{margin:0;font-family:\"Helvetica Neue\",Helvetica,Arial,sans-serif;font-size:14px;line-height:20px;color:#333333;background-color:#ffffff;}\n\
		a{color:#0088cc;text-decoration:none;}\n\
		a:hover,a:focus{color:#005580;text-decoration:underline;}\n\
		.img-rounded{-webkit-border-radius:6px;-moz-border-radius:6px;border-radius:6px;}\n\
		.img-polaroid{padding:4px;background-color:#fff;border:1px solid #ccc;border:1px solid rgba(0, 0, 0, 0.2);-webkit-box-shadow:0 1px 3px rgba(0, 0, 0, 0.1);-moz-box-shadow:0 1px 3px rgba(0, 0, 0, 0.1);box-shadow:0 1px 3px rgba(0, 0, 0, 0.1);}\n\
		.img-circle{-webkit-border-radius:500px;-moz-border-radius:500px;border-radius:500px;}\n\
		.row{margin-left:-20px;*zoom:1;}.row:before,.row:after{display:table;content:\"\";line-height:0;}\n\
		.row:after{clear:both;}\n\
		[class*=\"span\"]{float:left;min-height:1px;margin-left:20px;}\n\
		.container,.navbar-static-top .container,.navbar-fixed-top .container,.navbar-fixed-bottom .container{width:940px;}\n\
		[class*=\"span\"].hide,.row-fluid [class*=\"span\"].hide{display:none;}\n\
		[class*=\"span\"].pull-right,.row-fluid [class*=\"span\"].pull-right{float:right;}\n\
		.container{margin-right:auto;margin-left:auto;*zoom:1;}.container:before,.container:after{display:table;content:\"\";line-height:0;}\n\
		.container:after{clear:both;}\n\
		.container-fluid{padding-right:20px;padding-left:20px;*zoom:1;}.container-fluid:before,.container-fluid:after{display:table;content:\"\";line-height:0;}\n\
		.container-fluid:after{clear:both;}\n\
		p{margin:0 0 10px;}\n\
		.lead{margin-bottom:20px;font-size:21px;font-weight:200;line-height:30px;}\n\
		small{font-size:85%;}\n\
		strong{font-weight:bold;}\n\
		em{font-style:italic;}\n\
		cite{font-style:normal;}\n\
		.muted{color:#999999;}\n\
		a.muted:hover,a.muted:focus{color:#808080;}\n\
		.text-warning{color:#c09853;}\n\
		a.text-warning:hover,a.text-warning:focus{color:#a47e3c;}\n\
		.text-error{color:#b94a48;}\n\
		a.text-error:hover,a.text-error:focus{color:#953b39;}\n\
		.text-info{color:#3a87ad;}\n\
		a.text-info:hover,a.text-info:focus{color:#2d6987;}\n\
		.text-success{color:#468847;}\n\
		a.text-success:hover,a.text-success:focus{color:#356635;}\n\
		.text-left{text-align:left;}\n\
		.text-right{text-align:right;}\n\
		.text-center{text-align:center;}\n\
		h1,h2,h3,h4,h5,h6{margin:10px 0;font-family:inherit;font-weight:bold;line-height:20px;color:inherit;text-rendering:optimizelegibility;}h1 small,h2 small,h3 small,h4 small,h5 small,h6 small{font-weight:normal;line-height:1;color:#999999;}\n\
		h1,h2,h3{line-height:40px;}\n\
		h1{font-size:38.5px;}\n\
		h2{font-size:31.5px;}\n\
		h3{font-size:24.5px;}\n\
		h4{font-size:17.5px;}\n\
		h5{font-size:14px;}\n\
		h6{font-size:11.9px;}\n\
		h1 small{font-size:24.5px;}\n\
		h2 small{font-size:17.5px;}\n\
		h3 small{font-size:14px;}\n\
		h4 small{font-size:14px;}\n\
		.page-header{padding-bottom:9px;margin:20px 0 30px;border-bottom:1px solid #eeeeee;}\n\
		ul,ol{padding:0;margin:0 0 10px 25px;}\n\
		ul ul,ul ol,ol ol,ol ul{margin-bottom:0;}\n\
		li{line-height:20px;}\n\
		ul.unstyled,ol.unstyled{margin-left:0;list-style:none;}\n\
		ul.inline,ol.inline{margin-left:0;list-style:none;}ul.inline>li,ol.inline>li{display:inline-block;*display:inline;*zoom:1;padding-left:5px;padding-right:5px;}\n\
		dl{margin-bottom:20px;}\n\
		dt,dd{line-height:20px;}\n\
		dt{font-weight:bold;}\n\
		dd{margin-left:10px;}\n\
		.dl-horizontal{*zoom:1;}.dl-horizontal:before,.dl-horizontal:after{display:table;content:\"\";line-height:0;}\n\
		.dl-horizontal:after{clear:both;}\n\
		.dl-horizontal dt{float:left;width:160px;clear:left;text-align:right;overflow:hidden;text-overflow:ellipsis;white-space:nowrap;}\n\
		.dl-horizontal dd{margin-left:180px;}\n\
		hr{margin:20px 0;border:0;border-top:1px solid #eeeeee;border-bottom:1px solid #ffffff;}\n\
		abbr[title],abbr[data-original-title]{cursor:help;border-bottom:1px dotted #999999;}\n\
		abbr.initialism{font-size:90%;text-transform:uppercase;}\n\
"

#define TEMPLATE_BOOTSTRAP2 "\
		blockquote{padding:0 0 0 15px;margin:0 0 20px;border-left:5px solid #eeeeee;}blockquote p{margin-bottom:0;font-size:17.5px;font-weight:300;line-height:1.25;}\n\
		blockquote small{display:block;line-height:20px;color:#999999;}blockquote small:before{content:'\\2014 \\00A0';}\n\
		blockquote.pull-right{float:right;padding-right:15px;padding-left:0;border-right:5px solid #eeeeee;border-left:0;}blockquote.pull-right p,blockquote.pull-right small{text-align:right;}\n\
		blockquote.pull-right small:before{content:'';}\n\
		blockquote.pull-right small:after{content:'\\00A0 \\2014';}\n\
		q:before,q:after,blockquote:before,blockquote:after{content:\"\";}\n\
		address{display:block;margin-bottom:20px;font-style:normal;line-height:20px;}\n\
		code,pre{padding:0 3px 2px;font-family:Monaco,Menlo,Consolas,\"Courier New\",monospace;font-size:12px;color:#333333;-webkit-border-radius:3px;-moz-border-radius:3px;border-radius:3px;}\n\
		code{padding:2px 4px;color:#d14;background-color:#f7f7f9;border:1px solid #e1e1e8;white-space:nowrap;}\n\
		pre{display:block;padding:9.5px;margin:0 0 10px;font-size:13px;line-height:20px;word-break:break-all;word-wrap:break-word;white-space:pre;white-space:pre-wrap;background-color:#f5f5f5;border:1px solid #ccc;border:1px solid rgba(0, 0, 0, 0.15);-webkit-border-radius:4px;-moz-border-radius:4px;border-radius:4px;}pre.prettyprint{margin-bottom:20px;}\n\
		pre code{padding:0;color:inherit;white-space:pre;white-space:pre-wrap;background-color:transparent;border:0;}\n\
		.pre-scrollable{max-height:340px;overflow-y:scroll;}\n\
		.label,.badge{display:inline-block;padding:2px 4px;font-size:11.844px;font-weight:bold;line-height:14px;color:#ffffff;vertical-align:baseline;white-space:nowrap;text-shadow:0 -1px 0 rgba(0, 0, 0, 0.25);background-color:#999999;}\n\
		.label{-webkit-border-radius:3px;-moz-border-radius:3px;border-radius:3px;}\n\
		.badge{padding-left:9px;padding-right:9px;-webkit-border-radius:9px;-moz-border-radius:9px;border-radius:9px;}\n\
		.label:empty,.badge:empty{display:none;}\n\
		a.label:hover,a.label:focus,a.badge:hover,a.badge:focus{color:#ffffff;text-decoration:none;cursor:pointer;}\n\
		.label-important,.badge-important{background-color:#b94a48;}\n\
		.label-important[href],.badge-important[href]{background-color:#953b39;}\n\
		.label-warning,.badge-warning{background-color:#f89406;}\n\
		.label-warning[href],.badge-warning[href]{background-color:#c67605;}\n\
		.label-success,.badge-success{background-color:#468847;}\n\
		.label-success[href],.badge-success[href]{background-color:#356635;}\n\
		.label-info,.badge-info{background-color:#3a87ad;}\n\
		.label-info[href],.badge-info[href]{background-color:#2d6987;}\n\
		.label-inverse,.badge-inverse{background-color:#333333;}\n\
		.label-inverse[href],.badge-inverse[href]{background-color:#1a1a1a;}\n\
		.btn .label,.btn .badge{position:relative;top:-1px;}\n\
		.btn-mini .label,.btn-mini .badge{top:0;}\n\
		.btn{display:inline-block;*display:inline;*zoom:1;padding:4px 12px;margin-bottom:0;font-size:14px;line-height:20px;text-align:center;vertical-align:middle;cursor:pointer;color:#333333;text-shadow:0 1px 1px rgba(255, 255, 255, 0.75);background-color:#f5f5f5;background-image:-moz-linear-gradient(top, #ffffff, #e6e6e6);background-image:-webkit-gradient(linear, 0 0, 0 100%, from(#ffffff), to(#e6e6e6));background-image:-webkit-linear-gradient(top, #ffffff, #e6e6e6);background-image:-o-linear-gradient(top, #ffffff, #e6e6e6);background-image:linear-gradient(to bottom, #ffffff, #e6e6e6);background-repeat:repeat-x;filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#ffffffff', endColorstr='#ffe6e6e6', GradientType=0);border-color:#e6e6e6 #e6e6e6 #bfbfbf;border-color:rgba(0, 0, 0, 0.1) rgba(0, 0, 0, 0.1) rgba(0, 0, 0, 0.25);*background-color:#e6e6e6;filter:progid:DXImageTransform.Microsoft.gradient(enabled = false);border:1px solid #cccccc;*border:0;border-bottom-color:#b3b3b3;-webkit-border-radius:4px;-moz-border-radius:4px;border-radius:4px;*margin-left:.3em;-webkit-box-shadow:inset 0 1px 0 rgba(255,255,255,.2), 0 1px 2px rgba(0,0,0,.05);-moz-box-shadow:inset 0 1px 0 rgba(255,255,255,.2), 0 1px 2px rgba(0,0,0,.05);box-shadow:inset 0 1px 0 rgba(255,255,255,.2), 0 1px 2px rgba(0,0,0,.05);}.btn:hover,.btn:focus,.btn:active,.btn.active,.btn.disabled,.btn[disabled]{color:#333333;background-color:#e6e6e6;*background-color:#d9d9d9;}\n\
		.btn:active,.btn.active{background-color:#cccccc \\9;}\n\
		.btn:first-child{*margin-left:0;}\n\
		.btn:hover,.btn:focus{color:#333333;text-decoration:none;background-position:0 -15px;-webkit-transition:background-position 0.1s linear;-moz-transition:background-position 0.1s linear;-o-transition:background-position 0.1s linear;transition:background-position 0.1s linear;}\n\
		.btn:focus{outline:thin dotted #333;outline:5px auto -webkit-focus-ring-color;outline-offset:-2px;}\n\
		.btn.active,.btn:active{background-image:none;outline:0;-webkit-box-shadow:inset 0 2px 4px rgba(0,0,0,.15), 0 1px 2px rgba(0,0,0,.05);-moz-box-shadow:inset 0 2px 4px rgba(0,0,0,.15), 0 1px 2px rgba(0,0,0,.05);box-shadow:inset 0 2px 4px rgba(0,0,0,.15), 0 1px 2px rgba(0,0,0,.05);}\n\
		.btn.disabled,.btn[disabled]{cursor:default;background-image:none;opacity:0.65;filter:alpha(opacity=65);-webkit-box-shadow:none;-moz-box-shadow:none;box-shadow:none;}\n\
		.btn-large{padding:11px 19px;font-size:17.5px;-webkit-border-radius:6px;-moz-border-radius:6px;border-radius:6px;}\n\
		.btn-large [class^=\"icon-\"],.btn-large [class*=\" icon-\"]{margin-top:4px;}\n\
		.btn-small{padding:2px 10px;font-size:11.9px;-webkit-border-radius:3px;-moz-border-radius:3px;border-radius:3px;}\n\
		.btn-small [class^=\"icon-\"],.btn-small [class*=\" icon-\"]{margin-top:0;}\n\
		.btn-mini [class^=\"icon-\"],.btn-mini [class*=\" icon-\"]{margin-top:-1px;}\n\
		.btn-mini{padding:0 6px;font-size:10.5px;-webkit-border-radius:3px;-moz-border-radius:3px;border-radius:3px;}\n\
		.btn-block{display:block;width:100%;padding-left:0;padding-right:0;-webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box;}\n\
		.btn-block+.btn-block{margin-top:5px;}\n\
		input[type=\"submit\"].btn-block,input[type=\"reset\"].btn-block,input[type=\"button\"].btn-block{width:100%;}\n\
		.btn-primary.active,.btn-warning.active,.btn-danger.active,.btn-success.active,.btn-info.active,.btn-inverse.active{color:rgba(255, 255, 255, 0.75);}\n\
		.btn-primary{color:#ffffff;text-shadow:0 -1px 0 rgba(0, 0, 0, 0.25);background-color:#006dcc;background-image:-moz-linear-gradient(top, #0088cc, #0044cc);background-image:-webkit-gradient(linear, 0 0, 0 100%, from(#0088cc), to(#0044cc));background-image:-webkit-linear-gradient(top, #0088cc, #0044cc);background-image:-o-linear-gradient(top, #0088cc, #0044cc);background-image:linear-gradient(to bottom, #0088cc, #0044cc);background-repeat:repeat-x;filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#ff0088cc', endColorstr='#ff0044cc', GradientType=0);border-color:#0044cc #0044cc #002a80;border-color:rgba(0, 0, 0, 0.1) rgba(0, 0, 0, 0.1) rgba(0, 0, 0, 0.25);*background-color:#0044cc;filter:progid:DXImageTransform.Microsoft.gradient(enabled = false);}.btn-primary:hover,.btn-primary:focus,.btn-primary:active,.btn-primary.active,.btn-primary.disabled,.btn-primary[disabled]{color:#ffffff;background-color:#0044cc;*background-color:#003bb3;}\n\
		.btn-primary:active,.btn-primary.active{background-color:#003399 \\9;}\n\
		.btn-warning{color:#ffffff;text-shadow:0 -1px 0 rgba(0, 0, 0, 0.25);background-color:#faa732;background-image:-moz-linear-gradient(top, #fbb450, #f89406);background-image:-webkit-gradient(linear, 0 0, 0 100%, from(#fbb450), to(#f89406));background-image:-webkit-linear-gradient(top, #fbb450, #f89406);background-image:-o-linear-gradient(top, #fbb450, #f89406);background-image:linear-gradient(to bottom, #fbb450, #f89406);background-repeat:repeat-x;filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#fffbb450', endColorstr='#fff89406', GradientType=0);border-color:#f89406 #f89406 #ad6704;border-color:rgba(0, 0, 0, 0.1) rgba(0, 0, 0, 0.1) rgba(0, 0, 0, 0.25);*background-color:#f89406;filter:progid:DXImageTransform.Microsoft.gradient(enabled = false);}.btn-warning:hover,.btn-warning:focus,.btn-warning:active,.btn-warning.active,.btn-warning.disabled,.btn-warning[disabled]{color:#ffffff;background-color:#f89406;*background-color:#df8505;}\n\
		.btn-warning:active,.btn-warning.active{background-color:#c67605 \\9;}\n\
		.btn-danger{color:#ffffff;text-shadow:0 -1px 0 rgba(0, 0, 0, 0.25);background-color:#da4f49;background-image:-moz-linear-gradient(top, #ee5f5b, #bd362f);background-image:-webkit-gradient(linear, 0 0, 0 100%, from(#ee5f5b), to(#bd362f));background-image:-webkit-linear-gradient(top, #ee5f5b, #bd362f);background-image:-o-linear-gradient(top, #ee5f5b, #bd362f);background-image:linear-gradient(to bottom, #ee5f5b, #bd362f);background-repeat:repeat-x;filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#ffee5f5b', endColorstr='#ffbd362f', GradientType=0);border-color:#bd362f #bd362f #802420;border-color:rgba(0, 0, 0, 0.1) rgba(0, 0, 0, 0.1) rgba(0, 0, 0, 0.25);*background-color:#bd362f;filter:progid:DXImageTransform.Microsoft.gradient(enabled = false);}.btn-danger:hover,.btn-danger:focus,.btn-danger:active,.btn-danger.active,.btn-danger.disabled,.btn-danger[disabled]{color:#ffffff;background-color:#bd362f;*background-color:#a9302a;}\n\
		.btn-danger:active,.btn-danger.active{background-color:#942a25 \\9;}\n\
		.btn-success{color:#ffffff;text-shadow:0 -1px 0 rgba(0, 0, 0, 0.25);background-color:#5bb75b;background-image:-moz-linear-gradient(top, #62c462, #51a351);background-image:-webkit-gradient(linear, 0 0, 0 100%, from(#62c462), to(#51a351));background-image:-webkit-linear-gradient(top, #62c462, #51a351);background-image:-o-linear-gradient(top, #62c462, #51a351);background-image:linear-gradient(to bottom, #62c462, #51a351);background-repeat:repeat-x;filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#ff62c462', endColorstr='#ff51a351', GradientType=0);border-color:#51a351 #51a351 #387038;border-color:rgba(0, 0, 0, 0.1) rgba(0, 0, 0, 0.1) rgba(0, 0, 0, 0.25);*background-color:#51a351;filter:progid:DXImageTransform.Microsoft.gradient(enabled = false);}.btn-success:hover,.btn-success:focus,.btn-success:active,.btn-success.active,.btn-success.disabled,.btn-success[disabled]{color:#ffffff;background-color:#51a351;*background-color:#499249;}\n\
		.btn-success:active,.btn-success.active{background-color:#408140 \\9;}\n\
		.btn-info{color:#ffffff;text-shadow:0 -1px 0 rgba(0, 0, 0, 0.25);background-color:#49afcd;background-image:-moz-linear-gradient(top, #5bc0de, #2f96b4);background-image:-webkit-gradient(linear, 0 0, 0 100%, from(#5bc0de), to(#2f96b4));background-image:-webkit-linear-gradient(top, #5bc0de, #2f96b4);background-image:-o-linear-gradient(top, #5bc0de, #2f96b4);background-image:linear-gradient(to bottom, #5bc0de, #2f96b4);background-repeat:repeat-x;filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#ff5bc0de', endColorstr='#ff2f96b4', GradientType=0);border-color:#2f96b4 #2f96b4 #1f6377;border-color:rgba(0, 0, 0, 0.1) rgba(0, 0, 0, 0.1) rgba(0, 0, 0, 0.25);*background-color:#2f96b4;filter:progid:DXImageTransform.Microsoft.gradient(enabled = false);}.btn-info:hover,.btn-info:focus,.btn-info:active,.btn-info.active,.btn-info.disabled,.btn-info[disabled]{color:#ffffff;background-color:#2f96b4;*background-color:#2a85a0;}\n\
		.btn-info:active,.btn-info.active{background-color:#24748c \\9;}\n\
		.btn-inverse{color:#ffffff;text-shadow:0 -1px 0 rgba(0, 0, 0, 0.25);background-color:#363636;background-image:-moz-linear-gradient(top, #444444, #222222);background-image:-webkit-gradient(linear, 0 0, 0 100%, from(#444444), to(#222222));background-image:-webkit-linear-gradient(top, #444444, #222222);background-image:-o-linear-gradient(top, #444444, #222222);background-image:linear-gradient(to bottom, #444444, #222222);background-repeat:repeat-x;filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#ff444444', endColorstr='#ff222222', GradientType=0);border-color:#222222 #222222 #000000;border-color:rgba(0, 0, 0, 0.1) rgba(0, 0, 0, 0.1) rgba(0, 0, 0, 0.25);*background-color:#222222;filter:progid:DXImageTransform.Microsoft.gradient(enabled = false);}.btn-inverse:hover,.btn-inverse:focus,.btn-inverse:active,.btn-inverse.active,.btn-inverse.disabled,.btn-inverse[disabled]{color:#ffffff;background-color:#222222;*background-color:#151515;}\n\
		.btn-inverse:active,.btn-inverse.active{background-color:#080808 \\9;}\n\
		button.btn,input[type=\"submit\"].btn{*padding-top:3px;*padding-bottom:3px;}button.btn::-moz-focus-inner,input[type=\"submit\"].btn::-moz-focus-inner{padding:0;border:0;}\n\
		button.btn.btn-large,input[type=\"submit\"].btn.btn-large{*padding-top:7px;*padding-bottom:7px;}\n\
		button.btn.btn-small,input[type=\"submit\"].btn.btn-small{*padding-top:3px;*padding-bottom:3px;}\n\
		button.btn.btn-mini,input[type=\"submit\"].btn.btn-mini{*padding-top:1px;*padding-bottom:1px;}\n\
		.btn-link,.btn-link:active,.btn-link[disabled]{background-color:transparent;background-image:none;-webkit-box-shadow:none;-moz-box-shadow:none;box-shadow:none;}\n\
		.btn-link{border-color:transparent;cursor:pointer;color:#0088cc;-webkit-border-radius:0;-moz-border-radius:0;border-radius:0;}\n\
		.btn-link:hover,.btn-link:focus{color:#005580;text-decoration:underline;background-color:transparent;}\n\
		.btn-link[disabled]:hover,.btn-link[disabled]:focus{color:#333333;text-decoration:none;}\n\
		.btn-group{position:relative;display:inline-block;*display:inline;*zoom:1;font-size:0;vertical-align:middle;white-space:nowrap;*margin-left:.3em;}.btn-group:first-child{*margin-left:0;}\n\
		.btn-group+.btn-group{margin-left:5px;}\n\
		.btn-toolbar{font-size:0;margin-top:10px;margin-bottom:10px;}.btn-toolbar>.btn+.btn,.btn-toolbar>.btn-group+.btn,.btn-toolbar>.btn+.btn-group{margin-left:5px;}\n\
		.btn-group>.btn{position:relative;-webkit-border-radius:0;-moz-border-radius:0;border-radius:0;}\n\
		.btn-group>.btn+.btn{margin-left:-1px;}\n\
		.btn-group>.btn,.btn-group>.dropdown-menu,.btn-group>.popover{font-size:14px;}\n\
		.btn-group>.btn-mini{font-size:10.5px;}\n\
		.btn-group>.btn-small{font-size:11.9px;}\n\
		.btn-group>.btn-large{font-size:17.5px;}\n\
		.btn-group>.btn:first-child{margin-left:0;-webkit-border-top-left-radius:4px;-moz-border-radius-topleft:4px;border-top-left-radius:4px;-webkit-border-bottom-left-radius:4px;-moz-border-radius-bottomleft:4px;border-bottom-left-radius:4px;}\n\
		.btn-group>.btn:last-child,.btn-group>.dropdown-toggle{-webkit-border-top-right-radius:4px;-moz-border-radius-topright:4px;border-top-right-radius:4px;-webkit-border-bottom-right-radius:4px;-moz-border-radius-bottomright:4px;border-bottom-right-radius:4px;}\n\
		.btn-group>.btn.large:first-child{margin-left:0;-webkit-border-top-left-radius:6px;-moz-border-radius-topleft:6px;border-top-left-radius:6px;-webkit-border-bottom-left-radius:6px;-moz-border-radius-bottomleft:6px;border-bottom-left-radius:6px;}\n\
		.btn-group>.btn.large:last-child,.btn-group>.large.dropdown-toggle{-webkit-border-top-right-radius:6px;-moz-border-radius-topright:6px;border-top-right-radius:6px;-webkit-border-bottom-right-radius:6px;-moz-border-radius-bottomright:6px;border-bottom-right-radius:6px;}\n\
		.btn-group>.btn:hover,.btn-group>.btn:focus,.btn-group>.btn:active,.btn-group>.btn.active{z-index:2;}\n\
		.btn-group .dropdown-toggle:active,.btn-group.open .dropdown-toggle{outline:0;}\n\
		.btn-group>.btn+.dropdown-toggle{padding-left:8px;padding-right:8px;-webkit-box-shadow:inset 1px 0 0 rgba(255,255,255,.125), inset 0 1px 0 rgba(255,255,255,.2), 0 1px 2px rgba(0,0,0,.05);-moz-box-shadow:inset 1px 0 0 rgba(255,255,255,.125), inset 0 1px 0 rgba(255,255,255,.2), 0 1px 2px rgba(0,0,0,.05);box-shadow:inset 1px 0 0 rgba(255,255,255,.125), inset 0 1px 0 rgba(255,255,255,.2), 0 1px 2px rgba(0,0,0,.05);*padding-top:5px;*padding-bottom:5px;}\n\
		.btn-group>.btn-mini+.dropdown-toggle{padding-left:5px;padding-right:5px;*padding-top:2px;*padding-bottom:2px;}\n\
		.btn-group>.btn-small+.dropdown-toggle{*padding-top:5px;*padding-bottom:4px;}\n\
		.btn-group>.btn-large+.dropdown-toggle{padding-left:12px;padding-right:12px;*padding-top:7px;*padding-bottom:7px;}\n\
		.btn-group.open .dropdown-toggle{background-image:none;-webkit-box-shadow:inset 0 2px 4px rgba(0,0,0,.15), 0 1px 2px rgba(0,0,0,.05);-moz-box-shadow:inset 0 2px 4px rgba(0,0,0,.15), 0 1px 2px rgba(0,0,0,.05);box-shadow:inset 0 2px 4px rgba(0,0,0,.15), 0 1px 2px rgba(0,0,0,.05);}\n\
		.btn-group.open .btn.dropdown-toggle{background-color:#e6e6e6;}\n\
		.btn-group.open .btn-primary.dropdown-toggle{background-color:#0044cc;}\n\
		.btn-group.open .btn-warning.dropdown-toggle{background-color:#f89406;}\n\
		.btn-group.open .btn-danger.dropdown-toggle{background-color:#bd362f;}\n\
		.btn-group.open .btn-success.dropdown-toggle{background-color:#51a351;}\n\
		.btn-group.open .btn-info.dropdown-toggle{background-color:#2f96b4;}\n\
		.btn-group.open .btn-inverse.dropdown-toggle{background-color:#222222;}\n\
		.btn .caret{margin-top:8px;margin-left:0;}\n\
		.btn-large .caret{margin-top:6px;}\n\
		.btn-large .caret{border-left-width:5px;border-right-width:5px;border-top-width:5px;}\n\
		.btn-mini .caret,.btn-small .caret{margin-top:8px;}\n\
		.dropup .btn-large .caret{border-bottom-width:5px;}\n\
		.btn-primary .caret,.btn-warning .caret,.btn-danger .caret,.btn-info .caret,.btn-success .caret,.btn-inverse .caret{border-top-color:#ffffff;border-bottom-color:#ffffff;}\n\
		.btn-group-vertical{display:inline-block;*display:inline;*zoom:1;}\n\
		.btn-group-vertical>.btn{display:block;float:none;max-width:100%;-webkit-border-radius:0;-moz-border-radius:0;border-radius:0;}\n\
		.btn-group-vertical>.btn+.btn{margin-left:0;margin-top:-1px;}\n\
		.btn-group-vertical>.btn:first-child{-webkit-border-radius:4px 4px 0 0;-moz-border-radius:4px 4px 0 0;border-radius:4px 4px 0 0;}\n\
		.btn-group-vertical>.btn:last-child{-webkit-border-radius:0 0 4px 4px;-moz-border-radius:0 0 4px 4px;border-radius:0 0 4px 4px;}\n\
		.btn-group-vertical>.btn-large:first-child{-webkit-border-radius:6px 6px 0 0;-moz-border-radius:6px 6px 0 0;border-radius:6px 6px 0 0;}\n\
		.btn-group-vertical>.btn-large:last-child{-webkit-border-radius:0 0 6px 6px;-moz-border-radius:0 0 6px 6px;border-radius:0 0 6px 6px;}\n\
		.nav{margin-left:0;margin-bottom:20px;list-style:none;}\n\
		.nav>li>a{display:block;}\n\
		.nav>li>a:hover,.nav>li>a:focus{text-decoration:none;background-color:#eeeeee;}\n\
		.nav>li>a>img{max-width:none;}\n\
		.nav>.pull-right{float:right;}\n\
		.nav-header{display:block;padding:3px 15px;font-size:11px;font-weight:bold;line-height:20px;color:#999999;text-shadow:0 1px 0 rgba(255, 255, 255, 0.5);text-transform:uppercase;}\n\
		.nav li+.nav-header{margin-top:9px;}\n\
		.nav-list{padding-left:15px;padding-right:15px;margin-bottom:0;}\n\
		.nav-list>li>a,.nav-list .nav-header{margin-left:-15px;margin-right:-15px;text-shadow:0 1px 0 rgba(255, 255, 255, 0.5);}\n\
		.nav-list>li>a{padding:3px 15px;}\n\
		.nav-list>.active>a,.nav-list>.active>a:hover,.nav-list>.active>a:focus{color:#ffffff;text-shadow:0 -1px 0 rgba(0, 0, 0, 0.2);background-color:#0088cc;}\n\
		.nav-list [class^=\"icon-\"],.nav-list [class*=\" icon-\"]{margin-right:2px;}\n\
		.nav-list .divider{*width:100%;height:1px;margin:9px 1px;*margin:-5px 0 5px;overflow:hidden;background-color:#e5e5e5;border-bottom:1px solid #ffffff;}\n\
		.nav-tabs,.nav-pills{*zoom:1;}.nav-tabs:before,.nav-pills:before,.nav-tabs:after,.nav-pills:after{display:table;content:\"\";line-height:0;}\n\
		.nav-tabs:after,.nav-pills:after{clear:both;}\n\
		.nav-tabs>li,.nav-pills>li{float:left;}\n\
		.nav-tabs>li>a,.nav-pills>li>a{padding-right:12px;padding-left:12px;margin-right:2px;line-height:14px;}\n\
		.nav-tabs{border-bottom:1px solid #ddd;}\n\
		.nav-tabs>li{margin-bottom:-1px;}\n\
		.nav-tabs>li>a{padding-top:8px;padding-bottom:8px;line-height:20px;border:1px solid transparent;-webkit-border-radius:4px 4px 0 0;-moz-border-radius:4px 4px 0 0;border-radius:4px 4px 0 0;}.nav-tabs>li>a:hover,.nav-tabs>li>a:focus{border-color:#eeeeee #eeeeee #dddddd;}\n\
		.nav-tabs>.active>a,.nav-tabs>.active>a:hover,.nav-tabs>.active>a:focus{color:#555555;background-color:#ffffff;border:1px solid #ddd;border-bottom-color:transparent;cursor:default;}\n\
		.nav-pills>li>a{padding-top:8px;padding-bottom:8px;margin-top:2px;margin-bottom:2px;-webkit-border-radius:5px;-moz-border-radius:5px;border-radius:5px;}\n\
		"
#define TEMPLATE_BOOTSTRAP3 "\
		.nav-pills>.active>a,.nav-pills>.active>a:hover,.nav-pills>.active>a:focus{color:#ffffff;background-color:#0088cc;}\n\
		.nav-stacked>li{float:none;}\n\
		.nav-stacked>li>a{margin-right:0;}\n\
		.nav-tabs.nav-stacked{border-bottom:0;}\n\
		.nav-tabs.nav-stacked>li>a{border:1px solid #ddd;-webkit-border-radius:0;-moz-border-radius:0;border-radius:0;}\n\
		.nav-tabs.nav-stacked>li:first-child>a{-webkit-border-top-right-radius:4px;-moz-border-radius-topright:4px;border-top-right-radius:4px;-webkit-border-top-left-radius:4px;-moz-border-radius-topleft:4px;border-top-left-radius:4px;}\n\
		.nav-tabs.nav-stacked>li:last-child>a{-webkit-border-bottom-right-radius:4px;-moz-border-radius-bottomright:4px;border-bottom-right-radius:4px;-webkit-border-bottom-left-radius:4px;-moz-border-radius-bottomleft:4px;border-bottom-left-radius:4px;}\n\
		.nav-tabs.nav-stacked>li>a:hover,.nav-tabs.nav-stacked>li>a:focus{border-color:#ddd;z-index:2;}\n\
		.nav-pills.nav-stacked>li>a{margin-bottom:3px;}\n\
		.nav-pills.nav-stacked>li:last-child>a{margin-bottom:1px;}\n\
		.nav-tabs .dropdown-menu{-webkit-border-radius:0 0 6px 6px;-moz-border-radius:0 0 6px 6px;border-radius:0 0 6px 6px;}\n\
		.nav-pills .dropdown-menu{-webkit-border-radius:6px;-moz-border-radius:6px;border-radius:6px;}\n\
		.nav .dropdown-toggle .caret{border-top-color:#0088cc;border-bottom-color:#0088cc;margin-top:6px;}\n\
		.nav .dropdown-toggle:hover .caret,.nav .dropdown-toggle:focus .caret{border-top-color:#005580;border-bottom-color:#005580;}\n\
		.nav-tabs .dropdown-toggle .caret{margin-top:8px;}\n\
		.nav .active .dropdown-toggle .caret{border-top-color:#fff;border-bottom-color:#fff;}\n\
		.nav-tabs .active .dropdown-toggle .caret{border-top-color:#555555;border-bottom-color:#555555;}\n\
		.nav>.dropdown.active>a:hover,.nav>.dropdown.active>a:focus{cursor:pointer;}\n\
		.nav-tabs .open .dropdown-toggle,.nav-pills .open .dropdown-toggle,.nav>li.dropdown.open.active>a:hover,.nav>li.dropdown.open.active>a:focus{color:#ffffff;background-color:#999999;border-color:#999999;}\n\
		.nav li.dropdown.open .caret,.nav li.dropdown.open.active .caret,.nav li.dropdown.open a:hover .caret,.nav li.dropdown.open a:focus .caret{border-top-color:#ffffff;border-bottom-color:#ffffff;opacity:1;filter:alpha(opacity=100);}\n\
		.tabs-stacked .open>a:hover,.tabs-stacked .open>a:focus{border-color:#999999;}\n\
		.tabbable{*zoom:1;}.tabbable:before,.tabbable:after{display:table;content:\"\";line-height:0;}\n\
		.tabbable:after{clear:both;}\n\
		.tab-content{overflow:auto;}\n\
		.tabs-below>.nav-tabs,.tabs-right>.nav-tabs,.tabs-left>.nav-tabs{border-bottom:0;}\n\
		.tab-content>.tab-pane,.pill-content>.pill-pane{display:none;}\n\
		.tab-content>.active,.pill-content>.active{display:block;}\n\
		.tabs-below>.nav-tabs{border-top:1px solid #ddd;}\n\
		.tabs-below>.nav-tabs>li{margin-top:-1px;margin-bottom:0;}\n\
		.tabs-below>.nav-tabs>li>a{-webkit-border-radius:0 0 4px 4px;-moz-border-radius:0 0 4px 4px;border-radius:0 0 4px 4px;}.tabs-below>.nav-tabs>li>a:hover,.tabs-below>.nav-tabs>li>a:focus{border-bottom-color:transparent;border-top-color:#ddd;}\n\
		.tabs-below>.nav-tabs>.active>a,.tabs-below>.nav-tabs>.active>a:hover,.tabs-below>.nav-tabs>.active>a:focus{border-color:transparent #ddd #ddd #ddd;}\n\
		.tabs-left>.nav-tabs>li,.tabs-right>.nav-tabs>li{float:none;}\n\
		.tabs-left>.nav-tabs>li>a,.tabs-right>.nav-tabs>li>a{min-width:74px;margin-right:0;margin-bottom:3px;}\n\
		.tabs-left>.nav-tabs{float:left;margin-right:19px;border-right:1px solid #ddd;}\n\
		.tabs-left>.nav-tabs>li>a{margin-right:-1px;-webkit-border-radius:4px 0 0 4px;-moz-border-radius:4px 0 0 4px;border-radius:4px 0 0 4px;}\n\
		.tabs-left>.nav-tabs>li>a:hover,.tabs-left>.nav-tabs>li>a:focus{border-color:#eeeeee #dddddd #eeeeee #eeeeee;}\n\
		.tabs-left>.nav-tabs .active>a,.tabs-left>.nav-tabs .active>a:hover,.tabs-left>.nav-tabs .active>a:focus{border-color:#ddd transparent #ddd #ddd;*border-right-color:#ffffff;}\n\
		.tabs-right>.nav-tabs{float:right;margin-left:19px;border-left:1px solid #ddd;}\n\
		.tabs-right>.nav-tabs>li>a{margin-left:-1px;-webkit-border-radius:0 4px 4px 0;-moz-border-radius:0 4px 4px 0;border-radius:0 4px 4px 0;}\n\
		.tabs-right>.nav-tabs>li>a:hover,.tabs-right>.nav-tabs>li>a:focus{border-color:#eeeeee #eeeeee #eeeeee #dddddd;}\n\
		.tabs-right>.nav-tabs .active>a,.tabs-right>.nav-tabs .active>a:hover,.tabs-right>.nav-tabs .active>a:focus{border-color:#ddd #ddd #ddd transparent;*border-left-color:#ffffff;}\n\
		.nav>.disabled>a{color:#999999;}\n\
		.nav>.disabled>a:hover,.nav>.disabled>a:focus{text-decoration:none;background-color:transparent;cursor:default;}\n\
		.navbar{overflow:visible;margin-bottom:20px;*position:relative;*z-index:2;}\n\
		.navbar-inner{min-height:40px;padding-left:20px;padding-right:20px;background-color:#fafafa;background-image:-moz-linear-gradient(top, #ffffff, #f2f2f2);background-image:-webkit-gradient(linear, 0 0, 0 100%, from(#ffffff), to(#f2f2f2));background-image:-webkit-linear-gradient(top, #ffffff, #f2f2f2);background-image:-o-linear-gradient(top, #ffffff, #f2f2f2);background-image:linear-gradient(to bottom, #ffffff, #f2f2f2);background-repeat:repeat-x;filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#ffffffff', endColorstr='#fff2f2f2', GradientType=0);border:1px solid #d4d4d4;-webkit-border-radius:4px;-moz-border-radius:4px;border-radius:4px;-webkit-box-shadow:0 1px 4px rgba(0, 0, 0, 0.065);-moz-box-shadow:0 1px 4px rgba(0, 0, 0, 0.065);box-shadow:0 1px 4px rgba(0, 0, 0, 0.065);*zoom:1;}.navbar-inner:before,.navbar-inner:after{display:table;content:\"\";line-height:0;}\n\
		.navbar-inner:after{clear:both;}\n\
		.navbar .container{width:auto;}\n\
		.nav-collapse.collapse{height:auto;overflow:visible;}\n\
		.navbar .brand{float:left;display:block;padding:10px 20px 10px;margin-left:-20px;font-size:20px;font-weight:200;color:#777777;text-shadow:0 1px 0 #ffffff;}.navbar .brand:hover,.navbar .brand:focus{text-decoration:none;}\n\
		.navbar-text{margin-bottom:0;line-height:40px;color:#777777;}\n\
		.navbar-link{color:#777777;}.navbar-link:hover,.navbar-link:focus{color:#333333;}\n\
		.navbar .divider-vertical{height:40px;margin:0 9px;border-left:1px solid #f2f2f2;border-right:1px solid #ffffff;}\n\
		.navbar .btn,.navbar .btn-group{margin-top:5px;}\n\
		.navbar .btn-group .btn,.navbar .input-prepend .btn,.navbar .input-append .btn,.navbar .input-prepend .btn-group,.navbar .input-append .btn-group{margin-top:0;}\n\
		.navbar-form{margin-bottom:0;*zoom:1;}.navbar-form:before,.navbar-form:after{display:table;content:\"\";line-height:0;}\n\
		.navbar-form:after{clear:both;}\n\
		.navbar-form input,.navbar-form select,.navbar-form .radio,.navbar-form .checkbox{margin-top:5px;}\n\
		.navbar-form input,.navbar-form select,.navbar-form .btn{display:inline-block;margin-bottom:0;}\n\
		.navbar-form input[type=\"image\"],.navbar-form input[type=\"checkbox\"],.navbar-form input[type=\"radio\"]{margin-top:3px;}\n\
		.navbar-form .input-append,.navbar-form .input-prepend{margin-top:5px;white-space:nowrap;}.navbar-form .input-append input,.navbar-form .input-prepend input{margin-top:0;}\n\
		.navbar-search{position:relative;float:left;margin-top:5px;margin-bottom:0;}.navbar-search .search-query{margin-bottom:0;padding:4px 14px;font-family:\"Helvetica Neue\",Helvetica,Arial,sans-serif;font-size:13px;font-weight:normal;line-height:1;-webkit-border-radius:15px;-moz-border-radius:15px;border-radius:15px;}\n\
		.navbar-static-top{position:static;margin-bottom:0;}.navbar-static-top .navbar-inner{-webkit-border-radius:0;-moz-border-radius:0;border-radius:0;}\n\
		.navbar-fixed-top,.navbar-fixed-bottom{position:fixed;right:0;left:0;z-index:1030;margin-bottom:0;}\n\
		.navbar-fixed-top .navbar-inner,.navbar-static-top .navbar-inner{border-width:0 0 1px;}\n\
		.navbar-fixed-bottom .navbar-inner{border-width:1px 0 0;}\n\
		.navbar-fixed-top .navbar-inner,.navbar-fixed-bottom .navbar-inner{padding-left:0;padding-right:0;-webkit-border-radius:0;-moz-border-radius:0;border-radius:0;}\n\
		.navbar-static-top .container,.navbar-fixed-top .container,.navbar-fixed-bottom .container{width:940px;}\n\
		.navbar-fixed-top{top:0;}\n\
		.navbar-fixed-top .navbar-inner,.navbar-static-top .navbar-inner{-webkit-box-shadow:0 1px 10px rgba(0,0,0,.1);-moz-box-shadow:0 1px 10px rgba(0,0,0,.1);box-shadow:0 1px 10px rgba(0,0,0,.1);}\n\
		.navbar-fixed-bottom{bottom:0;}.navbar-fixed-bottom .navbar-inner{-webkit-box-shadow:0 -1px 10px rgba(0,0,0,.1);-moz-box-shadow:0 -1px 10px rgba(0,0,0,.1);box-shadow:0 -1px 10px rgba(0,0,0,.1);}\n\
		.navbar .nav{position:relative;left:0;display:block;float:left;margin:0 10px 0 0;}\n\
		.navbar .nav.pull-right{float:right;margin-right:0;}\n\
		.navbar .nav>li{float:left;}\n\
		.navbar .nav>li>a{float:none;padding:10px 15px 10px;color:#777777;text-decoration:none;text-shadow:0 1px 0 #ffffff;}\n\
		.navbar .nav .dropdown-toggle .caret{margin-top:8px;}\n\
		.navbar .nav>li>a:focus,.navbar .nav>li>a:hover{background-color:transparent;color:#333333;text-decoration:none;}\n\
		.navbar .nav>.active>a,.navbar .nav>.active>a:hover,.navbar .nav>.active>a:focus{color:#555555;text-decoration:none;background-color:#e5e5e5;-webkit-box-shadow:inset 0 3px 8px rgba(0, 0, 0, 0.125);-moz-box-shadow:inset 0 3px 8px rgba(0, 0, 0, 0.125);box-shadow:inset 0 3px 8px rgba(0, 0, 0, 0.125);}\n\
		.navbar .btn-navbar{display:none;float:right;padding:7px 10px;margin-left:5px;margin-right:5px;color:#ffffff;text-shadow:0 -1px 0 rgba(0, 0, 0, 0.25);background-color:#ededed;background-image:-moz-linear-gradient(top, #f2f2f2, #e5e5e5);background-image:-webkit-gradient(linear, 0 0, 0 100%, from(#f2f2f2), to(#e5e5e5));background-image:-webkit-linear-gradient(top, #f2f2f2, #e5e5e5);background-image:-o-linear-gradient(top, #f2f2f2, #e5e5e5);background-image:linear-gradient(to bottom, #f2f2f2, #e5e5e5);background-repeat:repeat-x;filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#fff2f2f2', endColorstr='#ffe5e5e5', GradientType=0);border-color:#e5e5e5 #e5e5e5 #bfbfbf;border-color:rgba(0, 0, 0, 0.1) rgba(0, 0, 0, 0.1) rgba(0, 0, 0, 0.25);*background-color:#e5e5e5;filter:progid:DXImageTransform.Microsoft.gradient(enabled = false);-webkit-box-shadow:inset 0 1px 0 rgba(255,255,255,.1), 0 1px 0 rgba(255,255,255,.075);-moz-box-shadow:inset 0 1px 0 rgba(255,255,255,.1), 0 1px 0 rgba(255,255,255,.075);box-shadow:inset 0 1px 0 rgba(255,255,255,.1), 0 1px 0 rgba(255,255,255,.075);}.navbar .btn-navbar:hover,.navbar .btn-navbar:focus,.navbar .btn-navbar:active,.navbar .btn-navbar.active,.navbar .btn-navbar.disabled,.navbar .btn-navbar[disabled]{color:#ffffff;background-color:#e5e5e5;*background-color:#d9d9d9;}\n\
		.navbar .btn-navbar:active,.navbar .btn-navbar.active{background-color:#cccccc \\9;}\n\
		.navbar .btn-navbar .icon-bar{display:block;width:18px;height:2px;background-color:#f5f5f5;-webkit-border-radius:1px;-moz-border-radius:1px;border-radius:1px;-webkit-box-shadow:0 1px 0 rgba(0, 0, 0, 0.25);-moz-box-shadow:0 1px 0 rgba(0, 0, 0, 0.25);box-shadow:0 1px 0 rgba(0, 0, 0, 0.25);}\n\
		.btn-navbar .icon-bar+.icon-bar{margin-top:3px;}\n\
		.navbar .nav>li>.dropdown-menu:before{content:'';display:inline-block;border-left:7px solid transparent;border-right:7px solid transparent;border-bottom:7px solid #ccc;border-bottom-color:rgba(0, 0, 0, 0.2);position:absolute;top:-7px;left:9px;}\n\
		.navbar .nav>li>.dropdown-menu:after{content:'';display:inline-block;border-left:6px solid transparent;border-right:6px solid transparent;border-bottom:6px solid #ffffff;position:absolute;top:-6px;left:10px;}\n\
		.navbar-fixed-bottom .nav>li>.dropdown-menu:before{border-top:7px solid #ccc;border-top-color:rgba(0, 0, 0, 0.2);border-bottom:0;bottom:-7px;top:auto;}\n\
		.navbar-fixed-bottom .nav>li>.dropdown-menu:after{border-top:6px solid #ffffff;border-bottom:0;bottom:-6px;top:auto;}\n\
		.navbar .nav li.dropdown>a:hover .caret,.navbar .nav li.dropdown>a:focus .caret{border-top-color:#333333;border-bottom-color:#333333;}\n\
		.navbar .nav li.dropdown.open>.dropdown-toggle,.navbar .nav li.dropdown.active>.dropdown-toggle,.navbar .nav li.dropdown.open.active>.dropdown-toggle{background-color:#e5e5e5;color:#555555;}\n\
		.navbar .nav li.dropdown>.dropdown-toggle .caret{border-top-color:#777777;border-bottom-color:#777777;}\n\
		.navbar .nav li.dropdown.open>.dropdown-toggle .caret,.navbar .nav li.dropdown.active>.dropdown-toggle .caret,.navbar .nav li.dropdown.open.active>.dropdown-toggle .caret{border-top-color:#555555;border-bottom-color:#555555;}\n\
		.navbar .pull-right>li>.dropdown-menu,.navbar .nav>li>.dropdown-menu.pull-right{left:auto;right:0;}.navbar .pull-right>li>.dropdown-menu:before,.navbar .nav>li>.dropdown-menu.pull-right:before{left:auto;right:12px;}\n\
		.navbar .pull-right>li>.dropdown-menu:after,.navbar .nav>li>.dropdown-menu.pull-right:after{left:auto;right:13px;}\n\
		.navbar .pull-right>li>.dropdown-menu .dropdown-menu,.navbar .nav>li>.dropdown-menu.pull-right .dropdown-menu{left:auto;right:100%;margin-left:0;margin-right:-1px;-webkit-border-radius:6px 0 6px 6px;-moz-border-radius:6px 0 6px 6px;border-radius:6px 0 6px 6px;}\n\
		.navbar-inverse .navbar-inner{background-color:#1b1b1b;background-image:-moz-linear-gradient(top, #222222, #111111);background-image:-webkit-gradient(linear, 0 0, 0 100%, from(#222222), to(#111111));background-image:-webkit-linear-gradient(top, #222222, #111111);background-image:-o-linear-gradient(top, #222222, #111111);background-image:linear-gradient(to bottom, #222222, #111111);background-repeat:repeat-x;filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#ff222222', endColorstr='#ff111111', GradientType=0);border-color:#252525;}\n\
		.navbar-inverse .brand,.navbar-inverse .nav>li>a{color:#999999;text-shadow:0 -1px 0 rgba(0, 0, 0, 0.25);}.navbar-inverse .brand:hover,.navbar-inverse .nav>li>a:hover,.navbar-inverse .brand:focus,.navbar-inverse .nav>li>a:focus{color:#ffffff;}\n\
		.navbar-inverse .brand{color:#999999;}\n\
		.navbar-inverse .navbar-text{color:#999999;}\n\
		.navbar-inverse .nav>li>a:focus,.navbar-inverse .nav>li>a:hover{background-color:transparent;color:#ffffff;}\n\
		.navbar-inverse .nav .active>a,.navbar-inverse .nav .active>a:hover,.navbar-inverse .nav .active>a:focus{color:#ffffff;background-color:#111111;}\n\
		.navbar-inverse .navbar-link{color:#999999;}.navbar-inverse .navbar-link:hover,.navbar-inverse .navbar-link:focus{color:#ffffff;}\n\
		.navbar-inverse .divider-vertical{border-left-color:#111111;border-right-color:#222222;}\n\
		.navbar-inverse .nav li.dropdown.open>.dropdown-toggle,.navbar-inverse .nav li.dropdown.active>.dropdown-toggle,.navbar-inverse .nav li.dropdown.open.active>.dropdown-toggle{background-color:#111111;color:#ffffff;}\n\
		.navbar-inverse .nav li.dropdown>a:hover .caret,.navbar-inverse .nav li.dropdown>a:focus .caret{border-top-color:#ffffff;border-bottom-color:#ffffff;}\n\
		.navbar-inverse .nav li.dropdown>.dropdown-toggle .caret{border-top-color:#999999;border-bottom-color:#999999;}\n\
		.navbar-inverse .nav li.dropdown.open>.dropdown-toggle .caret,.navbar-inverse .nav li.dropdown.active>.dropdown-toggle .caret,.navbar-inverse .nav li.dropdown.open.active>.dropdown-toggle .caret{border-top-color:#ffffff;border-bottom-color:#ffffff;}\n\
		.navbar-inverse .navbar-search .search-query{color:#ffffff;background-color:#515151;border-color:#111111;-webkit-box-shadow:inset 0 1px 2px rgba(0,0,0,.1), 0 1px 0 rgba(255,255,255,.15);-moz-box-shadow:inset 0 1px 2px rgba(0,0,0,.1), 0 1px 0 rgba(255,255,255,.15);box-shadow:inset 0 1px 2px rgba(0,0,0,.1), 0 1px 0 rgba(255,255,255,.15);-webkit-transition:none;-moz-transition:none;-o-transition:none;transition:none;}.navbar-inverse .navbar-search .search-query:-moz-placeholder{color:#cccccc;}\n\
		.navbar-inverse .navbar-search .search-query:-ms-input-placeholder{color:#cccccc;}\n\
		.navbar-inverse .navbar-search .search-query::-webkit-input-placeholder{color:#cccccc;}\n\
		.navbar-inverse .navbar-search .search-query:focus,.navbar-inverse .navbar-search .search-query.focused{padding:5px 15px;color:#333333;text-shadow:0 1px 0 #ffffff;background-color:#ffffff;border:0;-webkit-box-shadow:0 0 3px rgba(0, 0, 0, 0.15);-moz-box-shadow:0 0 3px rgba(0, 0, 0, 0.15);box-shadow:0 0 3px rgba(0, 0, 0, 0.15);outline:0;}\n\
		.navbar-inverse .btn-navbar{color:#ffffff;text-shadow:0 -1px 0 rgba(0, 0, 0, 0.25);background-color:#0e0e0e;background-image:-moz-linear-gradient(top, #151515, #040404);background-image:-webkit-gradient(linear, 0 0, 0 100%, from(#151515), to(#040404));background-image:-webkit-linear-gradient(top, #151515, #040404);background-image:-o-linear-gradient(top, #151515, #040404);background-image:linear-gradient(to bottom, #151515, #040404);background-repeat:repeat-x;filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#ff151515', endColorstr='#ff040404', GradientType=0);border-color:#040404 #040404 #000000;border-color:rgba(0, 0, 0, 0.1) rgba(0, 0, 0, 0.1) rgba(0, 0, 0, 0.25);*background-color:#040404;filter:progid:DXImageTransform.Microsoft.gradient(enabled = false);}.navbar-inverse .btn-navbar:hover,.navbar-inverse .btn-navbar:focus,.navbar-inverse .btn-navbar:active,.navbar-inverse .btn-navbar.active,.navbar-inverse .btn-navbar.disabled,.navbar-inverse .btn-navbar[disabled]{color:#ffffff;background-color:#040404;*background-color:#000000;}\n\
		.navbar-inverse .btn-navbar:active,.navbar-inverse .btn-navbar.active{background-color:#000000 \\9;}\n\
		.alert{padding:8px 35px 8px 14px;margin-bottom:20px;text-shadow:0 1px 0 rgba(255, 255, 255, 0.5);background-color:#fcf8e3;border:1px solid #fbeed5;-webkit-border-radius:4px;-moz-border-radius:4px;border-radius:4px;}\n\
		.alert,.alert h4{color:#c09853;}\n\
		.alert h4{margin:0;}\n\
		.alert .close{position:relative;top:-2px;right:-21px;line-height:20px;}\n\
		.alert-success{background-color:#dff0d8;border-color:#d6e9c6;color:#468847;}\n\
		.alert-success h4{color:#468847;}\n\
		.alert-danger,.alert-error{background-color:#f2dede;border-color:#eed3d7;color:#b94a48;}\n\
		.alert-danger h4,.alert-error h4{color:#b94a48;}\n\
		.alert-info{background-color:#d9edf7;border-color:#bce8f1;color:#3a87ad;}\n\
		.alert-info h4{color:#3a87ad;}\n\
		.alert-block{padding-top:14px;padding-bottom:14px;}\n\
		.alert-block>p,.alert-block>ul{margin-bottom:0;}\n\
		.alert-block p+p{margin-top:5px;}\n\
		.hero-unit{padding:60px;margin-bottom:30px;font-size:18px;font-weight:200;line-height:30px;color:inherit;background-color:#eeeeee;-webkit-border-radius:6px;-moz-border-radius:6px;border-radius:6px;}.hero-unit h1{margin-bottom:0;font-size:60px;line-height:1;color:inherit;letter-spacing:-1px;}\n\
		.hero-unit li{line-height:30px;}\n\
		.well{min-height:20px;padding:19px;margin-bottom:20px;background-color:#f5f5f5;border:1px solid #e3e3e3;-webkit-border-radius:4px;-moz-border-radius:4px;border-radius:4px;-webkit-box-shadow:inset 0 1px 1px rgba(0, 0, 0, 0.05);-moz-box-shadow:inset 0 1px 1px rgba(0, 0, 0, 0.05);box-shadow:inset 0 1px 1px rgba(0, 0, 0, 0.05);}.well blockquote{border-color:#ddd;border-color:rgba(0, 0, 0, 0.15);}\n\
		.well-large{padding:24px;-webkit-border-radius:6px;-moz-border-radius:6px;border-radius:6px;}\n\
		.well-small{padding:9px;-webkit-border-radius:3px;-moz-border-radius:3px;border-radius:3px;}\n\
"

#endif
