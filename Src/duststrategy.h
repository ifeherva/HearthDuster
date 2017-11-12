#ifndef DUSTSTRATEGY_H
#define DUSTSTRATEGY_H

#include "db/card.h"

typedef bool (*dustCard)(const CollectionCard&);

class DustStrategy {

public:

    /** Returns true for cards that have duplicates beyond individial playable level.
     *
     * This will not change functional & cosmetic property of the collection. */
    static bool ExcessCardsStrategy(const CollectionCard&);

    /** Returns true for cards that have duplicates beyond playable level.
     * Golden cards will be kept and normal ones will be returned.
     *
     * This will not change functional & but will change cosmetic property of the collection. */
    static bool ExcessPlayableCardsPreferGoldStrategy(const CollectionCard&);
};

#endif // DUSTSTRATEGY_H
