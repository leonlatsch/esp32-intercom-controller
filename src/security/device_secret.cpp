#include "device_secret.h"
#include "../PrefsWrapper.h"
#include "../uuid/uuid.h"

const char *PREFS_KEY_DEVICE_SECRET = "DSEC";

void DeviceSecretStore::generateDeviceSecret() {
    uuid_t uuid;
    char uuidStr[UUID_STR_LEN];

    uuid_generate(uuid);
    uuid_unparse(uuid, uuidStr);

    String newDeviceSecret(uuidStr);

    prefs.putString(PREFS_KEY_DEVICE_SECRET, newDeviceSecret);
    localDeviceSecret = newDeviceSecret;
}

String DeviceSecretStore::getDeviceSecret() {
    if (localDeviceSecret == NULL || localDeviceSecret.isEmpty()) {
        localDeviceSecret = prefs.getString(PREFS_KEY_DEVICE_SECRET);
    }

    if (!localDeviceSecret.isEmpty()) {
        return localDeviceSecret;
    }

    generateDeviceSecret();

    return localDeviceSecret;
}