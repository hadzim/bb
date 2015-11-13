BB.RoomLinkView = Backbone.View.extend({
    className: "room-link", 
    tagName: "li",
    template: $("#room-link-template").html(),
    initialize: function() {this.model.roomLink = this;},
    render: function () {
        var tmpl = _.template(this.template);
        var id, link; 
        if (this.model instanceof Backbone.Model) { // when model is actually a model, its the view for a room
          id = this.model.get("id");
          link = "#rooms/"+id;
        } else { // When model is a collection, this is the "All" view that has to be treated a bit differently
          id = "All"
          link = "#";
        }
        this.$el.html(tmpl({link: link, id: id}));
        return this;
    },
   });
