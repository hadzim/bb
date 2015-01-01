            function myDateFormats(){
              return [
                  {period:'fff',format:'JJ:NN:SS'},
                  {period:'ss',format:'JJ:NN:SS'},
                  {period:'mm',format:'JJ:NN'},
                  {period:'hh',format:'JJ:NN'},
                  {period:'DD',format:'D.M.'},
                  {period:'WW',format:'D.M.'},
                  {period:'MM',format:'MMM'},
                  {period:'YYYY',format:'YYYY'}
                ]
            }

            function createWeatherChart(elementId, data, name){
            
                var baltext = "<b>[[readabledate]]</b><br/>[[name]] (<b>[[temperature]]</b>&deg;C)<br /> Vítr: [[wind]]</b>m/s, Srážky: [[rain]]</b>mm";
                 // column chart
                var chart1 = AmCharts.makeChart(elementId, {
                    type: "serial",
                    theme:"dark",
                    dataProvider: data,
                    categoryField: "date",
                    //startDuration: 1,
    
                    categoryAxis: {
                        parseDates: true,
                        minPeriod: "6hh",
                        dateFormats: myDateFormats(),
                    },
                    graphs: [{
                          type: "column",
                          title: "Srážky",
                          valueField: "rain",  
                          fillColors: "#8888ff",
                          fillAlphas: 0.8,
                          lineAlpha: 0,
                         
                          
                        },{
                          type: "column",
                          title: "Vítr",
                          valueField: "wind",   
                          fillColors: "#ffffaa",
                          fillAlphas: 0.8,
                          lineAlpha: 0,
                          
                          
                        },{
                          type: "line",
                          title: "0",
                          valueField: "baseline",   
                          lineThickness: 1,
                          fillAlphas: 0,
                          lineColor: "white",
                        }, {
                          type: "smoothedLine",
                          title: "Teplota",
                          valueField: "temperature",   
                          lineThickness: 3,
                          fillAlphas: 0.1,
                          customBulletField: "image",
                          bulletOffset: 5,
                          bulletSize: 24,
                          balloonText: baltext,
                          lineColor: "#EC3C2D",
                          negativeLineColor: "#4FD3FD",
                    }], legend: {
                        useGraphSettings: true
                    }
                });           
                     
                                 // CURSOR
                var chartCursor = new AmCharts.ChartCursor();
                chartCursor.zoomable = false; // as the chart displayes not too many values, we disabled zooming
                chartCursor.cursorAlpha = 1;
                chartCursor.categoryBalloonDateFormat= "D.M. JJ:NN"
                chartCursor.categoryBalloonAlpha = 0.7
                      chartCursor.valueBalloonsEnabled = false;
                chart1.addChartCursor(chartCursor);
                chart1.write(elementId);
                                                               
                return chart1;
               
            }
            
            
            function createTemperatureChart(elementId, data, name){
            
                // column chart
                var chart1 = AmCharts.makeChart(elementId, {
                    type: "serial",
                    theme:"dark",
                    dataProvider: data,
                    categoryField: "date",
                    //startDuration: 1,
    
                    categoryAxis: {
                        gridPosition: "start",
                        parseDates: true,
                        minPeriod: "30ss",
                        dateFormats: myDateFormats(),
                    },
                    valueAxes: [{
                        title: "°C"
                    }],
                    graphs: [{
                        type: "line",
                        title: name,
                        valueField: "temperature",   
                        lineThickness: 3,
                        fillAlphas: 0.1,
                        bullet: "round",
                        bulletSize: 3,
                        balloonText: "[[title]]:<b>[[value]]</b>&deg;C",
                        lineColor: "#EC3C2D",
                        negativeLineColor: "#4FD3FD",
                    },{
                        type: "line",
                        title: "0",
                        valueField: "baseline",   
                        lineThickness: 1,
                        fillAlphas: 0,
                        lineColor: "white"
                    }],
                    legend: {
                        useGraphSettings: true
                    }
                    
    
                });
                                      
                                // CURSOR
                var chartCursor = new AmCharts.ChartCursor();
                chartCursor.zoomable = false; // as the chart displayes not too many values, we disabled zooming
                chartCursor.cursorAlpha = 1;
                chartCursor.categoryBalloonDateFormat= "D.M. JJ:NN"
                chart1.addChartCursor(chartCursor);
                chart1.write(elementId);
                                        
                  //console.log(chart1);
                return chart1; 
            }
            
           
            