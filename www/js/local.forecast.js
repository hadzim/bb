  
  function forecastData(rawData){
      var dn = new Date();
      var d = new Date(rawData.date);
      
      var obj = new Object();
      
      obj.date = dayNamesShort[d.getDay()] + " " + d.getDate().toString() + "." + d.getMonth().toString() + ".";
      obj.hour = d.getHours();
      obj.temperature = rawData.value;
      obj.datediff = daydiff(dn, d);
      //console.log(obj);
      $.each(rawData.textValue.split(";"), function( findex, fvalue ) {
           var name = fvalue.split(":")[0];
           var val = fvalue.split(":")[1];
           
           if (name == "n"){
           
              obj.name = val; 
           }
           if (name == "p"){
              obj.percipation = val; 
           }
           if (name == "w"){
              obj.wind = val; 
           }
           if (name == "i"){
              obj.img = "img/weather/" + (obj.hour > 6 && obj.hour < 20 ? "day" : "night") + "/" + val + ".png"; 
           }
           
      });
      return obj;
  }
  
  function renderSmallForecastWidget(fdata){
      var str = "";
      str += "<button class='btn btn-default fcoverview'>";
      str += "<span class='time'>" + fdata.hour + ":00</span><br />";
      str += "<span class='img'><img src='" + fdata.img + "'></span><br />";
      str += "<span class='temp'>" + fdata.temperature + "&deg;C</span><br />";  
      str += "</button>";
      return str;
  }
  function renderBigForecastWidget(data){
  
  }
  
  
  function updateForecastJson(item, sensorType, sensorName){
     //get DATA
     var sidata = tbsService.GetSensorData(sensorType, sensorName);
     sidata.done(function( data, forecastTextStatus, forecastJqXHR ) { 
          var days = new Object();
          var daysDiff = new Object();
          $.each(data.ReturnObject, function( findex, fvalue ) {
              var fdata = forecastData(fvalue);
              //if (fdata.hour == 7 || fdata.hour == 13 || fdata.hour == 19){
                if (!(fdata.date in days)){
                    days[fdata.date] = new Object();
                    daysDiff[fdata.date] = fdata.datediff;  
                }
                var hh = fdata.hour.toString();
                days[fdata.date][hh] = fdata;
              //}
          }); 
          
          var content = "";
          $.each( days, function( key, hours ) {
             if (daysDiff[key] < 0 || daysDiff[key] > 5) return;
             content += "<div class='btn btn-primary forecast'>";
             content += "<div><b>" + key + "</b></div>";
             var cnt = 0;   
             $.each(hours, function( findex2, fdata ) {
                      if (fdata == undefined) return;
                      if (++cnt != 3 && daysDiff[key] > 1) return;
                      content += renderSmallForecastWidget(fdata); 
            });
            content += "</div>";
        });
          
        item.html(content);
     });
  }
  
  function updateDefaultForecast(){
    updateForecast($('#forecasts'), activeForecast);
  }
  
  var activeForecast = "JosefuvDul";
  
  function updateForecast(item, name){
        var si = tbsService.GetSensors();
                                    
        si.done(function( data, textStatus, jqXHR ) {
            console.log("done sensors");
            console.log(data);
            $.each(data.ReturnObject, function( index, value ) {
                 if (value.sensorType == "ForecastTemperature" && value.sensorName == name){
                    updateForecastJson(item, value.sensorType, value.sensorName);
                }
            });
            
    		    
        });
  }
  