  BB.Connection = Backbone.Model.extend({
    defaults: { connectivity: "disconnected", 
                logging: "0", 
                port: "18883", 
                host: document.location.hostname || "127.0.0.1"
              },

    initialize: function() {
      for (var key in this.defaults)
        this.set(key, localStorage.getItem(key) || this.defaults[key]);
    },
    sync: function() {},// Overwritten to kill default sync behaviour
    save: function(data) {
      for (var key in data) {
        this.set(key, data[key]);
        localStorage.setItem(key, data[key]);
      }
      BB.router.back();
    }
  });

  BB.MQTT = Backbone.Model.extend({
	  
	  	mqttClient: undefined,
	  	
	  	connectivityTimeoutId: undefined,

	    initialize: function() {
	      
	    },
	  
	    connectivityChanged: function(e){
	    	
	    	  console.log("on changed", e);
	    	
		      if(this.connectivityTimeoutId){
		    	  clearTimeout(this.connectivityTimeoutId);
		      }
		      console.log("Connectivity changed to: %s", e.get("connectivity"));
		      
		      //this.connectivity.removeClass("visible");
		      //this.connectivity.html(e.get("connectivity"));
		      //this.connectivity.addClass("visible");
		      var that = this; 
		      this.connectivityTimeoutId = setTimeout(function(){that.connectivity.removeClass("visible")}, 5000);

	    },
	    
	    reconnect: function(){
		      console.log("Reconnecting");
		      this.disconnect();
		      this.connect(); 
	    },
	    
	    connect: function() {
	      
	    	 console.log("connect");
	    	
		      BB.connection.set("connectivity", "connecting");
		      
		      console.log("connect to", BB.connection.get("host"), parseInt(BB.connection.get("port")));
		      
		      this.mqttClient = new Paho.MQTT.Client(
		    		  BB.connection.get("host"), 
		    		  parseInt(BB.connection.get("port")), 
		    		  "bb-web-"+Math.random().toString(36).substring(6)
    		  );
		      
		      var options = {
		          timeout: 3,
		          useSSL: false,
		          cleanSession: false,
		          onSuccess: this.onconnected,
		          //onFailure: function (message) {
		          //    $('#status').val("Connection failed: " + message.errorMessage + "Retrying");
		          //    setTimeout(MQTTconnect, reconnectTimeout);
		          //}
		      };
		
		      this.mqttClient.onConnectionLost = this.connectionLost;
		      this.mqttClient.onMessageArrived = this.messageArrived;
		
		//      console.log("Host="+ host + ", port=" + port + " TLS = " + useTLS + " username=" + username + " password=" + password);
		      this.mqttClient.connect(options);
		      
	    },
	    
	    onconnected: function(response){
	    	
	    	  console.log("onconnected", response);
	    	
		      BB.connection.set("connectivity", "connected");
		    
		      console.log("connected1", BB.mqtt);
		      console.log("connected1", BB.mqtt.mqttClient);
		    
		      BB.mqtt.mqttClient.subscribe('devices/+/nodes/+', 0);
		      
		      BB.mqtt.mqttClient.subscribe('devices/+/nodes/+/settings/+/meta/+', 0);
		      BB.mqtt.mqttClient.subscribe('devices/+/nodes/+/settings/+', 0);
		      
		      BB.mqtt.mqttClient.subscribe('devices/+/nodes/+/data/+/meta/+', 0);
		      BB.mqtt.mqttClient.subscribe('devices/+/nodes/+/data/+', 0);
		      
		      BB.mqtt.mqttClient.subscribe('devices/+/nodes/+/data/+/history/+', 0);
		      
		      
		      console.log("connected2");
		    	
		      
		      //this.mqttClient.subscribe('/devices/+/meta/#', 0);
		      window.onbeforeunload = function(){ BB.mqtt.disconnect() };
		      
		      console.log("connected3");
		    	
	    },
	    
	    disconnect: function() {
	    		
	    	console.log("disconnect");
	    	
		      if(BB.connection.get("connectivity") == "connected"){
		    	  this.mqttClient.disconnect(); 
		      }
	    },
	    
	    disconnected: function() {
	    	
	    	console.log("disconnected");
	    	
		      BB.connection.set("connectivity", "disconnected");
		      console.log("Connection terminated");
	
		      for (var i = 0, l = BB.devices.length; i < l; i++){
		    	  BB.devices.pop();        
		      }
	
		      for (i = 0, l = BB.rooms.length; i < l; i++){
		    	  BB.rooms.pop();       
		      }
	    },
	    
	    connectionLost: function(response){ 
	    	
	    	console.log("lost");
	    	
		      if (response.errorCode !== 0) {
		    	
		    	console.log("onConnectionLost:", response);
		    	console.trace()   
		    	
		        setTimeout(function () {BB.mqtt.connect();}, 5000); // Schedule reconnect if this was not a planned disconnect
		      }
	
		      BB.mqtt.disconnected();
	    },
	    
	    messageArrived: function(message){
	    	console.log("message");
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
	    	    
		        var node = BB.nodes.get(topic[3]);
		        if (node == null) {
		        	node = new BB.Node({id: topic[3]});
		        	//assign node to device - later on
		        	BB.nodes.add(node);
		          //control.set("topic", "/devices/"+ topic[2] + "/controls/" + topic[4]);
		        } 
		        console.log("node ready", BB.nodes);
	        
		        if (topic[4] == "settings"){
		        	console.log("set 1");
			        var settingName = topic[5];
		        	var setting = node.settings.get(settingName);
		            if (setting == null) {
		              setting = new BB.Setting({id: settingName});
		              node.settings.add(setting);
		              setting.set("topic", "devices/"+ topic[1] + "/nodes/" + topic[3] + "/settings/" + settingName); 
		            } 
		            console.log("settings ready");
		           
		            if(topic[6] == null){
		            	 //value
		            	console.log("settings value");
		            	setting.set("value", payload);
		            	
		            	if (settingName == "name"){
		            		node.set("name", payload);
		            		console.log("update name", node);
		            	}
		            	if (settingName == "place"){
		            		node.set("place", payload);
		            		console.log("update place", node);
		            	}
		            	 
		                 
		            } 
		            if (topic[6] == "meta" && topic[7] != null) {
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
		        	var dataid = topic[3] + "@@" + topic[5]
		        	var data = node.data.get(dataid);
		            if (data == null) {
		            	
		              data = new BB.Data({id: dataid, name: topic[5]});
		              node.data.add(data);
		              data.set("topic", "devices/"+ topic[1] + "/nodes/" + topic[3] + "/data/" + topic[5]);
		            } 
		            
		            var coldata = BB.data.get(dataid);
			        if (coldata == null) {
			        	BB.data.add(data);
			        } 
		            
		            console.log("data ready");
		            
		            if(topic[6] == null){
		           	 //value
		            	console.log("data value");
		            	console.log("parse data", payload);
		            	var val = $.parseJSON(payload);
		            	console.log("parse data",val);
		            	data.set("value", val);
		            	data.set("tags", val.tags);
		            	
		            } 
		            if (topic[6] == "meta" && topic[7] != null) {
		            	//meta info
		            	console.log("data meta");
		            	if (topic[7] == 'unit' && payload == 'C'){
		            		payload = "&deg;C";
		            	}
		            	if (topic[7] == 'unit' && payload == 'null'){
		            		payload = "";
		            	}
		            	data.set(topic[7], payload);
		           }
		           if (topic[6] == "history" && topic[7] != null) {
		               //meta info
		               data.set("history-" + topic[7] , $.parseJSON(payload));
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
		      
		      var message = new Paho.MQTT.Message(value);
			  message.destinationName = topic;
			  message.retained = true;
			  this.mqttClient.send(message);
		      /*
		      var message = new Messaging.Message(value);
		      message.destinationName = topic;
		      message.retained = true;
		      this.mqttClient.send(message);*/ 
	    },
	    
	    publishForDevice: function(deviceId, subtopic, value) {
	    	this.publish("devices/"+deviceId+subtopic, value);
	    }

  });	

  