


// Create a new Layout.
BB.LayoutView = Backbone.Layout.extend({
  // Attach the Layout to the main container.
  el: "#layout",

  // Use the previous defined template.
  template: "#layout-template",

  // Declaratively bind a nested View to the Layout.
  initialize: function(){
	  
	  
	  this.insertViews({
         "#topmenu": new BB.LayoutTopMenuView(),
         "#sidemenu": new BB.LayoutSideMenuView(),
     	 "#headline": new BB.LayoutHeadlineView(),
      });
	  
	  console.log("mqtt connect");
	  BB.mqtt.connect();
	  console.log("mqtt connected");
	  
   },
   
   showView: function(view) {
	    
	   	this.removeView("#content");
	   	this.getView("#headline").headline(view.title, view.subtitle);
     	this.insertView("#content", view).render();
   },
   
});