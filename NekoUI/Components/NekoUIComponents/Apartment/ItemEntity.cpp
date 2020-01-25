//
//  ItemEntity.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 05/08/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "ItemEntity.hpp"

namespace NekoUI
{
    void ItemEntity::Init(Item* item)
    {
        movable = true; this->item = item; type = Type::Item;
        if (item)
        {
            sf::Texture* texture = ic::LoadTexture(utf16("Data/Items/" + item->name + ".png"));
            if ((spriteLoaded = texture))
            {
                sprite.setTexture(*texture, true);
                sprite.setOrigin(texture->getSize().x/2, texture->getSize().y);
                relScale = (static_cast<float>(gs::relativeHeight)/static_cast<float>(texture->getSize().y)) * 0.15 * item->scale;
                height = texture->getSize().y * 0.35 * relScale;
                
                texture = ic::LoadTexture(L"Data/Neko/Chibi/shadow.png");
                if (texture)
                {
                    shadow.setTexture(*texture);
                    shadow.setOrigin({texture->getSize().x/2.f, texture->getSize().y/2.f});
                    shadowOffsetYY = item->shadowOffsetYY;
                }
            }
        }
    }
    void ItemEntity::Destroy() { if (item && spriteLoaded) {
        ic::DeleteImage(utf16("Data/Items/" + item->name + ".png")); ic::LoadTexture(L"Data/Neko/Chibi/shadow.png"); } }
    void ItemEntity::Update(const sf::Time& elapsedTime) { }
    void ItemEntity::PollEvent(sf::Event& event) { }
    void ItemEntity::Resize()
    {
        sprite.setScale(0.35 * relScale * Room::scale * gs::scale, 0.35 * relScale * Room::scale * gs::scale);
        sprite.setPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
        if (drawShadow) {
            shadow.setScale(1.1*sprite.getGlobalBounds().width/shadow.getLocalBounds().width, sprite.getGlobalBounds().width/shadow.getLocalBounds().width);
            shadow.setPosition(sprite.getPosition().x, sprite.getPosition().y + shadowOffsetYY*gs::scale*Room::scale); }
    }
    void ItemEntity::UpdatePosition() {
        sprite.setPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
        if (drawShadow) shadow.setPosition(sprite.getPosition().x, sprite.getPosition().y + shadowOffsetYY*gs::scale*Room::scale); }
    void ItemEntity::Draw(sf::RenderWindow* window) { if (spriteLoaded) { window->draw(shadow); window->draw(sprite); } }
    void ItemEntity::Save(std::wofstream& wof) { wof << (int)type << L" " << x << L" " << y << L" " << count << L" " << utf16(item->name) << endl; }
}
