//
//  Background.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 09.02.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#ifndef Background_hpp
#define Background_hpp

#include <iostream>

#include <minEH/Engine/EntitySystem.hpp>
#include <minEH/Engine/NovelSystem.hpp>
#include <minEH/Engine/Collectors/Image.hpp>

#include "Abstract/Modes.hpp"

using namespace ns;

namespace NekoUI
{
    struct Background : NovelObject
    {
        sf::Sprite sprite;
        std::wstring image;
        
        Mode mode{ Mode::Appear }, switchTo{ Mode::Exist };
        MessageBack messageBack{ MessageBack::AtAppearance };
        bool visible{ true }, loaded{ false };
        sf::Uint8 alpha{ 0 }, maxAlpha{ 255 };
        float currentTime{ 0.f }, appearTime{ 0.6f }, disappearTime{ 0.6f };
        
        Background* hideAfter{ nullptr };
        
        bool doParallax{ gs::isParallaxEnabled };
        float parallaxPower { gs::defaultParallaxBackground };
        float scaleX{ 1.f }, scaleY{ 1.f }, defaultPositionX{ 0.f }, defaultPositionY{ 0.f };
        
        void LoadImage(const std::wstring&);
        void Destroy() override;
        void Update(const sf::Time& elapsedTime) override;
        void PollEvent(sf::Event& event) override;
        void Resize(const unsigned int& width, const unsigned int& height) override;
        void Draw(sf::RenderWindow* window) override;
        void ReceiveMessage(MessageHolder& message) override;
        void CalculateScale(unsigned int width, unsigned int height);
        void CalculateParallax(int mouseX, int mouseY);
        void SetStateMode(const Mode& newMode);
    };
}

#endif /* Background_hpp */
