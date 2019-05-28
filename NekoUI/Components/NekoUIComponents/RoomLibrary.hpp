//
//  RoomLibrary.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 18/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef RoomLibrary_hpp
#define RoomLibrary_hpp

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include "../../Engine/Settings.hpp"
#include "../../Engine/Collectors.hpp"

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;

using namespace ns;

namespace NekoUI
{
    struct Activity
    {
        std::string name; float x, y;
        std::wstring occupyString, actionString;
        float duration{ 24.f };
        unsigned int liking{ 1000 };
        
        Activity(const std::string name, const float& x, const float& y, const std::wstring& occupyString = L"", const std::wstring& actionString = L"");
    };
    
    struct RoomLibrary
    {
        static float x, y, xWidth, yHeight, scale, roomScale;
        static int width, height;
        static sf::Image* mask;
        
        static std::vector<Activity> activities;
        
        static bool scrolldownMenuOpened, drawDatePanel, drawScrolldownMenu, drawNeeds;
        static sf::FloatRect scrolldownMenuBounds;
    };
    
    typedef RoomLibrary Room;
    typedef RoomLibrary rm;
}

#endif /* RoomLibrary_hpp */
