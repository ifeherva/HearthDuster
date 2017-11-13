//
//  minimumplayable.h
//  Strategy optimizing cards to minimum playable, prefering gold cards
//
//  Created by Istvan Fehervari on 12/11/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#ifndef MINIMUMPLAYABLE_H
#define MINIMUMPLAYABLE_H

#include "../duststrategy.h"

class MinimumPlayableDustStrategy : public DustStrategy {
public:
    virtual QString getName() const;
    virtual QString getDescription() const;

    virtual DustPair getDustValue(const CollectionCard& card) const;
};

#endif // MINIMUMPLAYABLE_H
