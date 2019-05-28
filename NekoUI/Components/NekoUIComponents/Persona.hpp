//
//  Persona.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 20/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef Persona_hpp
#define Persona_hpp

#include <iostream>
#include <vector>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../../Essentials/ResourcePath.hpp"
#include "../../Engine/Settings.hpp"
#include "../../Engine/Collectors.hpp"

#include "Item.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::pair;
using ns::base::utf8;
using ns::base::utf16;

using namespace ns;

namespace NekoUI
{
    struct Cloth
    {
        bool personLoaded{ false }, chibiLoaded{ false }, imOnMyOwn{ false }, owner{ true };
        sf::Sprite person, chibi;
        
        std::wstring personPath, chibiPath;
        float localPersonScale{ 1.f }, localChibiScale{ 1.f };
        float relativePersonScale{ 1.f }, relativeChibiScale{ 1.f };
        pair<pair<int, int>, pair<int, int>> offsets{ {0,0}, {0,0} };
        int depth{ 0 };
        
        void Load(const std::wstring& pPath, const std::wstring& cPath);
        
        void setChibiAlpha(const sf::Uint8& alpha);
        void ResizeChibi(const int& relativeBodyHeight);
        void UpdatePositionChibi(const float& x, const float& y);
        void SetOffsetChibi(pair<int, int>);
        
        void setPersonAlpha(const sf::Uint8& alpha);
        void ResizePerson(const int& relativeBodyHeight);
        void SetOffsetPerson(pair<int, int>);
        void UpdatePositionPerson(const float& x, const float& y);
        
        void Draw(sf::RenderWindow* window, bool mode = true);
    };
    
    struct Persona
    {
        float x{ 0 }, y{ 0 };
        float personScale{ 1.f }, chibiScale{ 1.f };
        
        Cloth face, hair, body;
        vector<Cloth*> cloth, top, bottom, gloves, bra, pantsu, socks, legwear, accessory;
        
        ~Persona();
        void Init();
        
        void setChibiAlpha(const sf::Uint8& alpha);
        void setChibiPosition(float x, float y);
        void ResizeChibi();
        
        void setPersonAlpha(const sf::Uint8& alpha);
        void setPersonPosition(float x, float y);
        void ResizePerson();
        
        void Draw(sf::RenderWindow* window, bool mode = true);
    };
    
    /*struct Cloth
    {
        float x{ 0 }, y{ 0 }, localPersonScale{ 1.f }, localChibiScale{ 1.f };
        
        bool personLoaded{ false }, chibiLoaded{ false };
        sf::Sprite person, chibi;
        
        std::wstring personPath, chibiPath;
        int depth{ 0 };
        
        void Load(const Wearable& wear);
        void Unload();
        void Resize(const float& personScale, const float& chibiScale);
        void Draw(sf::RenderWindow* window, bool mode = true);
    };
    
    struct Persona
    {
        float x{ 0 }, y{ 0 };
        float personScale{ 1.f }, chibiScale{ 1.f };
        
        Cloth face, hair, body;
        vector<Cloth*> cloth, top, bottom, gloves, bra, pantsu, socks, legwear, accessory;
        
        Persona();
        ~Persona();
        void setPosition(float x, float y);
        void DressOn(Wearable* wear);
        void Undress(Wearable* wear);
        void Undress(ClothType type);
        void Resize();
        void Draw(sf::RenderWindow* window, bool mode = true);
    };*/
}

#endif /* Persona_hpp */
