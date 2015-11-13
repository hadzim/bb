// Create a View to be used with the Layout below.
BB.ComponentSettingEditView = Backbone.Layout.extend({
	
  template: "#component-setting-edit-template",
  
  tagName: "tr",
  className: "",
  somethingChanged: false,
  
  events: {
      //"click input.active[type=checkbox]" : "inputValueChanged",
      //"change input[type=range]"   : "inputValueChanged",
      //"change input[type=text]"    : "inputValueChanged",
      //"click .bootstrap-switch"    : "inputValueChanged",
    },
  
  initialize: function() {
	  console.log(this.model);
	
	  this.model.on('change:type', this.modelTypeChanged, this);  
      this.model.on('change:value', this.modelValueChanged, this);  
	  this.specialize();
      
  },
  
  specialize: function() {
  	
	    this.dynamicInputValue = function(){
	    	return this.model.get("value");
	    }
	    //this.dynamicInputValueConfirm = this.methodNotImplemented;
	    
	    if (this.model.get("type") == "switch") {
		  this.dynamicInputValue = this.switchInputValue;
	    } else if (this.model.get("type") == "range") {
	      this.dynamicInputValue = this.rangeInputValue;
	    } else if (this.model.get("type") == "text") {
	      this.dynamicInputValue = this.textInputValue;
	    }    
  },

  //Wrapper methods
  inputValueChanged: function(e) {this.dynamicInputValueChanged(e);},
  modelValueChanged: function(m) {this.render();},
  modelTypeChanged: function() {this.specialize(); this.render();},
  
  publish: function(){
	  var newvalue = this.dynamicInputValue();
	  console.log(this.model.get("id"), " newvalue:" , newvalue, " old value: ", this.model.get("value"));
	  if (newvalue != this.model.get("value")){
		  BB.mqtt.publish(this.model.get("topic")+"/new", newvalue);
	  } 
  },
  
  rangeInputValue: function() {
	  return $("input", this.$el).val();
  },

  switchInputValue: function(e) {
	  return $("input", this.$el).bootstrapSwitch("state") ? "1" : "0";
	  //console.log("switch changed: ", e); 
	  //BB.mqtt.publish(this.model.get("topic")+"/new", e.target.checked == 0 ? "0" : "1");
  },
  
  textInputValue: function(e) {
	  return $("input", this.$el).val();
	  //console.log("text changed: ", e);
	  //BB.mqtt.publish(this.model.get("topic")+"/new", e.target.value);
  },  
  
  afterRender: function(){
	  if (this.model.get("readonly") == "readonly") {
		  $("input", this.$el).attr('disabled', true) 
	  }
	  $("input[type='checkbox'], input[type='radio']", this.$el).not("[data-switch-no-init]").bootstrapSwitch(); 
	  if (this.model.get("type") == "range") {
		  $("#slider-" + this.model.get("id"), this.$el).slider(
				  {tooltip: 'always'}
		  );
	  }
	  
  },
  
});
