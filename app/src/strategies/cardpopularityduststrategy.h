//
//  cardpopularityduststrategy.h
//  Strategy selecting unpopular cards based on hsreplay data
//
//  Created by Istvan Fehervari on 12/11/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#ifndef CARDPOPULARITYDUSTSTRATEGY_H
#define CARDPOPULARITYDUSTSTRATEGY_H

#include "../duststrategy.h"
#include <QObject>
#include <QNetworkAccessManager>

struct PopularityCard {
    unsigned long dbfId; // "dbf_id":643,
    double popularity; // "popularity":"0.010749637617352016",
    double winrate; // "winrate":44.686414884039955,
    unsigned long long totalGames; // "total":23327
    PopularityCard() {}
    PopularityCard(unsigned long dbfId,
                   double popularity,
                   double winrate,
                   unsigned long long totalGames) : dbfId(dbfId),
        popularity(popularity), winrate(winrate), totalGames(totalGames) {}
};

class PopularityDataBase {

public:
    const PopularityCard* getCard(unsigned long dbfId) const;
    void deserialize(const QString& source);
    unsigned long long getTotalPlayedCardsCount() {return totalPlayedCardsCount;}

private:
    std::map<unsigned long, PopularityCard> cards;
    unsigned long long totalPlayedCardsCount;
};

class CardPopularityDustStrategy : public QObject, public DustStrategy
{
    Q_OBJECT
public:
    CardPopularityDustStrategy(QObject *parent = Q_NULLPTR);
    virtual ~CardPopularityDustStrategy();
    virtual QString getName() const;
    virtual QString getDescription() const;

    virtual DustPair getDustValue(const CollectionCard& card) const;
private:
    QNetworkAccessManager *networkAccessManager = NULL;
    PopularityDataBase database;

private slots:
    void replyFinished(QNetworkReply*);
};

#endif // CARDPOPULARITYDUSTSTRATEGY_H
