//
//  ItemDetailsUI.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 03/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "ItemDetailsUI.hpp"

namespace NekoUI
{
    void ItemDetailsUI::Init()
    {
        sf::Font* font = fc::GetFont(L"Noteworthy-Bold.ttf");
        if (font) { caption.setFont(*font); description.setFont(*font); }
        description.setLineSpacing(0.6f);
        
        shape.setFillColor({0,0,0, 170});
        shape.setOutlineColor(sf::Color::White);
    }
    void ItemDetailsUI::Destroy() { if (active) CleanUp(); }
    void ItemDetailsUI::Update(const sf::Time& elapsedTime)
    {
        if (!active) return;
        switch (mode)
        {
            case appearing:      //currentTime = appearTime;
                if (currentTime < appearTime) currentTime += elapsedTime.asSeconds();
                if (currentTime >= appearTime) { alpha = 255; currentTime = 0.f; mode = existing; }
                else alpha = (sf::Uint8)(255 * (currentTime / appearTime));
                UpdateAlpha(); gs::requestWindowRefresh = true;
                break;
                
            case disappearing:     //currentTime = disappearTime;
                if (currentTime < disappearTime) currentTime += elapsedTime.asSeconds();
                if (currentTime >= disappearTime) { active = false; CleanUp(); alpha = 0; currentTime = 0.f; mode = existing; }
                else alpha = (sf::Uint8)(255 - (255 * (currentTime / disappearTime)));
                UpdateAlpha(); gs::requestWindowRefresh = true;
                break;
            default: break;
        }
    }
    void ItemDetailsUI::CleanUp() { if (spriteLoaded && item) ic::DeleteImage(utf16("Data/Items/" + item->name + ".png")); item = nullptr; }
    void ItemDetailsUI::PollEvent(sf::Event& event)
    {
        if (!active || !gs::isActiveInterface(this)) return;
        if (((event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased) && !shape.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) || ((event.type == sf::Event::TouchBegan || event.type == sf::Event::TouchEnded) && !shape.getGlobalBounds().contains(event.touch.x, event.touch.y)))
        { entity->SendMessage({"DetailsUI :: Close"}); event = sf::Event(); }
    }
    void ItemDetailsUI::Resize(const unsigned int& width, const unsigned int& height)
    {
        if (!active) return;
        
        float shapeXX{ 380*gs::scale }, shapeYY{ 500*gs::scale };
        if (shapeXX > gs::width) shapeXX = gs::width;
        if (shapeYY > gs::height) shapeYY = gs::height;
        shape.setSize({shapeXX, shapeYY});
        shape.setOutlineThickness(2*gs::scale);
        
        float shapeX{ x - shapeXX/2 }, shapeY{ y - shapeYY/2 };
        if (shapeX < 10*gs::scale) shapeX = 10*gs::scale; else if (shapeX + shapeXX > gs::width - 10*gs::scale)
            shapeX = gs::width - shapeXX - 10*gs::scale;
        if (shapeY < 10*gs::scale + gs::screenOffsetTop) shapeY = 10*gs::scale + gs::screenOffsetTop*1.15f; else if (shapeY + shapeYY > gs::height - 10*gs::scale)
            shapeY = gs::height - shapeYY - 10*gs::scale;
        shape.setPosition(shapeX, shapeY);
        
        sprite.setPosition(shapeX + shapeXX/2, shapeY + shapeYY/6 + 15*gs::scale);
        float spriteScale = (shapeYY/3)/sprite.getLocalBounds().height;
        sprite.setScale(spriteScale, spriteScale);
        if (sprite.getGlobalBounds().width > shape.getGlobalBounds().width)
        {
            spriteScale = shape.getGlobalBounds().width/sprite.getGlobalBounds().width;
            sprite.setScale(spriteScale, spriteScale);
        }
        
        caption.setScale(1, 1);
        caption.setCharacterSize(72*gs::scale);
        caption.setOutlineThickness(2*gs::scale);
        if (caption.getGlobalBounds().width > shape.getGlobalBounds().width) caption.setScale((shape.getGlobalBounds().width - 10*gs::scale)/caption.getGlobalBounds().width, 1);
        caption.setPosition(shape.getGlobalBounds().left + shape.getGlobalBounds().width/2 - caption.getGlobalBounds().width/2, sprite.getGlobalBounds().top + sprite.getGlobalBounds().height);
        
        description.setScale(1, 1);
        description.setCharacterSize(40*gs::scale);
        description.setString(nss::GetStringWithLineBreaks(description, item->description, shape.getGlobalBounds().width - 5*gs::scale));
        description.setPosition(shape.getGlobalBounds().left + shape.getGlobalBounds().width/2 - description.getGlobalBounds().width/2, caption.getGlobalBounds().top + caption.getGlobalBounds().height);
        if (description.getGlobalBounds().height > (shape.getGlobalBounds().height - (description.getGlobalBounds().top - shape.getGlobalBounds().top))) description.setScale(1, (shape.getGlobalBounds().height - (description.getGlobalBounds().top - shape.getGlobalBounds().top))/description.getGlobalBounds().height);
        else description.setPosition(description.getPosition().x, description.getGlobalBounds().top + (shape.getGlobalBounds().height - (description.getGlobalBounds().top - shape.getGlobalBounds().top))/2 - description.getGlobalBounds().height/2 - 30*gs::scale);
    }
    void ItemDetailsUI::Draw(sf::RenderWindow* window)
    {
        if (!active) return;
        window->draw(shape);
        if (spriteLoaded) window->draw(sprite);
        window->draw(caption);
        window->draw(description);
    }
    void ItemDetailsUI::ReceiveMessage(MessageHolder& message)
    {
        if (!active && message.info == "DetailsUI :: Show") Switch(true);
        else if (active && message.info == "DetailsUI :: Close") Switch(false);
        else if (message.info == "DetailsUI :: Switch") Switch(!active);
        else if (message.info == "DetailsUI :: X") x = base::atof(message.additional);
        else if (message.info == "DetailsUI :: Y") y = base::atof(message.additional);
        else if (message.info == "DetailsUI :: Item") {
            if (item) ic::DeleteImage(utf16("Data/Items/" + item->name + ".png"));
            item = reinterpret_cast<Item*>(message.address); }
    }
    void ItemDetailsUI::Switch(const bool& on)
    {
        if (on)
        {
            if (active) mode = appearing;
            else
            {
                gs::PushInterface(this); active = true; mode = appearing; entity->SortAbove(this);
                if (item)
                {
                    sf::Texture* texture = ic::LoadTexture(utf16("Data/Items/" + item->name + ".png"));
                    if ((spriteLoaded = texture))
                    {
                        sprite.setTexture(*texture, true);
                        sprite.setOrigin(texture->getSize().x/2, texture->getSize().y/2);
                    }
                    caption.setString(item->display);
                    //description.setString(nss::GetStringWithLineBreaks(description, item->description, shape.getGlobalBounds().width));
                }
                Resize(gs::width, gs::height);
            }
        }
        else if (!on)
        {
            if (active)
            {
                gs::RemoveInterface(this); active = true; mode = disappearing; Resize(gs::width, gs::height);
                // if (item) ic::DeleteImage(utf16("Data/Items/" + item->name + ".png"));
            } else mode = disappearing;
        }
    }
    void ItemDetailsUI::UpdateAlpha()
    {
        sprite.setColor({sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, alpha});
        shape.setFillColor({shape.getFillColor().r, shape.getFillColor().g, shape.getFillColor().b, static_cast<sf::Uint8>(170 * (alpha/255.f)) });
        shape.setOutlineColor({shape.getOutlineColor().r, shape.getOutlineColor().g, shape.getOutlineColor().b, alpha});
        caption.setFillColor({caption.getFillColor().r, caption.getFillColor().g, caption.getFillColor().b, alpha});
        caption.setOutlineColor({caption.getOutlineColor().r, caption.getOutlineColor().g, caption.getOutlineColor().b, alpha});
    }
}
