//
//  NekoStatic.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 14/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "Static.hpp"

namespace NekoUI
{
    namespace Neko
    {
        unsigned long Static::money{ 0 };
        float Static::affection{ 0 }; /// +Opinion about you
        float Static::maxNeed{ 1000 }, Static::needHunger{ maxNeed }, Static::needThirst{ maxNeed }, Static::needWarmth{ maxNeed }, Static::needCommunication{ maxNeed }, Static::needHygiene{ maxNeed }, Static::needToilet{ maxNeed }, Static::needEnergy{ maxNeed }, Static::needEntertaiment{ maxNeed };
        float Static::hungerInSecond{ 0.3f }, Static::thirstInSecond{ 0.3f }, Static::communicationInSecond{ 0.2f }, Static::hygieneInSecond{ 0.06f }, Static::toiletInSecond{ 0.01f }, Static::energyInSecond{ 0.23f };
        float Static::autoCapHunger{ 120 }, Static::autoCapThirst{ 120 }, Static::autoCapWarmth{ 260 }, Static::autoCapCommunication{ 200 }, Static::autoCapHygiene{ 120 }, Static::autoCapToilet{ 120 }, Static::autoCapEnergy{ 120 }, Static::autoCapEntertaiment{ 320 };
    }
}
