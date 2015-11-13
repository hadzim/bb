BB.ApplicationView = Backbone.View.extend({
	    el: $("body"),
	    container: $("#content"),
	    
	    roomLinks: $("#room-links > ul"),
	    connectivity: $("#connectivity"),

	    mqttClient: undefined,
	    connectivityTimeoutId: undefined,


	    initialize: function() {
	      BB.connection.on('change:connectivity', this.connectivityChanged, this);
	      BB.rooms.on('add', this.addRoom, this);
	      BB.rooms.on('remove', this.removeRoom, this);
	      _.bindAll(this, 'connect', 'connected', 'publish', 'publishForDevice', 'connectionLost', 'disconnect', 'disconnected');
	      this.addRoom(BB.devices);
	      
	      
	    },
	    connectivityChanged: function(e){
	      if(this.connectivityTimeoutId)
	        clearTimeout(this.connectivityTimeoutId);
	      console.log("Connectivity changed to: %s", e.get("connectivity"));
	      this.connectivity.removeClass("visible");
	      this.connectivity.html(e.get("connectivity"));
	      this.connectivity.addClass("visible");
	      var that = this; 
	      this.connectivityTimeoutId = setTimeout(function(){that.connectivity.removeClass("visible")}, 5000);

	    },
	    addRoom: function(room) {
	      var roomLinkView = new BB.RoomLinkView({model: room});
	      this.roomLinks.append(roomLinkView.render().$el);
	    },
	    removeRoom: function(room) {room.roomLink.close();},
	    
	    showView: function(view) {
	    	if (this.currentView){
	    		this.currentView.close();
	    	}
	      	this.currentView = view;
	      	this.render(this.currentView);
	    },
	    render: function(view){
	      this.container.html(view.render().$el);
	      view.delegateEvents();
	      view.finish();
	    },
	    reconnect: function(){
	      console.log("Reconnecting");
	      this.disconnect();
	      this.connect(); 
	    },
	    connect: function() {
	    
	    	console.log("APP", BB.connection.get("host"), parseInt(BB.connection.get("port")));
	    	
	      BB.connection.set("connectivity", "connecting");
	      this.mqttClient = new Messaging.Client(BB.connection.get("host"), parseInt(BB.connection.get("port")), "homA-web-"+Math.random().toString(36).substring(6));
	      this.mqttClient.onConnectionLost = this.connectionLost;
	      this.mqttClient.onMessageArrived = this.messageArrived;
	      this.mqttClient.connect({onSuccess:this.connected, useSSL: false});
	    },
	    connected: function(response){
	      BB.connection.set("connectivity", "connected");
	      this.mqttClient.subscribe('devices/+/nodes/+', 0);
	      
	      this.mqttClient.subscribe('devices/+/nodes/+/settings/+/meta/+', 0);
	      this.mqttClient.subscribe('devices/+/nodes/+/settings/+', 0);
	      
	      this.mqttClient.subscribe('devices/+/nodes/+/data/+/meta/+', 0);
	      this.mqttClient.subscribe('devices/+/nodes/+/data/+', 0);
	      
	      
	      //this.mqttClient.subscribe('/devices/+/meta/#', 0);
	      window.onbeforeunload = function(){BB.app.disconnect()}; 
	    },
	    disconnect: function() {
	      if(BB.connection.get("connectivity") == "connected")
	        this.mqttClient.disconnect(); 
	    },
	    disconnected: function() {
	      BB.connection.set("connectivity", "disconnected");
	      console.log("Connection terminated");

	      for (var i = 0, l = Devices.length; i < l; i++)
	    	  BB.devices.pop();        

	      for (i = 0, l = Rooms.length; i < l; i++)
	    	  BB.rooms.pop();       
	    },
	    connectionLost: function(response){ 
	      if (response.errorCode !== 0) {
	        console.log("onConnectionLost:"+response.errorMessage, response, this.mqttClient);
	        setTimeout(function () {BB.app.connect();}, 5000); // Schedule reconnect if this was not a planned disconnect
	      }

	      this.disconnected();
	    },
	    messageArrived: function(message){
	      try {
	      // Topic array parsing:
	      // Received string:     devices/$deviceId/nodes/$nodeId/settings/$settingsId/meta/type
	      // topic array index:         0/        1/    2/      3/       4/          5/   6/ 7

	      var payload = message.payloadString;
	      var topic = message.destinationName.split("/");
	      console.log("-----------RECEIVED-----------");
	      console.log("topic", topic , "payload: ", payload);
	      
	      var device = BB.devices.get(topic[1]);
	      if (device == null) {
	    	  console.log("create device");
	        device = new BB.Device({id: topic[1]});
	        console.log("device created");
	        BB.devices.add(device);
	        //device.moveToRoom(undefined);
	      }
	      
	      console.log("device ready");
	      
	      if(topic[2] == "nodes") {
	    	    
	        var node = device.nodes.get(topic[3]);
	        if (node == null) {
	        	node = new BB.Node({id: topic[3], name: topic[3]});
	        	device.nodes.add(node);
	          //control.set("topic", "/devices/"+ topic[2] + "/controls/" + topic[4]);
	        } 
	        console.log("node ready");
	        
	        if (topic[4] == "settings"){
	        	console.log("set 1");
		        
	        	var setting = node.settings.get(topic[5]);
	        	console.log("set 2");
	            if (setting == null) {
	            	console.log("set 3");
	              setting = new BB.Setting({id: topic[5]});
	              console.log("set 4");
	              node.settings.add(setting);
	              console.log("set 5");
	              setting.set("topic", "/devices/"+ topic[1] + "/nodes/" + topic[3] + "/settings/" + topic[5]); 
	            } 
	            console.log("settings ready");
	           
	            if(topic[6] == null){
	            	 //value
	            	console.log("settings value");
	            	setting.set("value", payload);
	            	 
	                 
	            } if (topic[6] == "meta" && topic[7] != null) {
	            	//meta info
	            	console.log("settings meta", setting, topic[7], payload);
	            	try {
	            		setting.set(topic[7], payload);
	            	} catch (error){
	            		console.log("settings meta failed", setting, topic[7], payload, error);
	            	}
	            	
	            }
	        }
	        
	        console.log("settings done");
	        
	        if (topic[4] == "data"){
	        	var data = node.data.get(topic[5]);
	            if (data == null) {
	            	console.log("c1");
	    	        
	              data = new BB.Data({id: topic[5]});
	              console.log("c2");
	              node.data.add(data);
	              console.log("c3");
	              data.set("topic", "/devices/"+ topic[1] + "/nodes/" + topic[3] + "/data/" + topic[5]);
	              //control.set("topic", "/devices/"+ topic[2] + "/controls/" + topic[4]);
	            } 
	            
	            console.log("data ready");
	            
	            if(topic[6] == null){
	           	 //value
	            	console.log("data value");
	            	
	            	data.set("value", payload);
	            } if (topic[6] == "meta" && topic[7] != null) {
	            	//meta info
	            	console.log("data meta");
	            	data.set(topic[7], payload);
	           }
	            
	        }
	        
	        console.log("data done");
	        
	      } /*else if(topic[3] == "meta" ) {                             // TODO: Could be moved to the setter to facilitate parsing
	        if (topic[4] == "room")                                    // Device Room
	          device.moveToRoom(payload);
	        else if(topic[4] == "name")                                // Device name
	          device.set('name', payload);
	      }*/
	     console.log("-----------/ RECEIVED-----------");
	      } catch (error){
	    	  console.log("Processing MQTT error: ", error);
	      }
	    },
	    publish: function(topic, value) {
	      value = value != undefined ? value : "";
	      console.log("Publishing " + topic+":"+value);
	      var message = new Messaging.Message(value);
	      message.destinationName = topic;
	      message.retained = true;
	      this.mqttClient.send(message); 
	    },
	    publishForDevice: function(deviceId, subtopic, value) {
	      this.publish("/devices/"+deviceId+subtopic, value);
	    }
  });




