//
//  InterestsDB.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 10/08/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef InterestsDB_hpp
#define InterestsDB_hpp

#include <iostream>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "../../../Engine/Settings.hpp"

#include "../Engine/Interest.hpp"

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;

using namespace ns;

namespace NekoUI
{
    void InterestDatabaseCollection_LoadInterests();
    struct Interests
    {
        static std::unordered_map<std::string, Interest*> map;
    };
}

#endif /* InterestsDB_hpp */
