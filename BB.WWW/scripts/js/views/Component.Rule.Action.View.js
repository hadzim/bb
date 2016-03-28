BB.ComponentRuleActionView = Backbone.Layout.extend({
  template: "#component-rule-action-template",
  className: "action",
  
  initialize: function(){
	  
	  //this.model.on('change', this.render, this);
      //this.model.on('destroy', this.remove, this);
      
      //this.model.settings.on('add', this.render, this);
      //this.model.settings.on('remove', this.render, this);
      
      //this.model.data.on('add', this.render, this);
      //this.model.data.on('remove', this.render, this);
  },
  
  beforeRender: function() {
	  console.log("render action", this.model);
	  if (this.model && this.model.nested){
		  for (var i = 0, l = this.model.nested.length; i < l; i++){
			  var v = new BB.ComponentRuleActionView({model: this.model.nested[i]});
			  this.insertView(".nested", v);
	  	  } 
	  }
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
	  
	  //modalView = new BB.ComponentNodeEditView({model: this.model});
	  //modalView.show();
	  
    }
    
});
