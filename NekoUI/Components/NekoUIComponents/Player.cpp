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
    float Player::timePassed{ 0 };
    Persona Player::neko;
    
    std::wstring Player::display{ L"Ванилла" }, Player::backgroundCover{ L"cover2.jpg" };
    std::chrono::time_point<std::chrono::system_clock> Player::birthday;

    bool Player::noFood{ true }, Player::noDrink{ true };
    
    NekoS::EyebrowsEmotion Player::eyebrowsEmotion{ NekoS::EyebrowsEmotion::Normal };
    NekoS::EyesEmotion Player::eyesEmotion{ NekoS::EyesEmotion::Normal };
    NekoS::MouthEmotion Player::mouthEmotion{ NekoS::MouthEmotion::Smile };
    
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
                else if (nss::Command(command, L"m: ")) NekoS::money = static_cast<unsigned int>(nss::ParseAsInt(command));
                else if (nss::Command(command, L"a: ")) NekoS::affection = nss::ParseAsFloat(command);
                else if (nss::Command(command, L"1: ")) NekoS::needHunger = nss::ParseAsFloat(command);
                else if (nss::Command(command, L"2: ")) NekoS::needThirst = nss::ParseAsFloat(command);
                else if (nss::Command(command, L"3: ")) NekoS::needWarmth = nss::ParseAsFloat(command);
                else if (nss::Command(command, L"4: ")) NekoS::needCommunication = nss::ParseAsFloat(command);
                else if (nss::Command(command, L"5: ")) NekoS::needHygiene = nss::ParseAsFloat(command);
                else if (nss::Command(command, L"6: ")) NekoS::needToilet = nss::ParseAsFloat(command);
                else if (nss::Command(command, L"7: ")) NekoS::needEnergy = nss::ParseAsFloat(command);
                else if (nss::Command(command, L"8: ")) NekoS::needEntertaiment = nss::ParseAsFloat(command);
            }
            
            wif.close();
        }
        
        
        
        
        ActivityDatabaseCollection_LoadActivities();
        ItemDatabaseCollection_LoadItems();
        Inventory::LoadInventory();
        Inventory::LoadFridge();
        Inventory::LoadWardrobe();
        noFood = !Inventory::FridgeContainsAnyOfType(ItemType::Food);
        noDrink = !Inventory::FridgeContainsAnyOfType(ItemType::Drink);
        
        LoadPersona();
        
        
        
        auto nowatime = std::chrono::system_clock::now();
        UpdateCurrentDT();
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
            wof << L"m: " << NekoS::money << endl;
            wof << "a: " << NekoS::affection << endl;
            wof << "1: " << NekoS::needHunger << endl;
            wof << "2: " << NekoS::needThirst << endl;
            wof << "3: " << NekoS::needWarmth << endl;
            wof << "4: " << NekoS::needCommunication << endl;
            wof << "5: " << NekoS::needHygiene << endl;
            wof << "6: " << NekoS::needToilet << endl;
            wof << "7: " << NekoS::needEnergy << endl;
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
    void Player::UpdateCurrentDT()
    {
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
#ifdef _MSC_VER
                tm* timeinfo = new tm(); localtime_s(timeinfo, &localtime);
                cout << std::put_time(timeinfo, "%F %T") << endl;
#else
                cout << std::put_time(std::localtime(&localtime), "%F %T") << endl;
#endif
            }
        }
        if (nowatime != beforetime)
        {
            std::chrono::duration<double> diff = nowatime - beforetime;
            cout << "Passed: " << diff.count() << " seconds." << endl;
            timePassed = diff.count();
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
        if (eyebrowsEmotion != NekoP::eyebrowsEmotion)
        {
            if (NekoP::eyebrowsEmotion == NekoS::EyebrowsEmotion::DEFAULT)
                NekoEmotionAccordingToMood(RestoringEmotion::Eyebrows);
            eyebrowsEmotion = NekoP::eyebrowsEmotion;
            switch (eyebrowsEmotion)
            {
                case NekoS::EyebrowsEmotion::Normal: neko.eyebrows.Load(L"eyebrows_normal.png", L"eyebrows_normal.png");
                    neko.eyebrows.relativeChibiScale = 0.069701; neko.eyebrows.SetOffsetChibi({14, -654});
                    neko.eyebrows.relativePersonScale = 0.023205; neko.eyebrows.SetOffsetPerson({-9, -741}); break;
                case NekoS::EyebrowsEmotion::Embarrassed: neko.eyebrows.Load(L"eyebrows_embarrass.png", L"eyebrows_embarrass.png");
                    neko.eyebrows.relativeChibiScale = 0.113471; neko.eyebrows.SetOffsetChibi({0, -636});
                    neko.eyebrows.relativePersonScale = 0.035975; neko.eyebrows.SetOffsetPerson({-10, -738}); break;
                default: break;
            }
            neko.eyebrows.ResizeChibi(neko.body.chibi.getGlobalBounds().height);
            neko.eyebrows.UpdatePositionChibi(neko.body.chibi.getPosition().x, neko.body.chibi.getPosition().y);
            neko.eyebrows.ResizePerson(neko.body.person.getGlobalBounds().height);
            neko.eyebrows.UpdatePositionPerson(neko.body.person.getPosition().x, neko.body.person.getPosition().y);
        }
        if (eyesEmotion != NekoP::eyesEmotion)
        {
            if (NekoP::eyesEmotion == NekoS::EyesEmotion::DEFAULT)
                NekoEmotionAccordingToMood(RestoringEmotion::Eyes);
            eyesEmotion = NekoP::eyesEmotion;
            switch (eyesEmotion)
            {
                case NekoS::EyesEmotion::Normal: neko.eyes.Load(L"eyes_normal.png", L"eyes_normal.png");
                    neko.eyes.relativeChibiScale = 0.131615; neko.eyes.SetOffsetChibi({6, -601});
                    neko.eyes.relativePersonScale = 0.042294; neko.eyes.SetOffsetPerson({-11, -729}); break;
                case NekoS::EyesEmotion::Closed: neko.eyes.Load(L"eyes_closed.png", L"eyes_closed.png");
                    neko.eyes.relativeChibiScale = 0.084754; neko.eyes.SetOffsetChibi({-1, -555});
                    neko.eyes.relativePersonScale = 0.021370; neko.eyes.SetOffsetPerson({-17, -712}); break;
                case NekoS::EyesEmotion::ClosedHappy: neko.eyes.Load(L"eyes_closedhappy.png", L"eyes_closedhappy.png");
                    neko.eyes.relativeChibiScale = 0.083918; neko.eyes.SetOffsetChibi({-3, -557});
                    neko.eyes.relativePersonScale = 0.031657; neko.eyes.SetOffsetPerson({-15, -714}); break;
                case NekoS::EyesEmotion::Confused: neko.eyes.Load(L"eyes_confused1.png", L"eyes_confused1.png");
                    neko.eyes.relativeChibiScale = 0.154622; neko.eyes.SetOffsetChibi({2, -611});
                    neko.eyes.relativePersonScale = 0.051925; neko.eyes.SetOffsetPerson({-11, -735}); break;
                case NekoS::EyesEmotion::Embarrassed: neko.eyes.Load(L"eyes_closedembarrass.png", L"eyes_closedembarrass.png");
                    neko.eyes.relativeChibiScale = 0.086685; neko.eyes.SetOffsetChibi({-5, -553});
                    neko.eyes.relativePersonScale = 0.035793; neko.eyes.SetOffsetPerson({-15, -713}); break;
                default: break;
            }
            neko.eyes.ResizeChibi(neko.body.chibi.getGlobalBounds().height);
            neko.eyes.UpdatePositionChibi(neko.body.chibi.getPosition().x, neko.body.chibi.getPosition().y);
            neko.eyes.ResizePerson(neko.body.person.getGlobalBounds().height);
            neko.eyes.UpdatePositionPerson(neko.body.person.getPosition().x, neko.body.person.getPosition().y);
        }
        if (mouthEmotion != NekoP::mouthEmotion)
        {
            if (NekoP::mouthEmotion == NekoS::MouthEmotion::DEFAULT)
                NekoEmotionAccordingToMood(RestoringEmotion::Mouth);
            mouthEmotion = NekoP::mouthEmotion;
            switch (mouthEmotion)
            {
                case NekoS::MouthEmotion::Smile: neko.mouth.Load(L"mouth_smile.png", L"mouth_smile.png");
                    neko.mouth.relativeChibiScale = 0.008141; neko.mouth.SetOffsetChibi({-19, -445});
                    neko.mouth.relativePersonScale = 0.004675; neko.mouth.SetOffsetPerson({-17, -680}); break;
                case NekoS::MouthEmotion::Neutral: neko.mouth.Load(L"mouth_smile.png", L"mouth_smile.png");
                    neko.mouth.relativeChibiScale = 0.008141; neko.mouth.SetOffsetChibi({-19, -445});
                    neko.mouth.relativePersonScale = 0.004675; neko.mouth.SetOffsetPerson({-17, -680}); break;
                case NekoS::MouthEmotion::Dot: neko.mouth.Load(L"mouth_dot.png", L"mouth_dot.png");
                    neko.mouth.relativeChibiScale = 0.008141; neko.mouth.SetOffsetChibi({-19, -445});
                    neko.mouth.relativePersonScale = 0.004675; neko.mouth.SetOffsetPerson({-17, -678}); break;
                case NekoS::MouthEmotion::Open: neko.mouth.Load(L"mouth_open.png", L"mouth_open.png");
                    neko.mouth.relativeChibiScale = 0.055826; neko.mouth.SetOffsetChibi({-20, -473});
                    neko.mouth.relativePersonScale = 0.018589; neko.mouth.SetOffsetPerson({-18, -684}); break;
                case NekoS::MouthEmotion::Sad: neko.mouth.Load(L"mouth.png", L"mouth.png");
                    neko.mouth.relativeChibiScale = 0.131615; neko.mouth.SetOffsetChibi({6, -601});
                    neko.mouth.relativePersonScale = 0.042294; neko.mouth.SetOffsetPerson({-11, -729}); break;
                default: break;
            }
            neko.mouth.ResizeChibi(neko.body.chibi.getGlobalBounds().height);
            neko.mouth.UpdatePositionChibi(neko.body.chibi.getPosition().x, neko.body.chibi.getPosition().y);
            neko.mouth.ResizePerson(neko.body.person.getGlobalBounds().height);
            neko.mouth.UpdatePositionPerson(neko.body.person.getPosition().x, neko.body.person.getPosition().y);
        }
    }
    
    void Player::NekoEmotionAccordingToMood(const RestoringEmotion& restoring)
    {
        switch (NekoP::mood)
        {
            case NekoS::MoodEnum::Happy:
                if (restoring == RestoringEmotion::Eyebrows) NekoP::eyebrowsEmotion = NekoS::EyebrowsEmotion::Normal;
                else if (restoring == RestoringEmotion::Eyes) NekoP::eyesEmotion = NekoS::EyesEmotion::Normal;
                else if (restoring == RestoringEmotion::Mouth) NekoP::mouthEmotion = NekoS::MouthEmotion::Smile;
                break;
            default: break;
        }
    }
    
    void Player::NekoEmotionsAccordingToMood()
    {
        switch (NekoP::mood)
        {
            case NekoS::MoodEnum::Happy:
                NekoP::eyebrowsEmotion = NekoS::EyebrowsEmotion::Normal;
                NekoP::eyesEmotion = NekoS::EyesEmotion::Normal;
                NekoP::mouthEmotion = NekoS::MouthEmotion::Smile;
                break;
            default: break;
        }
    }
}
