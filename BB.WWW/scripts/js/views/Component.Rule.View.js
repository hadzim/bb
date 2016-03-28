BB.ComponentRuleView = Backbone.Layout.extend({
  template: "#component-rule-template",
  className: "col-xs-12 col-md-12 col-lg-12",
  
  initialize: function(){
	  
	  this.model.on('change', this.render, this);
      this.model.on('destroy', this.remove, this);
      
      //this.model.settings.on('add', this.render, this);
      //this.model.settings.on('remove', this.render, this);
      
      //this.model.data.on('add', this.render, this);
      //this.model.data.on('remove', this.render, this);
  },
  beforeRender: function() {
	  console.log("action", this.model.action);
	  console.log("naction", this.model.negativeAction);
	  if (!this.model.action.empty){
		  this.insertView(".rule-action", new BB.ComponentRuleActionView({model: this.model.action}));
	  }
	  if (!this.model.negativeAction.empty){
		  this.insertView(".rule-action-negative", new BB.ComponentRuleActionView({model: this.model.negativeAction}));
	  }
	  this.insertView(".rule-condition", new BB.ComponentRuleConditionView({model: this.model.condition}));
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
  
  afterRender: function(){
	$(".box", this.$el).activateBox();
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
