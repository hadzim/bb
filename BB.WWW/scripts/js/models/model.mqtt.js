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
		      
		      BB.mqtt.mqttClient.subscribe('devices/+/eventlog/history', 0);
		      
		      BB.mqtt.mqttClient.subscribe('devices/+/nodes/+/data/+/meta/+', 0);
		      BB.mqtt.mqttClient.subscribe('devices/+/nodes/+/data/+', 0);
		      
		      BB.mqtt.mqttClient.subscribe('devices/+/nodes/+/data/+/history/+', 0);
		      BB.mqtt.mqttClient.subscribe('devices/+/nodes/+/data/+/future', 0);
		      
		      
		      
		      
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
	    	
	      try {
		      // Topic array parsing:
		      // Received string:     devices/$deviceId/nodes/$nodeId/settings/$settingsId/meta/type
		      // topic array index:         0/        1/    2/      3/       4/          5/   6/ 7
	
		      var payload = message.payloadString;
		      var topic = message.destinationName.split("/");
		      console.log("topic", message.destinationName, "parsed", topic , "payload: ", payload);
	      
		      var device = BB.devices.get(topic[1]);
		      if (device == null) {
			    	//console.log("create device");
			        device = new BB.Device({id: topic[1]});
			        //console.log("device created");
			        BB.devices.add(device);
			        //device.moveToRoom(undefined);
		      }
	      
		      //console.log("device ready");
	      
		      if (topic[2] == "eventlog"){
		        	var msgs = $.parseJSON(payload);
		        	
		        	for (var i = 0, l = BB.eventlog.length; i < l; i++){
		        		BB.eventlog.pop();        
				    }
		        	
		        	for (var i = 0, l = msgs.length; i < l; i++){
		        		var msg = msgs[i];
		        		msg.time = new Date(msg.time);
		                ev = new BB.EventLog(msg);
			        	BB.eventlog.push(ev);        
				    }
		        }
		      
		      if(topic[2] == "nodes") {
	    	    
		        var node = BB.nodes.get(topic[3]);
		        if (node == null) {
		        	node = new BB.Node({id: topic[3]});
		        	//assign node to device - later on
		        	BB.nodes.add(node);
		          //control.set("topic", "/devices/"+ topic[2] + "/controls/" + topic[4]);
		        } 
		        //console.log("node ready", BB.nodes);
	        
		        if (topic[4] == "settings"){
		        	var settingName = topic[5];
		        	var setting = node.settings.get(settingName);
		            if (setting == null) {
		              setting = new BB.Setting({id: settingName});
		              node.settings.add(setting);
		              setting.set("topic", "devices/"+ topic[1] + "/nodes/" + topic[3] + "/settings/" + settingName); 
		            } 
		            
		            if(topic[6] == null){
		            	 //value
		            	setting.set("value", payload);
		            	
		            	if (settingName == "name"){
		            		node.set("name", payload);
		            	}
		            	if (settingName == "place"){
		            		var place = payload
		            		node.set("place", place);
		            		
		            		var placeItem = BB.places.get(place);
				            if (placeItem == null) {
				              placeItem = new BB.Place({id: place, name: place});
				              BB.places.add(placeItem);
				            }
		            		
		            	}
		            } 
		            if (topic[6] == "meta" && topic[7] != null) {
		            	try {
		            		setting.set(topic[7], payload);
		            	} catch (error){
		            		console.log("settings meta failed", setting, topic[7], payload, error);
		            	}
		            }
		        }
		        
		        
		        
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
		            
		            if(topic[6] == null){
		           	 //value
		            	var val = $.parseJSON(payload);
		            	data.set("value", val);
		            	data.set("tags", val.tags);
		            	
		            } 
		            if (topic[6] == "meta" && topic[7] != null) {
		            	//meta info
		            	if (topic[7] == 'unit' && payload == 'C'){
		            		payload = "&deg;C";
		            	}
		            	if (topic[7] == 'unit' && payload == 'null'){
		            		payload = "";
		            	}
		            	data.set(topic[7], payload);
		            	
		            	if (topic[7] == 'type'){
		            		var type = payload;
		            		
		            		var typeItem = BB.dataTypes.get(type);
				            if (typeItem == null) {
				              typeItem = new BB.DataType({id: type, name: type});
				              BB.dataTypes.add(typeItem);
				            }
		            		
		            	}
		            	
		           }
		           if (topic[6] == "history" && topic[7] != null) {
		               data.set("history-" + topic[7] , $.parseJSON(payload));
		           }
		           if (topic[6] == "future") {
		               data.set("future" , $.parseJSON(payload));
		           }
		            
		        }
		        
		      } 
		  
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
		      
	    },
	    
	    publishForDevice: function(deviceId, subtopic, value) {
	    	this.publish("devices/"+deviceId+subtopic, value);
	    }

  });	

  