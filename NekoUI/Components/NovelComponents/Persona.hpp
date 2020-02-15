//
//  Persona.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 13.02.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#ifndef NovelPersona_hpp
#define NovelPersona_hpp

#include <iostream>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <minEH/Essentials/Base.hpp>
#include <minEH/Engine/NovelSystem.hpp>
#include <minEH/Engine/Settings.hpp>
#include <minEH/Engine/NovelSomeScript.hpp>

#include "Abstract/Modes.hpp"
#include "../NekoUIComponents/Engine/Persona.hpp"
#include "../NekoUIComponents/Neko/Static.hpp"

using std::cin;
using std::cout;
using std::endl;
using NekoUI::Persona;

namespace ns
{
    namespace NovelComponents
    {
        struct Persona : NovelObject
        {
            std::wstring state;
            NekoUI::Persona* persona;
            
            bool loaded{ false }, visible{ true };
            sf::Uint8 alpha{ 0 }, maxAlpha{ 255 };
            float currentTime{ 0.f }, appearTime{ 0.6f }, disappearTime{ 0.6f };
            
            Position position { Position::Center };
            float defaultPositionX{ 0.f }, defaultPositionY{ 0.f }, customX{ 0 }, customY{ 0 };
            float scaleX{ 1.f }, scaleY{ 1.f }, scaleRawX{ 1.f }, scaleRawY{ 1.f };
            
            Mode mode{ Mode::Appear }, switchTo{ Mode::Exist };
            MessageBack messageBack{ MessageBack::AtAppearance };
            
            bool doParallax{ gs::isParallaxEnabled };
            float parallaxPower { gs::defaultParallaxNormal };
            
            
            bool blinking{ true }, allowRandomBlink{ true }, itisblink{ false };
            float elapsedBlinking{ 1.f }, blinkingFrequency{ 2.3f }, closedBlinkDuration{ 0.07f };
            NekoUI::NekoS::EyesEmotion blinkEmotion;
            
            
            Persona(NekoUI::Persona* persona = nullptr);
            void Resize(const unsigned int& width, const unsigned int& height) override;
            void Update(const sf::Time& elapsedTime) override;
            void Draw(sf::RenderWindow* window) override;
            void Destroy() override;
            void PollEvent(sf::Event& event) override;
            void CalculateParallax(int mouseX, int mouseY);
            void SetStateMode(const Mode& newMode);
            void SetPosition(const Position& pos, float x = 0, float y = 0);
        };
    }
}

#endif /* NovelPersona_hpp */
