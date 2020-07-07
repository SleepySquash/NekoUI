//
//  Inventory.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 18.02.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#ifndef Inventory_hpp
#define Inventory_hpp

#include <iostream>
#include <fstream>
#include <list>
#include <unordered_map>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <minEH/Essentials/ResourcePath.hpp>
#include <minEH/Essentials/Base.hpp>
#include <minEH/Engine/NovelSomeScript.hpp>
using namespace ns;

#include "../Engine/Item.hpp"

namespace NekoUI
{
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
        static InventoryBase<Item> items, fridge, transition;
        static InventoryBase<Wearable> wardrobeHead, wardrobeTop, wardrobeBottom, wardrobeOnepiece, wardrobeUnderwear, wardrobeSocks, wardrobeShoes, wardrobeAccessories;
        
        static void CalculateWearset(Wearable* item);
        
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
        void Add(const std::string& item, unsigned int count = 1, bool sort = true)
        {
            auto it = Inventory::map.find(item); if (it == Inventory::map.end()) return;
            for (auto itin = list.begin(); itin != list.end(); ++itin)
                if (itin->first == it->second) { itin->second += count; savingIsRequired = true; return; }
            T* reint = reinterpret_cast<T*>(it->second);
            if (reint) { list.push_back({reint, count}); savingIsRequired = true; if (sort) Sort(); }
        }
        void Add(Item* item, unsigned int count = 1, bool sort = true)
        {
            for (auto itin = list.begin(); itin != list.end(); ++itin)
                if (itin->first == item) { itin->second += count; savingIsRequired = true; return; }
            T* reint = reinterpret_cast<T*>(item);
            if (reint) { list.push_back({reint, count}); savingIsRequired = true; if (sort) Sort(); }
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

#endif /* Inventory_hpp */
