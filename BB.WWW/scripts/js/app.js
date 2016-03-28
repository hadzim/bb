/* MODELS */
  Backbone.View.prototype.close = function() {
    this.off();
    this.remove();
    if (this.onClose) //For custom handlers to clean up events bound to anything else than this.model and this.collection
      this.onClose();
    if (this.model)
      this.model.off(null, null, this);
    if (this.collection)
      this.collection.off(null, null, this);
  }
  Backbone.View.prototype.finish = function() {}


var BB = {

};

BB.Router = Backbone.Router.extend({
    routes: {
        "settings" : "settings",
        "devices/:device/settings": "deviceSettings",
        "rooms/:room": "room",
        "": "index",
        "/": "index",
      },
      initialize: function(){
          Backbone.history.start({pushState : false});
          this.routesHit = 0;
          Backbone.history.on('route', function() { this.routesHit++; }, this);
      },
      index: function() {
        var indexView = new BB.RoomView({model: BB.devices});
        BB.app.showView(indexView);   
      },
      // TODO: the room and deviceSettings route behave exactly the same. They should be merged to a single method
      room: function(id) {
        var room = BB.Rooms.get(id); // Room might not yet exists
        var view; 
        var matchId = id;

        if (room == null)
          view = new PlaceholderView({model: Rooms, callbackRoute: Backbone.history.fragment, readyComparator: function(addedObject){return addedObject.get("id") == matchId;}});
        else
          view = new BB.RoomView({model: room});
        BB.app.showView(view);
      },
      deviceSettings: function(id) {
        var device = BB.devices.get(id); // Device might not yet exists
        var view; 
        var matchId = id;
        console.log("Backbone.history");
        console.log(Backbone.history);

        if (device == null)
          view = new BB.PlaceholderView({model: BB.devices, callbackRoute: Backbone.history.fragment, readyComparator: function(addedObject){return addedObject.get("id") == matchId;}});
        else
          view = new BB.DeviceSettingsView({model: device});
        BB.app.showView(view);
      },
      settings: function () {
        var settingsView = new BB.ConnectionView({model: BB.connection});
        BB.app.showView(settingsView);
      },
        back: function() {
      if(this.routesHit > 1) {
        //more than one route hit -> user did not land to current page directly
        window.history.back();
      } else {
        //otherwise go to the home page. Use replaceState if available so
        //the navigation doesn't create an extra history entry
        this.navigate('/', {trigger:true, replace:true});
      }
    }

    });


BB.NewRouter = Backbone.Router.extend({
    routes: {
        "nodes" : "nodes",
        "data" : "data",
        "rules" : "rules",
        "graph" : "graph",
        "events" : "pevents",
        "": "home",
        "/": "home",
      },
      initialize: function(){
          Backbone.history.start({pushState : false});
          this.routesHit = 0;
          Backbone.history.on('route', function() { this.routesHit++; }, this);
      },
      home: function() {
        var homeView = new BB.PageHomeView();
        BB.layout.showView(homeView);   
      },

      nodes: function() {
          var nodesView = new BB.PageNodesView();
          BB.layout.showView(nodesView);   
      },

      data: function() {
          var dataView = new BB.PageDataView();
          BB.layout.showView(dataView);   
      },

      graph: function() {
          var graphView = new BB.PageGraphView();
          BB.layout.showView(graphView);   
      },

      rules: function() {
    	  console.log("Rules");
          var rulesView = new BB.PageRulesView();
          BB.layout.showView(rulesView);
          BB.rules.download();
      },
      
      pevents: function() {
    	  console.log("Event Log");
          var eView = new BB.PageEventLogView();
          BB.layout.showView(eView);
      },
      
     back: function() {
      if(this.routesHit > 1) {
        //more than one route hit -> user did not land to current page directly
        window.history.back();
      } else {
        //otherwise go to the home page. Use replaceState if available so
        //the navigation doesn't create an extra history entry
        this.navigate('/', {trigger:true, replace:true});
      }
    }

    });


var clockModel;

$(document).on("ready", function () {
	/*
    BB.connection = new BB.Connection();
    BB.logger = new BB.Logger();
    BB.devices = new BB.DeviceCollection;
    BB.rooms = new BB.RoomCollection;
    BB.app = new BB.ApplicationView;
    BB.menu = new BB.MenuView;
    BB.header = new BB.HeaderView;
    
    
    
    BB.app.connect();
    
    BB.dummy = new BB.DummyView();
    BB.dummy.render();*/
	
	clockModel = new Backbone.Model({ time: new Date() })

	setInterval(function () {
		  clockModel.set('time', new Date())
	}, 1000);

	
	BB.connection = new BB.Connection();
	BB.mqtt = new BB.MQTT(); 
	
	BB.nodes = new BB.NodeCollection;
	BB.data = new BB.DataCollection;
	BB.rules = new BB.RuleCollection;
	
	BB.devices = new BB.DeviceCollection;
    
	BB.rooms = new BB.RoomCollection;
	BB.places = new BB.PlaceCollection;
	BB.dataTypes = new BB.DataTypeCollection;
	BB.filter = new BB.Filter;
	BB.eventlog = new BB.EventLogCollection;
	
	BB.layout = new BB.LayoutView();
    BB.router = new BB.NewRouter();
	
	BB.layout.render();
	
	if ($('body').hasClass('sidebar-collapse')) {
      $("[data-layout='sidebar-collapse']").attr('checked', 'checked');
    }
});