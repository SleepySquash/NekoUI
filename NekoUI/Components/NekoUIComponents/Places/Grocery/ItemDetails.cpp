//
//  ItemDetails.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 30.01.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#include "ItemDetails.hpp"

namespace NekoUI
{
    namespace Places
    {
        ItemDetails::ItemDetails(Item* item, int count) : item(item), count(count) { }
        void ItemDetails::Init()
        {
            if (!item) { entity->PopComponent(this); return; }
            gs::PushInterface(this); mode = appearing; entity->SortAbove(this);
            
            shape.setFillColor({57, 78, 162});
            shape.setOutlineColor(sf::Color::White);
            shape.setCornerPointCount(14);
            panelShape.setFillColor(sf::Color::Black);
            panelShape.setCornerPointCount(5);
            
            sf::Font* font = fc::GetFont(L"Impact.ttf");
            if (font)
            {
                caption.setFont(*font);
                description.setFont(*font);
                bonuses.setFont(*font);
                qty.setFont(*font);
                price.setFont(*font);
            }
            caption.setString(item->display);
            description.setString(item->description);
            bonuses.setString(L"Голод:      " + std::to_wstring((int)item->calories) + L" ед\nЖажда:   " + std::to_wstring((int)item->thirstSatisfuction) + L" ед");
            bonuses.setFillColor({84, 255, 174});
            price.setString(std::to_string((int)item->price) + " P");
            qty.setString(std::to_string(count));
            
            sf::Texture* texture = ic::LoadTexture(utf16("Data/Items/" + item->name + ".png"));
            if (texture) { sprite.setTexture(*texture, true); sprite.setOrigin(texture->getSize().x/2, texture->getSize().y/2); }
            
            pButton.setString(L"+");
            pButton.setCharacterSize(46);
            pButton.setFont(L"Impact.ttf");
            ppButton.setString(L"++");
            ppButton.setCharacterSize(46);
            ppButton.setFont(L"Impact.ttf");
            mButton.setString(L"-");
            mButton.setCharacterSize(76);
            mButton.setFont(L"Impact.ttf");
            mmButton.setString(L"--");
            mmButton.setCharacterSize(76);
            mmButton.setFont(L"Impact.ttf");
            pButton.halign = ppButton.halign = mButton.halign = mmButton.halign = Halign::Center;
            pButton.valign = ppButton.valign = mButton.valign = mmButton.valign = Valign::Top;
            pButton.drawShape = ppButton.drawShape = mButton.drawShape = mmButton.drawShape = false;
        }
        void ItemDetails::Destroy() { ic::DeleteImage(utf16("Data/Items/" + item->name + ".png")); }
        void ItemDetails::Update(const sf::Time& elapsedTime)
        {
            switch (mode)
            {
                case appearing:
                    if (currentTime < appearTime) currentTime += elapsedTime.asSeconds();
                    if (currentTime >= appearTime) { alpha = 255; currentTime = 0.f; mode = existing; }
                    else alpha = (sf::Uint8)(255 * (currentTime / appearTime));
                    UpdateAlpha(); gs::requestWindowRefresh = true;
                    break;
                    
                case disappearing:
                    if (currentTime < disappearTime) currentTime += elapsedTime.asSeconds();
                    if (currentTime >= disappearTime) { entity->PopComponent(this); return; }
                    else alpha = (sf::Uint8)(255 - (255 * (currentTime / disappearTime)));
                    UpdateAlpha(); gs::requestWindowRefresh = true;
                    break;
                default: break;
            }
        }
        void ItemDetails::PollEvent(sf::Event& event)
        {
            if (!clickable || !gs::isActiveInterface(this)) return;
            if (event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::TouchEnded)
            {
                if (!contained && !shape.getGlobalBounds().contains(gs::lastMousePos.first, gs::lastMousePos.second)) { clickable = false; mode = disappearing; gs::RemoveInterface(this); return; }
            }
            else if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::TouchBegan)
                contained = shape.getGlobalBounds().contains(gs::lastMousePos.first, gs::lastMousePos.second);
            
            if (pButton.PollEvent(event)) { ++count; qty.setString(std::to_string(count)); qty.setPosition(shape.getPosition().x + shape.getSize().x/2 - qty.getGlobalBounds().width/2, shape.getPosition().y + shape.getSize().y - qty.getGlobalBounds().height - 20*gs::scale);
                entity->SendMessage({"++count"}); }
            else if (ppButton.PollEvent(event)) { count += 10; qty.setString(std::to_string(count)); qty.setPosition(shape.getPosition().x + shape.getSize().x/2 - qty.getGlobalBounds().width/2, shape.getPosition().y + shape.getSize().y - qty.getGlobalBounds().height - 20*gs::scale); entity->SendMessage({"count += 10"}); }
            else if (mButton.PollEvent(event) && count > 0) { --count; if (count < 0) count = 0;
                qty.setString(std::to_string(count)); qty.setPosition(shape.getPosition().x + shape.getSize().x/2 - qty.getGlobalBounds().width/2, shape.getPosition().y + shape.getSize().y - qty.getGlobalBounds().height - 20*gs::scale); entity->SendMessage({"--count"}); }
            else if (mmButton.PollEvent(event) && count > 0) { count -= 10; if (count < 0) count = 0;
                qty.setString(std::to_string(count)); qty.setPosition(shape.getPosition().x + shape.getSize().x/2 - qty.getGlobalBounds().width/2, shape.getPosition().y + shape.getSize().y - qty.getGlobalBounds().height - 20*gs::scale); entity->SendMessage({"count -= 10"}); }
            
        }
        void ItemDetails::Resize(unsigned int width, unsigned int height)
        {
            shape.setCornersRadius(170*gs::scale);
            shape.setOutlineThickness(10 * gs::scale);
            shape.setSize({580*gs::scale, 380*gs::scale});
            shape.setPosition(gs::width/2 - shape.getSize().x/2, gs::height/2 - shape.getSize().y/2);
            
            caption.setCharacterSize(50*gs::scale);
            description.setCharacterSize(34*gs::scale);
            qty.setCharacterSize(46*gs::scale);
            price.setCharacterSize(36*gs::scale);
            bonuses.setCharacterSize(36*gs::scale);
            
            caption.setPosition(shape.getPosition().x + shape.getSize().x/2 - caption.getGlobalBounds().width/2, shape.getPosition().y);
            
            qty.setPosition(shape.getPosition().x + shape.getSize().x/2 - qty.getGlobalBounds().width/2, shape.getPosition().y + shape.getSize().y - qty.getGlobalBounds().height - 20*gs::scale);
            panelShape.setCornersRadius(20*gs::scale);
            panelShape.setPosition(0, qty.getPosition().y - 1*gs::scale);
            panelShape.setSize({0.54f*shape.getSize().x, shape.getPosition().y + shape.getSize().y - panelShape.getPosition().y });
            panelShape.setPosition(shape.getPosition().x + shape.getSize().x/2 - panelShape.getSize().x/2, panelShape.getPosition().y);
            
            
            ppButton.setSize({panelShape.getSize().x/5, panelShape.getSize().y}); ppButton.Resize(width, height);
            ppButton.setPosition(panelShape.getPosition().x + panelShape.getSize().x - ppButton.shape.getSize().x, panelShape.getPosition().y);
            mmButton.setSize({panelShape.getSize().x/5, panelShape.getSize().y}); mmButton.Resize(width, height);
            mmButton.setPosition(panelShape.getPosition().x, panelShape.getPosition().y);
            mmButton.text.setPosition(mmButton.text.getPosition().x, mmButton.text.getPosition().y - 23*gs::scale);
            pButton.setSize({panelShape.getSize().x/5, panelShape.getSize().y}); pButton.Resize(width, height);
            pButton.setPosition(ppButton.shape.getPosition().x - pButton.shape.getSize().x, panelShape.getPosition().y);
            mButton.setSize({panelShape.getSize().x/5, panelShape.getSize().y}); mButton.Resize(width, height);
            mButton.setPosition(mmButton.shape.getPosition().x + mmButton.shape.getSize().x, panelShape.getPosition().y);
            mButton.text.setPosition(mButton.text.getPosition().x, mButton.text.getPosition().y - 23*gs::scale);
            
            sprite.setPosition(shape.getPosition().x + 0.24f*shape.getSize().x/2, shape.getPosition().y + shape.getSize().y/2);
            sprite.setScale(0.21f*shape.getSize().x/sprite.getLocalBounds().width, 0.21f*shape.getSize().x/sprite.getLocalBounds().width);
            if (sprite.getGlobalBounds().height > 0.6f*shape.getSize().y) sprite.setScale(0.6f*shape.getSize().y/sprite.getLocalBounds().height, 0.6f*shape.getSize().y/sprite.getLocalBounds().height);
            
            bonuses.setPosition(shape.getPosition().x + 0.24f*shape.getSize().x + 30*gs::scale, panelShape.getPosition().y - bonuses.getGlobalBounds().height - 15*gs::scale);
            int spacex = shape.getSize().x - 0.24f*shape.getSize().x - 20*gs::scale,
                spacey = bonuses.getPosition().y - (caption.getPosition().y + caption.getGlobalBounds().height + 10*gs::scale);
            description.setString(nss::GetStringWithLineBreaks(description, item->description, spacex));
            description.setPosition(shape.getPosition().x + 0.24f*shape.getSize().x + 10*gs::scale + spacex/2.f - description.getGlobalBounds().width/2, caption.getPosition().y + caption.getGlobalBounds().height + 10*gs::scale + spacey/2 - description.getGlobalBounds().height/2);
        }
        void ItemDetails::Draw(sf::RenderWindow* window)
        {
            window->draw(shape);
            window->draw(panelShape);
            
            window->draw(sprite);
            
            window->draw(caption);
            window->draw(description);
            window->draw(bonuses);
            window->draw(qty);
            window->draw(price);
            
            pButton.draw(window);
            ppButton.draw(window);
            mButton.draw(window);
            mmButton.draw(window);
        }
        void ItemDetails::UpdateAlpha()
        {
            pButton.setAlpha(alpha); ppButton.setAlpha(alpha); mButton.setAlpha(alpha); mmButton.setAlpha(alpha);
            shape.setFillColor({shape.getFillColor().r, shape.getFillColor().g, shape.getFillColor().b, static_cast<sf::Uint8>(0.93f*alpha)});
            shape.setOutlineColor({shape.getOutlineColor().r, shape.getOutlineColor().g, shape.getOutlineColor().b, static_cast<sf::Uint8>(0.5f*alpha)});
            panelShape.setFillColor({panelShape.getFillColor().r, panelShape.getFillColor().g, panelShape.getFillColor().b, static_cast<sf::Uint8>(0.6f*alpha)});
            caption.setFillColor({caption.getFillColor().r, caption.getFillColor().g, caption.getFillColor().b, alpha});
            caption.setOutlineColor({caption.getOutlineColor().r, caption.getOutlineColor().g, caption.getOutlineColor().b, alpha});
            description.setFillColor({description.getFillColor().r, description.getFillColor().g, description.getFillColor().b, alpha});
            qty.setFillColor({qty.getFillColor().r, qty.getFillColor().g, qty.getFillColor().b, alpha});
            bonuses.setFillColor({bonuses.getFillColor().r, bonuses.getFillColor().g, bonuses.getFillColor().b, alpha});
            price.setFillColor({price.getFillColor().r, price.getFillColor().g, price.getFillColor().b, alpha});
            sprite.setColor({sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, alpha});
        }
    }
}
