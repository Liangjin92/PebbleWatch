Pebble.addEventListener("appmessage", function(e) {
// while(1) {
//sendToServer(e.payload.hello_msg); 
//   sleep(1000);
// }
//   if (e.payload.hello_msg == "Celsius") {
//     setInterval(function(){sendToServer("Celcius")}, 20000);
//   }
  
//   if (e.payload.hello_msg == "Farenheit") {
//     setInterval(function(){sendToServer("Farenheit")}, 20000);
//   }
  sendToServer(e.payload.hello_msg);
});

function sendToServer(str) {
var req = new XMLHttpRequest();
var message =  str;
var ipAddress = "165.123.211.203"; // Hard coded IP address
var port = "3001"; // Same port specified as argument to server 
var url = "http://" + ipAddress + ":" + port + "/" + message ;
var method = "GET";
var async = true;
req.onload = function(e) {
// see what came back
var msg = "no response";
var response = JSON.parse(req.responseText); 
if (response) {
if (response.name) {
msg = response.name;
}
else msg = "noname"; 
} 
// else if (response == null) {
//   Pebble.sendAppMessage({"1": "server disconnect!"}); 
// }

if (req.readyState == 4 && req.status == 200) {
    Pebble.sendAppMessage({"1": msg});
  } else {
    Pebble.sendAppMessage({"1": "server disconnect!"}); 
}
// sends message back to pebble 
//Pebble.sendAppMessage({ "0": msg });
};
//handle server not connected error
req.onerror = function() {
  Pebble.sendAppMessage({
    "1": "server disconnected!"
  });
};
// while(1){
  req.open(method, url, async); 
  //req.timeout = 10000; // Set timeout to 4 seconds (4000 milliseconds)
  //req.ontimeout = function () { Pebble.sendAppMessage({"1": "Timed out!!!"}); }
  req.send(null);
//   sleep(1000);
//}
}


/*Pebble.addEventListener("appmessage", function(e) {
if (e.payload) {
if (e.payload.hello_msg) {
Pebble.sendAppMessage({ "0": "Recvd: " + e.payload.hello_msg }); }
else Pebble.sendAppMessage({ "0": "nokey" }); }
else Pebble.sendAppMessage({ "0": "nopayload" }); }
);*/