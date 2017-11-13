//
//  preferences.cpp
//  Dusting strategies
//
//  Created by Istvan Fehervari on 12/11/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#include "preferences.h"
#include <QSettings>

#define ORGANIZATION_DOMAIN "com.ifehervari"
#define APP_NAME "HearthDuster"

QSettings settings(ORGANIZATION_DOMAIN, APP_NAME);

QString Preferences::getLocale()
{
    return settings.value("locale", DEFAULT_LOCALE).toString();
}

bool Preferences::showMemoryReadingWarning()
{
    return settings.value("showMemoryReadinWarning", true).toBool();
}

void Preferences::setShowMemoryReadingWarning(bool value)
{
    settings.setValue("showMemoryReadinWarning", value);
}
