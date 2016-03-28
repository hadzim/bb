// Create a View to be used with the Layout below.


BB.PageRulesView = Backbone.Layout.extend({
  template: "#page-rules-template",
  
  title: "Rules",
  
  className: "row",
  
  initialize: function(){
	  this.collection = BB.rules;
	  
	  this.collection.on('add', this.render, this);
      this.collection.on('remove', this.render, this);
      this.collection.on('sort', this.render, this);
      
  },
  
  close: function(){
	  this.collection.off('add', this.render, this);
      this.collection.off('remove', this.render, this);
      this.collection.off('sort', this.render, this);
  },
  
  beforeRender: function() {
	    this.collection.each(function(r) {
	    	this.insertView(new BB.ComponentRuleView({model: r}));
	    }, this);
  },
  
});
