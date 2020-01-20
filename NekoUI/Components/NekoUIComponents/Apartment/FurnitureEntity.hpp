//
//  FurnitureEntity.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 12/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef FurnitureEntity_hpp
#define FurnitureEntity_hpp

#include <iostream>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include "../../../Essentials/ResourcePath.hpp"
#include "../../../Engine/MessageHolder.hpp"
#include "../../../Engine/Settings.hpp"
#include "../../../Engine/Collectors.hpp"
#include "../../../Engine/Collectors/Shader.hpp"

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
    struct FurnitureEntity : RoomEntity
    {
        float furnitureScale{ 1.f };
        bool spriteLoaded{ false };
        std::wstring spriteName;
        
        bool interactive{ false }, canDropNekoOn{ false }, highlighted{ false };
        
        sf::Shader* outlineShader;
        
        void Init();
        void Destroy() override;
        void Update(const sf::Time& elapsedTime) override;
        void PollEvent(sf::Event& event) override;
        void Resize() override;
        void UpdatePosition() override;
        void Draw(sf::RenderWindow* window) override;
        void LoadSprite(const std::wstring& filename, const bool& smoothTexture = false);
    };
}

#endif /* FurnitureEntity_hpp */
