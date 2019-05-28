//
//  Item.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 20/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef Item_hpp
#define Item_hpp

#include <iostream>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>

#include "../../Engine/MessageHolder.hpp"

using std::cin;
using std::cout;
using std::endl;

namespace NekoUI
{
    struct Item
    {
        std::string name; std::wstring display{ L"Item" }, description;
        unsigned char rarity; // 0 - 100
        sf::Color color;
        unsigned int count{ 0 };
        bool usable;
        
        Item(const std::string& name = "Item", const std::wstring& description = L"", const unsigned char& rarity = 0, bool usable = false);
    };
    
    enum class ClothType { Accessory, Top, Bottom, Gloves, Bra, Pantsu, Socks, Legwear };
    struct Wearable : Item
    {
        std::wstring personPath, chibiPath;
        ClothType type{ ClothType::Top };
        float localPersonScale{ 1.f }, localChibiScale{ 1.f };
        
        Wearable(const std::string&, const std::wstring& personPath, const std::wstring& chibiPath, const std::wstring& description = L"", const unsigned char& rarity = 0);
    };
}

#endif /* Item_hpp */
