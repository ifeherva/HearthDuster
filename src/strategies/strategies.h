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
#include "minimumplayable.h"

#define INSTALL(strategy, vector) vector.push_back(new strategy);


#define INSTALL_STRATEGIES(strategyVector) \
    strategyVector.clear(); \
    INSTALL(DuplicatesDustStrategy, strategyVector) \
    INSTALL(MinimumPlayableDustStrategy, strategyVector)



#endif // STRATEGIES_H
