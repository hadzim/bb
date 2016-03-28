// Create a View to be used with the Layout below.


BB.PageGraphViewOld = Backbone.Layout.extend({
  template: "#page-graph-template",
  
  title: "Graphs",
  
  className: "row",
  
  initialize: function(){
	  this.collection = BB.nodes;
	  
	  this.filter = BB.filter;
	  this.filter.on('change', this.render, this);
      
	  //this.collection.on('add', this.updateChart, this);
      //this.collection.on('remove', this.updateChart, this);
      //this.collection.on('sort', this.updateChart, this);
      
  },
  
  beforeRender: function() {
	  this.insertView(".filters", new BB.ComponentFilterView());
  },
  
  afterRender: function(){
	  this.updateChart();
  },
  updateChart: function(){
	//-------------
    //- LINE CHART -
    //--------------
	  
	  var alldata = [];
	  var graphs = [];
	  this.collection.each(function(n) {
	    	n.data.each(function(d) {
	    		
	    		if (this.filter.passNode(n)){
	    		
	    			var key = n.get("name") + "-" + n.get("place") + "-" + d.get("type");
	    			
	    		var g = {
	    		        "id": key,
	    		        "fillAlphas": 0.4,
	    		        "valueField": key,
	    		        "balloonText": "<div style='margin:5px; font-size:19px;'>" + key + ":<b>[[value]]</b></div>"
	    		    }	
	    		graphs.push(g);
	    			
	    		  var values = [];
    			  var hist = d.get("history-hour");
    			  
    			  console.log(key);
    			  console.log("hist", d, n, hist);
    			  if (hist != null){
    				  
    				  _.each(hist, function(item){
    					  var where = null;
    					  _.each(alldata, function(gi){
    						  if (gi.date == item.date){
    							  where = gi;
    						  }
    					  });
    					  
    					  if (where){
    						  where[key] = item.value;
    					  } else {
    						  alldata.push({
        						  'date': item.date,
        						  key: item.value
        					  });
    					  }
    				  });
    				  
    			  }
	    				    		
	    		}
		    	
		    }, this);
	    	
	    }, this);
	  
	  console.log("data", alldata);
	  
	  var chart;

      //AmCharts.ready(function () {
          // XY CHART

          chart = new AmCharts.AmXYChart();
          //chart.dataDateFormat = "YYYY-MM-DD HH:mm:SS";
          chart.dataProvider = alldata;
          chart.theme = "black";

          // AXES
          // X
          var xAxis = new AmCharts.ValueAxis();
          xAxis.title = "Time";
          xAxis.position = "bottom";
          xAxis.dashLength = 1;
          xAxis.axisAlpha = 0;
          xAxis.type = "date";
          xAxis.autoGridCount = true;
          chart.addValueAxis(xAxis);

          // Y
          var yAxis = new AmCharts.ValueAxis();
          yAxis.position = "left";
          //yAxis.title = "Y Axis";
          yAxis.dashLength = 1;
          yAxis.axisAlpha = 0;
          yAxis.autoGridCount = true;
          yAxis.minPeriod = "mm";
	      yAxis.parseDates = true;
          
	      chart.addValueAxis(yAxis);

          this.collection.each(function(n) {
  	    	n.data.each(function(d) {
	  	    		if (this.filter.passNode(n)){
	  	    		  var key = n.get("name") + "-" + n.get("place") + "-" + d.get("type");
	  	    		  var graph1 = new AmCharts.AmGraph();
		  	          //graph1.lineColor = "#FF6600";
		  	          graph1.balloonText = "date:[[x]]<br>value:[[y]]";
		  	          graph1.xField = "date";
		  	          graph1.yField = key;
		  	          graph1.lineAlpha = 1;
		  	          graph1.type = "smoothedLine";
		  	          graph1.bullet = "triangleUp";
		  	          chart.addGraph(graph1);
	  	    		}
  	      }, this);
	    	
  	    }, this);
  	  
          // CURSOR
          var chartCursor = new AmCharts.ChartCursor();
          chartCursor.categoryBalloonDateFormat = "JJ:NN, DD MMMM";
          chartCursor.cursorPosition = "mouse";
          chart.addChartCursor(chartCursor);

          // SCROLLBAR

          var chartScrollbar = new AmCharts.ChartScrollbar();
          chart.addChartScrollbar(chartScrollbar);
          
          
          // WRITE
          chart.write("dataChart");
          
	  
	  /*
	  var chart = AmCharts.makeChart("dataChart", {
		    "type": "serial",
		    "theme": "dark",
		    "marginRight": 80,
		    "dataProvider": alldata,
		    "valueAxes": [{
		        "position": "left",
		        "title": "TODO"
		    }],
		    "graphs": graphs,
		    "chartScrollbar": {
		        "graph": "g1",
		        "scrollbarHeight": 80,
		        "backgroundAlpha": 0,
		        "selectedBackgroundAlpha": 0.1,
		        "selectedBackgroundColor": "#888888",
		        "graphFillAlpha": 0,
		        "graphLineAlpha": 0.5,
		        "selectedGraphFillAlpha": 0,
		        "selectedGraphLineAlpha": 1,
		        "autoGridCount": true,
		        "color": "#AAAAAA"
		    },
		    "chartCursor": {
		        "categoryBalloonDateFormat": "JJ:NN, DD MMMM",
		        "cursorPosition": "mouse"
		    },
		    "categoryField": "date",
		    "categoryAxis": {
		        "minPeriod": "mm",
		        "parseDates": true
		    },
		    "export": {
		        "enabled": true
		    }
		});*/
	  
     // });
  }

});
