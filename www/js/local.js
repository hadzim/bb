  var temperatureRefresh = 1000;
  var forecastRefresh = 1000;
    
  function updateTime(){
       var dn = new Date();
       $("#datetime").html(readableTime(dn));
       $("#dateday").html(dayNamesLong[dn.getDay()]);
       $("#datedate").html(dn.getDate().toString() + ". " + monthNames[dn.getMonth()]);
       
       if (temperatureRefresh++ > 60){
          temperatureRefresh = 0;
          updateTemperatures();
       }
       
       if (forecastRefresh++ > 60 * 5){
          forecastRefresh = 0;
          updateDefaultForecast();
       }
  }
  
  $( document ).ready(function() {
      	//tbsService = new Query({url: "http://127.0.0.1:8111/"});
        tbsService = new Query({url: Settings.url});
        
        updateTime();
        window.setInterval(updateTime,1000);
       
        $("button[rel='fcb']").click(function(){
          console.log("clicked");
              $("button[rel='fcb']").removeClass("bg-blue");
              $(this).addClass("bg-blue");
              //updateForecast($('#forecasts'), $(this).attr("data-place"));
              activeForecast = $(this).attr("data-place");
              updateDefaultForecast();
        });
        
        $("#temperature").on("click", "div", function(e){
              showTemperatureDetail($(this).attr("data-place"));
        });
        
        $("#forecastTemperature").on("click", "div[rel='fdetail']", function(e){
              showForecastDetail($(this).attr("data-place"));
        });
        
        
        $('button.fcoverview').click(function(){
            $('#mymodal').modal();
            //$('#box').modal("ahoj");
        });
        

	});
