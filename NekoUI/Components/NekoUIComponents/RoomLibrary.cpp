//
//  RoomLibrary.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 18/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "RoomLibrary.hpp"

namespace NekoUI
{
    Activity::Activity(const std::string name, const float& x, const float& y, const std::wstring& occupyString, const std::wstring& actionString) : name(name), x(x), y(y), occupyString(occupyString), actionString(actionString) { }
    std::vector<Activity> rm::activities{ {"Table", 191, 93, L"(сидит)", L"Присесть рядышком"}, {"Computer", 37, 125, L"(сёрфит Интернет)", L"Посёрфить вместе"}, {"KitchenTable1", 328, 107, L"(смотрит на вкусняшки)", L"Покормить"} };
    
    
    float Room::x{ 0 }, Room::y{ 0 }, Room::xWidth{ 0 }, Room::yHeight{ 0 }, Room::scale{ 1.f }, Room::roomScale{ 3.3f };
    int Room::width{ 1 }, Room::height{ 1 };
    sf::Image* Room::mask{ nullptr };
    
    bool Room::scrolldownMenuOpened{ false }, Room::drawDatePanel{ true }, Room::drawScrolldownMenu{ true }, Room::drawNeeds{ true };
    sf::FloatRect Room::scrolldownMenuBounds;
}
