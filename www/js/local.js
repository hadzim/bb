  
  function updateTime(){
       var dn = new Date();
       $("#datetime").html(readableTime(dn));
       $("#dateday").html(dayNamesLong[dn.getDay()]);
       $("#datedate").html(dn.getDate().toString() + ". " + monthNames[dn.getMonth()]);
       
       if (temperatureRefresh++ > 60){
          temperatureRefresh = 0;
          updateTemperatures();
       }
  }
  
  $( document ).ready(function() {
      	//tbsService = new Query({url: "http://127.0.0.1:8111/"});
        tbsService = new Query({url: "http://192.168.137.6:8111/"});
        
        window.setInterval(updateTime,1000);
        updateTime();
        updateDefaultForecast();
      	
        updateTemperatures();
        
        $("button[rel='fcb']").click(function(){
              $("button[rel='fcb']").removeClass("btn-primary");
              $(this).addClass("btn-primary");
              //updateForecast($('#forecasts'), $(this).attr("data-place"));
              activeForecast = $(this).attr("data-place");
              updateDefaultForecast();
        });
        
        $("#temperature").on("click", "button", function(e){
              
              showTemperatureDetail($(this).attr("data-place"));
        });
        
        
        $('button.fcoverview').click(function(){
            $('#mymodal').modal();
            //$('#box').modal("ahoj");
        });
        

	});
