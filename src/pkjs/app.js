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
    console.log("keys"+messageKeys.KEY_BTC_ADDRESS_1);
    btcAddress = dict[messageKeys.KEY_BTC_ADDRESS_1];
    console.log("Wallet name: "+dict[messageKeys.KEY_BTC_NAME_1])
    console.log("Settings closed, got address"+dict[messageKeys.KEY_BTC_ADDRESS_1]);
    fetchAddress(0);

    // Send settings values to watch side
    Pebble.sendAppMessage(dict, function(e) {
        console.log('Sent config data to Pebble');
    }, function(e) {
        console.log('Failed to send config data!');
        console.log(JSON.stringify(e));
    });


});

function sendClaySettings() {
    settings = JSON.parse(localStorage.getItem('clay-settings')) || {};

    // Send settings values to watch side
    Pebble.sendAppMessage(settings, function(e) {
        console.log('Sent config data to Pebble');
    }, function(e) {
        console.log('Failed to send config data!');
        console.log(JSON.stringify(e));
    });
}

function sendBalance(message)
{
    if(message == 'undefined')
        balance = "Error";
    // Assemble dictionary using our keys
    var dictionary = {
        "KEY_FINAL_BALANCE": Math.floor(message / 1e8),
        "KEY_FINAL_BALANCE_COMMA" : message % 1e8
    };
    console.log('Balance is: '+Math.floor(message/1e8)+' after comma: '+message%1e8)

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
                       //var balance = Math.round(json.total_received);
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
                                btcAddress = settings.KEY_BTC_ADDRESS_1;
                                console.log("Read config address: "+settings.KEY_BTC_ADDRESS_1);
                            } catch (e) {}
                            fetchAddress();
                            sendClaySettings();
                        });

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
                        function(e) {
                            console.log("AppMessage received!");
                            fetchAddress();
                        }
                        );


