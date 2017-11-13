#ifndef DUSTSTRATEGY_H
#define DUSTSTRATEGY_H

#include "db/card.h"

struct DustPair {
    unsigned int normal;
    unsigned int premium;

    bool isEmpty() {
        return normal == 0 && premium == 0;
    }
};

class DustStrategy {

public:
    virtual QString getName() const = 0;
    virtual QString getDescription() const = 0;

    virtual DustPair getDustValue(const CollectionCard& card) const = 0;

protected:
    bool isCardElite(const CollectionCard& card) const;

    /** Returns true for cards that have duplicates beyond playable level.
     * Golden cards will be kept and normal ones will be returned.
     *
     * This will not change functional & but will change cosmetic property of the collection. */
    /*static bool ExcessPlayableCardsPreferGoldStrategy(const CollectionCard&);
    */
};

#endif // DUSTSTRATEGY_H
