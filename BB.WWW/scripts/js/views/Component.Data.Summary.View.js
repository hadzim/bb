// Create a View to be used with the Layout below.
BB.ComponentDataSummaryView = Backbone.Layout.extend({
  template: "#component-data-summary-template",
  className: "data-summary clearfix",
  
  initialize: function(){
	  this.model.on('change', this.render, this);
      this.model.on('destroy', this.remove, this);
  },
  
  afterRender: function(){
	  var values = [];
	  var hist = this.model.get("history-hour");
	  
	  if (hist != null){
		  
		  _.each(hist, function(item){
			  values.push(item.value);
		  });
		  
		  console.log("history", hist, values);
		  
		  $('.data-box-graph', this.$el).sparkline(values, {
			    type: 'line',
			    /*height: '45px',
			    width: '45px',*/
			    height: '25px',
			    lineColor: "white",
			    //fillColor: "rgba(0,0,0,0.1)",
			    fillColor: "transparent",
			    lineWidth: 1,
			    spotColor: "#3c8dbc",
			    minSpotColor: "#3c8dbc",
			    maxSpotColor: "#3c8dbc"
			    /*spotColor: "#f39c12",
			    minSpotColor: "#3c8dbc",
			    maxSpotColor: "#dd4b39"*/
		  });
	  }
	  
	  var img = this.model.get("value").image;
	  
	  if (img != null){
		  $('.data-box-image', this.$el).html(
				  "<img src='" + img + "'>"
		  );  
	  }
	  
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

function parseDate(input) {
	  if (typeof input !== 'undefined') {
		  return new Date();
	  }
	  if (typeof input !== 'string') {
		  return new Date();
	  }
	  if (!input){
		  return new Date();
	  }
	  var allparts = input.substring(0, input.length - 1).split('T');
	  var dateParts = allparts[0].split('-');
	  var timeParts = allparts[1].split(':');
	  return new Date(dateParts[0], dateParts[1]-1, dateParts[2], timeParts[0], timeParts[1], timeParts[2]);
	}  


function timeSince(date) {

    var seconds = Math.floor((new Date() - date) / 1000);

    var interval = Math.floor(seconds / 31536000);

    if (interval > 1) {
        return interval + " years";
    }
    interval = Math.floor(seconds / 2592000);
    if (interval > 1) {
        return interval + " months";
    }
    interval = Math.floor(seconds / 86400);
    if (interval > 1) {
        return interval + " days";
    }
    interval = Math.floor(seconds / 3600);
    if (interval > 1) {
        return interval + " hours";
    } 
    interval = Math.floor(seconds / 60);
    if (interval > 15) {
        return interval + " minutes";
    } else if (interval > 5){
    	return  "few minutes";
    }
    return "now";
}
