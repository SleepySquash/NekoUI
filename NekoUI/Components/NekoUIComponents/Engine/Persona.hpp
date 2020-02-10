//
//  Persona.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 20/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef Persona_hpp
#define Persona_hpp

#include <iostream>
#include <vector>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <minEH/Essentials/ResourcePath.hpp>
#include <minEH/Engine/Settings.hpp>
#include <minEH/Engine/Collectors.hpp>

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
        bool chibiReversed{ false }, personReversed{ false }, hidden{ false }, offline{ false };
        sf::Sprite person, chibi;
        
        Wearable* item{ nullptr };
        std::wstring personPath, chibiPath;
        float localPersonScale{ 1.f }, localChibiScale{ 1.f };
        float relativePersonScale{ 1.f }, relativeChibiScale{ 1.f };
        pair<pair<float, float>, pair<float, float>> offsets{ {0,0}, {0,0} };
        int depth{ 0 };
        
        void Set(const std::wstring& pPath);
        void Set(const std::wstring& pPath, const std::wstring& cPath);
        void Load(const std::wstring& pPath, const std::wstring& cPath);
        void Destroy();
        
        void setChibiAlpha(const sf::Uint8& alpha);
        void ResizeChibi(const int& relativeBodyHeight);
        void UpdatePositionChibi(const float& x, const float& y);
        void SetOffsetChibi(const pair<float, float>&);
        
        void setPersonAlpha(const sf::Uint8& alpha);
        void ResizePerson(const int& relativeBodyHeight);
        void SetOffsetPerson(const pair<float, float>&);
        void UpdatePositionPerson(const float& x, const float& y);
        
        void Draw(sf::RenderWindow* window, bool mode = true);
    };
    
    struct Persona
    {
        float x{ 0 }, y{ 0 };
        float personScale{ 1.f }, chibiScale{ 1.f };
        int chibiWidth{ 0 }, chibiHeight{ 0 }, personWidth{ 0 }, personHeight{ 0 };
        bool chibiReversed{ false }, personReversed{ false };
        bool savingIsRequired{ false };
        
        Cloth eyebrows, eyes, mouth, nose, blush, fronthair, backhair, tail, arms, body;
        vector<Cloth*> cloth;
        
        ~Persona();
        void Init();
        
        void OccupyMemory();
        void FreeMemory();
        
        void Dress(Wearable* item, bool sorting = true);
        void Undress(Wearable* item);
        void Undress(const ClothType& type);
        void Undress();
        
        void SortClothes();
        void RecalculateBounds(Cloth* ffor = nullptr);
        
        void setChibiAlpha(const sf::Uint8& alpha);
        void setChibiPosition(float x, float y);
        void ResizeChibi();
        
        void setPersonAlpha(const sf::Uint8& alpha);
        void setPersonPosition(float x, float y);
        void ResizePerson();
        
        void Draw(sf::RenderWindow* window, bool mode = true);
    };
}

#endif /* Persona_hpp */
