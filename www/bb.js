var express = require('express');

var app = express();
app.use('/', express.static(__dirname + '/public'));
app.listen(81, function() { console.log('listening')});
