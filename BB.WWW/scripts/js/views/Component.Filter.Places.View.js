BB.ComponentFilterPlacesView = Backbone.Layout.extend({
  template: "#component-filter-places-template",
  
  className: "form-group",
  
  initialize: function(){
	  
	  this.model = BB.filter;
	  this.model.on('change:places', this.render, this);
	  
	  this.placecollection = BB.places;
      this.placecollection.on('add', this.render, this);
      this.placecollection.on('remove', this.render, this);
  },
  
  afterRender: function(){
	  //$("#placeFilter").select2(); 
	  $("#placeFilter").multiselect({
			  includeSelectAllOption: true,
			  nonSelectedText: 'Choose a place'
	  });
	  $("#placeFilter").on("change", this.filterChanged);
  },
  
  serialize: function() {
	  var data = this.model ? _.clone(this.model.attributes) : {};
	  data["allplaces"] = this.placecollection;
	  return data;
  },
  
  filterChanged: function(name, evt) {
	  BB.filter.setPlaces($("#placeFilter").val());
	  console.log("value", $("#placeFilter").val(), " model ", BB.filter);
	  
  }


});
