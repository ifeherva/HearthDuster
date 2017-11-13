//
//  minimumplayable.cpp
//  Dusting strategies
//
//  Created by Istvan Fehervari on 12/11/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#include "minimumplayable.h"

QString MinimumPlayableDustStrategy::getName() const
{
    return "Minimum Playable (preferring gold)";
}

QString MinimumPlayableDustStrategy::getDescription() const
{
    return "Selects cards that have altogether more copies than the individual playable limit."
           " This will not change functional & but will change cosmetic property of the collection.";
}

DustPair MinimumPlayableDustStrategy::getDustValue(const CollectionCard& card) const
{
    DustPair result;
    bool isElite = isCardElite(card);

    unsigned int sumCards = card.normal_count + card.premium_count;

    if (isElite) {
        if (sumCards > 1) {
            if (card.premium_count > 0) {
                result.normal = card.normal_count;
                if (card.premium_count > 1) {
                    result.premium = card.premium_count - 1;
                }
            } else {
                if (card.normal_count > 1) {
                    result.normal = card.normal_count - 1;
                }
            }
        }
    } else {
        if (sumCards > 2) {
            if (card.premium_count > 1) {
                result.normal = card.normal_count;
                if (card.premium_count > 2) {
                    result.premium = card.premium_count - 2;
                }
            } else {
                result.normal = card.normal_count - (2 - card.premium_count);
            }
        }
    }

    return result;
}
