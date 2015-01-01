var sensorsOverview = {
     
}
  
function renderPage(pageData){
       var content = "<table class='table striped bordered hovered sensor-overview'>";
     
     content += "<tr>";
        content += "<th>Name</td>";
        content += "<th>Raw Name</td>";
        content += "<th>Unit</td>";
        content += "<th class='cfg'><span class='icon-wrench'></span></td>";
        content += "</tr>";
     
     $.each(pageData, function( findex, fvalue ) {
        content += "<tr>";
        content += "<td>" + fvalue.sensorName + "</td>";
        content += "<td>" + fvalue.sensorRawName + "</td>";
        content += "<td>" + fvalue.sensorType + "</td>";
        content += "<td class='cfg'>";
        content += "<button class='small info configure-sensor' data-rawname='" + fvalue.sensorRawName + "' data-name='" + fvalue.sensorName + "' data-type='" + fvalue.sensorType + "'>Configure</button>";
        content += " <button class='small warning clear-sensor' data-rawname='" + fvalue.sensorRawName + "' data-name='" + fvalue.sensorName + "' data-type='" + fvalue.sensorType + "'>Clear</button>";
        content += "</td>";
        content += "</tr>";
     });
     
     content += "</table>";
     return content;
}  
  
function renderOverview(){   
    var body = "";
    var tabs = "";

     var step = 5;
     var i = 0;
     for (i = 0; i < sensorsOverview.length; i+= step){
         var pageData = [];
         var from = i;
         var too = i + step;
         var j = 0;
         if (too > sensorsOverview.length){
            too = sensorsOverview.length;
         }

         for (j = from; j < too; j++){
            
             pageData.push(sensorsOverview[j]);
         }
         
         body += "<div class='frame' id='tab_sensorpage_" + i + "'>" + renderPage(pageData) + "</div>";
         tabs += "<li><a href='#tab_sensorpage_" + i + "'>" + ((i + step) / step) + "</a></li>";
     }
     
     var content = "<div class='tab-control' data-role='tab-control'>";
     content += "<ul class='tabs'>" + tabs + "</ul>";
     content += "<div class='frames'>" + body + "</div>";
     content += "</div>";
    return content;     
}


function renderConfigure(sensorType, rawSensorName, sensorName){
    var content = "";
    content += "<div class='likeform'>";
    content += "<legend>" + rawSensorName + " (" + sensorType + ")</legend>";
    content += "<label>Name</label>   ";
    content += "<div class='input-control text' data-role='input-control'>";
    content += "    <input type='text' value='" + sensorName + "' id='new-sensor-name'>";
    content += "</div>";
    content += "<button class='info' id='new-sensor-submit'>Submit</button> <button id='new-sensor-cancel'>Cancel</button>";
    content += "</div>";
     
    return content;
}  
  
  
function refreshOverview(){
    var c = renderOverview();
    $("#all-sensors").html(c);
    $.Metro.initTabs();
    $(".configure-sensor").click(function(){
        var rname = $(this).attr("data-rawname");
        var name = $(this).attr("data-name");
        var type = $(this).attr("data-type");
        
        $("#all-sensors").html(renderConfigure(type, rname, name));
        
        $("#new-sensor-submit").click(function(){
              var newname = $("#new-sensor-name").val();
              
              var si = tbsConfiguration.SetSensorProperty(type, rname, 'Name', newname);
              si.done(function( data, tempTextStatus, forecastJqXHR ) {
                  updateSensors();  
              });
        });    
        
        $("#new-sensor-cancel").click(function(){
           refreshOverview();
        });    
        
            
    });
    
    $(".clear-sensor").click(function(){
        var rname = $(this).attr("data-rawname");
        var name = $(this).attr("data-name");
        var type = $(this).attr("data-type");
        
        var si = tbsService.ClearSensorData(type, name);
        si.done(function( data, tempTextStatus, forecastJqXHR ) {
            updateSensors();  
        });
    });
}

function updateSensors(){
      var isOk = false;
      var si = tbsService.GetSensors();
      si.done(function( data, tempTextStatus, forecastJqXHR ) {
        isOk = true;
        
        sensorsOverview = data.ReturnObject;
        
        refreshOverview();  
      }); 
      
}

  
  function showSensorOverview(){
    
    $.Dialog({
            overlay: true,
            shadow: true,
            flat: true,
            width: 640,
            title: 'Senzory',
            content: '',
            onShow: function(_dialog){
                var win = _dialog.children('.caption');
                var content = _dialog.children('.content');
                content.html("<div id='all-sensors' style='min-width: 610px; min-height: 300px;'>loading...</div>");
                win.html("Sensors:");
            }
          });
    updateSensors();
  }
  