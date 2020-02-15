//
//  Player.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 17/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <iomanip>
#include <sstream>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Neko/Body.hpp"
#include "Engine/Persona.hpp"
#include "Database/ItemDB.hpp"
#include "Database/SkillDB.hpp"
#include "Database/InterestsDB.hpp"
#include "Database/ActivityDB.hpp"

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
        static unsigned long daysTogether; // считаться за "день" будут только дни, когда заходишь в игру.
        static float timePassed;
        
        static std::wstring display, backgroundCover;
        static std::chrono::time_point<std::chrono::system_clock> birthday;
        
        static bool tutorial, noFood, noDrink;
        
        
        static void Init();
        
        static void SaveData();
        static void SaveCurrentDT();
        static void UpdateCurrentDT();
        
        static void SavePersona(bool forced = false);
        static void LoadPersona();
        
        static void Birth();
        static void UpdateDaysTogether();
    };
}

#endif /* Player_hpp */
