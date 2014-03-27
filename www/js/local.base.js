
var tbsService;

var dayNamesShort = new Array("Ne", "Po", "Út", "St", "Čt", "Pá", "So");
var dayNamesLong = new Array("Neděle", "Pondělí", "Úterý", "Středa", "Čtvrtek", "Pátek", "Sobota");
var monthNames = new Array("Leden", "Únor", "Březen", "Duben", "Květen", "Červen", "Červenec", "Srpen", "Září", "Říjen", "Listopad", "Prosinec");

                       
function daydiff(first, second) {
    return (second-first)/(1000*60*60*24);
}
  
  
  
  
  function pad(num) {
    return ("0"+num).slice(-2);
  }
  
  function readableTime(dn){
      return pad(dn.getHours())+":"+pad(dn.getMinutes());
  }
