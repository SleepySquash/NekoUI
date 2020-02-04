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
#include <list>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>

#include "../../../Engine/MessageHolder.hpp"

using std::cin;
using std::cout;
using std::endl;

namespace NekoUI
{
    enum class ItemType { Wearable, Food, Drink, Toy, Container, Other };
    struct Item
    {
        std::string name; std::wstring description;
        ItemType type{ ItemType::Other };
        float calories{ 0 }, thirstSatisfuction{ 0 };
        
        std::wstring display{ L"Item" };
        unsigned char rarity; // 0 - 100
        sf::Color color;
        
        bool usable, sellable{ true };
        int price{ 10 };
        float scale{ 1.f };
        int shadowOffsetYY{ 0 };
        
        Item(const std::string& name = "Item", const std::wstring& description = L"", const ItemType& type = ItemType::Other, const unsigned char& rarity = 0, bool usable = false);
    };
    
    struct WearSet;
    enum class ClothType { Accessory, Head, Top, Bottom, Onepiece, Gloves, Bra, Pantsu, Socks, Legwear, Non };
    struct Wearable : Item
    {
        ClothType clothing{ ClothType::Top };
        
        std::wstring personPath, chibiPath;
        float localPersonScale{ 1.f }, localChibiScale{ 1.f };
        float relativePersonScale{ 1.f }, relativeChibiScale{ 1.f };
        sf::Color chibiColor{ sf::Color::White }, personColor{ sf::Color::White };
        std::pair<std::pair<float, float>, std::pair<float, float>> offsets{ {0,0}, {0,0} };
        int depth{ 0 }; bool dressed{ false };
        
        WearSet* dependencies{ nullptr };
        
        Wearable(const std::string&, const std::wstring& personPath, const std::wstring& chibiPath, const std::wstring& description = L"", const unsigned char& rarity = 0);
        ~Wearable();
        void AddDependency(const std::string& str);
    };

    struct WearSet
    {
        bool calculated{ false };
        std::list<Wearable*> set;
        std::list<std::string> dependencies;
    };
}

#endif /* Item_hpp */
