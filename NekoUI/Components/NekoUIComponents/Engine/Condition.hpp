//
//  Buff.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 10/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef Condition_hpp
#define Condition_hpp

#include <iostream>
#include <optional>

#include <SFML/Graphics.hpp>
#include <minEH/Engine/Settings.hpp>

using namespace ns;

namespace NekoUI
{
    struct Condition
    {
        std::optional<unsigned int> duration;
        std::wstring display{ L"" };
    };
}

#endif /* Condition_hpp */
