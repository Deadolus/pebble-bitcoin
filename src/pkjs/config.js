module.exports = [
  {
    "type": "heading",
    "defaultValue": "App Configuration"
  },
  {
    "type": "text",
    "defaultValue": "Configure your Wallet information here"
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Wallet"
      },
      {
  "type": "input",
  "messageKey": "KEY_BTC_NAME_1",
  "defaultValue": "Wallet",
  "label": "Wallet Name",
  "attributes": {
    "placeholder": "eg: MyWallet",
    "limit": 36,
    "type": "text"
  }
}  ,
{
  "type": "input",
  "messageKey": "KEY_BTC_ADDRESS_1",
  "defaultValue": "",
  "label": "Bitcoin Address",
  "attributes": {
    "placeholder": "eg: 12345...",
    "limit": 34,
    "type": "text"
  }
}
    ]
  },

        {
    "type": "submit",
    "defaultValue": "Save Settings"
  }

  ];
