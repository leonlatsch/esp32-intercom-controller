#include "PrefsWrapper.h"

const char *PREFS_NAMESPACE = "icc";

String PrefsWrapper::getString(const char *key) {
    prefs.begin(PREFS_NAMESPACE, false);
    String value = prefs.getString(key);
    prefs.end();

    return value;
}

bool PrefsWrapper::getBool(const char *key) {
    prefs.begin(PREFS_NAMESPACE, false);
    bool value = prefs.getBool(key);
    prefs.end();

    return value;
}

bool PrefsWrapper::putString(const char *key, String value) {
    prefs.begin(PREFS_NAMESPACE, false);
    size_t res = prefs.putString(key, value);
    prefs.end();

    return res != 0;
}

bool PrefsWrapper::putBool(const char *key, bool value) {
    prefs.begin(PREFS_NAMESPACE, false);
    size_t res = prefs.putBool(key, value);
    prefs.end();

    return res != 0;
}

void PrefsWrapper::clear() {
    prefs.begin(PREFS_NAMESPACE, false);
    prefs.clear();
    prefs.end();
}
