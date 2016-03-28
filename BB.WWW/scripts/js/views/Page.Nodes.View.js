// Create a View to be used with the Layout below.


BB.PageNodesView = Backbone.Layout.extend({
  template: "#page-nodes-template",
  
  title: "Nodes",
  
  className: "row",
  
  initialize: function(){
	  this.collection = BB.nodes;
	  
	  this.collection.on('add', this.render, this);
      this.collection.on('remove', this.render, this);
      this.collection.on('sort', this.render, this);
      
      this.filter = BB.filter;
	  this.filter.on('change', this.render, this);
      
  },
  
  close: function(){
	  this.collection.off('add', this.render, this);
      this.collection.off('remove', this.render, this);
      this.collection.off('sort', this.render, this);
      this.filter.off('change', this.render, this);
  },
  
  beforeRender: function() {
	  
	  
	    this.insertView(".places", new BB.ComponentFilterPlacesView());
	  
	    var sum = 0;
	    this.collection.each(function(node) {
	    	if (this.filter.nodePass(node)){
		    	this.insertView(new BB.ComponentNodeView({model: node}));
		    	/*
		    	sum++;
		    	
		    	if (sum && !(sum % 3)){
		    		this.insertView(new Backbone.Layout({className: "clearfix visible-lg-block"}))
		    	}
		    	if (sum && !(sum % 2)){
		    		this.insertView(new Backbone.Layout({className: "clearfix visible-md-block"}))
		    	}*/
	    	}
	    }, this);
  },
  /*
  addNode: function(node) {
	  
	  this.insertView(new BB.ComponentNodeView({model: node}));
  	//console.log("adding node to device");
    //var nodeView = new BB.NodeView({model: node});
    //this.$(".subviews").append(nodeView.render().el);
    //console.log("adding node to device done");
  },*/
});
