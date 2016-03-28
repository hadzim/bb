
  BB.Condition = Backbone.Model.extend({
    defaults: function() {return {type: "Condition" };},
    
    initialize: function() {
    	this.nested = [];
    },
    load: function(data){
    	this.set("attr", data);
    }
    
  });
  BB.NodeTagCondition = BB.Condition.extend({
	    defaults: function() {return {type: "NodeTagCondition" };},
	    load: function(data){
	    	this.set("attr", data);
	    }
  });
  BB.OrCondition = BB.Condition.extend({
	    defaults: function() {return {type: "OrCondition" };},
	    load: function(data){
	    	this.set("attr", data);
	    	for (var i = 0, l = data.or.length; i < l; i++){
	    		this.nested.push(CreateCondition(data.or[i]));
	    	}
	    }
  });
  BB.AndCondition = BB.Condition.extend({
	    defaults: function() {return {type: "AndCondition" };},
	    load: function(data){
	    	this.set("attr", data);
	    	for (var i = 0, l = data.and.length; i < l; i++){
	    		this.nested.push(CreateCondition(data.and[i]));
	    	}
	    }
	});
  BB.NotCondition = BB.Condition.extend({
	    defaults: function() {return {type: "NotCondition" };},
	    load: function(data){
	    	this.set("attr", data);
	    	this.nested.push(CreateCondition(data.not));
	    }
	});
  
  function CreateCondition(data){
	  	var a = new BB.Condition;
	  	if (data){
		  	if (data.name == "NodeTagCondition"){
		  		a = new BB.NodeTagCondition;
		  	}
		  	if (data.name == "Or"){
		  		a = new BB.OrCondition;
		  	}
		  	if (data.name == "And"){
		  		a = new BB.AndCondition;
		  	}
		  	if (data.name == "Not"){
		  		a = new BB.NotCondition;
		  	}
		  	a.load(data);
	  	}
	  	
	  	return a;
	}
  
  
  
  
  
  
  
  
  
  
  BB.Action = Backbone.Model.extend({
	    defaults: function() {return {type: "Action", attr: {} };},
	    load: function(data){
	    	this.set("attr", data);
	    },
	    initialize: function() {
	    	this.nested = [];
	    },
	    empty: true
  });
  
  
  BB.PublishAction = BB.Action.extend({
	    defaults: function() {return {type: "PublishAction" };},
	    load: function(data){
	    	this.set("attr", data);
	    },
	    empty: false
  });
  BB.NewNodeSettingsAction = BB.Action.extend({
	    defaults: function() {return {type: "NewNodeSettings" };},
	    load: function(data){
	    	this.set("attr", data);
	    },
	    empty: false
});
BB.MultipleAction = BB.Action.extend({
	    defaults: function() {return {type: "Multiple" };},
	    load: function(data){
	    	this.set("attr", data);
	    	for (var i = 0, l = data.actions.length; i < l; i++){
	    		this.nested.push(CreateAction(data.actions[i]));
	    	}
	    },
	    empty: false 
});
  
function CreateAction(data){
	  	var a = new BB.Action;
	  	if (data){
		  	if (data.name == "PublishAction"){
		  		a = new BB.PublishAction;
		  	}
		  	if (data.name == "Multiple"){
		  		a = new BB.MultipleAction;
		  	}
		  	if (data.name == "NewNodeSettings"){
		  		a = new BB.NewNodeSettingsAction;
		  	}
		  	a.load(data);
	  	}
	  	
	  	return a;
 }


    
  BB.Rule = Backbone.Model.extend({
    defaults: {
    	name: "unknown", 
    }, 
    initialize: function() {
    	this.condition = new BB.Condition;
    	this.action = new BB.Action;
    	this.negativeAction = new BB.Action;
    },
  });
  
  BB.RuleCollection = Backbone.Collection.extend({
	  model: BB.Rule,
	  download: function(){
		  $.get( "rules.json", function( data ) {
			  console.log("RULE DATA", data);
			  for (var i = 0, l = BB.rules.length; i < l; i++){
				  BB.rules.pop();
			  }
			  for (var i = 0, l = data.length; i < l; i++){
				  
		    	var rule = new BB.Rule({id: data[i].name});
			    
		    	rule.set("name", data[i].name);
		    	
		    	rule.action = CreateAction(data[i].action);
		    	rule.negativeAction = CreateAction(data[i].negativeAction);
		    	rule.condition = CreateCondition(data[i].condition);
		    	
		    	BB.rules.add(rule);
			    
		      }
			  
			  console.log("final RULES", BB.rules);
			        
			  
		   });	
	  }
  });
  