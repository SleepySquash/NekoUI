//
//  NekoInterfaceUI.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 17/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef NekoInterfaceUI_hpp
#define NekoInterfaceUI_hpp

#include <iostream>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include "../../../Essentials/ResourcePath.hpp"
#include "../../../Engine/Settings.hpp"
#include "../../../Engine/Collectors/Font.hpp"
#include "../../../Engine/Collectors/Image.hpp"
#include "../../../Engine/EntitySystem.hpp"
#include "../../../Engine/GUIInterface.hpp"

#include "../Player.hpp"
#include "../NekoEntity.hpp"
#include "../RoomLibrary.hpp"

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
        sf::RectangleShape nintShape; sf::Text nintNameText, nintText;
        sf::Sprite nintNekoSprite; GUI::TextButton nintCloseButton, nintSelectMainButton;
        GUI::SpriteButton quitButton;
        sf::Color nintRarityColor{ sf::Color::White }, nintMoodColor{ sf::Color::White };
        std::wstring nintDescriptionText{ L"" }, nintRarityText{ L"" }, nintMoodText{ L"" };
        float nintYYInfoStart{ 0 };
        bool nintDontDrawPersonNeko{ false };
        sf::Sprite nintBodyNeko, nintHeart; float nintBodyNekoScale{ 1.f };
        GUI::TextButton nintTalkButton, nintWardrobeButton, nintGiftButton, nintActionButton;
        
        enum modeEnum { appearing, existing, disappearing };
        modeEnum mode{ appearing }; sf::Uint8 alpha{ 0 };
        float currentTime{ 0.f }, appearTime{ 0.3f }, disappearTime{ 0.2f };
        
        void Init() override;
        void Destroy() override;
        void Update(const sf::Time& elapsedTime) override;
        void CleanUp();
        void PollEvent(sf::Event& event) override;
        void Resize(unsigned int width, unsigned int height) override;
        void Draw(sf::RenderWindow* window) override;
        void RecieveMessage(MessageHolder& message) override;
        void Switch(const bool& on);
        void OpenNekoInterface(NekoEntity* entity);
        void UpdateAlpha();
    };
}

#endif /* NekoInterfaceUI_hpp */
