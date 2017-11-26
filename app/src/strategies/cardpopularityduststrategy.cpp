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

#define DATABASE_URL_STANDARD "https://hsreplay.net/analytics/query/card_played_popularity_report/?GameType=RANKED_STANDARD&RankRange=ALL&TimeRange=LAST_14_DAYS"
#define DATABASE_URL_WILD "https://hsreplay.net/analytics/query/card_played_popularity_report/?GameType=RANKED_WILD&RankRange=ALL&TimeRange=LAST_14_DAYS"

const PopularityCard* PopularityDataBase::getCard(unsigned long dbfId, bool isStandard) const
{
    if (isStandard) {
        if (standardCards.count(dbfId)) {
            return &standardCards.at(dbfId);
        }
    } else {
        if (nonStandardCards.count(dbfId)) {
            return &nonStandardCards.at(dbfId);
        }
    }
    return NULL;
}

QJsonArray getDataArray(const QString& data, QJsonParseError& error)
{
    QJsonDocument document =  QJsonDocument().fromJson(data.toUtf8(), &error);
    if (error.error != QJsonParseError::NoError) {
        return QJsonArray();
    }

    QJsonObject rootObject = document.object();
    QJsonObject seriesObject = rootObject["series"].toObject();
    QJsonObject dataObject = seriesObject["data"].toObject();
    return dataObject["ALL"].toArray();
}

void PopularityDataBase::deserialize(const QString& standardData, const QString& wildData, const DustStrategy* dustStrategy)
{
    QJsonParseError error;

    QJsonArray standardArray = getDataArray(standardData, error);
    if (error.error != QJsonParseError::NoError) {
        return;
    }
    QJsonArray wildArray = getDataArray(wildData, error);
    if (error.error != QJsonParseError::NoError) {
        return;
    }

    std::map<unsigned long, unsigned long long> gamesPlayedStandard;
    std::map<unsigned long, unsigned long long> gamesPlayedNonStandard;

    totalPlayedCardsCountStandard = 0;
    totalPlayedCardsCountNonStandard = 0;

    for (auto it = standardArray.constBegin(); it != standardArray.end(); it++) {
        QJsonObject entry = (*it).toObject();
        PopularityCard card(entry["dbf_id"].toVariant().toULongLong(),
                entry["popularity"].toDouble(), entry["winrate"].toDouble(), entry["total"].toVariant().toULongLong());

        const Card* cardDef = CardsDb::cardFordbfId(card.dbfId);
        if (dustStrategy->isStandard(cardDef)) {
            standardCards[card.dbfId] = card;
            totalPlayedCardsCountStandard += card.totalGames;
            gamesPlayedStandard[card.dbfId] = card.totalGames;
        } else {
            nonStandardCards[card.dbfId] = card;
            totalPlayedCardsCountNonStandard += card.totalGames;
            gamesPlayedNonStandard[card.dbfId] = card.totalGames;
        }
    }

    for (auto it = wildArray.constBegin(); it != wildArray.end(); it++) {
        QJsonObject entry = (*it).toObject();
        PopularityCard card(entry["dbf_id"].toVariant().toULongLong(),
                entry["popularity"].toDouble(), entry["winrate"].toDouble(), entry["total"].toVariant().toULongLong());

        const Card* cardDef = CardsDb::cardFordbfId(card.dbfId);
        if (dustStrategy->isStandard(cardDef)) {
            standardCards[card.dbfId] = card;
            totalPlayedCardsCountStandard += card.totalGames;
            if (gamesPlayedStandard.count(card.dbfId)) {
                gamesPlayedStandard[card.dbfId] = gamesPlayedStandard[card.dbfId] + card.totalGames;
            } else {
                gamesPlayedStandard[card.dbfId] = card.totalGames;
            }
        } else {
            nonStandardCards[card.dbfId] = card;
            totalPlayedCardsCountNonStandard += card.totalGames;
            if (gamesPlayedNonStandard.count(card.dbfId)) {
                gamesPlayedNonStandard[card.dbfId] = gamesPlayedNonStandard[card.dbfId] + card.totalGames;
            } else {
                gamesPlayedNonStandard[card.dbfId] = card.totalGames;
            }
        }
    }

    // compute threshold based on Q1
    std::vector<unsigned long long> standardPlayed;
    std::vector<unsigned long long> nonStandardPlayed;
    for (auto it = gamesPlayedStandard.begin(); it != gamesPlayedStandard.end(); ++it) {
        standardPlayed.push_back(it->second);
    }
    for (auto it = gamesPlayedNonStandard.begin(); it != gamesPlayedNonStandard.end(); ++it) {
        nonStandardPlayed.push_back(it->second);
    }
    std::sort(standardPlayed.begin(), standardPlayed.end());
    std::sort(nonStandardPlayed.begin(), nonStandardPlayed.end());

    standardThreshold = (double)standardPlayed[standardPlayed.size()/4] / totalPlayedCardsCountStandard;
    nonStandardThreshold = (double)nonStandardPlayed[nonStandardPlayed.size()/4] / totalPlayedCardsCountNonStandard;
}

CardPopularityDustStrategy::CardPopularityDustStrategy()
{
    m_extraParams.clear();
    m_extraParams.push_back("Popularity");
    emit sendMessage((const DustStrategy*)this, "Retrieving HSReplay.net data...");
    // download database
    networkAccessManager = new QNetworkAccessManager(this);
    connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    standardReply = networkAccessManager->get(QNetworkRequest(QUrl(DATABASE_URL_STANDARD)));
}

CardPopularityDustStrategy::~CardPopularityDustStrategy()
{
    if (networkAccessManager) delete networkAccessManager;
}

void CardPopularityDustStrategy::replyFinished(QNetworkReply* reply)
{
    if (reply->error()) {
        reply->deleteLater();
        emit sendErrorMessage(this, "Failed to retrieve HSReplay.net data");
        return;
    }
    if (reply == standardReply) {
        standardStringData = (QString) reply->readAll();
        wildReply = networkAccessManager->get(QNetworkRequest(QUrl(DATABASE_URL_WILD)));
    } else {
        wildStringData = (QString) reply->readAll();
        database.deserialize(standardStringData, wildStringData, this);
        emit sendMessage((const DustStrategy*)this, "Finished retrieving HSReplay.net data");
    }
}

QString CardPopularityDustStrategy::name() const
{
    return "Popularity";
}

QString CardPopularityDustStrategy::description() const
{
    return "Selects cards that are unpopular in the current meta based on the last 14 days using HSReplay.net data.";
}

DustPair CardPopularityDustStrategy::dustValue(const CollectionCard& card) const
{
    DustPair result;
    const Card* cardDef = CardsDb::cardForId(card.id);
    double popularity = 0.0;
    double popularityThreshold = 1;
    const PopularityCard* popularityCard;
    if (isStandard(cardDef)) {
        popularityCard = database.getCard(cardDef->dbfId, true);
        if (popularityCard != NULL) {
            popularity = (double)popularityCard->totalGames / database.getTotalPlayedCardsCountStandard();
            popularityThreshold = database.getStandardThreshold();
        } else {
            return result;
        }
    } else {
        popularityCard = database.getCard(cardDef->dbfId, false);
        if (popularityCard != NULL) {
            popularity = (double)popularityCard->totalGames / database.getTotalPlayedCardsCountNonStandard();
            popularityThreshold = database.getNonStandardThreshold();
        } else {
            return result;
        }
    }

    if (popularity < popularityThreshold) {
        result.normal = card.normalCount;
        result.premium = card.premiumCount;
        result.userData.push_back(QVariant(QString().asprintf("%.2f%%",popularity*100.0)));
    }

    return result;
}
