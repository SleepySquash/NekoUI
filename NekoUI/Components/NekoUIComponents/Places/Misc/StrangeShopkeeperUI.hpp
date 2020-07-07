//
//  StrangeShopkeeperUI.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 25.01.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#ifndef StrangeShopkeeperUI_hpp
#define StrangeShopkeeperUI_hpp

#include <iostream>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include <minEH/Essentials/ResourcePath.hpp>
#include <minEH/Engine/Settings.hpp>
#include <minEH/Engine/Collectors/Font.hpp>
#include <minEH/Engine/Collectors/Image.hpp>
#include <minEH/Engine/EntitySystem.hpp>
#include <minEH/Engine/GUI/Button/SomeButtons.hpp>

#include "../../Neko/Static.hpp"
#include "../../Apartment/RoomLibrary.hpp"
#include "../../Engine/Inventory.hpp"

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;

namespace NekoUI
{
    namespace Places
    {
        struct StrangeShopkeeperUI : Component
        {
            bool active{ true };
            
            GUI::TextButton shopButton, talkButton, goButton;
            sf::Uint8 alpha{ 255 };
            
            sf::Sprite background;
            bool spriteLoaded{ false };
            
            void Init() override;
            void Destroy() override;
            void Update(const sf::Time& elapsedTime) override;
            void PollEvent(sf::Event& event) override;
            void Resize(const unsigned int& width, const unsigned int& height) override;
            void Draw(sf::RenderWindow* window) override;
            void ReceiveMessage(MessageHolder& message) override;
            void UpdateAlpha();
        };
    }
}

#endif /* StrangeShopkeeperUI_hpp */
