
  BB.Setting = Backbone.Model.extend({
    defaults: function() {return {topic: "unknown", value:"", type: "undefined" };},
    
  });	

  BB.Place = Backbone.Model.extend({
	    defaults: function() {return {name: "undefined" };},
  });
  BB.DataType = Backbone.Model.extend({
	    defaults: function() {return {name: "undefined" };},
  });
  
  BB.Data = Backbone.Model.extend({
    defaults: function() {return {name: "", topic: "unknown", value:"", type: "undefined", unit: "", icon: "", tags: [] };}, 
    
    initialize: function() {
    	this.on('change', this.updateAttributes, this);
    },
    
    fgColor: function(){
  	  var color = "white";
  	  if ($.inArray("alarm", this.get("tags")) != -1){
  		  color = "red";
  	  }
  	  if ($.inArray("extremelyLow", this.get("tags")) != -1){
  		  color = "blue";  
  	  }
  	  if ($.inArray("low", this.get("tags")) != -1){
		  color = "lightBlue";  
	  }
  	  if ($.inArray("high", this.get("tags")) != -1){
		  color = "orange";  
	  }
  	  if ($.inArray("extremelyHigh", this.get("tags")) != -1){
		  color = "red";  
	  }
  	  return color;
    },
    
    updateAttributes: function() {
    	if (this.get("type") != "undefined" && this.get("icon") == ""){
    		var icon  = "fa-asterisk";
    		var type = this.get("type");
    		if (type == "Temperature"){
    			icon = "fa-cloud";
    		}
    		if (type == "Motion"){
    			icon = "fa-street-view";
    		}
    		if (type == "Contact"){
    			icon = "fa-sign-in";
    		}
    		if (type == "Camera"){
    			icon = "fa-camera";
    		}
    		this.set("icon", icon);
    	}
    },
    
  });	

  
  BB.Node = Backbone.Model.extend({
    defaults: {
    	name: "unknown", 
    	place: "undefined",
    	icon: "",
    }, 
    initialize: function() {
    	this.settings = new BB.SettingCollection;
    	this.data = new BB.DataCollection;
    	this.on('change', this.updateAttributes, this);
    },
    
    updateAttributes: function() {
    	if (this.get("type") != "undefined" && this.get("icon") == ""){
    		var icon  = "fa-asterisk";
    		var type = this.get("type");
    		if (type == "Temperature"){
    			icon = "fa-cloud";
    		}
    		if (type == "Motion"){
    			icon = "fa-street-view";
    		}
    		if (type == "Contact"){
    			icon = "fa-sign-in";
    		}
    		if (type == "Camera"){
    			icon = "fa-camera";
    		}
    		this.set("icon", icon);
    	}
    },

  });

  BB.EventLog = Backbone.Model.extend({
	    defaults: function() {return {message: "undefined", "data": "", "dataType": "", "level": 6, "time": new Date() };},
  });
  
  BB.Device = Backbone.Model.extend({
    defaults: function() {return {name: "", room: undefined };},
    initialize: function() {this.nodes = new BB.NodeCollection;},
    /*
    hasRoom: function(){return this.get("room") != undefined && this.get("room") != null;},
    removeFromCurrentRoom: function() {
      if (this.hasRoom()) {
        this.get("room").devices.remove(this);
        if (this.get("room").devices.length == 0)
          Rooms.remove(this.get("room"));
      } 
    },
    moveToRoom: function(roomName) {
      var cleanedName = roomName || "unassigned";
      var targetRoom = Rooms.get(cleanedName);

      if(targetRoom != null && this.hasRoom() && this.get("room").get("id") == cleanedName)// Dont move when current room == target room
        return;

      console.log("Moving Device to room: %s", cleanedName);

      this.removeFromCurrentRoom();
      if (targetRoom == null) {
        console.log("Creating room %s", cleanedName);
        targetRoom = new Room({id: cleanedName});
        Rooms.add(targetRoom);
      } 
      targetRoom.devices.add(this);
      this.set("room", targetRoom);
    },*/
  });

  
  BB.SettingCollection = Backbone.Collection.extend({model: BB.Setting});
  BB.DataCollection = Backbone.Collection.extend({model: BB.Data}); 
  BB.DeviceCollection = Backbone.Collection.extend({model: BB.Device});
  BB.PlaceCollection = Backbone.Collection.extend({model: BB.Place});
  BB.DataTypeCollection = Backbone.Collection.extend({model: BB.DataType});
  BB.EventLogCollection = Backbone.Collection.extend({model: BB.EventLog});
  
  
  BB.Room = Backbone.Model.extend({initialize: function() {this.devices = new BB.DeviceCollection;}});
  BB.RoomCollection = Backbone.Collection.extend({model: BB.Room});
  
  BB.NodeCollection = Backbone.Collection.extend({model: BB.Node, initialize: function() {
	    this.sort_key = 'order';
  },
  comparator: function(a, b) {
      // Assuming that the sort_key values can be compared with '>' and '<',
      // modifying this to account for extra processing on the sort_key model
      // attributes is fairly straight forward.
      a = a.get(this.sort_key);
      b = b.get(this.sort_key);
      return a > b ?  1
           : a < b ? -1
           :          0;
  } }
  );
  
  