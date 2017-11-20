//
//  jointduplicates.cpp
//  Joint duplicates dusting strategy
//
//  Created by Istvan Fehervari on 12/11/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#include "jointduplicates.h"

QString JointDuplicatesDustStrategy::getName() const
{
    return "Minimum Playable (preferring gold)";
}

QString JointDuplicatesDustStrategy::getDescription() const
{
    return "Selects cards that have altogether more copies than the individual playable limit."
           " This will not change functional & but will change cosmetic property of the collection.";
}

DustPair JointDuplicatesDustStrategy::getDustValue(const CollectionCard& card) const
{
    DustPair result;
    bool isElite = isCardElite(card);

    unsigned int sumCards = card.normalCount + card.premiumCount;

    if (isElite) {
        if (sumCards > 1) {
            if (card.premiumCount > 0) {
                result.normal = card.normalCount;
                if (card.premiumCount > 1) {
                    result.premium = card.premiumCount - 1;
                }
            } else {
                if (card.normalCount > 1) {
                    result.normal = card.normalCount - 1;
                }
            }
        }
    } else {
        if (sumCards > 2) {
            if (card.premiumCount > 1) {
                result.normal = card.normalCount;
                if (card.premiumCount > 2) {
                    result.premium = card.premiumCount - 2;
                }
            } else {
                result.normal = card.normalCount - (2 - card.premiumCount);
            }
        }
    }

    return result;
}
