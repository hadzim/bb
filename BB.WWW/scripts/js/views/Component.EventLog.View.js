// Create a View to be used with the Layout below.
BB.ComponentEventLogView = Backbone.Layout.extend({
	
  template: "#component-eventlog-template",
  
  tagName: "tr",
  className: "",
  
  initialize: function(){
	  var classNames = "";
	  //alert(this.model.get("level"));
	if (this.model.get("level") > 5){
		classNames = "text-muted";
	} 
	if (this.model.get("level") == 4){
		classNames = "text-yellow";
	}
	if (this.model.get("level") <= 3){
		classNames = "text-red";
	} 
	this.model.set("classNames", classNames);
  },
  
  events: {
      //"click input.active[type=checkbox]" : "inputValueChanged",
      //"change input[type=range]"   : "inputValueChanged",
      //"change input[type=text]"    : "inputValueChanged",
      //"click .bootstrap-switch"    : "inputValueChanged",
   },
  
  
});
