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

void Collection::sync()
{
    // check if game is running
    int pid = getHearthstonePID();

    if (pid == 0) return;

    m_cardcollection.clear();
    
    std::vector<hearthmirror::Card> cards;
    
    try {
        hearthmirror::Mirror* mirror = new hearthmirror::Mirror(pid,false);

        cards = mirror->getCardCollection();
        delete mirror;
    } catch (std::exception&) {
        return;
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
}

std::vector<const Card*> Collection::getCardsFor(dustCard dustcardFunction, bool excludeNonDustable)
{
    std::vector<const Card*> result;
    for (auto it = m_cardcollection.begin(); it != m_cardcollection.end(); it++) {
        const Card* cardDef = CardsDb::CardForId(it->first);
        if (!cardDef) continue;
        
        if ((cardDef->rarity == RARITY_FREE) && (excludeNonDustable)) continue;
        
        if (dustcardFunction(it->second)) {
            
            if (cardDef) {
                result.push_back(cardDef);
            }
        }
    }
    return result;
}

