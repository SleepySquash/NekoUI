//
//  NekoStatic.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 14/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "NekoStatic.hpp"

namespace NekoUI
{
    unsigned long NekoStatic::money{ 0 };
    float NekoStatic::affection{ 0 }; /// +Opinion about you
    float NekoStatic::maxNeed{ 1000 }, NekoStatic::needHunger{ maxNeed }, NekoStatic::needThirst{ maxNeed }, NekoStatic::needWarmth{ maxNeed }, NekoStatic::needCommunication{ maxNeed }, NekoStatic::needHygiene{ maxNeed }, NekoStatic::needToilet{ maxNeed }, NekoStatic::needEnergy{ maxNeed }, NekoStatic::needEntertaiment{ maxNeed };
    float NekoStatic::hungerInSecond{ 0.3 }, NekoStatic::thirstInSecond{ 0.3 }, NekoStatic::communicationInSecond{ 0.2 }, NekoStatic::hygieneInSecond{ 0.06 }, NekoStatic::toiletInSecond{ 0.01 }, NekoStatic::energyInSecond{ 0.23 };
}
