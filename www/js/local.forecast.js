  
  function forecastData(rawData){
      var dn = new Date();
      var d = parseDate(rawData.date);
      
      var obj = new Object();
      
      obj.date = d; //dayNamesShort[d.getDay()] + " " + d.getDate().toString() + "." + d.getMonth().toString() + ".";
      obj.hour = d.getHours();
      obj.temperature = rawData.value;
      obj.datediff = daydiff(dn, d);
      obj.readabledate = dayNamesShort[d.getDay()] + " " + obj.hour + ":00";
      obj.baseline = 0;
      obj.unique = (Math.floor(obj.hour / 4) * 4).toString() + "." + d.getDate().toString() + "." + d.getMonth().toString();
      //console.log(obj);
      $.each(rawData.textValue.split(";"), function( findex, fvalue ) {
           var name = fvalue.split(":")[0];
           var val = fvalue.split(":")[1];
           
           if (name == "n"){
              obj.name = val; 
           }
           if (name == "p"){
              obj.rain = val; 
           }
           if (name == "w"){
              obj.wind = val; 
           }
           if (name == "i"){
              obj.image = "img/weather/" + (obj.hour >= 6 && obj.hour <= 20 ? "day" : "night") + "/" + val + ".png"; 
           }
           
      });
      return obj;
  }
  
 
  function renderOverviewPartial(name, data, place){
      if (data == null) return "";
      var content = "<div rel='fdetail' data-place='" + place + "' class='bg-dark temperature temperature-big temperature-fc'>";
       content += "<img src='" + data.image + "' >";
                        
       content += "<p>"; 
       content += name;
       content += "</p>";
       content += "<h2>";
       content += (data.temperature > 0 ? "<span class='temp temp-above'>" : "<span class='temp temp-under'>");
       content += (data.temperature == undefined) ? "?" : data.temperature | 0;
       content += "&deg;</span>";
       content += "</h2>";
       content += "</div>";
      return content;     
  }
  function renderOverviewHtml(data, place){
       var content = "";
       content += renderOverviewPartial("Dnes", data.today, place);
       content += renderOverviewPartial("Noc", data.night, place);
       content += renderOverviewPartial("Zítra", data.tommorow, place);
       $("#forecastTemperature").html(content);
  }
  
  function updateForecastOverview(sensorType, sensorName){
     //get DATA
    
    var at = new Date();
    var todayTime = new Date(at.getFullYear(), at.getMonth(), at.getDay(), 12);
    var times = {
      today: todayTime,
      night: new Date(todayTime.getTime() + (12 * 3600 * 1000)),
      tommorow: new Date(todayTime.getTime() + (24 * 3600 * 1000)),
    }
     var dates = {
      today: null,
      night: null,
      tommorow: null,
    }
    
    var datetimes = new Object();
     
     var sidata = tbsService.GetSensorData(sensorType, sensorName);
     sidata.done(function( data, forecastTextStatus, forecastJqXHR ) { 
          
          $.each(data.ReturnObject, function( findex, fvalue ) {
             var fdata = forecastData(fvalue);
            
             $.each(times, function( timesKey, timesValue ) {
                if (datetimes[timesKey] == null){
                  //console.log(timesValue, fdata.date, daydiff(timesValue, fdata.date));
                    if (daydiff(timesValue, fdata.date) >= 0){
                        datetimes[timesKey] = fdata;    
                    }
                }        
             });
                
              
          }); 
         
        //createWeatherChart(item, graphData, sensorName);
         renderOverviewHtml(datetimes, sensorName);
     });
  }
  
  function updateDefaultForecast(){
    updateForecast("forecasts", activeForecast);
  }
  
  var activeForecast = "JosefuvDul";
  
  function updateForecast(item, name){
        try {
        var si = tbsService.GetSensors();
                                    
        si.done(function( data, textStatus, jqXHR ) {
            console.log("done foreacast sensors");
            console.log(data);
            $.each(data.ReturnObject, function( index, value ) {
                 if (value.sensorType == "ForecastTemperature" && value.sensorName == name){
                    //updateForecastJson(item, value.sensorType, value.sensorName);
                    updateForecastOverview(value.sensorType, value.sensorName);
                }
            });
            
    		    $("#noForecast").hide();
        });
        si.fail(function( jqXHR, textStatus, errorThrown ) {
          console.log("failed");
          $("#noForecast").show(); 
      });
      
      } catch (e){
          $("#noForecast").show();
      }
  }
  
  function showForecastDetail(sensorName){
  
        $.Dialog({
            overlay: true,
            shadow: true,
            flat: true,
            width: 660,
            title: 'Předpověď',
            content: '',
            onShow: function(_dialog){
                var win = _dialog.children('.caption');
                var content = _dialog.children('.content');
                content.html("<div id='detailFc' style='width: 630px; height: 330px;'>loading...</div>");
                //win.addClass("bg-dark");
                //win.addClass("fg-white");
                //content.addClass("bg-steel");
            }
          });
  
          var sidata = tbsService.GetSensorData("ForecastTemperature", sensorName);
              sidata.done(function( data, forecastTextStatus, forecastJqXHR ) { 
              var graphData = new Array();
              var last = "";
              $.each(data.ReturnObject, function( findex, fvalue ) {
                  try {
                    var fdata = forecastData(fvalue);
                    
                    if (fdata.datediff < 0){
                       return;
                    }
                    if (fdata.datediff > 6.0){
                       return;
                    }
                    
                    
                    if (last == fdata.unique){
                        return;    
                    }
                    
                    graphData.push(fdata);
                    last = fdata.unique;
                    
                  } catch (e){
                  }
                  
                  
              }); 
             
            createWeatherChart("detailFc", graphData, sensorName);
             
         });
  }
  