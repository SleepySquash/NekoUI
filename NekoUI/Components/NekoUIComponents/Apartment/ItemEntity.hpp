//
//  ItemEntity.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 05/08/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef ItemEntity_hpp
#define ItemEntity_hpp

#include <iostream>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include "../../../Essentials/ResourcePath.hpp"
#include "../../../Engine/MessageHolder.hpp"
#include "../../../Engine/Settings.hpp"
#include "../../../Engine/Collectors.hpp"

#include "../Engine/Item.hpp"
#include "RoomEntity.hpp"
#include "RoomLibrary.hpp"

using std::cin;
using std::cout;
using std::endl;

using namespace ns;

namespace NekoUI
{
    struct ItemEntity : RoomEntity
    {
        bool loaded{ false }, drawShadow{ true };
        int count{ 1 }, shadowOffsetYY{ 0 };
        Item* item{ nullptr };
        sf::Sprite shadow;
        
        void Init(Item* item = nullptr);
        void Destroy() override;
        void Resize() override;
        void UpdatePosition() override;
        void Draw(sf::RenderWindow* window) override;
        void Save(std::wofstream& wof) override;
    };
}

#endif /* ItemEntity_hpp */
