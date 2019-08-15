//
//  Item.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 20/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef Item_hpp
#define Item_hpp

#include <iostream>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>

#include "../../../Engine/MessageHolder.hpp"

using std::cin;
using std::cout;
using std::endl;

namespace NekoUI
{
    enum class ItemType { Wearable, Food, Drink, Toy, Other };
    struct Item
    {
        std::string name; std::wstring description;
        ItemType type{ ItemType::Other };
        float calories{ 0 }, thirstSatisfuction{ 0 };
        
        std::wstring display{ L"Item" };
        unsigned char rarity; // 0 - 100
        sf::Color color;
        
        bool usable;
        float scale{ 1.f };
        
        Item(const std::string& name = "Item", const std::wstring& description = L"", const ItemType& type = ItemType::Other, const unsigned char& rarity = 0, bool usable = false);
    };
    
    enum class ClothType { Accessory, Head, Top, Bottom, Onepiece, Gloves, Bra, Pantsu, Socks, Legwear };
    struct Wearable : Item
    {
        ClothType clothing{ ClothType::Top };
        
        std::wstring personPath, chibiPath;
        float localPersonScale{ 1.f }, localChibiScale{ 1.f };
        float relativePersonScale{ 1.f }, relativeChibiScale{ 1.f };
        std::pair<std::pair<int, int>, std::pair<int, int>> offsets{ {0,0}, {0,0} };
        int depth{ 0 }; bool dressed{ false };
        
        Wearable(const std::string&, const std::wstring& personPath, const std::wstring& chibiPath, const std::wstring& description = L"", const unsigned char& rarity = 0);
    };
}

#endif /* Item_hpp */
