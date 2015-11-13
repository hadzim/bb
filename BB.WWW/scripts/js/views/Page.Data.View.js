// Create a View to be used with the Layout below.


BB.PageDataView = Backbone.Layout.extend({
  template: "#page-data-template",
  
  title: "Data",
  
  className: "row",
  
  initialize: function(){
	  this.collection = BB.data;
	  
	  this.collection.on('add', this.render, this);
      this.collection.on('remove', this.render, this);
      this.collection.on('sort', this.render, this);
      
  },
  
  beforeRender: function() {
	    var sum = 0;
	    this.collection.each(function(d) {
	    	this.insertView(new BB.ComponentDataView({model: d}));
	    	
	    	sum++;
	    	
	    	if (sum && !(sum % 3)){
	    		this.insertView(new Backbone.Layout({className: "clearfix visible-lg-block"}))
	    	}
	    	if (sum && !(sum % 2)){
	    		this.insertView(new Backbone.Layout({className: "clearfix visible-md-block"}))
	    	}
	    	
	    }, this);
  },

});
