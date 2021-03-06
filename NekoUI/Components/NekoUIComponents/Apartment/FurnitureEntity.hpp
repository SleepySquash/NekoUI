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

#include <SFML/Graphics.hpp>

#include <minEH/Engine/Settings.hpp>
#include <minEH/Engine/Collectors.hpp>
#include <minEH/Engine/Collectors/Shader.hpp>

#include "RoomEntity.hpp"
#include "RoomLibrary.hpp"

using std::cin;
using std::cout;
using std::endl;

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
        
        void Init() override;
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
