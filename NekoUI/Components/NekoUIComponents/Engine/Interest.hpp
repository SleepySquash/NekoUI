//
//  Interest.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 10/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef Interest_hpp
#define Interest_hpp

#include <iostream>
#include <vector>
#include <list>

#include <SFML/Graphics.hpp>

#include <minEH/Engine/Settings.hpp>

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;
using std::list;
using std::vector;

using namespace ns;

namespace NekoUI
{
    struct Interest
    {
        std::string name;
        std::wstring display;
        float liking{ 0 };
        
        Interest(const std::string& name);
    };
}

#endif /* Interest_hpp */
