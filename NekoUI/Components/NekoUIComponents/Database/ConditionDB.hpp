//
//  BuffDB.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 10/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef ConditionDB_hpp
#define ConditionDB_hpp

#include <iostream>
#include <list>

#include <SFML/Graphics.hpp>
#include <minEH/Engine/Settings.hpp>

#include "../Engine/Condition.hpp"

using namespace ns;

namespace NekoUI
{
    struct Conditions
    {
        static std::list<Condition> list;
    };
}

#endif /* ConditionDB_hpp */
