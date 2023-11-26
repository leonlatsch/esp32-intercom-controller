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
6. The blue led will blink while trying to connect
7. The ESP will try for 60 seconds to connect to the configured wifi
    7.1. If the led blinks 5 times its connected
    7.2. If the led is led up again, connection failed. Its back in setup mode configuration your credentials are still saved. Eg. if your wifi was down, just restart the ESP and try again

## API Endpoints

**Setup Mode**
| Endpoint      | Secured | Possible Responses |
|---------------|---------|--------------------|
| `/opendoor`   | yes     | 200, 403           |
| `/wificonfig` | yes     | 200, 403           |
| `/reset`      | no      | 200                |

**Operational Mode**
| Endpoint | Secured | Possible Responses |
|----------|---------|--------------------|
| `/setup` | no      | 200, 400           |

## Device secret
The ESP generates a device secret when calling `/setup`. It is only shown once in the response.

Send this secret as a header for all secured endpoints like this: `Secret: <device-secret>`.

If you forget the secret, or didn't catch it in the response, you need to call `/reset` and `/setup` again.