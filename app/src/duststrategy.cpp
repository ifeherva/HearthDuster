//
//  duststrategy.cpp
//  Dusting strategies
//
//  Created by Istvan Fehervari on 01/05/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#include "duststrategy.h"
#include "db/cardsdb.h"

bool DustStrategy::isCardElite(const CollectionCard& card) const
{
    const Card* cardDef = CardsDb::cardForId(card.id);
    return cardDef->elite;
}

bool DustStrategy::isStandard(const CollectionCard& card) const
{
    const Card* cardDef = CardsDb::cardForId(card.id);
    return (cardDef->set == CARD_SET_BASIC ||
            cardDef->set == CARD_SET_CLASSIC ||
            cardDef->set == CARD_SET_OLD_GODS ||
            cardDef->set == CARD_SET_KARAZHAN ||
            cardDef->set == CARD_SET_MEAN_STREETS_OF_GADGETZAN ||
            cardDef->set == CARD_SET_UNGORO ||
            cardDef->set == CARD_SET_KNIGHTS_OF_THE_FROZEN_THRONE ||
            cardDef->set == CARD_SET_KOBOLDS_AND_CARACOMBS);
}
