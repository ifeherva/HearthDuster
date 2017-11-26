//
//  collection.cpp
//  Collection
//
//  Created by Istvan Fehervari on 01/05/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#include "collection.h"
#ifdef __APPLE__
    #include "utils/macutils.h"
#else
    #include "utils/winutils.h"
#endif
#include "db/cardsdb.h"

#include <Mirror.hpp>

unsigned int DustStrategyResult::dustValue() {

    unsigned int result = 0;
    if (this->cardData->rarity == RARITY_LEGENDARY) {
        result += superfluous_normal * 400;
        result += superfluous_premium * 1600;
    } else if (this->cardData->rarity == RARITY_EPIC) {
        result += superfluous_normal * 100;
        result += superfluous_premium * 400;
    } else if (this->cardData->rarity == RARITY_RARE) {
        result += superfluous_normal * 20;
        result += superfluous_premium * 100;
    } else if (this->cardData->rarity == RARITY_COMMON) {
        result += superfluous_normal * 5;
        result += superfluous_premium * 50;
    }

    return result;
}

Collection::Collection()
{

}

Collection::~Collection()
{

}

SynchError Collection::sync()
{
    // check if game is running
    int pid = getHearthstonePID();

    if (pid == 0) return HearthstoneNotRunning;

    m_cardcollection.clear();
    
    std::vector<hearthmirror::Card> cards;
    
    try {
        hearthmirror::Mirror* mirror = new hearthmirror::Mirror(pid,false);

        cards = mirror->getCardCollection();
        delete mirror;
    } catch (std::exception&) {
        return InvalidCollectionData;
    }

    if (cards.size() == 0) {
        return InvalidCollectionData;
    }
    
    for (auto card : cards) {
        QString cardid = QString::fromStdU16String(card.id);
        if (!m_cardcollection.count(cardid)) {
            m_cardcollection[cardid] = CollectionCard(cardid);
        }

        if (card.premium) {
            m_cardcollection[cardid].premiumCount = card.count;
        } else {
            m_cardcollection[cardid].normalCount = card.count;
        }
    }
    return NoError;
}

std::vector<DustStrategyResult> Collection::getCardsFor(const DustStrategy* const dustStrategy, bool excludeNonDustable)
{
    std::vector<DustStrategyResult> result;
    for (auto it = m_cardcollection.begin(); it != m_cardcollection.end(); it++) {
        const Card* cardDef = CardsDb::cardForId(it->first);
        if (!cardDef) continue;
        
        if ((cardDef->rarity == RARITY_FREE) && (excludeNonDustable)) continue;

        auto dustValue = dustStrategy->dustValue(it->second);
        if (!dustValue.isEmpty()) {
            result.push_back(DustStrategyResult(cardDef, dustValue.normal, dustValue.premium, dustValue.userData));
        }
    }
    return result;
}

