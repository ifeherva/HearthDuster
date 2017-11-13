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
           " This will not change functional or cosmetic property of the collection.";
}

DustPair DuplicatesDustStrategy::getDustValue(const CollectionCard& card) const
{
    DustPair result;
    bool isElite = isCardElite(card);

    if (isElite) {
        if (card.normalCount > 0) {
            result.normal = card.normalCount - 1;
        }
        if (card.premiumCount > 0) {
            result.premium = card.premiumCount - 1;
        }
    } else {
        if (card.normalCount > 1) {
            result.normal = card.normalCount - 2;
        }
        if (card.premiumCount > 1) {
            result.premium = card.premiumCount - 2;
        }
    }

    return result;
}
