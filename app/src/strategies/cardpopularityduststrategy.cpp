//
//  cardpopularityduststrategy.cpp
//  Strategy selecting unpopular cards based on hsreplay data
//
//  Created by Istvan Fehervari on 12/11/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#include "cardpopularityduststrategy.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include "../db/cardsdb.h"

#define DATABASE_URL "https://hsreplay.net/analytics/query/card_played_popularity_report/?GameType=RANKED_STANDARD&RankRange=ALL&TimeRange=LAST_14_DAYS"
#define JOINT_POPULARITY_THRESHOLD 0.001

const PopularityCard* PopularityDataBase::getCard(unsigned long dbfId) const
{
    if (cards.count(dbfId)) {
        return &cards.at(dbfId);
    }
    return NULL;
}

void PopularityDataBase::deserialize(const QString& source)
{
    QJsonParseError error;
    QJsonDocument document =  QJsonDocument().fromJson(source.toUtf8(), &error);
    if (error.error != QJsonParseError::NoError) {
        return;
    }

    QJsonObject rootObject = document.object();
    QJsonObject seriesObject = rootObject["series"].toObject();
    QJsonObject metaDataObject = seriesObject["metadata"].toObject();
    totalPlayedCardsCount = metaDataObject["total_played_cards_count"].toVariant().toLongLong();
    QJsonObject dataObject = seriesObject["data"].toObject();
    QJsonArray dataArray = dataObject["ALL"].toArray();

    for (auto it = dataArray.constBegin(); it != dataArray.end(); it++) {
        QJsonObject entry = (*it).toObject();
        PopularityCard card(entry["dbf_id"].toVariant().toULongLong(),
                entry["popularity"].toDouble(), entry["winrate"].toDouble(), entry["total"].toVariant().toULongLong());
        cards[card.dbfId] = card;
    }
}

CardPopularityDustStrategy::CardPopularityDustStrategy(QObject *parent) : QObject(parent)
{
    // download database
    networkAccessManager = new QNetworkAccessManager(this);
    connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    networkAccessManager->get(QNetworkRequest(QUrl(DATABASE_URL)));
}

CardPopularityDustStrategy::~CardPopularityDustStrategy()
{
    if (networkAccessManager) delete networkAccessManager;
}

void CardPopularityDustStrategy::replyFinished(QNetworkReply* reply)
{
    if (reply->error()) {
        reply->deleteLater();
        return;
    }
    QString data = (QString) reply->readAll();
    database.deserialize(data);
}

QString CardPopularityDustStrategy::getName() const
{
    return "Popularity";
}

QString CardPopularityDustStrategy::getDescription() const
{
    return "Selects cards that are unpopular in the current meta based on the last 14 days using HSReplay.net data.";
}

DustPair CardPopularityDustStrategy::getDustValue(const CollectionCard& card) const
{
    DustPair result;
    const Card* cardDef = CardsDb::cardForId(card.id);
    auto popularityCard = database.getCard(cardDef->dbfId);
    if (popularityCard != NULL) {
        // TODO: separate popularity from wild to standard
        if (popularityCard->winrate < 35.0f || (popularityCard->popularity < JOINT_POPULARITY_THRESHOLD && popularityCard->winrate < 50.0f)) {
            result.normal = card.normalCount;
            result.premium = card.premiumCount;
        }
    }

    return result;
}
