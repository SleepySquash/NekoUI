//
//  NekoLibrary.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 25/02/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "Player.hpp"

namespace NekoNinja
{
    Player* Player::self = new Player();
    
    Player::Player() { }
    Player::~Player() { SaveData(); if (NekoLibrary::self) { delete NekoLibrary::self; NekoLibrary::self = nullptr; } }
    void Player::Init()
    {
        if (inited) return;
        inited = true;
        
        settingsPath = base::utf16(documentsPath()) + L"Jobs/NekoNinja/Settings.nekoninja";
        nekoPath = base::utf16(documentsPath()) + L"Jobs/NekoNinja/Neko.nekoninja";
        scoresPath = base::utf16(documentsPath()) + L"Jobs/NekoNinja/Progress.nekoninja";
        
        
        
        std::wifstream wif;
#ifdef _WIN32
        wif.open(settingsPath);
#else
        wif.open(base::utf8(settingsPath));
#endif
        wif.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        
        if (wif.is_open())
        {
            std::wstring line;
            
            std::getline(wif, line);
            level = (unsigned int)std::atol(base::utf8(line).c_str());
            std::getline(wif, line);
            exp = (unsigned int)std::atol(base::utf8(line).c_str());
            
            wif.close();
        }
        switch (level)
        {
            case 1: expNeeded = 140; break;
            case 2: expNeeded = 230; break;
            case 3: expNeeded = 320; break;
            case 4: expNeeded = 380; break;
            case 5: expNeeded = 450; break;
            case 6: expNeeded = 520; break;
            case 7: expNeeded = 590; break;
            case 8: expNeeded = 680; break;
            case 9: expNeeded = 700; break;
            case 10: expNeeded = 790; break;
            case 11: expNeeded = 900; break;
            case 12: expNeeded = 1100; break;
            case 13: expNeeded = 1300; break;
            case 14: expNeeded = 1500; break;
            case 15: expNeeded = 1800; break;
            default: expNeeded = 2500; break;
        }
        expRatio = (float)exp/expNeeded;
        
        
        
        
        
        std::wifstream wif1;
#ifdef _WIN32
        wif1.open(scoresPath);
#else
        wif1.open(base::utf8(scoresPath));
#endif
        wif1.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        
        if (wif1.is_open())
        {
            std::wstring line;
            
            std::getline(wif1, line);
            topScore = (unsigned int)std::atol(base::utf8(line).c_str());
            std::getline(wif1, line);
            topStrike = (unsigned int)std::atol(base::utf8(line).c_str());
            std::getline(wif1, line);
            totalScore = (unsigned int)std::atol(base::utf8(line).c_str());
            std::getline(wif1, line);
            lootboxes = (unsigned int)std::atol(base::utf8(line).c_str());
            
            wif1.close();
        }
        
        
        
        
        
        if (NekoLibrary::self) delete NekoLibrary::self;
        NekoLibrary::self = new NekoLibrary();
        NekoDatabaseCollection_LoadNeko();
        std::wstring nekosFolderPath = base::utf16(documentsPath()) + L"Jobs/NekoNinja/Neko/";
        for (auto& n : nl::self->neko)
            if (base::FileExists(nekosFolderPath + n->name + L".neko"))
            {
                std::wifstream wifn;
#ifdef _WIN32
                wifn.open(nekosFolderPath + n->name + L".neko");
#else
                wifn.open(base::utf8(nekosFolderPath + n->name + L".neko"));
#endif
                wifn.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
                
                if (wifn.is_open())
                {
                    std::wstring line; nss::CommandSettings command;
                    while (!wifn.eof())
                    {
                        std::getline(wifn, line); command.Command(line);
                        if (nss::Command(command, L"base "))
                        {
                            n->tamed = true;
                            int pos{ 0 }; std::wstring levelStr = nss::ParseUntil(command, L' ');
                            n->affection = base::atof(levelStr); pos += levelStr.length() + 1;
                        }
                        else n->Load(command);
                    }
                }
            }
        for (auto& n : nl::self->neko) if (n->tamed) {
            if (n->chanceAfterTamed >= 0) n->possibility = n->chanceAfterTamed; }
        //for (auto& n : nl::neko) cout << " " << n->chance << " " << n->possibility << " " << utf8(n->name) << endl;
        if (level >= 6) activeNekoUnlocked = true;
        if (level >= 16) ++passiveNekosUnlocked;
        if (level >= 25) ++passiveNekosUnlocked;
        if (level >= 35) ++passiveNekosUnlocked;
    }
    void Player::SaveData()
    {
        if (!inited) Init();
        if (!base::FileExists(settingsPath)) base::CreateDirectory(base::GetFolderPath(settingsPath));
        
        std::wofstream wof;
#ifdef _WIN32
        wof.open(settingsPath);
#else
        wof.open(base::utf8(settingsPath));
#endif
        wof.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        if (wof.is_open())
        {
            wof << level << endl;
            wof << exp << endl;
        }
    }
    void Player::SaveNekos()
    {
        if (!inited) Init();
        std::wstring nekosFolderPath = base::utf16(documentsPath()) + L"Jobs/NekoNinja/Neko/";
        if (!base::FileExists(nekosFolderPath)) base::CreateDirectory(nekosFolderPath);
        
        std::wofstream wof;
        for (auto& n : nl::self->neko)
            if (n->tamed)
            {
    #ifdef _WIN32
                wof.open(nekosFolderPath + n->name + L".neko");
    #else
                wof.open(base::utf8(nekosFolderPath + n->name + L".neko"));
    #endif
                wof.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
                
                if (wof.is_open())
                {
                    wof << "base " << n->affection << L' ' << endl;
                    n->Save(wof);
                }
                wof.close();
            }
    }
    void Player::ResetNekos()
    {
        std::wstring nekosFolderPath = utf16(documentsPath()) + L"Jobs/NekoPlace/Neko/";
        std::wofstream wof;
        for (auto& n : nl::self->neko)
            if (n->tamed && base::FileExists(nekosFolderPath + n->name + L".neko") && n->name != L"Shigure")
            {
#ifdef _WIN32
                wof.open(nekosFolderPath + n->name + L".neko");
#else
                wof.open(base::utf8(nekosFolderPath + n->name + L".neko"));
#endif
                wof.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
                
                if (wof.is_open()) { wof << " " << endl; }
                wof.close();
            }
    }
    void Player::AddExperience(unsigned int xp)
    {
        if (!inited) Init();
        exp += xp;
        while (exp >= expNeeded)
        {
            exp -= expNeeded;
            ++level;
            ++lootboxes;
        }
        expRatio = (float)exp/expNeeded;
    }
}
