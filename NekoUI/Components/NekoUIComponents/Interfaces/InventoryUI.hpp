//
//  InventoryUI.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 28/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef InventoryUI_hpp
#define InventoryUI_hpp

#include <iostream>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <minEH/Engine/EntitySystem.hpp>
#include <minEH/Engine/Settings.hpp>
#include <minEH/Engine/Collectors.hpp>
#include <minEH/Engine/GUI/Button/SomeButtons.hpp>

#include "../Engine/Inventory.hpp"
#include "../Apartment/RoomLibrary.hpp"
#include "../Player.hpp"

using std::cin;
using std::cout;
using std::endl;

using namespace ns;

namespace NekoUI
{
    struct InventoryUI : Component
    {
        bool active{ false };
        enum { items, fridge, wardrobe } display{ items };

        sf::Sprite slotSprite; GUI::SpriteButtons itemButtons;
        int slotsXXCount{ 0 }; float startSlotsXX{ 0 };
        sf::Text itemCountText, emptyText;
        
        bool draggingMode{ false }; sf::Sprite draggingSprite;
        std::list<std::pair<Item*, int>>::iterator draggingItem;
        sf::RectangleShape dropzone; bool dropzoneOnRight{ true };
        float elapsedDraggingTime{ 0.f }; bool inAreaToMoveView{ false };
        
        sf::Sprite background; bool spriteLoaded{ false }, isEmpty{ false };;
        float yyZoom{ 0.f };
        
        sf::Vector2i dot;
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
        void Save();
    };
}

#endif /* InventoryUI_hpp */
