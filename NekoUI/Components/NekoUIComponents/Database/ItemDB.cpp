//
//  ItemDB.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 20/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
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
        Inventory::map.emplace("WaterBottle", new idc::WaterBottle());
        
        Inventory::map.emplace("MaidUniform", new idc::MaidUniform());
        Inventory::map.emplace("MaidHeadwear", new idc::MaidHeadwear());
        Inventory::map.emplace("MaidGloves", new idc::MaidGloves());
        Inventory::map.emplace("WhiteStockings", new idc::WhiteStockings());
        Inventory::map.emplace("BlackStockings", new idc::BlackStockings());
        Inventory::map.emplace("MaidShoes", new idc::MaidShoes());
        Inventory::map.emplace("NekoCollar", new idc::NekoCollar());
        Inventory::map.emplace("PleatedSailorSkirt", new idc::PleatedSailorSkirt());
        Inventory::map.emplace("SailorBlouse", new idc::SailorBlouse());
        Inventory::map.emplace("SailorBlouse_top", new idc::SailorBlouse_top());
    }
    
    
    
    
    
    
    InventoryBase<Item> Inventory::items, Inventory::fridge, Inventory::transition;
    InventoryBase<Wearable> Inventory::wardrobeHead, Inventory::wardrobeTop, Inventory::wardrobeBottom, Inventory::wardrobeOnepiece, Inventory::wardrobeUnderwear, Inventory::wardrobeSocks, Inventory::wardrobeShoes, Inventory::wardrobeAccessories;
    std::unordered_map<std::string, Item*> Inventory::map;
    void ItemsBase::Push(const std::string& item, unsigned int count)
    {
        auto it = Inventory::map.find(item); if (it == Inventory::map.end()) return;
        for (auto itin = list.begin(); itin != list.end(); ++itin)
            if (itin->first == it->second) { itin->second += count; savingIsRequired = true; return; }
        list.push_back({it->second, count}); savingIsRequired = true;
    }
    void ItemsBase::Add(const std::string& item, unsigned int count)
    {
        auto it = Inventory::map.find(item); if (it == Inventory::map.end()) return;
        for (auto itin = list.begin(); itin != list.end(); ++itin)
            if (itin->first == it->second) { itin->second += count; savingIsRequired = true; return; }
        list.push_back({it->second, count}); savingIsRequired = true; Sort();
    }
    void ItemsBase::Remove(const std::string& item, unsigned int count)
    {
        auto it = Inventory::map.find(item); if (it == Inventory::map.end()) return;
        for (auto itin = list.begin(); itin != list.end(); ++itin)
            if (itin->first == it->second)
            {
                if (itin->second > count) itin->second -= count; else list.erase(itin);
                savingIsRequired = true; return;
            }
    }
    void ItemsBase::Sort() { list.sort([](const std::pair<Item*, int> a, const std::pair<Item*, int> b) { return a.first->rarity > b.first->rarity; }); }
    void ItemsBase::Clear() { list.clear(); }
    
    /* template<typename T> void InventoryBase<T>::Push(const std::string& item, unsigned int count)
    {
        auto it = Inventory::map.find(item); if (it == Inventory::map.end()) return;
        for (auto itin = list.begin(); itin != list.end(); ++itin)
            if (itin->first == it->second) { itin->second += count; savingIsRequired = true; return; }
        T* reint = reinterpret_cast<T*>(it->second);
        if (reint) { list.push_back({reint, count}); savingIsRequired = true; }
    }
    template<typename T> void InventoryBase<T>::Add(const std::string& item, unsigned int count)
    {
        auto it = Inventory::map.find(item); if (it == Inventory::map.end()) return;
        for (auto itin = list.begin(); itin != list.end(); ++itin)
            if (itin->first == it->second) { itin->second += count; savingIsRequired = true; return; }
        T* reint = reinterpret_cast<T*>(it->second);
        if (reint) { list.push_back({reint, count}); savingIsRequired = true; Sort(); }
    }
    template<typename T> void InventoryBase<T>::Remove(const std::string& item, unsigned int count)
    {
        auto it = Inventory::map.find(item); if (it == Inventory::map.end()) return;
        for (auto itin = list.begin(); itin != list.end(); ++itin)
            if (itin->first == it->second)
            {
                if (itin->second > count) itin->second -= count; else list.erase(itin);
                savingIsRequired = true; return;
            }
    }
    template<typename T> void InventoryBase<T>::Sort() { list.sort([](const std::pair<T*, int> a, const std::pair<T*, int> b) { return a.first->rarity > b.first->rarity; }); }
    template<typename T> void InventoryBase<T>::Clear() { list.clear(); } */
    


    void Inventory::CalculateWearset(Wearable* item)
    {
        if (item->dependencies && !item->dependencies->calculated)
        {
            for (auto it : item->dependencies->dependencies)
            {
                auto found = Inventory::map.find(it);
                if (found != Inventory::map.end())
                    item->dependencies->set.push_back(reinterpret_cast<Wearable*>((*found).second));
            }
            item->dependencies->calculated = true;
        }
    }

    
    
    void Inventory::SaveInventory(bool forced)
    {
        if (!Inventory::items.savingIsRequired && !forced) return; else Inventory::items.savingIsRequired = false;
        if (!base::FileExists(utf16(documentsPath())))
            base::CreateDirectory(base::utf16(documentsPath()));
        
        std::wofstream wof;
#ifdef _WIN32
        wof.open(base::utf16(documentsPath()) + L"Items.nekoui");
#else
        wof.open(documentsPath() + "Items.nekoui");
#endif
        wof.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        if (wof.is_open())
            for (auto& i : Inventory::items.list)
                wof << i.second << L" " << utf16(i.first->name) << endl;
    }
    void Inventory::SaveFridge(bool forced)
    {
        if (!Inventory::fridge.savingIsRequired && !forced) return; else Inventory::fridge.savingIsRequired = false;
    }
    void Inventory::SaveWardrobe(bool forced)
    {
        if (!Inventory::wardrobeHead.savingIsRequired && !forced) return; else Inventory::wardrobeHead.savingIsRequired = false;
    }
    
    
    
    void Inventory::LoadInventory()
    {
        Inventory::items.savingIsRequired = false;
        Inventory::items.Clear();
        if (base::FileExists(utf16(documentsPath()) + L"Items.nekoui"))
        {
            std::wifstream wifi;
#ifdef _WIN32
            wifi.open(utf16(documentsPath()) + L"Items.nekoui");
#else
            wifi.open(documentsPath() + "Items.nekoui");
#endif
            wifi.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
            
            if (wifi.is_open())
            {
                std::wstring line; nss::CommandSettings command;
                while (!wifi.eof())
                {
                    std::getline(wifi, line); command.Command(line);
                    int count = base::atoi(nss::ParseUntil(command, L' '));
                    if (count)
                    {
                        std::wstring str = nss::ParseUntil(command, L'\0');
                        std::string sname = utf8(str);
                        Inventory::items.Push(sname, static_cast<unsigned int>(count));
                    }
                }
                Inventory::items.Sort();
            }
        }
        if (Inventory::items.list.empty())
        {
            Inventory::items.Push("Cupcake", 2);
            Inventory::items.Push("Star", 5);
            Inventory::items.Push("Lootbox", 1);
            Inventory::items.Push("Icecream", 23);
            Inventory::items.Push("Donut", 6);
            Inventory::items.Sort();
        }
    }
    void Inventory::LoadFridge() { }
    void Inventory::LoadWardrobe()
    {
        Inventory::wardrobeHead.savingIsRequired = false;
        Inventory::wardrobeHead.Clear();
        Inventory::wardrobeTop.Clear();
        Inventory::wardrobeBottom.Clear();
        Inventory::wardrobeOnepiece.Clear();
        Inventory::wardrobeUnderwear.Clear();
        Inventory::wardrobeSocks.Clear();
        Inventory::wardrobeShoes.Clear();
        Inventory::wardrobeAccessories.Clear();
        if (base::FileExists(utf16(documentsPath()) + L"Wardrobe.nekoui"))
        {
            std::wifstream wifi;
#ifdef _WIN32
            wifi.open(utf16(documentsPath()) + L"Wardrobe.nekoui");
#else
            wifi.open(documentsPath() + "Wardrobe.nekoui");
#endif
            wifi.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
            
            InventoryBase<Wearable>* base{ nullptr };
            if (wifi.is_open())
            {
                std::wstring line; nss::CommandSettings command;
                while (!wifi.eof())
                {
                    std::getline(wifi, line); command.Command(line);
                    if (nss::Command(command, L"head")) base = &Inventory::wardrobeHead;
                    else if (nss::Command(command, L"top")) base = &Inventory::wardrobeTop;
                    else if (nss::Command(command, L"bottom")) base = &Inventory::wardrobeBottom;
                    else if (nss::Command(command, L"onepiece")) base = &Inventory::wardrobeOnepiece;
                    else if (nss::Command(command, L"underwear")) base = &Inventory::wardrobeUnderwear;
                    else if (nss::Command(command, L"socks")) base = &Inventory::wardrobeSocks;
                    else if (nss::Command(command, L"shoes")) base = &Inventory::wardrobeShoes;
                    else if (nss::Command(command, L"accessories")) base = &Inventory::wardrobeAccessories;
                    else if (nss::Command(command, L"e")) base = nullptr;
                    else if (base)
                    {
                        std::wstring str = nss::ParseUntil(command, L'\0');
                        std::string sname = utf8(str);
                        base->Push(sname);
                    }
                }
                Inventory::wardrobeHead.Sort();
                Inventory::wardrobeTop.Sort();
                Inventory::wardrobeBottom.Sort();
                Inventory::wardrobeOnepiece.Sort();
                Inventory::wardrobeUnderwear.Sort();
                Inventory::wardrobeSocks.Sort();
                Inventory::wardrobeShoes.Sort();
                Inventory::wardrobeAccessories.Sort();
            }
        }
        else
        {
            Inventory::wardrobeOnepiece.Push("MaidUniform");
            Inventory::wardrobeHead.Push("MaidHeadwear");
            Inventory::wardrobeSocks.Push("WhiteStockings");
            Inventory::wardrobeSocks.Push("BlackStockings");
            Inventory::wardrobeAccessories.Push("MaidGloves");
            Inventory::wardrobeShoes.Push("MaidShoes");
            Inventory::wardrobeAccessories.Push("NekoCollar");
            Inventory::wardrobeBottom.Push("PleatedSailorSkirt");
            Inventory::wardrobeTop.Push("SailorBlouse");
        }
    }

    bool Inventory::FridgeContainsAnyOfType(const ItemType& type)
    {
        for (auto& i : Inventory::items.list)
            if (i.first->type == type) return true;
        return false;
    }
}
