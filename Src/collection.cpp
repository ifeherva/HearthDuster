//
//  collection.cpp
//  Collection
//
//  Created by Istvan Fehervari on 01/05/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#include "collection.h"
#include "utils/macutils.h"

#include <Mirror.hpp>

using namespace hearthmirror;

Collection::Collection()
{

}

void Collection::sync()
{
    // check if game is running
    int pid = getHearthstonePID();

    if (pid == 0) return;

    Mirror* mirror = new Mirror(pid,false);

    delete mirror;
}


