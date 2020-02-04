//
//  ItemDetails.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 30.01.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#ifndef GroceryItemDetails_hpp
#define GroceryItemDetails_hpp

#include <iostream>
#include "../../../../Engine/EntitySystem.hpp"
#include "../../../../Engine/GUI/RoundedRectangleShape.hpp"
#include "../../../../Engine/GUI/Button/SomeButtons.hpp"
#include "../../Engine/Item.hpp"

using namespace ns;

namespace NekoUI
{
    namespace Places
    {
        struct ItemDetails : Component
        {
            bool clickable{ true }, contained{ false };
            sf::RoundedRectangleShape shape, panelShape;
            sf::Text caption, description, bonuses, qty, price;
            sf::Sprite sprite;
            GUI::RectangleButton pButton, mButton, ppButton, mmButton;
            Item* item;
            int count;
            
            enum modeEnum { appearing, existing, disappearing };
            modeEnum mode{ appearing }; sf::Uint8 alpha{ 0 };
            float currentTime{ 0.f }, appearTime{ 0.3f }, disappearTime{ 0.2f };
            
            ItemDetails(Item* item, int count = 0);
            void Init() override;
            void Destroy() override;
            void Update(const sf::Time& elapsedTime) override;
            void PollEvent(sf::Event& event) override;
            void Resize(unsigned int width, unsigned int height) override;
            void Draw(sf::RenderWindow* window) override;
            void UpdateAlpha();
        };
    }
}

#endif /* GroceryItemDetails_hpp */
