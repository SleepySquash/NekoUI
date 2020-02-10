//
//  NekoInterfaceUI.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 17/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef NekoInterfaceUI_hpp
#define NekoInterfaceUI_hpp

#include <iostream>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include <minEH/Essentials/ResourcePath.hpp>
#include <minEH/Engine/Settings.hpp>
#include <minEH/Engine/Collectors/Font.hpp>
#include <minEH/Engine/Collectors/Image.hpp>
#include <minEH/Engine/EntitySystem.hpp>
#include <minEH/Engine/GUI/Button/SomeButtons.hpp>
#include <minEH/Engine/GUI/Blur.hpp>
#include <minEH/Engine/Language.hpp>

#include "../Player.hpp"
#include "../Apartment/NekoEntity.hpp"
#include "../Apartment/RoomLibrary.hpp"

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;

namespace NekoUI
{
    struct NekoInterfaceUI : Component
    {
        bool active{ false };
        
        sf::RectangleShape blackScreenShape;
        NekoEntity* neko{ nullptr };
        bool nekoIsSleeping{ false };
        sf::Texture blurTexture, blurScreenTexture;
        sf::Sprite blurSprite;
        bool blurRendered{ false };
        
        volatile bool applyBlurAlpha{ false };
        sf::Uint8 bluralpha{ 0 };
        float blurCurrentTime{ 0.f }, blurAppearTime{ 0.3f };
        
        bool nintDontDrawPersonNeko{ false };
        sf::Text nameText, affectionStatusText;
        sf::Sprite heartSprite;
        GUI::TextButton talkButton, requestButton, engageButton, actionButton;
        
        enum class Screen { main, talk, talkAboutNeko, talkAboutYourself, request, engage, action } screen{ Screen::main };
        
        sf::RectangleShape choiceBackShape;
        GUI::TextButton talkNekoButton, talkSelfButton, talkInterestButton, talkSkillButton;
        GUI::SpriteButton talkChooseCircle;
        GUI::TextButtons choiceButtons;
        
        bool drawSpeechBubble{ true };
        float elapsedSpeechBubble{ 0.f };
        std::wstring speechBubbleLine{ L"..." };
        sf::Sprite speechBubbleSprite;
        sf::Text speechBubbleText;
        
        enum modeEnum { appearing, existing, disappearing };
        modeEnum mode{ appearing }; sf::Uint8 alpha{ 0 };
        float currentTime{ 0.f }, appearTime{ 0.3f }, disappearTime{ 0.2f };
        
        void Init() override;
        void Destroy() override;
        void Update(const sf::Time& elapsedTime) override;
        void CleanUp();
        void PollEvent(sf::Event& event) override;
        void Resize(const unsigned int& width, const unsigned int& height) override;
        void Draw(sf::RenderWindow* window) override;
        void ReceiveMessage(MessageHolder& message) override;
        void Switch(const bool& on);
        void OpenNekoInterface(NekoEntity* entity);
        void UpdateAlpha();
    };
}

#endif /* NekoInterfaceUI_hpp */
