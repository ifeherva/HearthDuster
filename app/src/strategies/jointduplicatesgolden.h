//
//  jointduplicatesgolden.h
//  Strategy optimizing cards to minimum playable, keeping normal cards
//
//  Created by Istvan Fehervari on 12/11/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#ifndef JOINTDUPLICATESGOLDEN_H
#define JOINTDUPLICATESGOLDEN_H

#include "../duststrategy.h"

class JointDuplicatesGoldenDustStrategy : public DustStrategy {
    Q_OBJECT
public:
    virtual QString getName() const;
    virtual QString getDescription() const;

    virtual DustPair getDustValue(const CollectionCard& card) const;
};

#endif // JOINTDUPLICATESGOLDEN_H
