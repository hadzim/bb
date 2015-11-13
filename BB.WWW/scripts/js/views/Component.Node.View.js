// Create a View to be used with the Layout below.
BB.ComponentNodeView = Backbone.Layout.extend({
  template: "#component-node-template2",
  className: "col-xs-12 col-md-6 col-lg-4",
  
  initialize: function(){
	  
	  this.model.on('change', this.render, this);
      this.model.on('destroy', this.remove, this);
      
      this.model.settings.on('add', this.render, this);
      this.model.settings.on('remove', this.render, this);
      
      this.model.data.on('add', this.render, this);
      this.model.data.on('remove', this.render, this);
  },
  
  beforeRender: function() {
	  
	    this.model.data.each(function(d) {
	    	this.insertView(".data", new BB.ComponentDataSummaryView2({model: d}));
	    }, this);
	    
	    this.model.settings.each(function(s) {
	    	if (s.id != "name" && s.id != "aaa"){
	    		this.insertView(".settings", new BB.ComponentSettingSummaryView({model: s}));
	    	}
	    	
	    }, this);
  },
  

  events: {
    "click button.edit": "edit",
	"click .edit": "edit"
  },
  
  edit: function(ev) {
	  /*
	  modalView = new BaseModalView({model: this.model});
	  modalView.show();
	  */
	  
	  modalView = new BB.ComponentNodeEditView({model: this.model});
	  modalView.show();
	  
    }
    
});
