// Create a View to be used with the Layout below.
BB.ComponentSettingSummaryView = Backbone.Layout.extend({
	
  template: "#component-setting-summary-template",
  
  className: "setting-box pull-left",
  
  initialize: function(){
	  this.model.on('change', this.render, this);
      this.model.on('destroy', this.remove, this);
  },
  
	afterRender: function(){
		/*
		if (this.model.get("value")){
			$(".setting-box", this.$el).addClass("bg-aqua");
		} else {
			$(".setting-box", this.$el).addClass("bg-gray");
		}
		*/
		if (this.model.get("value")){
			$(this.$el).addClass("bg-aqua");
		} else {
			$(this.$el).addClass("bg-gray");
		}
	},

});
