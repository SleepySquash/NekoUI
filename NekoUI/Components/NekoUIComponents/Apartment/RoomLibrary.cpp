//
//  RoomLibrary.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 18/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "RoomLibrary.hpp"

namespace NekoUI
{
    float Room::x{ 0 }, Room::y{ 0 }, Room::xWidth{ 0 }, Room::yHeight{ 0 }, Room::scale{ 1.f }, Room::roomScale{ 3.3f };
    int Room::width{ 1 }, Room::height{ 1 };
    sf::Image* Room::mask{ nullptr };
    
    bool Room::scrolldownMenuOpened{ false }, Room::drawDatePanel{ true }, Room::drawScrolldownMenu{ true }, Room::drawNeeds{ true }, Room::requestCloseButton{ false };
    bool Room::canOpenNekoUI{ true };
    sf::FloatRect Room::scrolldownMenuBounds;
    
    bool RoomLibrary::Collision(float x, float y)
    {
        if (x <= 0 || y <= 0) return true;
        float xi = floor(x/roomScale), yi = floor(y/roomScale);
        return (xi >= Room::mask->getSize().x || yi >= Room::mask->getSize().y || Room::mask->getPixel(xi, yi).r);
    }
    
    
    
    std::wstring GetMonthStringFromItsNumber(const int& num)
    {
        switch (num)
        {
            case 0: return L"января"; break;
            case 1: return L"февраля"; break;
            case 2: return L"марта"; break;
            case 3: return L"апреля"; break;
            case 4: return L"мая"; break;
            case 5: return L"июня"; break;
            case 6: return L"июля"; break;
            case 7: return L"августа"; break;
            case 8: return L"сентября"; break;
            case 9: return L"октября"; break;
            case 10: return L"ноября"; break;
            case 11: return L"декабря"; break;
            default: return L"unknown"; break;
        }
    }
}
