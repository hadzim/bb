// Create a View to be used with the Layout below.

function compareGraphData(a, b) {
  if (a.date < b.date)
    return -1;
  if (a.date > b.date)
    return 1;
  return 0;
}

var dayNamesShort = new Array("Ne", "Po", "Út", "St", "Čt", "Pá", "So");

BB.ComponentGraphView = Backbone.Layout.extend({
  template: "#component-graph-template",
 
  className: "",
  
  period: "hour",
  
  data: [],
  
  initialize: function(){
	  
	  var that = this;
	  
	  _.each(this.data, function(item){
		  var dataItem = item.data;
		  dataItem.on("change:history-"+that.period, that.render, that);
		  dataItem.on("change:future", that.render, that);
		  dataItem.on("change:value", that.render, that);
	  });
	  
	  this.when = new Date();
	  
	  console.log("openc view", this.when);
	  
  },
  
  close: function(){
	  var that = this;
	  _.each(this.data, function(item){
		  var dataItem = item.data;
		  dataItem.off("change:history-"+that.period, that.render, that);
		  dataItem.off("change:future", that.render, that);
		  dataItem.off("change:value", that.render, that);
	  });
	  
	  console.log("close view", this.when);
	 
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
  beforeRender: function(){
	  console.log("before render", this.when);
	  
  },
  
  afterRender: function(){
	  console.log("after render", this.when);
	  this.updateChart();
  },
  updateChart: function(){
	  
	  //-------------
      //- LINE CHART -
      //--------------
	  var alldata = [];
	  var allgraphs = [];
	  
	  var that = this;
	  
	  _.each(this.data, function(item){
	    		
			var n = item.node;
			var d = item.data;
				
			var isForecast = d.get("type") == "Forecast";
			
			if (isForecast){
				  //append forecast
				  
				  var hist = d.get("future");

				  if (hist){
					
				  
					  console.log("graph future", hist);
					
					  var kk = n.get("name") + "-" + n.get("place") + "-" + d.get("type");
					  var kkImage = n.get("name") + "-" + n.get("place") + "-" + d.get("type") + "-image";
					  var kkWind = n.get("name") + "-" + n.get("place") + "-" + d.get("type") + "-wind";
					  var kkPerc = n.get("name") + "-" + n.get("place") + "-" + d.get("type") + "-perc";
					  
					  var read = n.get("name") + "@" + n.get("place");
					
					  var baltext = "<b>[[rdate]]</b><br/>[[title]]<br/><b>[[" + kk + "]]</b>&deg;C<br /> Wind: [[" + kkWind + "]]</b>m/s, Perc: [[" + kkPerc + "]]</b>mm";
		              
					  allgraphs.push({
	                          type: "smoothedLine",
	                          title: read + " Temp",
	                          valueField: kk,
							  lineThickness: 3,
	                          fillAlphas: 0.1,
	                          customBulletField: kkImage,
	                          bulletOffset: 5,
	                          bulletSize: 24,
	                          balloonText: baltext,
	                          lineColor: "#EC3C2D",
	                          negativeLineColor: "#4FD3FD",
	                  });
					  allgraphs.push({
	                      type: "column",
	                      title: read + " Perc",
	                      valueField: kkPerc,  
	                      fillColors: "#8888ff",
	                      fillAlphas: 0.8,
	                      lineAlpha: 0,
	                      fixedColumnWidth: 15
	                  });
					  allgraphs.push({
	                      type: "column",
	                      title: read + " Wind",
	                      valueField: kkWind,   
	                      fillColors: "#ffffaa",
	                      fillAlphas: 0.8,
	                      lineAlpha: 0,
	                  });
						
					  var values = [];
					    
					  _.each(hist, function(item){
						  var where = null;
						  _.each(alldata, function(gi){
							  if (gi.date == item.date){
								  where = gi;
							  }
						  });
						  
						  var ddd = new Date(item.date);
						  var hour = ddd.getHours();
						  var image = "images/weather/" + (hour >= 6 && hour <= 20 ? "day" : "night") + "/" + item.image + ".png"; 
				          
						  
						  if (where){
							  where[kk] = item.value;
							  where[kkImage] = image;
							  where[kkWind] = item.wind;
							  where[kkPerc] = item.precipitation;
						  } else {
							  var dd = new Date(item.date);
							  var d = {
									  'date': item.date,
									  'rdate': dayNamesShort[ddd.getDay()] + " " + hour + ":00"
						      }
							  d[kk] = item.value;
							  d[kkImage] = image;
							  d[kkWind] = item.wind;
							  d[kkPerc] = item.precipitation;
							  alldata.push(d);
						  }
					  });
				  }
		
			  } else {
				  
				  var kk = n.get("name") + "-" + n.get("place") + "-" + d.get("type");
				  var read = n.get("name") + "@" + n.get("place");
				
				  var g = {
						  type: "line",
						  title: read,
						  valueField: kk,
						  lineThickness: 2,
						  fillAlphas: 0,
						  bullet: "round",
						  balloonText: "<b>[[title]]</b><br />" + d.get("type") + "<br />[[rdate]]<br /><big><b>[[value]]</b></big>" + d.get("unit"),
						  balloonDateFormat: "JJ:NN",
				  }
				  allgraphs.push(g);
					
				  var values = [];
				  
				  var hist = d.get("history-" + that.period);
				  
				  if (hist){
					  var current = d.get("value"); 
					  if (current){
						  hist.push(current);
					  }
					  
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
							  var dd = new Date(item.date);
							  var d = {
									  'date': item.date,
									  'rdate': AmCharts.formatDate(dd, "JJ:NN")
						      }
							  d[kk] = item.value;
							  alldata.push(d);
						  }
					  });
					  
				  }
			  }
			 
	    });
	  
	   //data ready now draw a chart
	// 
	  alldata.sort(compareGraphData);
	  console.log("all Data", alldata, "when", this.when);
	  //console.log("graphs", allgraphs, this.when);
	  var chart1 = AmCharts.makeChart("dataChart", {
          type: "serial",
          theme: "black",
          dataProvider: alldata,
          categoryField: "date",
          startDuration: 0,
          fontFamily: "'Source Sans Pro', 'Helvetica Neue', Helvetica, Arial, sans-serif",
          //dataDateFormat: "HH:mm DD.MM.YYYY",

          categoryAxis: {
              gridPosition: "start",
              parseDates: true,
              minPeriod: "mm",
              dateFormats: [{
                  period: 'fff',
                  format: 'JJ:NN:SS'
              }, {
                  period: 'ss',
                  format: 'JJ:NN:SS'
              }, {
                  period: 'mm',
                  format: 'JJ:NN'
              }, {
                  period: 'hh',
                  format: 'JJ:NN'
              }, {
                  period: 'DD',
                  format: 'DD'
              }, {
                  period: 'WW',
                  format: 'DD'
              }, {
                  period: 'MM',
                  format: 'MMM'
              }, {
                  period: 'YYYY',
                  format: 'YYYY'
              }]
          },
          
          valueAxes: [{
              title: "Value"
          }],
          
          graphs: allgraphs,
          
          legend: {
              useGraphSettings: true
          },
          
          balloon: {
        	    "adjustBorderColor": true,
        	    "color": "#000000",
        	    "cornerRadius": 5,
        	    "fillColor": "#FFFFFF"
          }
          /*
          ballon: new {
        	  fillColor: "#999",
        	  color: "white",
        	  textAlign: "left",
        	  fontSize: 15
          }*/
      });
	  
      //TODO
  }

});
