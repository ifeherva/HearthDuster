//
//  strategies.h
//  Dusting strategies
//
//  Created by Istvan Fehervari on 12/11/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//
#ifndef STRATEGIES_H
#define STRATEGIES_H

#include "duplicates.h"
#include "jointduplicates.h"
#include "jointduplicatesgolden.h"
#include "wild.h"
#include "cardpopularityduststrategy.h"

#define INSTALL(strategy, vector) \
    auto lc_##strategy = new strategy(); \
    connect(lc_##strategy, SIGNAL(sendErrorMessage(const DustStrategy*, QString)), this, SLOT(on_strategyError(const DustStrategy*, QString)) ); \
    connect(lc_##strategy, SIGNAL(sendMessage(const DustStrategy*, QString)), this, SLOT(on_strategyMessage(const DustStrategy*, QString)) ); \
    vector.push_back(lc_##strategy);

#define INSTALL_STRATEGIES(strategyVector) \
    strategyVector.clear(); \
    INSTALL(DuplicatesDustStrategy, strategyVector) \
    INSTALL(JointDuplicatesDustStrategy, strategyVector) \
    INSTALL(JointDuplicatesGoldenDustStrategy, strategyVector) \
    INSTALL(WildDustStrategy, strategyVector) \
    INSTALL(CardPopularityDustStrategy, strategyVector)



#endif // STRATEGIES_H
