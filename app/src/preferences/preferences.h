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
#include <QDateTime>

#define DEFAULT_LOCALE "enUS"

class Preferences {
public:
    static QString getLocale();

    static bool showMemoryReadingWarning();
    static void setShowMemoryReadingWarning(bool value);

    // strategy specific preferences
    // -----------------------------

    // CardPopularityDustStrategy
    static void CardPopularityDustStrategy_setDataFetchDate(QDateTime time);
    static QDateTime CardPopularityDustStrategy_dataFetchDate();
};

#endif // PREFERENCES_H
