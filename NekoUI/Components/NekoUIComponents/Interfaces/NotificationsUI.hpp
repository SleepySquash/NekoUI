//
//  NotificationsUI.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 15.01.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#ifndef NotificationsUI_hpp
#define NotificationsUI_hpp

#include <iostream>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include "../../../Essentials/ResourcePath.hpp"
#include "../../../Engine/Settings.hpp"
#include "../../../Engine/Collectors/Font.hpp"
#include "../../../Engine/Collectors/Image.hpp"
#include "../../../Engine/EntitySystem.hpp"
#include "../../../Engine/GUI/Button/SomeButtons.hpp"

#include "../Apartment/RoomLibrary.hpp"

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;

namespace NekoUI
{
    struct PopupMessage : Component
    {
        bool active{ true };
        
        sf::RectangleShape shape;
        sf::Text text;
        GUI::SpriteButton quitButton;
        
        enum modeEnum { appearing, existing, disappearing };
        modeEnum mode{ appearing }; sf::Uint8 alpha{ 0 };
        float currentTime{ 0.f }, appearTime{ 0.3f }, disappearTime{ 0.2f };
        
        PopupMessage(const std::wstring& message);
        void Init() override;
        void Update(const sf::Time& elapsedTime) override;
        void PollEvent(sf::Event& event) override;
        void Resize(unsigned int width, unsigned int height) override;
        void Draw(sf::RenderWindow* window) override;
        void RecieveMessage(MessageHolder& message) override;
        void Close();
        void UpdateAlpha();
    };

    struct NotificationMessage : Component
    {
        enum modeEnum { appearing, existing, disappearing };
        modeEnum mode{ appearing }; sf::Uint8 alpha{ 0 };
        float currentTime{ 0.f }, appearTime{ 0.3f }, disappearTime{ 0.2f };
    };

    struct NotificationsUI : Component
    {
        bool active{ true };
        
        void AddPopupMessage(const std::wstring& message);
        void RecieveMessage(MessageHolder& message) override;
    };
}

#endif /* NotificationsUI_hpp */
