BB.RoomView = Backbone.View.extend({
    className: "view", 
    id: "room-view",

    initialize: function() {
      if(this.model instanceof Backbone.Model) {// this.model == ordinary room
        this.collection = this.model.devices;
        this.model.bind('remove', this.removeSelf, this);
        this.model.view = this;
      } else {// this.model == Rooms collection 
        this.collection = this.model;
      }
      this.collection.on('add', this.addDevice, this);
      this.collection.on('remove', this.removeDevice, this);
      this.layoutCardsOptions = {autoResize:true, container:this.$el,offset:25};
      _.bindAll(this, 'layoutCards');
    },
    onClose: function() {this.collection.off(); this.model.roomLink.$el.removeClass("active");},
    layoutCards: function(){
    
    	//this.$('.card').wookmark(this.layoutCardsOptions);
    	
    },
    finish: function(){
      this.layoutCards();
      if(this.model.roomLink)
        this.model.roomLink.$el.addClass("active");
    },
    render: function () {
    	console.log("render room");
      for (var i = 0, l = this.collection.length; i < l; i++)
          this.addDevice(this.collection.models[i]);
      return this;
    },
    addDevice: function(device) {
    	console.log("Add device to room");
      var deviceView = new BB.DeviceView({model: device});
      var renderedDeviceView = deviceView.render()
      this.$el.append(renderedDeviceView.el);
      renderedDeviceView.$el.resize(this.layoutCards);
      this.layoutCards();
      console.log("Add device to room done");
    },
    removeDevice: function(device) {device.view.close();},
    removeSelf: function(room) {Backbone.history.loadUrl( "rooms/"+room.get("id") )}
  });
