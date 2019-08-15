//
//  SkillDB.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 10/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef SkillDB_hpp
#define SkillDB_hpp

#include <iostream>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "../../../Engine/Settings.hpp"

#include "../Engine/Skill.hpp"

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;

using namespace ns;

namespace NekoUI
{
    void SkillDatabaseCollection_LoadSkills();
    struct Skills
    {
        static std::unordered_map<std::string, Skill*> map;
    };
}

#endif /* SkillDB_hpp */
