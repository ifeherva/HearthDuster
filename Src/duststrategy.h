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
    unsigned int normal;
    unsigned int premium;

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

protected:
    bool isCardElite(const CollectionCard& card) const;
};

#endif // DUSTSTRATEGY_H
