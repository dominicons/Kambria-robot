/* var imported=document.createElement('script');
imported.scr='./mqttws31';
document.head.appendChild(imported); */

var host="broker.hivemq.com";
var port=8000;
var clientID="Ohmnibot";
var client;
var reconnectTimeout=2000;

var topic="/swa/temperature";

function MQTTconnect() {
	client = new Paho.MQTT.Client(
			host,
			port,
            clientID
	);
        var options = {
            timeout: 3,
            onSuccess: onConnect,
            onFailure: function (message) {
                console.log("Error to connect"),
                setTimeout(MQTTconnect, reconnectTimeout);
            }
        };

        client.onConnectionLost = onConnectionLost;
        client.onMessageArrived = onMessageArrived;

        client.connect(options);
    }
function onConnect() {
// Once a connection has been made, make a subscription and send a message.
    console.log("onConnect");
    client.subscribe(topic);
    pub(location);
}
function pub(location){
    if (location!==" ")
    {
        message = new Paho.MQTT.Message(location);
        message.destinationName = "/swa/location";
        client.send(message);
    }
    else
    {
        console.log(" wait")
    }
    
}

function onConnectionLost(responseObject) {
    if (responseObject.errorCode !== 0) {
      console.log("onConnectionLost:"+responseObject.errorMessage);
    }
  }
  
  // called when a message arrives
  function onMessageArrived(message) {
    console.log("onMessageArrived:"+message.payloadString);
    var valueLocation = document.getElementById("locNow");
    valueLocation.innerHTML=message.payloadString;
  }
