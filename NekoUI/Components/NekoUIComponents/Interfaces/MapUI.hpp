//
//  MapUI.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 28/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef MapUI_hpp
#define MapUI_hpp

#include <iostream>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include <minEH/Essentials/ResourcePath.hpp>
#include <minEH/Engine/Settings.hpp>
#include <minEH/Engine/Collectors/Font.hpp>
#include <minEH/Engine/Collectors/Image.hpp>
#include <minEH/Engine/EntitySystem.hpp>
#include <minEH/Engine/GUI/Button/SomeButtons.hpp>

#include "../Apartment/RoomLibrary.hpp"

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;

namespace NekoUI
{
    struct MapUI : Component
    {
        bool active{ false }, clickable{ false };
        
        sf::RectangleShape blackScreenShape;
        sf::Sprite background; bool spriteLoaded{ false };
        GUI::SpriteButton scrolldownMenu, homeButton, groceryButton, shopkeeperButton, parkButton;
        sf::CircleShape currentPosition; GUI::SpriteButton* buttonPosition{ nullptr };
        
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
        void UpdateAlpha();
    };
}

#endif /* MapUI_hpp */
