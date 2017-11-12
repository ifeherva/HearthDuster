#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QString>

#define DEFAULT_LOCALE "enUS"

class Preferences {
public:
    static QString GetLocale();
    static bool ShowMemoryReadingWarning();
    static void SetShowMemoryReadingWarning(bool value);
};

#endif // PREFERENCES_H
