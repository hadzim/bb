// Create a View to be used with the Layout below.
BB.ComponentDataView = Backbone.Layout.extend({
  template: "#component-data-template",
  className: "col-xs-12 col-md-6 col-lg-4",
  
  beforeRender: function() {
	  this.insertView(new BB.ComponentDataSummaryView({model: this.model}));
  },
  
});

