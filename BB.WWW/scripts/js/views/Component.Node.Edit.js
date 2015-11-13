var BaseModalView = Backbone.View.extend({

    id: 'base-modal',
    className: 'modal modal-default fade',
    t: "#component-node-edit-template",

    events: {
      'hidden': 'teardown'
    },

    initialize: function() {
      _(this).bindAll();
      this.render();
    },

    show: function() {
      this.$el.modal('show');
    },

    teardown: function() {
      this.$el.data('modal', null);
      this.remove();
    },

    getTemplate: function(name, cb){
    	console.log(name, cb);
    	cb.call(this, $(name).html());
    },
    
    render: function() {
      this.getTemplate(this.t, this.renderView);
      return this;
    },

    renderView: function(ttt) {
    	var tpl = _.template( ttt, this.model.toJSON() );
        this.$el.html(tpl);
        this.$el.modal({show:false}); // dont show modal on instantiation
    }
 });

BB.ComponentNodeEditView = Backbone.Layout.extend({

    //id: 'base-modal',
    className: 'modal modal-default',
    template: "#component-node-edit-template",

    events: {
      'hidden.bs.modal': 'teardown',
      'click button.confirm': 'confirm'
    },

    initialize: function() {
      //_(this).bindAll();
      //this.render();
    },

    beforeRender: function() {
  	    console.log("edit node", this.model.settings);
	    this.model.settings.each(function(s) {
	    	console.log("setting", this.model.settings);
	    	//alert(s);
	    	//if (s.id != "name" && s.id != "room"){
	    		this.insertView(".editsettings", new BB.ComponentSettingEditView({model: s}));
	    	//}
	    	
	    }, this);
  },

    
    show: function() {
    	
    	
        this.render();
        //$('body').append(this.$el);
        console.log(this.$el);
        this.$el.modal('show');
    },
    
    confirm: function(){
    	console.log("confirm", this.getViews((".editsettings")));
    	_.each(this.getViews(".editsettings").value(), function(v){ v.publish();});
    	this.teardown();
    },

    teardown: function() {
    	console.log("teardown");
        this.$el.data('modal', null);
        this.remove();
    },

 });