/*
var temperatureSensors = {
    "Outside": {
      name: "Venku",
      css: "temperature-big bg-dark", //btn-info
      temp: undefined
    },
    "Inside": {
      name: "Uvnitř",
      css: "temperature-big bg-dark", //btn-warning
      temp: undefined
    }/*,
    "Hall": {
      name: "chodba",
      css: "normaltemp",
      temp: undefined
    },
    "Heating": {
      name: "kotel",
      css: "normaltemp btn-danger",
      temp: undefined
    }  */ 
/*}*/
  
/*  
function renderTemperature(key){
     var content = "<div rel='temper' data-place='" + key + "' class='temperature " + temperatureSensors[key].css + "'>";
     content += "<p>"; 
     content += temperatureSensors[key].name;
     content += "</p>";
     content += "<h2>";
     content += (temperatureSensors[key].temp > 0 ? "<span class='temp temp-above'>" : "<span class='temp temp-under'>");
     content += (temperatureSensors[key].temp == undefined) ? "?" : temperatureSensors[key].temp | 0;
     content += "&deg;</span>";
     content += "</h2>";
     content += "</div>";
    return content;     
}
  
function renderTemperatures(){
    var c = "";
    $.each(temperatureSensors, function( findex, fvalue ) {
      c += renderTemperature(findex);
    });
    $("#temperature").html(c);
} */

function updateStatus(){
      var isOk = false;
      var si = tbsService.GetRuntimeStatus();
      si.done(function( data, tempTextStatus, forecastJqXHR ) {
        isOk = true;
        console.log("status", data);
        /*$.each(data.ReturnObject, function( findex, fvalue ) {
              
            if(temperatureSensors.hasOwnProperty(fvalue.sensorName)){
               temperatureSensors[fvalue.sensorName].temp = fvalue.value;
            }
        }); 
          */
        //renderTemperatures();
        //$("#noTemperature").hide();
        //setIconStatus("info-connection", true);
        
      });
}

   /*
  function showTemperatureDetail(place){
    
    $.Dialog({
            overlay: true,
            shadow: true,
            flat: true,
            width: 640,
            title: 'Teplota',
            content: '',
            onShow: function(_dialog){
                var win = _dialog.children('.caption');
                var content = _dialog.children('.content');
                content.html("<div id='detailTemperature' style='width: 610px; height: 300px;'>loading...</div>");
                win.html("Teplota: " + place);
                //win.addClass("bg-dark");
                //win.addClass("fg-white");
                //content.addClass("bg-steel");
            }
          });
    
    //prepare data
    var sidata = tbsService.GetSensorData("Temperature", place);
    sidata.done(function( data, tempTextStatus, forecastJqXHR ) {
          
          var tempData = new Array();
          $.each(data.ReturnObject, function( findex, fvalue ) {
              var temp = {
                  temperature: fvalue.value,
                  date: parseDate(fvalue.date),
                  baseline: 0
              }
              tempData.push(temp);   
          }); 
    
          var chart = createTemperatureChart("detailTemperature", tempData, place);
    
          
      });
  
  }
  */