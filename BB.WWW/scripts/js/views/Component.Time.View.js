// Create a View to be used with the Layout below.
BB.ComponentTimeView = Backbone.Layout.extend({

  template: "#component-clock-template",
  tagName: "span",
  
  initialize: function(){
	  this.model = clockModel;
	  this.model.on('change', this.render, this);
  },
  
  getTime: function() {
	    var today = this.model.get('time');
	    var h = today.getHours();
	    var m = today.getMinutes();
	    var s = today.getSeconds();
	    m = this.checkTime(m);
	    return "" + h + ":" + m;
    },
    
    checkTime: function(i) {
	    if (i < 10) {i = "0" + i};  // add zero in front of numbers < 10
	    return i;
	},
  
  serialize: function() {
	  var data = []
	  data["clock"] = this.getTime();
	  return data;
  },

});
