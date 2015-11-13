// Create a View to be used with the Layout below.
BB.LayoutTopMenuView = Backbone.Layout.extend({

  template: "#layout-topmenu-template",
  
  initialize: function(){
	  this.model = BB.connection;
	  this.model.on('change', this.render, this);
      
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
