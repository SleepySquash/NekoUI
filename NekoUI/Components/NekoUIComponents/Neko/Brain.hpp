//
//  Brain.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 18.02.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#ifndef Brain_hpp
#define Brain_hpp

#include <iostream>

#include <iostream>
#include <minEH/Engine/EntitySystem.hpp>

#include "Static.hpp"

using namespace ns;

namespace NekoUI
{
    namespace Neko
    {
        struct Brain
        {
            static bool active;
        };
        struct BrainComponent : Component
        {
            void FixedUpdate(const unsigned int& elapsedTime) override;
        };
    }
}

#endif /* Brain_hpp */
