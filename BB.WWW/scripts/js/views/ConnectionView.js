BB.ConnectionView = Backbone.View.extend({
    className: "view", 
    id: "settings-view",
    template: $("#settings-template").html(),
    events: {"click .button.save":  "save"},

    initialize: function() {
      this.model.view = this; 
      // this.model.on('change', this.render, this);
    },

    render: function () {
        var tmpl = _.template(this.template);
        this.$el.html(tmpl(this.model.toJSON()));
        return this;
    },
    save: function(e) { 
      var arr = this.$el.find('form').serializeArray();
      var data = _(arr).reduce(function(acc, field){acc[field.name] = field.value;return acc;}, {});
      var reconnect = (this.model.get("host") != data["host"] || this.model.get("port") != data["port"])
      this.model.save(data);
      if(reconnect)
        BB.app.reconnect();
    },
   });




