// Create a View to be used with the Layout below.
BB.ComponentDataBlockView = Backbone.Layout.extend({
  template: "#component-data-block-template",
  className: "col-xs-6 col-sm-4 col-md-3 col-lg-3  col-xl-2",
  
  initialize: function(){
	  
	  this.node.on('change', this.render, this);
	  this.model.on('change', this.render, this);
      this.model.on('destroy', this.remove, this);
  },
  
  beforeRender: function(){
	  this.color = "fg-" + this.model.fgColor();
  },
  
  serialize: function() {
	  var data = this.model ? _.clone(this.model.attributes) : {};
	  data["fgcolor"] = this.color;
	  data["node"] = _.clone(this.node.attributes);
	  
	  data["extra"] = "";
	  
	  //temperature
	  var hour = 12;
	  var date = this.model.get("value").date;
	  if (date){
		  hour = new Date(this.model.get("value").date).getHours();
		  console.log("date", this.model.get("value").date);
	  }
	  
	  if (this.model.get("value").image){
		  var isNight = hour > 20 || hour < 5;
		  data["extra"] += " <img title='" + hour + ":00' src='images/weather/" + (isNight ? "night" : "day") + "/" + this.model.get("value").image + ".png' style='position: absolute; right: 60px;'>"  
	  }
	  if (this.model.get("value").wind){
		  data["extra"] += "<span>" + this.model.get("value").wind + "&nbsp;m/s</span><br />"  
	  } 
	  if (this.model.get("value").precipitation){
		  data["extra"] += "<span>" + this.model.get("value").precipitation + "&nbsp;mm</span>"  
	  } 
	  
	  
	  return data;
  },
  
  events: {
	    "click .showgraph": "showgraph"
	  },
	  
  showgraph: function(ev) {
	  var modalView = new BB.ComponentDataGraphModalView({node: this.node, data: this.model});
	  modalView.show();
  }
  
});


