//
//  OffsetHelper.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 20/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef OffsetHelper_hpp
#define OffsetHelper_hpp

#include <iostream>
#include <vector>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

// #include <nfd.h>

#include <minEH/Engine/EntitySystem.hpp>
#include <minEH/Engine/Collectors/Image.hpp>
#include <minEH/Engine/Collectors/Font.hpp>

using std::cout;
using std::endl;

using namespace ns;

namespace NekoUI
{
    struct OffsetHelperSettings
    {
        static bool disableSmoothing;
    };
    
    struct BodyPartHelper
    {
        sf::Sprite sprite;
        bool spriteLoaded{ false };
        float scaledPerson{ 1.f }, scaledChibi{ 1.f };
        std::wstring spritepath; bool mode;
        
        sf::Uint8 alpha{ 255 };
        std::pair<int, int> offPerson, offChibi;
        
        BodyPartHelper(const std::wstring& spritepath, const bool& mode = false);
        void Init();
        void Resize(const float& constantScaling, const sf::Vector2f& pos, const float& bodyHeight);
        void UpdatePosition(const float& constantScaling, const sf::Vector2f& pos);
        void Draw(sf::RenderWindow* window);
        void Switch(bool moded);
        void Load(const std::wstring& path);
        void SetAlpha(const sf::Uint8& alpha);
    };
    
    struct OffsetHelper : Component
    {
        sf::Sprite body, sprite;
        std::vector<BodyPartHelper*> parts;
        BodyPartHelper* part;
        bool bodyLoaded{ false }, spriteLoaded{ false };
        
        int editor{ 1 };
        bool mode{ true }, dxAllowed{ false };
        float x{ 0 }, y{ 0 }, dx{ 0 }, dy{ 0 };
        float x1{ 0 }, x2{ 0 }, y1{ 0 }, y2{ 0 }, requiredScaling1{ 0.82f }, requiredScaling2{ 0.82f };
        sf::Vector2f lastMouse{ 0, 0 };
        sf::Vector2f pos{ 0, 0 };
        
        float scaled{ 1.f }, bodyRelative{ 1.f }, constantScaling{ 1.f }, requiredScaling{ 1.f };
        int mousex{ 0 }, mousey{ 0 };
        std::wstring spritepath;
        sf::Text offsetText, modeText, alphaText, constrolsText;
        
        OffsetHelper(const std::wstring& spritepath, const bool& mode = false, const float& requiredScaling = 0.82f);
        void Init() override;
        void PollEvent(sf::Event& event) override;
        void Resize(const unsigned int& width, const unsigned int& height) override;
        void UpdatePosition();
        void Draw(sf::RenderWindow* window) override;
        void UpdateInformation();
    };
}

#endif /* OffsetHelper_hpp */
