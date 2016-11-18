var Clay = require('pebble-clay');
var clayConfig = require('./config');
var clay = new Clay(clayConfig, null, { autoHandleEvents: false });
var btcAddress = "1Mo2JwCR6d5NGKqTRasiyfCVnGorHfGEtC";
Pebble.addEventListener('showConfiguration', function(e) {

  // This is an example of how you might load a different config based on platform.
  var platform = clay.meta.activeWatchInfo.platform ;
  Pebble.openURL(clay.generateUrl());
});


Pebble.addEventListener('webviewclosed', function(e) {
  if (e && !e.response) {
    return;
  }

  // Get the keys and values from each config item
  var dict = clay.getSettings(e.response);
  var messageKeys = require('message_keys');
  console.log("keys"+messageKeys.KEY_BTC_ADDRESS);
  btcAddress = dict[messageKeys.KEY_BTC_ADDRESS];
  console.log("Settings closed, got address"+dict[messageKeys.KEY_BTC_ADDRESS]);
  fetchAddress(0);

  
});

function sendBalance(message)
{
  if(message == 'undefined')
    message = "Error"
        // Assemble dictionary using our keys
        var dictionary = {
        "KEY_FINAL_BALANCE": message
      };

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log("Info sent to Pebble successfully!");
        },
        function(e) {
          console.log("Error sending info to Pebble!");
        }
      );
}


var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function fetchAddress(_address) {
  var address = btcAddress;
  console.log("Getting balance of "+address);
  // Construct URL
  var url = "https://blockchain.info/address/"+address+"?format=json";

  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      try {
      var balance = -1;
      var json = JSON.parse(responseText);

      console.log(json.final_balance);

      // Temperature in Kelvin requires adjustment
      var balance = Math.round(json.final_balance);
     } catch (e) {  }
      console.log("Balance is" + balance);
      sendBalance(balance);
    }      
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log("PebbleKit JS ready!");
    //get btc address from settings
    var settings = {};
try {
settings = JSON.parse(localStorage.getItem('clay-settings')) || {};
  btcAddress = settings.KEY_BTC_ADDRESS;
    console.log("Read config address: "+settings.KEY_BTC_ADDRESS);
} catch (e) {}
    fetchAddress();
  });

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received!");
    fetchAddress();
  }                     
);


