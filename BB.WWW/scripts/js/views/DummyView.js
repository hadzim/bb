// Create a View to be used with the Layout below.
BB.DummySubview = Backbone.Layout.extend({
  template: "#dummysub",

  // When you click the View contents, it will wrap them in a bold tag.
  events: {
    "click": "wrapElement"
  },

  wrapElement: function() {
    this.$el.wrap("<b>");
  }
});

// Create a new Layout.
BB.DummyView = Backbone.Layout.extend({
  // Attach the Layout to the main container.
  el: "#main",

  // Use the previous defined template.
  template: "#dummy",

  // Declaratively bind a nested View to the Layout.
  views: {
    "p": new BB.DummySubview()
  }
});