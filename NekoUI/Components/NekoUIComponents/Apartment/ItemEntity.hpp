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
using ns::base::utf8;
using ns::base::utf16;

using namespace ns;

namespace NekoUI
{
    struct ItemEntity : RoomEntity
    {
        bool spriteLoaded{ false };
        Item* item{ nullptr };
        bool drawShadow{ true };
        sf::Sprite shadow; int shadowOffsetYY{ 0 };
        int count{ 1 };
        
        void Init(Item* item = nullptr);
        void Destroy() override;
        void Update(const sf::Time& elapsedTime) override;
        void PollEvent(sf::Event& event) override;
        void Resize() override;
        void UpdatePosition() override;
        void Draw(sf::RenderWindow* window) override;
        void Save(std::wofstream& wof) override;
    };
}

#endif /* ItemEntity_hpp */
