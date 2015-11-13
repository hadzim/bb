// Create a View to be used with the Layout below.
BB.LayoutHeadlineView = Backbone.Layout.extend({
  template: "#layout-headline-template",
  
  model: {
	  attributes: {
		  title: "Unknown",
		  subtitle: "Unknown"
	  }  
  },
 
  headline: function(text, subtext){
	  this.model.attributes.title = text;
	  this.model.attributes.subtitle = subtext;
	  this.render();
  },
/*
  // When you click the View contents, it will wrap them in a bold tag.
  events: {
    "click": "wrapElement"
  },

  wrapElement: function() {
    this.$el.wrap("<b>");
  }
  */
});
