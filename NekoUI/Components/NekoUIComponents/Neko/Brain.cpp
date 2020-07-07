//
//  Brain.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 18.02.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#include "Brain.hpp"

namespace NekoUI
{
    namespace Neko
    {
        bool Brain::active = false;
        void BrainComponent::FixedUpdate(const unsigned int& elapsedTime)
        {
            if (Brain::active)
            {
                
            }
        }
    }
}
