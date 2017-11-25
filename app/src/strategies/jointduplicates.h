//
//  jointduplicates.h
//  Strategy optimizing cards to minimum playable, keeping golden cards
//
//  Created by Istvan Fehervari on 12/11/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#ifndef JOINTDUPLICATES_H
#define JOINTDUPLICATES_H

#include "../duststrategy.h"

class JointDuplicatesDustStrategy : public DustStrategy {
    Q_OBJECT
public:
    virtual QString getName() const;
    virtual QString getDescription() const;

    virtual DustPair getDustValue(const CollectionCard& card) const;
};

#endif // JOINTDUPLICATES_H
