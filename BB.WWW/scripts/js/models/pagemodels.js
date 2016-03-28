  
  

  BB.Logger = Backbone.Model.extend({
    initialize: function(){
      this.set("logger", console.log); 
      //Settings.get("logging")==='1' ? this.enable() : this.disable();
	this.enable()
    },
    enable: function(){window['console']['log'] = this.get("logger");},
    disable: function(){  
      console.log("Console.log disabled. Set local storage logging=1 to enable");
      console.log=function(){};}
  });

  BB.Filter = Backbone.Model.extend({
	    initialize: function(){
	    	this.places = [];
	    	this.dataTypes = [];
	        this.set("places", []);
	        this.set("dataTypes", []);
	    },
	    updatePlaces: function(){
	    	this.set("places", this.places);
	    	this.trigger('change:places', this);
	    	this.trigger('change', this);
	    },
	    updateDataTypes: function(){
	    	console.log("update data types: ", this.dataTypes);
	    	this.set("dataTypes", this.dataTypes);
	    	this.trigger('change:dataTypes', this);
	    	this.trigger('change', this);
	    },
	    
	    addPlace: function(place){
	    	if ($.inArray(place, this.places) == -1){
	    		this.places.push(place);
	    	}
	    	this.updatePlaces();
	    },
	    removePlace: function(place){
	    	this.places = jQuery.grep( this.places, function(value) {
				  return value != place;
			});
	    	this.updatePlaces();
	    },
	    clearPlaces: function(){
	    	this.places = [];
	    	this.updatePlaces();
	    },
	    setPlaces: function(p){
	    	if (p != null && p != undefined){
		    	this.places = p;
		    	this.updatePlaces();
	    	} else {
	    		this.clearPlaces();
	    	}
	    },
	    
	    clearDataTypes: function(){
	    	this.dataTypes = [];
	    	this.updateDataTypes();
	    },
	    setDataTypes: function(p){
	    	if (p != null && p != undefined){
		    	this.dataTypes = p;
		    	this.updateDataTypes();
	    	} else {
	    		this.clearDataTypes();
	    	}
	    },
	    
	    nodePass: function(node){
	    	if (this.places.length > 0 && $.inArray(node.get("place"), this.places) == -1){
	    		return false;
    		}
	    	return true;
	    },
	    
	    dataPassType: function(node, data){
	    	if (this.dataTypes.length > 0 && $.inArray(data.get("type"), this.dataTypes) == -1){
	    		return false;
    		}
	    	return true;
	    },
	    
	    dataPassPlaces: function(node, data){
	    	return this.nodePass(node, data);
	    },
	    
	    dataPass: function(node, data){
	    	return this.dataPassType(node, data) && this.dataPassPlaces(node, data);
	    }
	    
	    
  });
  
  