//
//  duststrategy.cpp
//  Dusting strategies
//
//  Created by Istvan Fehervari on 01/05/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#include "duststrategy.h"
#include "Database/cardsdb.h"

bool DustStrategy::ExcessCardsStrategy(const CollectionCard& card)
{
    const Card* cardDef = CardsDb::CardForId(card.id);
    if (cardDef == NULL) {
        return false;
    }

    unsigned int playlimit = cardDef->elite ? 1 : 2;

    return (card.normal_count > playlimit) || (card.premium_count > playlimit);
}

bool DustStrategy::ExcessPlayableCardsPreferGoldStrategy(const CollectionCard& card)
{
    const Card* cardDef = CardsDb::CardForId(card.id);
    if (cardDef == NULL) {
        return false;
    }

    unsigned int playlimit = cardDef->elite ? 1 : 2;

    return (card.normal_count + card.premium_count) > playlimit;
}
