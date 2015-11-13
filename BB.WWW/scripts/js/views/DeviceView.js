BB.DeviceView = Backbone.View.extend({
    template: $("#device-template").html(),
    className: "card", 
    initialize: function() {
    	
      console.log("device view init");
    	
      this.model.on('change', this.render, this);
      this.model.on('destroy', this.remove, this);
      this.model.nodes.on('add', this.addNode, this);
      this.model.nodes.on('remove', this.render, this);
      this.model.nodes.on('sort', this.render, this);
      this.model.view = this;
      
      console.log("device view init done");
      
    },  
    render: function() {
    	console.log("device render begin");
      var tmpl = _.template(this.template);
      this.$el.html(tmpl(this.model.toJSON()));
      for (var i = 0, l = this.model.nodes.length; i < l; i++)
        this.addNode(this.model.nodes.models[i]);
      console.log("device render end");
      return this;
    },
    addNode: function(node) {
    	console.log("adding node to device");
      var nodeView = new BB.NodeView({model: node});
      this.$(".subviews").append(nodeView.render().el);
      console.log("adding node to device done");
    },
  });


