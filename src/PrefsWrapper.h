#ifndef PrefsWrapper_H
#define PrefsWrapper_H

#include <Preferences.h>

class PrefsWrapper;

class PrefsWrapper {
    private:
        Preferences prefs;
    public:
        PrefsWrapper() {
            prefs = Preferences();
        }
        String getString(const char *key);
        bool getBool(const char *key);
        bool putString(const char *key, String value);
        bool putBool(const char *key, bool value);
        void clear();
};

#endif