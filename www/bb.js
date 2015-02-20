var express = require('express');

var app = express();
app.use('/', express.static(__dirname + '/public'));
app.listen(99, function() { console.log('local app running')});
