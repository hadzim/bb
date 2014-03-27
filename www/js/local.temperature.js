var temperatureSensors = {
    "Inside": {
      name: "uvnitř",
      css: "bigtemp", //btn-warning
      temp: undefined
    },
    "Outside": {
      name: "venku",
      css: "bigtemp", //btn-info
      temp: undefined
    },
    "Hall": {
      name: "chodba",
      css: "normaltemp",
      temp: undefined
    },
    "Heating": {
      name: "kotel",
      css: "normaltemp btn-danger",
      temp: undefined
    }
}
  
function renderTemperature(key){
     var content = "<button rel='temper' data-place='" + key + "' class='btn btn-default " + temperatureSensors[key].css + "'>"; 
     content += temperatureSensors[key].name;
     content += ": ";
     content += (temperatureSensors[key].temp > 0 ? "<span class='temp temp-above'>" : "<span class='temp temp-under'>");
     content += (temperatureSensors[key].temp == undefined) ? "?" : temperatureSensors[key].temp | 0;
     content += "</span>&deg;C";
     content += "</button>";
    return content;     
}
  
function renderTemperatures(){
    var c = "";
    $.each(temperatureSensors, function( findex, fvalue ) {
      c += renderTemperature(findex);
    });
    $("#temperature").html(c);
}

function updateTemperatures(){
      
      console.log("Get temperature: ");
   
      var si = tbsService.GetSensorsData("Temperature");
      si.done(function( data, tempTextStatus, forecastJqXHR ) {
        console.log(data);
        $.each(data.ReturnObject, function( findex, fvalue ) {
              
            if(temperatureSensors.hasOwnProperty(fvalue.sensorName)){
               temperatureSensors[fvalue.sensorName].temp = fvalue.value;
            }
        }); 
        
        renderTemperatures();
        console.log("Get temperatures: ", " done");     
      });
   
}

  
  
  var temperatureRefresh = 0;
  
  function showTemperatureDetail(place){
    
    //prepare data
       
       var sidata = tbsService.GetSensorData("Temperature", place);
        sidata.done(function( data, tempTextStatus, forecastJqXHR ) {
          
          var labels = new Array();
          var values = new Array();
          var zerovalues = new Array();
          
          var len = data.ReturnObject.length;
          var cnt = 0;
          $.each(data.ReturnObject, function( findex, fvalue ) {
              values.push(fvalue.value);   
              zerovalues.push(0);         
              if (len < 10 || (cnt++) % 3 == 0){
                labels.push(readableTime(new Date(fvalue.date)));
              } else {
                labels.push("");
              }
          }); 
          
                //Get context with jQuery - using jQuery's .get() method.
            var ctx = $("#temperatureChart").get(0).getContext("2d");
            //This will get the first returned node in the jQuery collection.
            var myNewChart = new Chart(ctx);
        
            var data = {
              labels: labels,
            	datasets : [
            		{
                  fillColor : "rgba(200,200,200,0.0)",
            			strokeColor : "rgba(200,220,220,1)",
            			pointColor : "rgba(200,255,255,1)",
            			pointStrokeColor : "#aaf",
            			data : zerovalues
            		},
                {
            			fillColor : "rgba(200,200,200,0.3)",
            			strokeColor : "rgba(220,220,220,1)",
            			pointColor : "rgba(255,200,200,1)",
            			pointStrokeColor : "#fff",
            			data : values
            		},
                
            	]
            }
            var opts = {
              	animation : false,  
                scaleFontColor : "#eee",
                pointDotRadius : 2,
                datasetStrokeWidth : 1,
                scaleGridLineColor : "rgba(255,255,255,.2)",	
            };
            new Chart(ctx).Line(data, opts);   
        });
  
    
    $("#temperatureModal").modal();
  }
  