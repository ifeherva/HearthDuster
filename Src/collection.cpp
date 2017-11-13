//
//  collection.cpp
//  Collection
//
//  Created by Istvan Fehervari on 01/05/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#include "collection.h"
#include "utils/macutils.h"
#include "db/cardsdb.h"

#include <Mirror.hpp>

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
    
    for (auto card : cards) {
        QString cardid = QString::fromStdU16String(card.id);
        if (!m_cardcollection.count(cardid)) {
            m_cardcollection[cardid] = CollectionCard(cardid);
        }

        if (card.premium) {
            m_cardcollection[cardid].premium_count = card.count;
        } else {
            m_cardcollection[cardid].normal_count = card.count;
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

        auto dustValue = dustStrategy->getDustValue(it->second);
        if (!dustValue.isEmpty()) {
            result.push_back(DustStrategyResult(cardDef, dustValue.normal, dustValue.premium));
        }
    }
    return result;
}

