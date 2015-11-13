  
  BB.Logger = Backbone.Model.extend({
    initialize: function(){
      this.set("logger", console.log); 
      //Settings.get("logging")==='1' ? this.enable() : this.disable();
	this.enable()
    },
    enable: function(){window['console']['log'] = this.get("logger");},
    disable: function(){  
      console.log("Console.log disabled. Set local storage logging=1 to enable");
      console.log=function(){};}
  });


  
  