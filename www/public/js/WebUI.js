/**
 * THIS FILE IS GENERATED BY genTool, DO NOT CHANGE IT!!!!!
 */

function Query(params){
  var settings = {
      url: params.url + "TBS.BB.WebUI.Query",
  };
  
  
 //methods 
          this.GetSensors = function(){
			return $.ajax({
    			url: settings.url,
    			jsonp: "callback",
    			dataType: "jsonp",
    			data: {
    				method: "GetSensors"
				}
			});
		};
        this.GetSensorData = function(_sensorType, 
_sensorName){
			return $.ajax({
    			url: settings.url,
    			jsonp: "callback",
    			dataType: "jsonp",
    			data: {
    				method: "GetSensorData",
sensorType: _sensorType, 
sensorName: _sensorName
				}
			});
		};
        this.ClearSensorData = function(_sensorType, 
_sensorName){
			return $.ajax({
    			url: settings.url,
    			jsonp: "callback",
    			dataType: "jsonp",
    			data: {
    				method: "ClearSensorData",
sensorType: _sensorType, 
sensorName: _sensorName
				}
			});
		};
        this.GetSensorsData = function(_sensorType){
			return $.ajax({
    			url: settings.url,
    			jsonp: "callback",
    			dataType: "jsonp",
    			data: {
    				method: "GetSensorsData",
sensorType: _sensorType
				}
			});
		};
        this.GetRuntimeStatus = function(){
			return $.ajax({
    			url: settings.url,
    			jsonp: "callback",
    			dataType: "jsonp",
    			data: {
    				method: "GetRuntimeStatus"
				}
			});
		};

}

function Configuration(params){
  var settings = {
      url: params.url + "TBS.BB.WebUI.Configuration",
  };
  
  
 //methods 
          this.GetSensorProperties = function(_sensorType, 
_sensorRawName){
			return $.ajax({
    			url: settings.url,
    			jsonp: "callback",
    			dataType: "jsonp",
    			data: {
    				method: "GetSensorProperties",
sensorType: _sensorType, 
sensorRawName: _sensorRawName
				}
			});
		};
        this.SetSensorProperty = function(_sensorType, 
_sensorRawName, 
_sensorPropertyName, 
_sensorPropertyValue){
			return $.ajax({
    			url: settings.url,
    			jsonp: "callback",
    			dataType: "jsonp",
    			data: {
    				method: "SetSensorProperty",
sensorType: _sensorType, 
sensorRawName: _sensorRawName, 
sensorPropertyName: _sensorPropertyName, 
sensorPropertyValue: _sensorPropertyValue
				}
			});
		};
        this.SetRuntimeStatus = function(_status){
			return $.ajax({
    			url: settings.url,
    			jsonp: "callback",
    			dataType: "jsonp",
    			data: {
    				method: "SetRuntimeStatus",
status: _status
				}
			});
		};

}


