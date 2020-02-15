//
//  PlacesInterfaceUI.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 19.01.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#ifndef PlacesInterfaceUI_hpp
#define PlacesInterfaceUI_hpp

#include <minEH/Engine/EntitySystem.hpp>
#include "../../NovelComponents/Novel.hpp"

#include "../Apartment/Apartment.hpp"

#include "Grocery/GroceryUI.hpp"
#include "Misc/StrangeShopkeeperUI.hpp"

#include "Jobs/FNAF/FNAF.hpp"
#include "Jobs/NekoPlace/NekoNinja.hpp"

namespace NekoUI
{
    struct PlacesInterfaceUI : Component
    {
        void ReceiveMessage(MessageHolder& message) override;
    };

    struct PlacesTransitionScreen : Component
    {
        std::string info;
        
        sf::RectangleShape shape; sf::Text text; sf::Uint8 alpha;
        float elapsedFor{ 0.5f }, elapsedSeconds{ 0.f }; int stage{ 0 };
        std::list<Component*>::iterator it;
        
        PlacesTransitionScreen(const std::string& info);
        void Init() override;
        void Destroy() override;
        void Update(const sf::Time& elapsedTime) override;
        void Draw(sf::RenderWindow* window) override;
        void Resize(const unsigned int& width, const unsigned int& height) override;
    };
}

#endif /* PlacesInterfaceUI_hpp */
