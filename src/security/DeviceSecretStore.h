#ifndef DEVICE_SECRET_H
#define DEVICE_SECRET_H

#include <WString.h>
#include "../PrefsWrapper.h"

class DeviceSecretStore;

class DeviceSecretStore {
    private:
        String localDeviceSecret;
        PrefsWrapper prefs;
        void generateDeviceSecret();
    public:
        DeviceSecretStore(PrefsWrapper _prefs) {
            prefs = _prefs;
        }
        String getDeviceSecret();
};

#endif