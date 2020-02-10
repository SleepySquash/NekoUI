//
//  NovelListener.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 09.02.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#ifndef NovelListener_hpp
#define NovelListener_hpp

#include <iostream>

#include <minEH/Engine/EntitySystem.hpp>
#include "Novel.hpp"

using namespace ns;

namespace NekoUI
{
    struct NovelListener : Component
    {
        void ReceiveMessage(MessageHolder& message) override;
    };
}

#endif /* NovelListener_hpp */
