/**
 * THIS FILE IS GENERATED BY genTool, DO NOT CHANGE IT!!!!!
 */

function Query(params){
  var settings = {
      url: params.url + "TBS.BB.WebUI.Query",
      username: params.username,
      password: params.password,
  };
  
  
 //methods 
          this.GetSensors = function(){
		var jdata;
    	$.jsonrpcTBS({
    		url: settings.url,
    		username: settings.username,
    		password: settings.password,
    		method: "GetSensors",
    		params: {},
		onRequest: function (r) {
			//console.log(r);
		},
		onResponse: function (r) {
			//console.log(r);
		},
    		success: function (response) {
    		  jdata = response; //$.parseJSON(response);
		      //return jdata;
    		},
    		fault: function (response, errordata) {
				throw('Failed: ' + response);
    		},
    		error: function (request, status, error) {
    			throw('Failed: ' + error + ')');
    		}
    	});
		
		return jdata; 
        }
        this.GetSensorData = function(_sensorType, 
_sensorName){
		var jdata;
    	$.jsonrpcTBS({
    		url: settings.url,
    		username: settings.username,
    		password: settings.password,
    		method: "GetSensorData",
    		params: {sensorType: _sensorType, 
sensorName: _sensorName},
		onRequest: function (r) {
			//console.log(r);
		},
		onResponse: function (r) {
			//console.log(r);
		},
    		success: function (response) {
    		  jdata = response; //$.parseJSON(response);
		      //return jdata;
    		},
    		fault: function (response, errordata) {
				throw('Failed: ' + response);
    		},
    		error: function (request, status, error) {
    			throw('Failed: ' + error + ')');
    		}
    	});
		
		return jdata; 
        }
        this.ClearSensorData = function(_sensorType, 
_sensorName){
		var jdata;
    	$.jsonrpcTBS({
    		url: settings.url,
    		username: settings.username,
    		password: settings.password,
    		method: "ClearSensorData",
    		params: {sensorType: _sensorType, 
sensorName: _sensorName},
		onRequest: function (r) {
			//console.log(r);
		},
		onResponse: function (r) {
			//console.log(r);
		},
    		success: function (response) {
    		  jdata = response; //$.parseJSON(response);
		      //return jdata;
    		},
    		fault: function (response, errordata) {
				throw('Failed: ' + response);
    		},
    		error: function (request, status, error) {
    			throw('Failed: ' + error + ')');
    		}
    	});
		
		return jdata; 
        }
        this.GetSensorsData = function(_sensorType){
		var jdata;
    	$.jsonrpcTBS({
    		url: settings.url,
    		username: settings.username,
    		password: settings.password,
    		method: "GetSensorsData",
    		params: {sensorType: _sensorType},
		onRequest: function (r) {
			//console.log(r);
		},
		onResponse: function (r) {
			//console.log(r);
		},
    		success: function (response) {
    		  jdata = response; //$.parseJSON(response);
		      //return jdata;
    		},
    		fault: function (response, errordata) {
				throw('Failed: ' + response);
    		},
    		error: function (request, status, error) {
    			throw('Failed: ' + error + ')');
    		}
    	});
		
		return jdata; 
        }
        this.GetRuntimeStatus = function(){
		var jdata;
    	$.jsonrpcTBS({
    		url: settings.url,
    		username: settings.username,
    		password: settings.password,
    		method: "GetRuntimeStatus",
    		params: {},
		onRequest: function (r) {
			//console.log(r);
		},
		onResponse: function (r) {
			//console.log(r);
		},
    		success: function (response) {
    		  jdata = response; //$.parseJSON(response);
		      //return jdata;
    		},
    		fault: function (response, errordata) {
				throw('Failed: ' + response);
    		},
    		error: function (request, status, error) {
    			throw('Failed: ' + error + ')');
    		}
    	});
		
		return jdata; 
        }

}

function Configuration(params){
  var settings = {
      url: params.url + "TBS.BB.WebUI.Configuration",
      username: params.username,
      password: params.password,
  };
  
  
 //methods 
          this.GetSensorProperties = function(_sensorType, 
_sensorRawName){
		var jdata;
    	$.jsonrpcTBS({
    		url: settings.url,
    		username: settings.username,
    		password: settings.password,
    		method: "GetSensorProperties",
    		params: {sensorType: _sensorType, 
sensorRawName: _sensorRawName},
		onRequest: function (r) {
			//console.log(r);
		},
		onResponse: function (r) {
			//console.log(r);
		},
    		success: function (response) {
    		  jdata = response; //$.parseJSON(response);
		      //return jdata;
    		},
    		fault: function (response, errordata) {
				throw('Failed: ' + response);
    		},
    		error: function (request, status, error) {
    			throw('Failed: ' + error + ')');
    		}
    	});
		
		return jdata; 
        }
        this.SetSensorProperty = function(_sensorType, 
_sensorRawName, 
_sensorPropertyName, 
_sensorPropertyValue){
		var jdata;
    	$.jsonrpcTBS({
    		url: settings.url,
    		username: settings.username,
    		password: settings.password,
    		method: "SetSensorProperty",
    		params: {sensorType: _sensorType, 
sensorRawName: _sensorRawName, 
sensorPropertyName: _sensorPropertyName, 
sensorPropertyValue: _sensorPropertyValue},
		onRequest: function (r) {
			//console.log(r);
		},
		onResponse: function (r) {
			//console.log(r);
		},
    		success: function (response) {
    		  jdata = response; //$.parseJSON(response);
		      //return jdata;
    		},
    		fault: function (response, errordata) {
				throw('Failed: ' + response);
    		},
    		error: function (request, status, error) {
    			throw('Failed: ' + error + ')');
    		}
    	});
		
		return jdata; 
        }
        this.SetRuntimeStatus = function(_status){
		var jdata;
    	$.jsonrpcTBS({
    		url: settings.url,
    		username: settings.username,
    		password: settings.password,
    		method: "SetRuntimeStatus",
    		params: {status: _status},
		onRequest: function (r) {
			//console.log(r);
		},
		onResponse: function (r) {
			//console.log(r);
		},
    		success: function (response) {
    		  jdata = response; //$.parseJSON(response);
		      //return jdata;
    		},
    		fault: function (response, errordata) {
				throw('Failed: ' + response);
    		},
    		error: function (request, status, error) {
    			throw('Failed: ' + error + ')');
    		}
    	});
		
		return jdata; 
        }
        this.SendTask = function(_what, 
_params, 
_from, 
_to){
		var jdata;
    	$.jsonrpcTBS({
    		url: settings.url,
    		username: settings.username,
    		password: settings.password,
    		method: "SendTask",
    		params: {what: _what, 
params: _params, 
from: _from, 
to: _to},
		onRequest: function (r) {
			//console.log(r);
		},
		onResponse: function (r) {
			//console.log(r);
		},
    		success: function (response) {
    		  jdata = response; //$.parseJSON(response);
		      //return jdata;
    		},
    		fault: function (response, errordata) {
				throw('Failed: ' + response);
    		},
    		error: function (request, status, error) {
    			throw('Failed: ' + error + ')');
    		}
    	});
		
		return jdata; 
        }

}



