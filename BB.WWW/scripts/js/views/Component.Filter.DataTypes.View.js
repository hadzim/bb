BB.ComponentFilterDataTypesView = Backbone.Layout.extend({
  template: "#component-filter-datatypes-template",
  
  className: "form-group",
  
  initialize: function(){
	  
	  this.model = BB.filter;
	  this.model.on('change:dataTypes', this.render, this);
	  
	  this.dataTypes = BB.dataTypes;
      this.dataTypes.on('add', this.render, this);
      this.dataTypes.on('remove', this.render, this);
  },
  
  afterRender: function(){
	  $("#typeFilter").multiselect({
			  nonSelectedText: 'Choose a type'
	  });
	  $("#typeFilter").on("change", this.filterChanged);
  },
  
  serialize: function() {
	  var data = this.model ? _.clone(this.model.attributes) : {};
	  data["alldatatypes"] = this.dataTypes;
	  return data;
  },
  
  filterChanged: function(name, evt) {
	  BB.filter.setDataTypes($("#typeFilter").val());
	  console.log("value", $("#typeFilter").val(), " model ", BB.filter);
  }


});
