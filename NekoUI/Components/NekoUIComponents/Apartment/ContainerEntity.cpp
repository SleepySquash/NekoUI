//
//  ContainerEntity.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 02.02.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#include "ContainerEntity.hpp"

namespace NekoUI
{
    void ContainerEntity::Init() { type = Type::ContainerItem; movable = canhover = true; }
    void ContainerEntity::Destroy() { if (loaded) { ic::DeleteImage(L"Data/Items/" + spriteName); ic::LoadTexture(L"Data/Neko/Chibi/shadow.png"); } }
    void ContainerEntity::Resize()
    {
        sprite.setScale(0.35 * relScale * Room::scale * gs::scale, 0.35 * relScale * Room::scale * gs::scale);
        sprite.setPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
        if (drawShadow) {
            shadow.setScale(1.3*sprite.getGlobalBounds().width/shadow.getLocalBounds().width, sprite.getGlobalBounds().width/shadow.getLocalBounds().width);
            shadow.setPosition(sprite.getPosition().x, sprite.getPosition().y + shadowOffsetYY*gs::scale*Room::scale); }
    }
    void ContainerEntity::UpdatePosition() {
        sprite.setPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
        if (drawShadow) shadow.setPosition(sprite.getPosition().x, sprite.getPosition().y + shadowOffsetYY*gs::scale*Room::scale); }
    void ContainerEntity::Draw(sf::RenderWindow* window) { if (loaded) { if (drawShadow) window->draw(shadow); window->draw(sprite); } }
    void ContainerEntity::LoadSprite(const std::wstring& filename, const bool& smoothTexture)
    {
        spriteName = filename;
        sf::Texture* texture = ic::LoadTexture(L"Data/Items/" + filename);
        if ((loaded = texture))
        {
            sprite.setTexture(*texture, true);
            sprite.setOrigin(texture->getSize().x/2, 0.95*texture->getSize().y);
            relScale = (static_cast<float>(gs::relativeHeight)/static_cast<float>(texture->getSize().y)) * scale;
            
            texture = ic::LoadTexture(L"Data/Neko/Chibi/shadow.png");
            if (texture)
            {
                shadow.setTexture(*texture);
                shadow.setOrigin({texture->getSize().x/2.f, texture->getSize().y/2.f});
            }
        }
    }
    void ContainerEntity::Save(std::wofstream& wof)
    {
        wof << (int)type << L" " << x << L" " << y << L" " << spriteName << endl;
        for (auto i : container.list)
            wof << i.second << L" " << utf16(i.first->name) << endl;
        wof << "e" << endl;
    }
}
