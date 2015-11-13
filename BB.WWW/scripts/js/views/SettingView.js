BB.SettingView = Backbone.View.extend({
    className: "subview node-view",
    events: {
      "click input[type=checkbox]" : "inputValueChanged",
      "change input[type=range]" : "inputValueChanged",
      "mousedown input[type=range]" : "inhibitInputUpdates",
      "mouseup input[type=range]" : "allowInputUpdates"
    },
    initialize: function() {
      this.model.on('change:type', this.modelTypeChanged, this);  
      this.model.on('change:value', this.modelValueChanged, this);  

      try {
      
      this.specialize();
      this.model.view = this;
      this.allowInputUpdates();
      
      } catch (err){
  		console.log("catch in inicialize");
  	}
    },
    specialize: function() {
    	console.log("specialize");
    	try {
      this.dynamicInputValueChanged = this.dynamicRender = this.dynamicInhibitInputUpdates = this.dynamicAllowInputUpdates = this.dynamicModelValueChanged = this.methodNotImplemented;

      if (this.model.get("type") == "switch") {
        this.dynamicRender = this.switchRender;
        this.dynamicInputValueChanged = this.switchInputValueChanged;
        this.dynamicModelValueChanged = this.switchModelValueChanged;
      } else if (this.model.get("type") == "range") {
        this.dynamicRender = this.rangeRender;
        this.dynamicInputValueChanged = this.rangeInputValueChanged;
        this.dynamicModelValueChanged = this.rangeModelValueChanged;
        this.dynamicInhibitInputUpdates = this.rangeInhibitInputUpdates;
        this.dynamicAllowInputUpdates = this.rangeAllowInputUpdates;
        this.dynamicAllowInputUpdates();
      } else if (this.model.get("type") == "text") {
        this.dynamicRender = this.textRender;
        this.dynamicModelValueChanged = this.textModelValueChanged;
      } else if (this.model.get("type") == "image") {
        this.dynamicRender = this.imageRender;
        this.dynamicModelValueChanged = this.imageModelValueChanged;
      } else {
        this.dynamicRender = this.undefinedRender;
      }
      
    	} catch (err){
    		console.log("catch in specialize");
    	}
      
    },

    // Wrapper methods
    render: function() { console.log("render"); return this.dynamicRender();},
    inputValueChanged: function(e) {this.dynamicInputValueChanged(e);},
    modelValueChanged: function(m) {this.dynamicModelValueChanged(m);},
    modelTypeChanged: function() {this.specialize();this.render();},
    inhibitInputUpdates: function(e) {this.dynamicInhibitInputUpdates(e);},
    allowInputUpdates: function(e) {this.dynamicAllowInputUpdates(e);},

    // Specialized methods for type range
    rangeRender: function() {
      var tmpl = this.templateByType("range");
      this.$el.html(tmpl(this.model.toJSON()));
      this.input = this.$('input');
      this.input.attr('max', this.model.get("max") || 255)
      return this;
    },
    rangeInhibitInputUpdates: function(e) {this.allowUpdates = false;},    
    rangeAllowInputUpdates: function(e) {this.allowUpdates = true;},
    rangeInputValueChanged: function(e) {App.publish(this.model.get("topic")+"/on", e.target.value);},
    rangeModelValueChanged: function(m) {/*if (this.allowUpdates)*/ this.render();},

    // Specialized methods for type switch
    switchRender: function() {
    	
      var tmpl = this.templateByType("switch");
      this.$el.html(tmpl(_.extend(this.model.toJSON(), {checkedAttribute: this.model.get("value") == 1 ? "checked=\"true\"" : ""})));
      this.input = this.$('input');
      return this;
    },
    switchInputValueChanged: function(event) {App.publish(this.model.get("topic")+"/on", event.target.checked == 0 ? "0" : "1");},
    switchModelValueChanged: function(model) {this.render();},

    // Specialized methods for type text (read-only)
    textRender: function() {
      console.log("template by type");
      var tmpl = this.templateByType("text");
      this.$el.html(tmpl(this.model.toJSON()));
	  console.log("template by type done");
      return this;
    },
    textModelValueChanged: function(model) {this.render();},

    // Specialized methods for type image (read-only)
    imageRender: function() {
      var tmpl = this.templateByType("image");
      this.$el.html(tmpl(this.model.toJSON()));
      return this;
    },
    imageModelValueChanged: function(model) {this.render();},


    // Specialized methods for type undefined
    undefinedRender: function() {
      var tmpl = this.templateByType("undefined");
      this.$el.html(tmpl(this.model.toJSON()));
      return this;
    },

    // Helper methods
    methodNotImplemented: function() {},
    templateByType: function(type) {return _.template($("#" + type +"-setting-template").html());},
 });
