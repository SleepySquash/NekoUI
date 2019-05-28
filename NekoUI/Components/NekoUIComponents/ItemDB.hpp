//
//  ItemDB.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 20/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef ItemDB_hpp
#define ItemDB_hpp

#include <iostream>
#include <list>
#include <unordered_map>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../../Essentials/ResourcePath.hpp"
#include "../../Engine/Settings.hpp"

#include "Item.hpp"

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;

namespace NekoUI
{
    namespace idc
    {
        struct Cupcake : Item { Cupcake() : Item("Cupcake", L"Вкусный пирожочек с вишенкой.") { display = L"Пирожок"; } };
        struct Cake : Item { Cake() : Item("Cake", L"Шоколадный тортик с вишенкой наверху.", 10) { display = L"Тортик"; } };
        struct Candy : Item { Candy() : Item("Candy", L"М-м, тот самый ностальгический вкус.") { display = L"Конфетка"; } };
        struct Star : Item { Star() : Item("Star", L"Звёздочка с неба.", 90) { display = L"Звезда"; } };
        struct Icecream : Item { Icecream() : Item("Icecream", L"Приятно тает во рту.") { display = L"Мороженка"; } };
        struct Donut : Item { Donut() : Item("Donut", L"Шоколадный пончик в клубничной глазури.") { display = L"Пончик"; } };
        struct FractionOfPie : Item { FractionOfPie() : Item("FractionOfPie", L"Кусочек клубничного панкейка.") { display = L"Кусок тортика"; } };
        struct Lootbox : Item { Lootbox() : Item("Lootbox", L"Интересно, что внутри?", 100, true) { display = L"Лутбокс"; } };
    }
    
    void ItemDatabaseCollection_LoadItems();
    struct InventoryBase
    {
        std::list<Item*> list;
        void Push(const std::string& item, unsigned int count = 1);
        void Sort();
        void Add(const std::string& item, unsigned int count = 1);
        void Remove(const std::string& item, unsigned int count = 1);
    };
    
    struct Inventory
    {
        static std::unordered_map<std::string, Item*> map;
        static InventoryBase items, fridge, wardrobe;
    };
}

#endif /* ItemDB_hpp */
