BB.ComponentRuleConditionView = Backbone.Layout.extend({
  template: "#component-rule-condition-template",
  className: "condition",
  
  initialize: function(){
	  
	  this.model.on('change', this.render, this);
      this.model.on('destroy', this.remove, this);
      
      //this.model.settings.on('add', this.render, this);
      //this.model.settings.on('remove', this.render, this);
      
      //this.model.data.on('add', this.render, this);
      //this.model.data.on('remove', this.render, this);
  },
  
  beforeRender: function() {
	  if (this.model){
		  for (var i = 0, l = this.model.nested.length; i < l; i++){
			  var v = new BB.ComponentRuleConditionView({model: this.model.nested[i]});
			  this.insertView(".nested", v);
	  	  } 
	  }
	  /*
	    this.model.data.each(function(d) {
	    	this.insertView(".data", new BB.ComponentDataSummaryView2({model: d}));
	    }, this);
	    
	    this.model.settings.each(function(s) {
	    	if (s.id != "name" && s.id != "aaa"){
	    		this.insertView(".settings", new BB.ComponentSettingSummaryView({model: s}));
	    	}
	    	
	    }, this);*/
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
