//
//  ItemDB.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 20/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "ItemDB.hpp"

namespace NekoUI
{
    void ItemDatabaseCollection_LoadItems()
    {
        Inventory::map.emplace("Cupcake", new idc::Cupcake());
        Inventory::map.emplace("Cake", new idc::Cake());
        Inventory::map.emplace("Candy", new idc::Candy());
        Inventory::map.emplace("Star", new idc::Star());
        Inventory::map.emplace("Icecream", new idc::Icecream());
        Inventory::map.emplace("Donut", new idc::Donut());
        Inventory::map.emplace("FractionOfPie", new idc::FractionOfPie());
        Inventory::map.emplace("Lootbox", new idc::Lootbox());
    }
    
    
    
    
    
    
    InventoryBase Inventory::items, Inventory::fridge, Inventory::wardrobe;
    std::unordered_map<std::string, Item*> Inventory::map;
    void InventoryBase::Push(const std::string& item, unsigned int count)
    {
        auto it = Inventory::map.find(item); if (it == Inventory::map.end()) return;
        if (it->second->count) it->second->count += count;
        else { list.push_back(it->second); it->second->count += count; }
    }
    void InventoryBase::Add(const std::string& item, unsigned int count)
    {
        auto it = Inventory::map.find(item); if (it == Inventory::map.end()) return;
        if (it->second->count) it->second->count += count;
        else { list.push_back(it->second); it->second->count += count; Sort(); }
    }
    void InventoryBase::Remove(const std::string& item, unsigned int count)
    {
        auto it = Inventory::map.find(item); if (it == Inventory::map.end()) return;
        if (it->second->count > count) it->second->count -= count;
        else { list.remove_if([&item](const Item* base){ return (base->name == item); }); it->second->count = 0; }
    }
    void InventoryBase::Sort() { list.sort([](const Item* a, const Item* b) { return a->rarity > b->rarity; }); }
}
