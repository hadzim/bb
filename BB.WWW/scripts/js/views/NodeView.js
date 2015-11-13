BB.NodeView = Backbone.View.extend({
	
		  className: "nodeview col-md-4",
	
	      template: $("#node-template").html(),
	      templateSettings: $("#node-settings-template").html(),
	      templateData: $("#node-data-template").html(),
	      
	      initialize: function() {
	    	  
	        this.model.on('change', this.render, this);
	        this.model.on('destroy', this.remove, this);
	        
	        this.model.settings.on('add', this.addSetting, this);
	        this.model.settings.on('remove', this.renderSettings, this);
	        
	        this.model.data.on('add', this.addData, this);
	        this.model.data.on('remove', this.renderData, this);
	        
	        this.model.view = this;
	      },
	      
	      renderSettings: function() {
	    	this.$(".settings").html("");
	    	
	        for (var i = 0, l = this.model.settings.length; i < l; i++){
	          this.addSetting(this.model.settings.models[i]);
	        }
	        return this;
	      },

	      renderData: function() {
	    	this.$(".data").html("");
	    	
	        for (var i = 0, l = this.model.data.length; i < l; i++){
	          this.addData(this.model.data.models[i]);
	        }
	        return this;
	      },

	      
	      renderStatic: function() {
	        var tmpl = _.template(this.template);
	        this.$el.html(tmpl(this.model.toJSON()));	        
	        return this;
	      },

	      render: function() {
	        return this.renderStatic().renderSettings().renderData();
	      },
	      
	      addSetting: function(setting) {
	    	  
	    	  setting.on('change', this.renderSettings, this);
	    	  var tt = _.template(this.templateSettings);
	    	  
	    	  console.log("add setting");
	    	  console.log(setting.toJSON());
	    	  //console.log(tt(setting.toJSON()));
	    	  this.$(".settings").append(tt(setting.toJSON())); 
	      },
	      
	      addData: function(data) {
	    	  
	    	  console.log("add data 1");
	    	  
	    	  data.on('change', this.renderData, this);
	    	  
	    	  console.log("add data 2");
	    	  
	    	  
	    	  var tt = _.template(this.templateData);
	    	  
	    	  console.log("add data");
	    	  console.log(data.toJSON());
	    	  //console.log(tt(setting.toJSON()));
	    	  this.$(".data").append(tt(data.toJSON())); 
	      },
	    });


