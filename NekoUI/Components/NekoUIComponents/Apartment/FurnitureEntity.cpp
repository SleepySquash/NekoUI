//
//  FurnitureEntity.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 12/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "FurnitureEntity.hpp"

namespace NekoUI
{
    void FurnitureEntity::Init() { type = Type::Furniture; outlineShader = shaderc::LoadShader("Outline"); }
    void FurnitureEntity::Destroy() { if (spriteLoaded) ic::DeleteImage(L"Data/Apartment/Furniture/" + spriteName); }
    void FurnitureEntity::Update(const sf::Time& elapsedTime) { }
    void FurnitureEntity::PollEvent(sf::Event& event) { }
    void FurnitureEntity::Resize()
    {
        sprite.setScale(relScale * Room::scale * gs::scale, relScale * Room::scale * gs::scale);
        sprite.setPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
    }
    void FurnitureEntity::UpdatePosition() { sprite.setPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale); }
    void FurnitureEntity::Draw(sf::RenderWindow* window)
    {
        if (spriteLoaded) {
            if (outlineShader && highlighted)
            {
                outlineShader->setUniform( "textureOffset", 5.0f / static_cast<float>( sprite.getGlobalBounds().width ) );
                outlineShader->setUniform( "texture", sf::Shader::CurrentTexture );
                window->draw(sprite, outlineShader);
            }
            else window->draw(sprite);
        }
    }
    void FurnitureEntity::LoadSprite(const std::wstring &filename, const bool& smoothTexture)
    {
        spriteName = filename;
        sf::Texture* texture = ic::LoadTexture(L"Data/Apartment/Furniture/" + spriteName);
        if ((spriteLoaded = texture))
        {
            sprite.setTexture(*texture, true); texture->setSmooth(smoothTexture);
            sprite.setOrigin(texture->getSize().x/2, texture->getSize().y);
            relScale = (static_cast<float>(gs::relativeHeight)/static_cast<float>(texture->getSize().y)) * 0.5f * furnitureScale;
            height = texture->getSize().y * relScale;
        }
    }
}
