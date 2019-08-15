//
//  SkillDB.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 10/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "SkillDB.hpp"

namespace NekoUI
{
    std::unordered_map<std::string, Skill*> Skills::map;
    void SkillDatabaseCollection_LoadSkills()
    {
        Skills::map["Movement"] = new Skill("Movement");
        Skills::map["Cooking"] = new Skill("Cooking");
        Skills::map["Computer"] = new Skill("Computer");
        Skills::map["Sitting"] = new Skill("Sitting");
    }
}
