//
//  jointduplicatesgolden.cpp
//  Joint duplicates dusting strategy keeping non-normal cards
//
//  Created by Istvan Fehervari on 12/11/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#include "jointduplicatesgolden.h"

QString JointDuplicatesGoldenDustStrategy::getName() const
{
    return "Minimum Playable Golden";
}

QString JointDuplicatesGoldenDustStrategy::getDescription() const
{
    return "Selects golden cards that have altogether more copies than the individual playable limit."
           " This will not change functional & but will change cosmetic property of the collection.";
}

DustPair JointDuplicatesGoldenDustStrategy::getDustValue(const CollectionCard& card) const
{
    DustPair result;
    bool isElite = isCardElite(card);

    unsigned int sumCards = card.normalCount + card.premiumCount;

    if (isElite) {
        if (sumCards > 1) {
            if (card.normalCount > 0) {
                result.normal = card.normalCount - 1;
                result.premium = card.premiumCount;
            } else {
                if (card.premiumCount > 0) {
                    result.premium = card.premiumCount - 1;
                }
            }
        }
    } else {
        if (sumCards > 2) {
            if (card.normalCount > 1) {
                result.premium = card.premiumCount;
                if (card.normalCount > 2) {
                    result.normal = card.normalCount - 2;
                }
            } else {
                result.premium = card.premiumCount - (2 - card.normalCount);
            }
        }
    }

    return result;
}
