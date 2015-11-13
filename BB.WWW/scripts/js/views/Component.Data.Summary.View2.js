// Create a View to be used with the Layout below.
BB.ComponentDataSummaryView2 = Backbone.Layout.extend({
  template: "#component-data-summary-template2",
  className: "col-lg-4 col-xs-6 center",
  
  initialize: function(){
	  this.model.on('change', this.render, this);
      this.model.on('destroy', this.remove, this);
  },
  
  afterRender: function(){
	  var values = [];
	  
	  
	  /*
	  
	  console.log()
	  
	  var myvalues = [1000, 1200, 920, 927, 931, 1027, 819, 930, 1021];
	  $('#sparkline-1').sparkline(myvalues, {
	    type: 'line',
	    lineColor: '#92c1dc',
	    fillColor: "#ebf4f9",
	    height: '50',
	    width: '80'
	  });*/
  },
  
});

/*
 * var myvalues = [1000, 1200, 920, 927, 931, 1027, 819, 930, 1021];
  $('#sparkline-1').sparkline(myvalues, {
    type: 'line',
    lineColor: '#92c1dc',
    fillColor: "#ebf4f9",
    height: '50',
    width: '80'
  });
 * */




