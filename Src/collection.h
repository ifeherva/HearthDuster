//
//  collection.h
//  Collection
//
//  Created by Istvan Fehervari on 01/05/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#ifndef COLLECTION_H
#define COLLECTION_H

#include <map>
#include "db/card.h"
#include "duststrategy.h"

class Collection
{
public:
    Collection();
    ~Collection();

    void sync();

    std::vector<const Card*> getCardsFor(dustCard dustcardFunction, bool excludeNonDustable = true);

private:
    std::map<QString, CollectionCard> m_cardcollection;
};

#endif // COLLECTION_H
