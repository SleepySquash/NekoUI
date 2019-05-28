//
//  Player.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 17/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "Player.hpp"

namespace NekoUI
{
    unsigned long Player::daysTogether{ 0 };
    Persona Player::neko;
    
    std::wstring Player::display{ L"Ванилла" };
    Player::Mood Player::mood{ Player::Mood::peaceful };
    std::wstring Player::status{ L"" };
    
    unsigned long Player::money{ 0 };
    unsigned int Player::affection{ 0 }; /// +Opinion about you
    int Player::maxNeed{ 1000 }, Player::needHunger{ maxNeed }, Player::needThirst{ maxNeed }, Player::needWarmth{ maxNeed }, Player::needCommunication{ maxNeed }, Player::needHygiene{ maxNeed }, Player::needToilet{ maxNeed }, Player::needEntertaiment{ maxNeed };
    
    void Player::Init()
    {
        std::wifstream wif;
#ifdef _WIN32
        wif.open(utf16(documentsPath()) + L"Player.nekoui");
#else
        wif.open(documentsPath() + "Settings.nekoninja");
#endif
        wif.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        
        if (wif.is_open())
        {
            std::wstring line;
            nss::CommandSettings command;
            
            while (!wif.eof())
            {
                std::getline(wif, line); command.Command(line);
                if (nss::Command(command, L"dtgr: ")) daysTogether = static_cast<unsigned int>(nss::ParseAsInt(command));
                else if (nss::Command(command, L"m: ")) money = static_cast<unsigned int>(nss::ParseAsInt(command));
                else if (nss::Command(command, L"a: ")) affection = static_cast<unsigned int>(nss::ParseAsInt(command));
                else if (nss::Command(command, L"1: ")) needHunger = nss::ParseAsInt(command);
                else if (nss::Command(command, L"2: ")) needThirst = nss::ParseAsInt(command);
                else if (nss::Command(command, L"3: ")) needWarmth = nss::ParseAsInt(command);
                else if (nss::Command(command, L"4: ")) needCommunication = nss::ParseAsInt(command);
                else if (nss::Command(command, L"5: ")) needHygiene = nss::ParseAsInt(command);
                else if (nss::Command(command, L"6: ")) needToilet = nss::ParseAsInt(command);
                else if (nss::Command(command, L"7: ")) needEntertaiment = nss::ParseAsInt(command);
            }
            
            wif.close();
        }
        
        
        std::wifstream wif1;
#ifdef _WIN32
        wif1.open(utf16(documentsPath()) + L"Neko.nekoui");
#else
        wif1.open(documentsPath() + "Neko.nekoui");
#endif
        wif1.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        
        if (wif1.is_open())
        {
            std::wstring line;
            
            std::getline(wif1, line);
            //topScore = (unsigned int)std::atol(base::utf8(line).c_str());
            std::getline(wif1, line);
            //topStrike = (unsigned int)std::atol(base::utf8(line).c_str());
            std::getline(wif1, line);
            //totalScore = (unsigned int)std::atol(base::utf8(line).c_str());
            std::getline(wif1, line);
            //lootboxes = (unsigned int)std::atol(base::utf8(line).c_str());
            
            wif1.close();
        }
        
        
        std::wifstream wif2;
#ifdef _WIN32
        wif2.open(utf16(documentsPath()) + L"Room.nekoui");
#else
        wif2.open(documentsPath() + "Room.nekoui");
#endif
        wif2.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        if (wif2.is_open())
        {
            std::wstring line;
            while (!wif2.eof())
            {
                std::getline(wif2, line);
            }
        }
        
        
        
        
        ItemDatabaseCollection_LoadItems();
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
                        std::wstring str = nss::ArgumentAsString(command);
                        std::string sname = utf8(str);
                        Inventory::items.Push(sname, static_cast<unsigned int>(count));
                    }
                }
                Inventory::items.Sort();
            }
        }
    }
    
    void Player::SaveData()
    {
        if (!base::FileExists(utf16(documentsPath())))
            base::CreateDirectory(base::utf16(documentsPath()));
        
        std::wofstream wof;
#ifdef _WIN32
        wof.open(utf16(documentsPath()) + L"Player.nekoui");
#else
        wof.open(documentsPath() + "Player.nekoui");
#endif
        wof.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        if (wof.is_open())
        {
            wof << L"dtgr: " << daysTogether << endl;
            wof << L"m: " << money << endl;
            wof << "a: " << affection << endl;
            wof << "1: " << needHunger << endl;
            wof << "2: " << needThirst << endl;
            wof << "3: " << needWarmth << endl;
            wof << "4: " << needCommunication << endl;
            wof << "5: " << needHygiene << endl;
            wof << "6: " << needToilet << endl;
            wof << "7: " << needEntertaiment << endl;
        }
    }
    void Player::SaveInventory()
    {
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
                wof << i->count << L" " << utf16(i->name) << endl;
    }
}
