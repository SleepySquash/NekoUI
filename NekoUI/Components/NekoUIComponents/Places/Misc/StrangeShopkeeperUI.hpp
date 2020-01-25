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

#include "../../../../Essentials/ResourcePath.hpp"
#include "../../../../Engine/Settings.hpp"
#include "../../../../Engine/Collectors/Font.hpp"
#include "../../../../Engine/Collectors/Image.hpp"
#include "../../../../Engine/EntitySystem.hpp"
#include "../../../../Engine/GUI/Button/SomeButtons.hpp"

#include "../../Neko/Static.hpp"
#include "../../Apartment/RoomLibrary.hpp"

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
            void Resize(unsigned int width, unsigned int height) override;
            void Draw(sf::RenderWindow* window) override;
            void RecieveMessage(MessageHolder& message) override;
            void UpdateAlpha();
        };
    }
}

#endif /* StrangeShopkeeperUI_hpp */
