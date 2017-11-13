//
//  duplicates.cpp
//  Dusting strategies
//
//  Created by Istvan Fehervari on 12/11/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#include "duplicates.h"

QString DuplicatesDustStrategy::getName() const
{
    return "Duplicates";
}

QString DuplicatesDustStrategy::getDescription() const
{
    return "Selects cards that have more copies than the playable limit."
           "This will not change functional or cosmetic property of the collection.";
}

DustPair DuplicatesDustStrategy::getDustValue(const CollectionCard& card) const
{
    DustPair result;
    bool isElite = isCardElite(card);

    if (isElite) {
        if (card.normal_count > 0) {
            result.normal = card.normal_count - 1;
        }
        if (card.premium_count > 0) {
            result.premium = card.premium_count - 1;
        }
    } else {
        if (card.normal_count > 1) {
            result.normal = card.normal_count - 2;
        }
        if (card.premium_count > 1) {
            result.premium = card.premium_count - 2;
        }
    }

    return result;
}
