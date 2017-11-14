//
//  wild.cpp
//  Dusting Wild cards
//
//  Created by Istvan Fehervari on 12/11/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#include "wild.h"

QString WildDustStrategy::getName() const
{
    return "Wild cards";
}

QString WildDustStrategy::getDescription() const
{
    return "Selects cards that do not belong to the current standard rotation."
           " This will greatly change functional and cosmetic property of the collection.";
}

DustPair WildDustStrategy::getDustValue(const CollectionCard& card) const
{
    DustPair result;

    if (!isStandard(card)) {
        result.normal = card.normalCount;
        result.premium = card.premiumCount;
    }

    return result;
}
