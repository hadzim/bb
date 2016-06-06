// Create a View to be used with the Layout below.


BB.PageFullView = Backbone.Layout.extend({
  template: "#page-full-template",
  
  title: "Full",
  
  className: "fullrow",
  
  that: null,
  
  page: "Home",
  
  initialize: function(){
	  that = this;
	  
	  this.datacollection = BB.data;
	  this.collection = BB.nodes;
	  
	  this.datacollection.on('add', this.render, this);
      this.datacollection.on('remove', this.render, this);
      this.datacollection.on('sort', this.render, this);
      
  },
  
  close: function(){
	  
	  this.datacollection.off('add', this.render, this);
      this.datacollection.off('remove', this.render, this);
      this.datacollection.off('sort', this.render, this);
  },
  
  beforeRender: function() {
	  
	  this.insertView(".time", new BB.ComponentTimeView({}));
	  this.insertView(".date", new BB.ComponentDateView({}));
	 
	  
	  if (this.page == "Home"){
		  this.collection.each(function(n) {
		    	n.data.each(function(d) {
		    		
		    		if (d.get("type") == "Forecast"){
		    			if (d.get("name") != "D) Day After"){
		    				this.insertView("#full-content", new BB.ComponentDataBlockView({model: d, node: n}));
		    			}
		    		}
		    		
			    }, this);
		    	
		    }, this);
		  this.collection.each(function(n) {
		    	n.data.each(function(d) {
		    		if (d.get("type") == "Temperature"){
		    			if (n.get("place") == "inside"){
		    				this.insertView("#full-content", new BB.ComponentDataBlockView({model: d, node: n}));
		    			}
		    			if (n.get("place") == "outside"){
		    				this.insertView("#full-content", new BB.ComponentDataBlockView({model: d, node: n}));
		    			}
		    		}
		    		
		    	
			    }, this);
		    	
		    }, this);
	  }
	  if (this.page == "Temperature"){
		  
	  		this.collection.each(function(n) {
		    	n.data.each(function(d) {
		    		if (d.get("type") == "Temperature"){
		    			this.insertView("#full-content", new BB.ComponentDataBlockView({model: d, node: n}));
		    		}
			    }, this);
		    	
		    }, this);
	  }
	  if (this.page == "Forecast"){
		  
	  		this.collection.each(function(n) {
		    	n.data.each(function(d) {
		    		if (d.get("type") == "Forecast"){
		    			this.insertView("#full-content", new BB.ComponentDataBlockView({model: d, node: n}));
		    		}
			    }, this);
		    	
		    }, this);
	  }
	  if (this.page == "Others"){
		  
	  		this.collection.each(function(n) {
		    	n.data.each(function(d) {
		    		if (d.get("type") != "Forecast" && d.get("type") != "Temperature"){
		    			this.insertView("#full-content", new BB.ComponentDataBlockView({model: d, node: n}));
		    		}
			    }, this);
		    	
		    }, this);
	  }
  },
  
  events: {
    "click #full-go-home": "goHome",
    "click #full-go-temperature": "goTemperature",
	"click #full-go-forecast": "goForecast",
	"click #full-go-others": "goOthers"
  },
  
  serialize: function() {
	  var data = []
	  data["page"] = this.page;
	  return data;
  },
  
  goHome: function(){
	  that.page = "Home";
	  that.render();
  },
  
  goTemperature: function(){
	  that.page = "Temperature";
	  that.render();
  },
  goForecast: function(){
	  that.page = "Forecast";
	  that.render();
  },
  goOthers: function(){
	  that.page = "Others";
	  that.render();
  }
  
});
