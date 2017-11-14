//
//  wil.h
//  Strategy selecting wild cards
//
//  Created by Istvan Fehervari on 12/11/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//
#ifndef WILD_H
#define WILD_H

#include "../duststrategy.h"

class WildDustStrategy : public DustStrategy {
public:
    virtual QString getName() const;
    virtual QString getDescription() const;

    virtual DustPair getDustValue(const CollectionCard& card) const;
};

#endif // WILD_H

