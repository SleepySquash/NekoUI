//
//  Skill.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 10/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef Skill_hpp
#define Skill_hpp

#include <iostream>
#include <SFML/Graphics.hpp>
#include <minEH/Engine/Settings.hpp>

using namespace ns;

namespace NekoUI
{
    struct Skill
    {
        std::string name;
        std::wstring display;
        unsigned int level{ 0 };
        float exp{ 0 }, expTilNextLevel{ 0 };
        
        Skill(const std::string& name, const unsigned int& level = 0);
    };
}

#endif /* Skill_hpp */
