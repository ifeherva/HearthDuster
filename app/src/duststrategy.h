//
//  duststrategy.h
//  Dusting strategy abstract class
//
//  Created by Istvan Fehervari on 12/11/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//
#ifndef DUSTSTRATEGY_H
#define DUSTSTRATEGY_H

#include "db/card.h"

struct DustPair {
    unsigned int normal = 0;
    unsigned int premium = 0;

    bool isEmpty() {
        return normal == 0 && premium == 0;
    }
};

class DustStrategy {

public:
    DustStrategy() {}
    virtual ~DustStrategy() {}

    virtual QString getName() const = 0;
    virtual QString getDescription() const = 0;

    virtual DustPair getDustValue(const CollectionCard& card) const = 0;

    bool isCardElite(const CollectionCard& card) const;
    bool isStandard(const Card* cardDef) const;
    bool isStandard(const CollectionCard& card) const;
};

#endif // DUSTSTRATEGY_H
