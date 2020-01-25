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

#include "../Essentials/ResourcePath.hpp"
#include "Settings.hpp"
#include "Collectors/Font.hpp"
#include "Collectors/Image.hpp"
#include "EntitySystem.hpp"
#include "GUI/Button/SomeButtons.hpp"
#include "GUI/RoundedRectangleShape.hpp"

#include "NotificationHolder.hpp"

// #include "../Components/NekoUIComponents/Apartment/RoomLibrary.hpp"

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;

namespace ns
{
    struct PopupMessage : Component
    {
        bool active{ true };
        
        sf::RoundedRectangleShape shape;
        sf::RectangleShape backshape;
        sf::Text text, caption;
        std::wstring textStr;
        GUI::TextButton okButton;
        
        enum modeEnum { appearing, existing, disappearing };
        modeEnum mode{ appearing }; sf::Uint8 alpha{ 0 };
        float currentTime{ 0.f }, appearTime{ 0.3f }, disappearTime{ 0.2f };
        
        PopupMessage(const std::wstring& caption, const std::wstring& message);
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
        
        void RecieveMessage(MessageHolder& message) override;
    };
}

#endif /* NotificationsUI_hpp */
