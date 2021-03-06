//
//  collection.h
//  Collection model class
//
//  Created by Istvan Fehervari on 01/05/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#ifndef COLLECTION_H
#define COLLECTION_H

#include <map>
#include "db/card.h"
#include "duststrategy.h"

struct DustStrategyResult {
    const Card* cardData;
    const unsigned int superfluous_normal;
    const unsigned int superfluous_premium;
    const std::vector<QVariant> userData;

    DustStrategyResult(const Card* cardData,
                       unsigned int superfluous_normal,
                       unsigned int superfluous_premium,
                       std::vector<QVariant>& userData) :
        cardData(cardData),
        superfluous_normal(superfluous_normal),
        superfluous_premium(superfluous_premium),
        userData(userData){}

    bool isEmpty() {
        return superfluous_normal == 0 && superfluous_premium == 0;
    }

    unsigned int dustValue();
};

enum SynchError {
    NoError,
    HearthstoneNotRunning,
    InvalidCollectionData,
    UnknownError
};

enum ExportFormat {
    CSV
};

class Collection
{
public:
    Collection();
    ~Collection();

    /** Synchronizes collection from Hearthstone.
     * @returns true if synchronization is successful, false otherwise
     */
    SynchError sync();

    std::vector<DustStrategyResult> getCardsFor(const DustStrategy* const dustStrategy, bool excludeNonDustable = true);

    bool exportToFile(const QString& fileName, const ExportFormat& exportFormat);

private:
    std::map<QString, CollectionCard> m_cardcollection;
};

#endif // COLLECTION_H
