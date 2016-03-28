// Create a View to be used with the Layout below.


BB.PageEventLogView = Backbone.Layout.extend({
  template: "#page-eventlog-template",
  
  title: "Event Log",
  className: "row",
  
  initialize: function(){
	  this.collection = BB.eventlog;
	  
	  this.collection.on('add', this.render, this);
      this.collection.on('remove', this.render, this);
  },
  
  close: function(){
	  
	  this.collection.off('add', this.render, this);
      this.collection.off('remove', this.render, this);
  },
  
  beforeRender: function() {
	  
	    this.collection.each(function(eve) {
	    	this.insertView(".logs", new BB.ComponentEventLogView({model: eve}));
	    }, this);
  }

});
