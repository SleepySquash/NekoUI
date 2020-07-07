//
//  Skill.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 10/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "Skill.hpp"

namespace NekoUI
{
    Skill::Skill(const std::string& name, const unsigned int& level) : name(name), display(utf16(name)), level(level) { }
}
