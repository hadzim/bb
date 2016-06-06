// Create a new Layout.
BB.LayoutView = Backbone.Layout.extend({
  // Attach the Layout to the main container.
  el: "#layouta",

  // Use the previous defined template.
  template: "#layout-template",

  lastPageView: null,
  // Declaratively bind a nested View to the Layout.
  initialize: function(){
	  
	  
	  this.insertViews({
         "#topmenu": new BB.LayoutTopMenuView(),
         "#sidemenu": new BB.LayoutSideMenuView(),
     	 "#headline": new BB.LayoutHeadlineView(),
      });
	  
	  //console.log("mqtt connect");
	  BB.mqtt.connect();
	  //console.log("mqtt connected");
	  
   },
   
   showView: function(view) {
	    
	   	console.log("SHOW", view);
	    if (this.lastPageView){
	    	console.log("remove old", view);
	    	this.removeView("#content");
	    	this.lastPageView.close();
	    	this.lastPageView = null;
	    
	    } 
	    
	    $("body").removeClass("full");
	   	
	   	this.getView("#headline").headline(view.title, view.subtitle);
     	this.insertView("#content", view).render();
     	this.lastPageView = view;
     	
     	console.log("SHOW DONE", view);
   },
   
   showFull: function(view) {
	    
	   	console.log("SHOW full", view);
	    if (this.lastPageView){
	    	console.log("remove old", view);
	    	this.removeView("#content");
	    	this.lastPageView.close();
	    	this.lastPageView = null;
	    } 
	   	
	    $("body").addClass("full");
	    
	   	//this.getView("#headline").headline(view.title, view.subtitle);
    	this.insertView("#content", view).render();
    	this.lastPageView = view;
    	
    	console.log("SHOW full DONE", view);
  },
   
});