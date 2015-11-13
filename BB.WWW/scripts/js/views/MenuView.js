BB.MenuView = Backbone.View.extend({

	template: $("#menu-template").html(),
    className: "menu-view", 
    container: $('#menu'),
	
    initialize: function () {
        //this.searchResults = new directory.EmployeeCollection();
        //this.searchresultsView = new directory.EmployeeListView({model: this.searchResults, className: 'dropdown-menu'});
    	this.render();
    },

    render: function(){
    	var tmpl = _.template(this.template);
        this.container.html(tmpl);
    	//this.container.html(this.template);
    },
/*
    events: {
        "keyup .search-query": "search",
        "keypress .search-query": "onkeypress"
    },
*/
    search: function (event) {
        var key = $('#searchText').val();
        this.searchResults.fetch({reset: true, data: {name: key}});
        var self = this;
        setTimeout(function () {
            $('.dropdown').addClass('open');
        });
    },

    onkeypress: function (event) {
        if (event.keyCode === 13) { // enter key pressed
            event.preventDefault();
        }
    },

    selectMenuItem: function(menuItem) {
        $('.navbar .nav li').removeClass('active');
        if (menuItem) {
            $('.' + menuItem).addClass('active');
        }
    }

});


