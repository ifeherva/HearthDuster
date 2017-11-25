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
    const PopularityCard* getCard(unsigned long dbfId, bool isStandard) const;
    void deserialize(const QString& standardData, const QString& wildData, const DustStrategy* dustStrategy);

    unsigned long long getTotalPlayedCardsCountNonStandard() const {return totalPlayedCardsCountNonStandard;}
    unsigned long long getTotalPlayedCardsCountStandard() const {return totalPlayedCardsCountStandard;}
    double getStandardThreshold() const {return standardThreshold;}
    double getNonStandardThreshold() const {return nonStandardThreshold;}

private:
    unsigned long long totalPlayedCardsCountNonStandard = 0;
    unsigned long long totalPlayedCardsCountStandard = 0;
    double standardThreshold = 0;
    double nonStandardThreshold = 0;

    std::map<unsigned long, PopularityCard> nonStandardCards;
    std::map<unsigned long, PopularityCard> standardCards;
};

class CardPopularityDustStrategy : public DustStrategy
{
    Q_OBJECT
public:
    CardPopularityDustStrategy();
    virtual ~CardPopularityDustStrategy();
    virtual QString getName() const;
    virtual QString getDescription() const;

    virtual DustPair getDustValue(const CollectionCard& card) const;
private:
    QNetworkAccessManager *networkAccessManager = NULL;
    PopularityDataBase database;

    QNetworkReply* standardReply = NULL;
    QNetworkReply* wildReply = NULL;

    QString standardStringData = "";
    QString wildStringData = "";

private slots:
    void replyFinished(QNetworkReply*);
};

#endif // CARDPOPULARITYDUSTSTRATEGY_H
