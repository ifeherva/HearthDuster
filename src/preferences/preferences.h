//
//  preferences.h
//  Dusting strategies
//
//  Created by Istvan Fehervari on 12/11/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//
#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QString>

#define DEFAULT_LOCALE "enUS"

class Preferences {
public:
    static QString getLocale();
    static bool showMemoryReadingWarning();
    static void setShowMemoryReadingWarning(bool value);
};

#endif // PREFERENCES_H
