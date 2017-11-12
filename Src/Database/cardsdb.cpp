//
//  cardsdb.cpp
//  cardsdb
//
//  Created by Istvan Fehervari on 01/05/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#include "cardsdb.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>

static CardsDb* instance = NULL;

CardsDb::CardsDb() {

}

CardsDb::~CardsDb() {

}

void CardsDb::Init() {
    if (instance == NULL) {
        instance = new CardsDb();
    }
}

void CardsDb::Clear() {
    for (auto it = instance->cardsmap.begin(); it != instance->cardsmap.end(); it++) {
        delete it.value();
    }
    instance->cardsmap.clear();
}

int CardsDb::InitFromFile(const QString& cardsDbFile) {
    CardsDb::Init();

    CardsDb::Clear();

    QFile jsonFile(cardsDbFile);

    if (!jsonFile.open(QFile::ReadOnly)) {
        return CARDSDBERROR_FILE_OPEN_FAILED;
    }

    QJsonParseError error;
    QJsonDocument document =  QJsonDocument().fromJson(jsonFile.readAll(), &error);
    if (error.error != QJsonParseError::NoError) {
        return CARDSDBERROR_JSON_PARSE_FAILED;
    }

    QJsonArray cardarray = document.array();

    for (auto it = cardarray.constBegin(); it != cardarray.end(); it++) {

        Card* card = CardFactory::FromJson((*it).toObject());
        if (card != NULL) {
            instance->cardsmap[card->id] = card;
        }
    }

    return CARDSDBERROR_NOERROR;
}

const Card* CardsDb::CardForId(const QString& id)
{
    if (instance->cardsmap.contains(id)) {
        return instance->cardsmap[id];
    }
    return NULL;
}