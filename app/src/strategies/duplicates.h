//
//  duplicates.h
//  Strategy selecting duplicated cards
//
//  Created by Istvan Fehervari on 12/11/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#ifndef DUPLICATES_H
#define DUPLICATES_H

#include "../duststrategy.h"

class DuplicatesDustStrategy : public DustStrategy {
public:
    virtual QString getName() const;
    virtual QString getDescription() const;

    virtual DustPair getDustValue(const CollectionCard& card) const;
};

#endif // DUPLICATES_H
