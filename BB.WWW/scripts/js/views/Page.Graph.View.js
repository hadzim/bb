// Create a View to be used with the Layout below.


BB.PageGraphView = Backbone.Layout.extend({
  template: "#page-graph-template",
  
  title: "Graphs",
  
  className: "row",
  
  period: "hour",
  graphView: null,
  
  initialize: function(){
	  this.collection = BB.nodes;
	  
	  this.filter = BB.filter;
	  this.filter.on('change', this.render, this);
      
	  this.datacollection = BB.data;
	  
	  this.datacollection.on('add', this.render, this);
      this.datacollection.on('remove', this.render, this);
      
      
  },
  
  close: function(){
	  this.filter.off('change', this.render, this);
	  this.datacollection.off('add', this.render, this);
      this.datacollection.off('remove', this.render, this);
      
      if (this.graphView){
		  this.graphView.close();
	  }
  },
  /*
  events: {
      "click select#period" : "periodChanged",
  },
  *//*
  periodChanged: function(name, evt){
	  console.log(this, name, evt);
	  var element = $("#periodFilter");
	  this.period = element.val();
	  this.render();
  },*/
  
  serialize: function() {
	  var data = this.model ? _.clone(this.model.attributes) : {};
	  data["period"] = this.period;
	  return data;
  },
  
  
  
  beforeRender: function() {
	  this.insertView(".places", new BB.ComponentFilterPlacesView());
	  this.insertView(".types", new BB.ComponentFilterDataTypesView());
	  
	  var graphRows = [];
	  this.collection.each(function(n) {
	    	n.data.each(function(d) {
	    		
	    		if (this.filter.dataPass(n, d)){
	    			graphRows.push({
	    				node: n,
	    				data: d
    				});
	    		}
		    	
		    }, this);
	  }, this);
	  
	  if (this.graphView){
		  this.graphView.close();
	  }
	  
	  this.graphView = new BB.ComponentGraphView({
		  data: graphRows, 
		  period: this.period,
		  pp: this.period
      });
	  
	  console.log("above period", this.period);
	  this.insertView(".graph", this.graphView);
  },
  
  afterRender: function(){
	  var that = this;
	  $("#periodFilter").multiselect({
		  nonSelectedText: 'Choose a period'
	  });
	  $("#periodFilter").on("change", function(name, evt){
		  var element = $("#periodFilter");
		  that.period = element.val();
		  that.render(); 
	  });
	  
	  
	  //this.updateChart();
	  
  },
  updateChart: function(){
	//-------------
    //- LINE CHART -
    //--------------
	  
	  var alldata = [];
	  var allgraphs = [];
	  this.collection.each(function(n) {
	    	n.data.each(function(d) {
	    		
	    		if (this.filter.dataPass(n, d)){
	    		
	    		  var kk = n.get("name") + "-" + n.get("place") + "-" + d.get("type");
	    		  var read = n.get("name") + "@" + n.get("place");
	    			
	    		  var g = {
    				  type: "line",
    	              title: read,
    	              valueField: kk,
    	              lineThickness: 2,
    	              fillAlphas: 0,
    	              bullet: "round",
    	              balloonText: "[[title]]<br />" + d.get("type") + "<br />[[category]]<br /><b>[[value]]</b>"
	    		  }
	    		  allgraphs.push(g);
	    			
	    		  var values = [];
    			  var hist = d.get("history-" + this.period);
    			  
    			  if (hist != null){
    				  
    				  _.each(hist, function(item){
    					  var where = null;
    					  _.each(alldata, function(gi){
    						  if (gi.date == item.date){
    							  where = gi;
    						  }
    					  });
    					  
    					  if (where){
    						  where[kk] = item.value;
    					  } else {
    						  var d=  {
            						  'date': item.date,
            			      }
    						  d[kk] = item.value;
    						  alldata.push(d);
    					  }
    				  });
    				  
    			  }
	    				    		
	    		}
		    	
		    }, this);
	    	
	    }, this);
	  
	   //data ready now draw a chart
	// 
	  console.log("all Data", alldata);
	  console.log("graphs", allgraphs);
	  console.log("AmCharts", AmCharts);
      var chart1 = AmCharts.makeChart("dataChart", {
          type: "serial",
          theme: "black",
          dataProvider: alldata,
          categoryField: "date",
          startDuration: 0,

          categoryAxis: {
              gridPosition: "start",
              parseDates: true,
              minPeriod: "mm",
          },
          
          valueAxes: [{
              title: "Value"
          }],
          graphs: allgraphs,
          legend: {
              useGraphSettings: true
          }

      });
      //TODO
  }

});
