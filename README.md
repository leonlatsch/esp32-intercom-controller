# ESP32 controller for an old school intercom

***DEVELOPMENT IN PROGRESS***

## Additional components needed:
- Some source of power
- Relay Module

## Setup
1. Compile software with PlatformIO and Upload to ESP32
2. Wait for the blue LED to light up. Now its in setup mode
3. Connect to the `ESP32 Intercom Controller` WiFi Access Point
4. Call the setup endpoint 
```json
POST http://192.168.4.1/setup

{
    "ssid": "YourWifiNetworkName",
    "pass": "YourWifiPassword"
}
```
5. ESP will restart in 5 seconds
6. The blue led will blink 2 times if you provided correct wifi credentials

## API Endpoints

- `/opendoor` - Open the door / currently just enable blue led
- `/wificonfig` - Return the stored wifi config
- `/reset` - Hardware reset. ESP will reboot and enter setup mode
- `/setup` - Only available in setup mode

## Device secret
TBD