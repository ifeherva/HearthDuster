//
//  duplicates.h
//  Strategy selecting duplicated cards
//
//  Created by Istvan Fehervari on 12/11/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#include "../duststrategy.h"

class DuplicatesDustStrategy : public DustStrategy {

    virtual QString getName() const;
    virtual QString getDescription() const;

    virtual DustPair getDustValue(const CollectionCard& card) const;
};
