// Create a View to be used with the Layout below.
BB.ComponentDateView = Backbone.Layout.extend({

  template: "#component-date-template",
  tagName: "span",
  
  initialize: function(){
	  this.model = clockModel;
	  this.model.on('change', this.render, this);
  },
  
  getTime: function() {
	  
	    var names = Array(
	    		 "Sunday",
	    		 "Monday",
	    		 "Tuesday",
	    		 "Wednesday",
	    		 "Thursday",
	    		 "Friday",
	    		 "Saturday"
	    );
	  
	    var today = this.model.get('time');
	    var day = today.getDate();
	    var month = today.getMonth() + 1;
	    var name = names[today.getDay()];
	    return name + " " + day + "." + month + ".";
    },
	
    checkTime: function(i) {
	    if (i < 10) {i = "0" + i};  // add zero in front of numbers < 10
	    return i;
	},
  
  serialize: function() {
	  var data = []
	  data["date"] = this.getTime();
	  return data;
  },

});
