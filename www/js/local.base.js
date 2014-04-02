
var tbsService;

var dayNamesShort = new Array("Ne", "Po", "Út", "St", "Čt", "Pá", "So");
var dayNamesLong = new Array("Neděle", "Pondělí", "Úterý", "Středa", "Čtvrtek", "Pátek", "Sobota");
var monthNames = new Array("Leden", "Únor", "Březen", "Duben", "Květen", "Červen", "Červenec", "Srpen", "Září", "Říjen", "Listopad", "Prosinec");

                       
function daydiff(first, second) {
    return (second-first)/(1000*60*60*24);
}
  
function parseDate(input) {
  var allparts = input.substring(0, input.length - 1).split('T');
  var dateParts = allparts[0].split('-');
  var timeParts = allparts[1].split(':');
  return new Date(dateParts[0], dateParts[1]-1, dateParts[2], timeParts[0], timeParts[1], timeParts[2]);
}  
  
  
  function pad(num) {
    return ("0"+num).slice(-2);
  }
  
  function readableTime(dn){
      return pad(dn.getHours())+":"+pad(dn.getMinutes());
  }
