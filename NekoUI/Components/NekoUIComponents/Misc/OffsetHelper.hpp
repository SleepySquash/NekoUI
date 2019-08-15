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

#include "../../../Engine/EntitySystem.hpp"
#include "../../../Engine/Collectors/Image.hpp"
#include "../../../Engine/Collectors/Font.hpp"

using std::cout;
using std::endl;

using namespace ns;

namespace NekoUI
{
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
        void Draw(sf::RenderWindow* window);
        void Switch(bool moded);
    };
    
    struct OffsetHelper : Component
    {
        sf::Sprite body, sprite;
        std::vector<BodyPartHelper*> parts;
        bool bodyLoaded{ false }, spriteLoaded{ false };
        
        float scaled{ 1.f }, bodyRelative{ 1.f }, constantScaling{ 1.f }, requiredScaling{ 0.9 };
        int mousex{ 0 }, mousey{ 0 };
        std::wstring spritepath; bool mode;
        sf::Text text;
        
        OffsetHelper(const std::wstring& spritepath, const bool& mode = false, const float& requiredScaling = 0.82);
        void Init() override;
        void PollEvent(sf::Event& event) override;
        void Resize(unsigned int width, unsigned int height) override;
        void Draw(sf::RenderWindow* window) override;
    };
}

#endif /* OffsetHelper_hpp */
