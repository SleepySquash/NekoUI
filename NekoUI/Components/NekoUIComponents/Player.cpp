//
//  Player.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 17/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "Player.hpp"

namespace NekoUI
{
    unsigned long Player::daysTogether{ 0 };
    Persona Player::neko;
    
    std::wstring Player::display{ L"Ванилла" };
    std::chrono::time_point<std::chrono::system_clock> Player::birthday;
    
    NekoStatic::EyebrowsEmotion Player::eyebrowsEmotion{ NekoStatic::EyebrowsEmotion::Normal };
    NekoStatic::EyesEmotion Player::eyesEmotion{ NekoStatic::EyesEmotion::Normal };
    NekoStatic::MouthEmotion Player::mouthEmotion{ NekoStatic::MouthEmotion::Smile };
    
    void Player::Init()
    {
        std::wifstream wif;
#ifdef _WIN32
        wif.open(utf16(documentsPath()) + L"Player.nekoui");
#else
        wif.open(documentsPath() + "Player.nekoui");
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
                else if (nss::Command(command, L"m: ")) NStat::money = static_cast<unsigned int>(nss::ParseAsInt(command));
                else if (nss::Command(command, L"a: ")) NStat::affection = nss::ParseAsFloat(command);
                else if (nss::Command(command, L"1: ")) NStat::needHunger = nss::ParseAsFloat(command);
                else if (nss::Command(command, L"2: ")) NStat::needThirst = nss::ParseAsFloat(command);
                else if (nss::Command(command, L"3: ")) NStat::needWarmth = nss::ParseAsFloat(command);
                else if (nss::Command(command, L"4: ")) NStat::needCommunication = nss::ParseAsFloat(command);
                else if (nss::Command(command, L"5: ")) NStat::needHygiene = nss::ParseAsFloat(command);
                else if (nss::Command(command, L"6: ")) NStat::needToilet = nss::ParseAsFloat(command);
                else if (nss::Command(command, L"7: ")) NStat::needEnergy = nss::ParseAsFloat(command);
                else if (nss::Command(command, L"8: ")) NStat::needEntertaiment = nss::ParseAsFloat(command);
            }
            
            wif.close();
        }
        
        
        
        
        ActivityDatabaseCollection_LoadActivities();
        ItemDatabaseCollection_LoadItems();
        Inventory::LoadInventory();
        Inventory::LoadFridge();
        Inventory::LoadWardrobe();
        
        LoadPersona();
        
        
        
        auto nowatime = std::chrono::system_clock::now(), beforetime = nowatime;
        std::ifstream wif3;
#ifdef _WIN32
        wif3.open(utf16(documentsPath()) + L"dt", std::ios::binary);
#else
        wif3.open(documentsPath() + "dt", std::ios::binary);
#endif
        wif3.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        if (wif3.is_open())
        {
            std::chrono::system_clock::rep file_time_rep;
            if (wif3.read(reinterpret_cast<char*>(&file_time_rep), sizeof file_time_rep))
            {
                std::chrono::system_clock::time_point const cache_valid_time{ std::chrono::system_clock::duration{ file_time_rep } };
                beforetime = cache_valid_time;
                time_t const localtime = std::chrono::system_clock::to_time_t(cache_valid_time);
                cout << std::put_time(std::localtime(&localtime), "%F %T") << endl;
            }
        }
        if (nowatime != beforetime)
        {
            std::chrono::duration<double> diff = nowatime - beforetime;
            cout << "Passed: " << diff.count() << " seconds." << endl;
            float elapsedTime = diff.count()/60.f;
            
            // TODO?: Some simulations based on the time passed.
            
            NStat::needHunger -= elapsedTime * NStat::hungerInSecond;
            NStat::needThirst -= elapsedTime * NStat::thirstInSecond;
            NStat::needCommunication -= elapsedTime * NStat::communicationInSecond;
            NStat::needHygiene -= elapsedTime * NStat::hygieneInSecond;
            NStat::needToilet -= elapsedTime * NStat::toiletInSecond;
            NStat::needEnergy -= elapsedTime * NStat::energyInSecond;
            if (NStat::needHunger < 0) NStat::needHunger = 0;
            if (NStat::needThirst < 0) NStat::needThirst = 0;
            if (NStat::needCommunication < 0) NStat::needCommunication = 0;
            if (NStat::needHygiene < 0) NStat::needHygiene = 0;
            if (NStat::needToilet < 0) NStat::needToilet = 0;
            if (NStat::needEnergy < 0) NStat::needEnergy = 0;
        }
        SaveCurrentDT();
        
        
        
        std::ifstream wifbd; birthday = nowatime;
#ifdef _WIN32
        wifbd.open(utf16(documentsPath()) + L"bd", std::ios::binary);
#else
        wifbd.open(documentsPath() + "bd", std::ios::binary);
#endif
        wifbd.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        if (wifbd.is_open())
        {
            std::chrono::system_clock::rep file_time_rep;
            if (wifbd.read(reinterpret_cast<char*>(&file_time_rep), sizeof file_time_rep))
            {
                std::chrono::system_clock::time_point const cache_valid_time{ std::chrono::system_clock::duration{ file_time_rep } };
                birthday = cache_valid_time;
            }
        }
        if (birthday != nowatime)
        {
            std::chrono::duration<double> diff = nowatime - birthday;
            daysTogether = diff.count() / 86400;
        }
        else Birth();
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
            wof << L"m: " << NStat::money << endl;
            wof << "a: " << NStat::affection << endl;
            wof << "1: " << NStat::needHunger << endl;
            wof << "2: " << NStat::needThirst << endl;
            wof << "3: " << NStat::needWarmth << endl;
            wof << "4: " << NStat::needCommunication << endl;
            wof << "5: " << NStat::needHygiene << endl;
            wof << "6: " << NStat::needToilet << endl;
            wof << "7: " << NStat::needEnergy << endl;
            // wof << "8: " << needEntertaiment << endl;
        }
    }
    
    
    
    
    void Player::SaveCurrentDT()
    {
        if (!base::FileExists(utf16(documentsPath())))
            base::CreateDirectory(base::utf16(documentsPath()));
        
        std::ofstream wof;
#ifdef _WIN32
        wof.open(base::utf16(documentsPath()) + L"dt", std::ios::binary);
#else
        wof.open(documentsPath() + "dt", std::ios::binary);
#endif
        wof.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        if (wof.is_open())
        {
            /* auto now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now); */
            auto const cache_time = (std::chrono::system_clock::now()).time_since_epoch().count();
            wof.write(reinterpret_cast<char const*>(&cache_time), sizeof cache_time);
            /* std::stringstream stream;
            stream << std::put_time(std::localtime(&now_c), "%F %T");
            wof << utf16(stream.str()) << endl; */
        }
    }
    
    
    
    void Player::SavePersona(bool forced)
    {
        if (!neko.savingIsRequired && !forced) return;
        if (!base::FileExists(utf16(documentsPath())))
            base::CreateDirectory(base::utf16(documentsPath()));
        
        std::wofstream wof;
#ifdef _WIN32
        wof.open(base::utf16(documentsPath()) + L"Persona.nekoui");
#else
        wof.open(documentsPath() + "Persona.nekoui");
#endif
        wof.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        if (wof.is_open())
            for (auto& c : neko.cloth)
                if (c->item)
                    wof << utf16(c->item->name) << endl;
        neko.savingIsRequired = false;
    }
    
    
    
    void Player::LoadPersona()
    {
        neko.Undress();
        if (base::FileExists(utf16(documentsPath()) + L"Persona.nekoui"))
        {
            std::wifstream wifi;
#ifdef _WIN32
            wifi.open(utf16(documentsPath()) + L"Persona.nekoui");
#else
            wifi.open(documentsPath() + "Persona.nekoui");
#endif
            wifi.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
            
            if (wifi.is_open())
            {
                std::wstring line; nss::CommandSettings command;
                while (!wifi.eof())
                {
                    std::getline(wifi, line); command.Command(line);
                    std::wstring str = nss::ParseUntil(command, L'\0');
                    std::string sname = utf8(str);
                    auto it = Inventory::map.find(sname);
                    if (it != Inventory::map.end())
                    {
                        Wearable* wear = reinterpret_cast<Wearable*>((*it).second);
                        if (wear) neko.Dress(wear, false);
                    }
                }
                neko.SortClothes();
            }
        }
        neko.savingIsRequired = false;
    }
    
    
    
    
    void Player::Birth()
    {
        if (!base::FileExists(utf16(documentsPath())))
            base::CreateDirectory(base::utf16(documentsPath()));
        
        std::ofstream wof;
#ifdef _WIN32
        wof.open(utf16(documentsPath()) + L"bd");
#else
        wof.open(documentsPath() + "bd");
#endif
        wof.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        if (wof.is_open())
        {
            auto const cache_time = (std::chrono::system_clock::now()).time_since_epoch().count();
            wof.write(reinterpret_cast<char const*>(&cache_time), sizeof cache_time);
        }
    }
    void Player::UpdateDaysTogether()
    {
        auto nowatime = std::chrono::system_clock::now();
        if (birthday != nowatime)
        {
            std::chrono::duration<double> diff = nowatime - birthday;
            daysTogether = diff.count() / 86400;
        }
    }
    
    
    
    void Player::UpdateNekoEmotion()
    {
        if (eyebrowsEmotion != Neko::eyebrowsEmotion)
        {
            if (Neko::eyebrowsEmotion == NekoStatic::EyebrowsEmotion::DEFAULT)
                NekoEmotionAccordingToMood(RestoringEmotion::Eyebrows);
            eyebrowsEmotion = Neko::eyebrowsEmotion;
            switch (eyebrowsEmotion)
            {
                case NekoStatic::EyebrowsEmotion::Normal: neko.eyebrows.Load(L"eyebrows.png", L"eyebrows.png"); break;
                default: break;
            }
        }
        if (eyesEmotion != Neko::eyesEmotion)
        {
            if (Neko::eyesEmotion == NekoStatic::EyesEmotion::DEFAULT)
                NekoEmotionAccordingToMood(RestoringEmotion::Eyes);
            eyesEmotion = Neko::eyesEmotion;
            switch (eyesEmotion)
            {
                case NekoStatic::EyesEmotion::Normal: neko.eyes.Load(L"eyes.png", L"eyes.png"); break;
                case NekoStatic::EyesEmotion::Closed: neko.eyes.Load(L"eyes_closed1.png", L"eyes_closed1.png"); break;
                case NekoStatic::EyesEmotion::Confused: neko.eyes.Load(L"eyes_confused1.png", L"eyes_confused1.png"); break;
                default: break;
            }
        }
        if (mouthEmotion != Neko::mouthEmotion)
        {
            if (Neko::mouthEmotion == NekoStatic::MouthEmotion::DEFAULT)
                NekoEmotionAccordingToMood(RestoringEmotion::Mouth);
            mouthEmotion = Neko::mouthEmotion;
            switch (mouthEmotion)
            {
                case NekoStatic::MouthEmotion::Smile: neko.mouth.Load(L"mouth.png", L"mouth.png"); break;
                case NekoStatic::MouthEmotion::Neutral: neko.mouth.Load(L"mouth.png", L"mouth.png"); break;
                case NekoStatic::MouthEmotion::Sad: neko.mouth.Load(L"mouth.png", L"mouth.png"); break;
                default: break;
            }
        }
    }
    
    void Player::NekoEmotionAccordingToMood(const RestoringEmotion& restoring)
    {
        switch (Neko::mood)
        {
            case NekoStatic::MoodEnum::Happy:
                if (restoring == RestoringEmotion::Eyebrows) Neko::eyebrowsEmotion = NekoStatic::EyebrowsEmotion::Normal;
                else if (restoring == RestoringEmotion::Eyes) Neko::eyesEmotion = NekoStatic::EyesEmotion::Normal;
                else if (restoring == RestoringEmotion::Mouth) Neko::mouthEmotion = NekoStatic::MouthEmotion::Smile;
                break;
            default: break;
        }
    }
    
    void Player::NekoEmotionsAccordingToMood()
    {
        switch (Neko::mood)
        {
            case NekoStatic::MoodEnum::Happy:
                Neko::eyebrowsEmotion = NekoStatic::EyebrowsEmotion::Normal;
                Neko::eyesEmotion = NekoStatic::EyesEmotion::Normal;
                Neko::mouthEmotion = NekoStatic::MouthEmotion::Smile;
                break;
            default: break;
        }
    }
}
