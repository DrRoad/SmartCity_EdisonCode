/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */

/*
Node.js application for connecting the Intel Edison Arduino to IBM Bluemix.
Sends data from an analog sensor on analog pin zero.
Requires registration on Bluemix. Edit the following to your Bluemix registration values:
ORG
TYPE
ID
AUTHTOKEN
*/

var ORG = 'mfv4dh';
var TYPE = 'SmartCityPlugin';
var ID = '784b87aa3ee3'; //784b87aa3ee3
var AUTHTOKEN = '45J@D+Ti-2VpKPgB9c';

//Uses mqtt.js, see package.json. More info at: 
//https://www.npmjs.com/package/mqtt
var mqtt    = require('mqtt');

var PROTOCOL = 'mqtt';
var BROKER = ORG + '.messaging.internetofthings.ibmcloud.com';
var PORT = 1883;

//Create the url string
var URL = PROTOCOL + '://' + BROKER;
URL += ':' + PORT; 
//URL is e.g. 'mqtt://xrxlila.messaging.internetofthings.ibmcloud.com:1883'

var CLIENTID= 'd:' + ORG;
CLIENTID += ':' + TYPE;
CLIENTID += ':' + ID;
//CLIENTID -s e.g. d:xrxila:edison-air:784b87a801e9
var count1 = 100;
var count2 = 200;
var count3 = 1;

var AUTHMETHOD = 'use-token-auth';

var client  = mqtt.connect(URL, { clientId: CLIENTID, username: AUTHMETHOD, password: AUTHTOKEN });

var TOPIC = 'iot-2/evt/eid/fmt/json';
console.log(TOPIC);

client.on('connect', function () {
  setInterval(function()
  {
    //client.publish(TOPIC, '{"d":{"Volts":' + analogVolts() + '} }');//Payload is JSON
    //client.publish(TOPIC, '{"d":{"Dal":' + 500 + "," + "Rice:" + 500 + '} }');//Payload is JSON  
    client.publish(TOPIC, '{"d":{"Dal":' + count1 + ', "Rice":' + count2 + '}}');//Payload is JSON   
      console.log("published : Dal =  " + count1 + "Rise= " + count2 ); 
      
  }, 5000);//Keeps publishing every 2000 milliseconds.
});

//Connect to an analog sensor on Edison Arduino pin A0.
//Uses mraa included with Edison image.  More info at: 
//http://iotdk.intel.com/docs/master/mraa/index.html
//Edison Arduino returns drifting values if you have no sensor; you can see
//the "data" on Bluemix if you have no sensor connected on pin A0.
var m = require('mraa'); //require mraa
console.log('MRAA Version: ' + m.getVersion()); //write the mraa version to the console

var fs = require('fs');

var subscriber_pin = new m.Gpio(7);
    subscriber_pin.dir(m.DIR_IN);
    subscriber_pin.isr(m.EDGE_RISING, fileEvent); //Subscribe to interrupt notifications from Arduino
    
function fileEvent() {
    var contents = fs.readFileSync('/arduino_notification_out.txt').toString();
  //  console.log("Message from Arduino:" + contents);  
    
    var arr = contents.split(",");

    console.log("Token  from string " + arr );      

    count1 = arr[0];    
    count2 = arr[1];   
    count3 = arr[2];
        
//   console.log("Message from :" + count1  );  
//    console.log("Message from :" + count2 ); 
 //   console.log("Message from :" + count3 ); 
    
    
    
}


