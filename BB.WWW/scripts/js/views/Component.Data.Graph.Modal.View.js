BB.ComponentDataGraphModalView = Backbone.Layout.extend({

    //id: 'base-modal',
    className: 'modal modal-default',
    template: "#component-data-graph-modal-template",

    events: {
      'hidden.bs.modal': 'teardown',
      'click button.confirm': 'teardown'
    },

    initialize: function() {
      //_(this).bindAll();
      //this.render();
    },
    
    graphView: null,

    beforeRender: function() {
    	this.graphView  = new BB.ComponentGraphView({data:[{node: this.node, data: this.data}]});
    	this.insertView(".graphArea", this.graphView);
    },
    
    afterRender: function() {
    	
    },

    show: function() {
    	console.log("show modal graph");
        this.render();
        //$('body').append(this.$el);
        
        //console.log(this.$el);
        console.log("jquery modal");
        this.$el.modal('show');
        console.log("jquery modal done");
    },
    
    teardown: function() {
    	console.log("teardown");
    	this.graphView.close();
        this.$el.data('modal', null);
        this.remove();
        
    },

 });