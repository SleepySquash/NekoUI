//
//  InventoryUI.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 28/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef InventoryUI_hpp
#define InventoryUI_hpp

#include <iostream>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../../../Engine/EntitySystem.hpp"
#include "../../../Engine/Settings.hpp"
#include "../../../Engine/Collectors.hpp"
#include "../../../Engine/GUIInterface.hpp"

#include "../ItemDB.hpp"
#include "../RoomLibrary.hpp"

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;

using namespace ns;

namespace NekoUI
{
    struct InventoryUI : Component
    {
        bool active{ false };
        enum { items, fridge, wardrobe } display{ items };
        
        
        sf::Sprite cellSprite;
        GUI::SpriteButton quitButton;
        sf::Sprite background; bool spriteLoaded{ false };
        GUI::SpriteButtons itemButtons;
        float yyZoom{ 0.f };
        
        sf::RectangleShape detailsShape;
        sf::Sprite detailsSprite;
        sf::Text detailsCaption, detailsDescription;
        
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
        void UpdateAlpha();
    };
}

#endif /* InventoryUI_hpp */
