//
//  Player.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 17/05/2019.
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

#include "../../Essentials/ResourcePath.hpp"
#include "../../Engine/EntitySystem.hpp"
#include "../../Engine/Settings.hpp"
#include "../../Engine/Collectors.hpp"
#include "../../Engine/GUIInterface.hpp"

#include "Persona.hpp"
#include "ItemDB.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::list;
using ns::base::utf8;
using ns::base::utf16;

using namespace ns;

namespace NekoUI
{
    struct Player
    {
        static Persona neko;
        static unsigned long daysTogether;
        
        static std::wstring display;
        
        enum class Mood { happy, sad, peaceful, desprate, angry, horny };
        static Mood mood;
        static std::wstring status;
        
        static unsigned long money;
        static unsigned int affection; /// +Opinion about you
        static int maxNeed, needHunger, needThirst, needWarmth, needCommunication, needHygiene, needToilet, needEntertaiment;
        
        static void Init();
        static void SaveData();
        static void SaveInventory();
        static void SaveFridge();
        static void SaveWardrobe();
    };
}

#endif /* Player_hpp */
