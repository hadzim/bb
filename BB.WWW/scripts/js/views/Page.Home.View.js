// Create a View to be used with the Layout below.


BB.PageHomeView = Backbone.Layout.extend({
  template: "#page-home-template",
  
  title: "Home",
  className: "row",
  
  initialize: function(){
	  this.datacollection = BB.data;
	  this.collection = BB.nodes;
	  
	  this.filter = BB.filter;
	  this.filter.on('change', this.render, this);
      
	  this.datacollection.on('add', this.render, this);
      this.datacollection.on('remove', this.render, this);
      this.datacollection.on('sort', this.render, this);
      
      
  },
  
  close: function(){
	  this.filter.off('change', this.render, this);
      
	  this.datacollection.off('add', this.render, this);
      this.datacollection.off('remove', this.render, this);
      this.datacollection.off('sort', this.render, this);
  },
  
  beforeRender: function() {
	  
	    this.insertView(".places", new BB.ComponentFilterPlacesView());
	    this.insertView(".types", new BB.ComponentFilterDataTypesView());
	  
	    this.collection.each(function(n) {
	    	n.data.each(function(d) {
	    		
	    		if (this.filter.dataPass(n, d)){
	    		
	    			this.insertView(new BB.ComponentDataBlockView({model: d, node: n}));
	    		
	    		}
		    	
		    }, this);
	    	
	    }, this);
  }

});
