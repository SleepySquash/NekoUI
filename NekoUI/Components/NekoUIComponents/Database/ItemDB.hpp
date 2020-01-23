//
//  ItemDB.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 20/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef ItemDB_hpp
#define ItemDB_hpp

#include <iostream>
#include <list>
#include <unordered_map>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../../../Essentials/ResourcePath.hpp"
#include "../../../Engine/Settings.hpp"

#include "../Engine/Item.hpp"

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;

namespace NekoUI
{
    namespace idc
    {
        struct Cupcake : Item { Cupcake() : Item("Cupcake", L"Вкусный пирожочек с вишенкой.", ItemType::Food)
            { display = L"Пирожок"; calories = 150; thirstSatisfuction = -50; } };
        struct Cake : Item { Cake() : Item("Cake", L"Шоколадный тортик с вишенкой наверху.", ItemType::Food, 10)
            { display = L"Тортик"; calories = 700; thirstSatisfuction = -350; scale = 1.1f; } };
        struct Candy : Item { Candy() : Item("Candy", L"М-м, тот самый ностальгический вкус.", ItemType::Food)
            { display = L"Конфетка"; calories = 50; thirstSatisfuction = -15; scale = 0.5; } };
        struct Star : Item { Star() : Item("Star", L"Звёздочка с неба.", ItemType::Other, 90) { display = L"Звезда"; } };
        struct Icecream : Item { Icecream() : Item("Icecream", L"Приятно тает во рту.", ItemType::Food)
            { display = L"Мороженка"; calories = 150; thirstSatisfuction = 50; } };
        struct Donut : Item { Donut() : Item("Donut", L"Шоколадный пончик в клубничной глазури.", ItemType::Food)
            { display = L"Пончик"; calories = 100; thirstSatisfuction = -20; scale = 0.75f; } };
        struct FractionOfPie : Item { FractionOfPie() : Item("FractionOfPie", L"Кусочек клубничного панкейка.", ItemType::Food)
            { display = L"Кусок тортика"; calories = 200; thirstSatisfuction = -100; } };
        struct Lootbox : Item { Lootbox() : Item("Lootbox", L"Интересно, что внутри?", ItemType::Other, 100, true)
            { display = L"Лутбокс"; scale = 3; } };
        struct WaterBottle : Item { WaterBottle() : Item("WaterBottle", L"Освежающая водичка идеальной температуры из самой-самой чистой скважины.", ItemType::Drink)
            { display = L"Бутылка воды"; calories = 0; thirstSatisfuction = 660; scale = 1.1; } };
        
        struct MaidUniform : Wearable
        {
            MaidUniform() : Wearable("MaidUniform", L"maid uniform.png", L"maid uniform.png", L"Костюм мейдочки.")
            {
                display = L"Униформа мейдочки"; scale = 1.5f; depth = 60; clothing = ClothType::Onepiece;
                relativeChibiScale  = 0.329595; offsets.first = {-28, -427};
                relativePersonScale = 0.455800; offsets.second = {27, -659};
            }
        };
        struct MaidHeadwear : Wearable
        {
            MaidHeadwear() : Wearable("MaidHeadwear", L"headband.png", L"headband.png", L"Ободок мейдочки.")
            {
                display = L"Обруч мейдочки"; scale = 0.8f; depth = 90; clothing = ClothType::Head;
                relativeChibiScale  = 0.24;  offsets.first = {17, -820};
                relativePersonScale = 0.074; offsets.second = {-6, -795};
            }
        };
        struct MaidGloves : Wearable
        {
            MaidGloves() : Wearable("MaidGloves", L"handcliff.png", L"handcliff.png", L"Аккуратные нежные перчаточки~ <3")
            {
                display = L"Перчатки мейдочки"; depth = 75; clothing = ClothType::Gloves;
                relativeChibiScale  = 0.045; offsets.first = {-1, -264};
                relativePersonScale = 0.032333; offsets.second = {84, -693};
            }
        };
        struct WhiteStockings : Wearable
        {
            WhiteStockings() : Wearable("WhiteStockings", L"stockings.png", L"stockings.png", L"Белые, как снежок, чулочки.")
            {
                display = L"Белые чулочки"; depth = 10; clothing = ClothType::Socks;
                relativeChibiScale  = 0.180708; offsets.first = {-19, -149};
                relativePersonScale = 0.425710; offsets.second = {-24, -341};
            }
        };
        struct BlackStockings : Wearable
        {
            BlackStockings() : Wearable("BlackStockings", L"stockings.png", L"stockings.png", L"Чёрные, как ночка, чулочки.")
            {
                display = L"Чёрные чулочки"; depth = 10; clothing = ClothType::Socks;
                relativeChibiScale  = 0.180708; offsets.first = {-19, -149};
                relativePersonScale = 0.425710; offsets.second = {-24, -341};
                chibiColor = personColor = sf::Color(40, 40, 40);
            }
        };
        struct MaidShoes : Wearable
        {
            MaidShoes() : Wearable("MaidShoes", L"shoes.png", L"shoes.png", L"Словно хрустальные.")
            {
                display = L"Туфельки"; depth = 15; clothing = ClothType::Legwear;
                relativeChibiScale  = 0.024013; offsets.first = {-17, -19};
                relativePersonScale = 0.145778; offsets.second = {-30, -110};
            }
        };
        struct NekoCollar : Wearable
        {
            NekoCollar() : Wearable("NekoCollar", L"collar.png", L"collar.png", L"Для самой лучшей кошечки.")
            {
                display = L"Неко ошейник"; depth = 10; clothing = ClothType::Accessory;
                relativeChibiScale  = 0.027470; offsets.first = {-20, -418};
                relativePersonScale = 0.049989; offsets.second = {-26, -655};
            }
        };
    }
    
    void ItemDatabaseCollection_LoadItems();
    struct ItemsBase
    {
        bool savingIsRequired{ false };
        std::list<std::pair<Item*, int>> list;
        void Push(const std::string& item, unsigned int count = 1);
        void Sort();
        void Add(const std::string& item, unsigned int count = 1);
        void Remove(const std::string& item, unsigned int count = 1);
        void Clear();
    };
    
    template<typename T> struct InventoryBase;
    struct Inventory
    {
        static std::unordered_map<std::string, Item*> map;
        static InventoryBase<Item> items, fridge;
        static InventoryBase<Wearable> wardrobeHead, wardrobeTop, wardrobeBottom, wardrobeOnepiece, wardrobeUnderwear, wardrobeSocks, wardrobeShoes, wardrobeAccessories;
        
        static void SaveInventory(bool forced = false);
        static void SaveFridge(bool forced = false);
        static void SaveWardrobe(bool forced = false);
        
        static void LoadInventory();
        static void LoadFridge();
        static void LoadWardrobe();
        
        static bool FridgeContainsAnyOfType(const ItemType& type);
    };
    
    template<typename T> struct InventoryBase
    {
        bool savingIsRequired{ false };
        std::list<std::pair<T*, int>> list;
        
        void Push(const std::string& item, unsigned int count = 1)
        {
            auto it = Inventory::map.find(item); if (it == Inventory::map.end()) return;
            for (auto itin = list.begin(); itin != list.end(); ++itin)
                if (itin->first == it->second) { itin->second += count; savingIsRequired = true; return; }
            T* reint = reinterpret_cast<T*>(it->second);
            if (reint) { list.push_back({reint, count}); savingIsRequired = true; }
        }
        void Add(const std::string& item, unsigned int count = 1)
        {
            auto it = Inventory::map.find(item); if (it == Inventory::map.end()) return;
            for (auto itin = list.begin(); itin != list.end(); ++itin)
                if (itin->first == it->second) { itin->second += count; savingIsRequired = true; return; }
            T* reint = reinterpret_cast<T*>(it->second);
            if (reint) { list.push_back({reint, count}); savingIsRequired = true; Sort(); }
        }
        void Remove(const std::string& item, unsigned int count = 1)
        {
            auto it = Inventory::map.find(item); if (it == Inventory::map.end()) return;
            for (auto itin = list.begin(); itin != list.end(); ++itin)
                if (itin->first == it->second)
                {
                    if (itin->second > count) itin->second -= count; else list.erase(itin);
                    savingIsRequired = true; return;
                }
        }
        void Sort() { list.sort([](const std::pair<T*, int> a, const std::pair<T*, int> b) { return a.first->rarity > b.first->rarity; }); }
        void Clear() { list.clear(); }
    };
}

#endif /* ItemDB_hpp */
