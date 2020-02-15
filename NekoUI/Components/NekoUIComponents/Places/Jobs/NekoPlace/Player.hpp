//
//  NekoLibrary.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 25/02/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <list>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <minEH/minEH.hpp>

#include "NekoBase.hpp"
#include "NekoLibrary.hpp"
#include "NekoDatabaseCollection.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::list;
using ns::base::utf8;
using ns::base::utf16;

using namespace ns;

namespace NekoNinja
{
    struct Player
    {
        static Player* self;
        
        bool inited{ false };
        std::wstring username{ L"Хозяин228" };
        
        unsigned int level{ 1 };
        unsigned int exp{ 0 }, expNeeded{ 100 }, lootboxes{ 0 };
        
        NekoBase* neko{ nullptr };
        vector<NekoBase*> passiveNeko;
        unsigned long totalScore{ 0 };
        unsigned int topScore{ 0 }, topStrike{ 0 }, maxlifes{ 3 };
        
        float expRatio{ 0 };
        
        bool roomUnlocked{ false }, activeNekoUnlocked{ false };
        int passiveNekosUnlocked{ 0 };
        
        std::wstring settingsPath{ base::utf16(documentsPath()) + L"Settings.nekoninja" };
        std::wstring nekoPath{ base::utf16(documentsPath()) + L"Neko.nekoninja" };
        std::wstring scoresPath{ base::utf16(documentsPath()) + L"Progress.nekoninja" };
        
        Player();
        ~Player();
        void Init();
        void SaveData();
        void SaveNekos();
        void ResetNekos();
        void AddExperience(unsigned int xp);
    };
}

#endif /* Player_hpp */
