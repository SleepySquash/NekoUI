//
//  ContainerEntity.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 02.02.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#ifndef ContainerEntity_hpp
#define ContainerEntity_hpp

#include <iostream>

#include <SFML/Graphics.hpp>

#include "../../../Engine/Settings.hpp"
#include "../../../Engine/Collectors.hpp"

#include "../Database/ItemDB.hpp"
#include "RoomEntity.hpp"
#include "RoomLibrary.hpp"

using std::cin;
using std::cout;
using std::endl;

using namespace ns;

namespace NekoUI
{
    struct ContainerEntity : RoomEntity
    {
        float scale{ 1.f };
        std::wstring spriteName;
        bool loaded{ false }, drawShadow{ true };
        int shadowOffsetYY{ 0 };
        sf::Sprite shadow;
        
        InventoryBase<Item> container;
        
        void Init() override;
        void Destroy() override;
        void Resize() override;
        void UpdatePosition() override;
        void Draw(sf::RenderWindow* window) override;
        void LoadSprite(const std::wstring& filename, const bool& smoothTexture = false);
        void Save(std::wofstream& wof) override;
    };
}

#endif /* ContainerEntity_hpp */
