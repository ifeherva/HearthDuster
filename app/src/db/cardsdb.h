//
//  cardsdb.h
//  cardsdb
//
//  Created by Istvan Fehervari on 01/05/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#ifndef CARDSDB_H
#define CARDSDB_H

#include <QString>
#include <QMap>

#include "card.h"

enum CardsDbError {
  CARDSDBERROR_NOERROR = 0,
  CARDSDBERROR_FILE_OPEN_FAILED,
  CARDSDBERROR_JSON_PARSE_FAILED
};

class CardsDb
{
public:

    static int initFromFile(const QString& cardsDbFile);
    static const Card* cardForId(const QString& id);

private:

    CardsDb();
    ~CardsDb();
    static void init();
    static void clear();

    QMap<QString, Card*> cardsmap;
};

#endif // CARDSDB_H
