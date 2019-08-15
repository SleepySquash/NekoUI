//
//  InventoryUI.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 28/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "InventoryUI.hpp"

namespace NekoUI
{
    void InventoryUI::Init()
    {
        sf::Font* font = fc::GetFont(L"Pacifica.ttf");
        if (font) { emptyText.setFont(*font); itemCountText.setFont(*font); }
        emptyText.setOutlineColor(sf::Color::Black);
        emptyText.setString(L"Empty");
        itemCountText.setOutlineColor(sf::Color::Black);
        dropzone.setFillColor(sf::Color(255, 255, 255, 140));
    }
    void InventoryUI::Destroy() { if (active) CleanUp(); Save(); }
    void InventoryUI::Update(const sf::Time& elapsedTime)
    {
        if (!active) return;
        switch (mode)
        {
            case appearing:      //currentTime = appearTime;
                if (currentTime < appearTime) currentTime += elapsedTime.asSeconds();
                if (currentTime >= appearTime) { gs::ignoreDraw = true; alpha = 255; currentTime = 0.f; mode = existing; }
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
        if (elapsedDraggingTime > 0.f)
        {
            elapsedDraggingTime -= elapsedTime.asSeconds();
            if (elapsedDraggingTime <= 0.f)
            {
                draggingMode = true;
                sf::Texture* texture = ic::FindTexture(utf16("Data/Items/" + (*draggingItem).first->name + ".png"));
                if (texture)
                {
                    draggingSprite.setTexture(*texture, true);
                    float scaleX = 126.f / texture->getSize().x, scaleY = 126.f / texture->getSize().y;
                    draggingSprite.setScale(scaleX > scaleY ? scaleY*gs::scale : scaleX*gs::scale, scaleX > scaleY ? scaleY*gs::scale : scaleX*gs::scale);
                }
                sf::Vector2i pos{ 0, 0 };
                if (sf::Touch::isDown(0)) pos = sf::Touch::getPosition(0, *gs::window); else pos = sf::Mouse::getPosition(*gs::window);
                draggingSprite.setPosition(pos.x, pos.y);
                dropzoneOnRight = pos.x < gs::width/2;
                dropzone.setPosition(dropzoneOnRight ? (2.f/3 * gs::width) : 0, 0);
            }
        }
    }
    void InventoryUI::CleanUp()
    {
        for (auto& item : Inventory::items.list) ic::DeleteImage(utf16("Data/Items/" + item.first->name + ".png"));
        ic::DeleteImage(L"Data/Images/Backgrounds/fridge.jpg");
        ic::DeleteImage(L"Data/Images/UI/inventoryslot.png");
    }
    void InventoryUI::PollEvent(sf::Event& event)
    {
        if (event.type == sf::Event::Closed || event.type == sf::Event::LostFocus) Save();
        if (!active || !gs::isActiveInterface(this)) return;
        
        if (elapsedDraggingTime > 0.f && (event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::TouchEnded)) elapsedDraggingTime = 0.f;
        if (spriteLoaded && !isEmpty && !draggingMode && (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::TouchBegan || event.type == sf::Event::TouchEnded))
        {
            int xx = startSlotsXX, yy = gs::height/10, inrow = 0, i = 0;
            for (auto it = Inventory::items.list.begin(); it != Inventory::items.list.end(); ++it)
            {
                if (ic::FindTexture(utf16("Data/Items/" + (*it).first->name + ".png")))
                {
                    itemButtons.index = i;
                    sf::Texture* texture = ic::FindTexture(utf16("Data/Items/" + (*it).first->name + ".png"));
                    itemButtons.setTexture(texture);
                    if (texture) {
                        float scaleX = 126.f / texture->getSize().x, scaleY = 126.f / texture->getSize().y;
                        itemButtons.setScale(scaleX > scaleY ? scaleY : scaleX); }
                    itemButtons.setPosition(xx + slotSprite.getGlobalBounds().width/2, yy + slotSprite.getGlobalBounds().height/2);
                    if ((event.type == sf::Event::MouseButtonPressed && itemButtons.sprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) || (event.type == sf::Event::TouchBegan && itemButtons.sprite.getGlobalBounds().contains(event.touch.x, event.touch.y)))
                    { elapsedDraggingTime = 0.14f; draggingItem = it; }
                    if (itemButtons.PollEvent(event))
                    {
                        entity->SendMessage({"DetailsUI :: X", std::to_wstring(xx + slotSprite.getGlobalBounds().width/2)});
                        entity->SendMessage({"DetailsUI :: Y", std::to_wstring(yy + slotSprite.getGlobalBounds().height/2)});
                        entity->SendMessage({"DetailsUI :: Item", (*it).first});
                        entity->SendMessage({"DetailsUI :: Show"});
                    }
                    ++i;
                }
                ++inrow; if (inrow >= slotsXXCount) { inrow = 0; xx = startSlotsXX; yy += 126*gs::scale; } else xx += 126*gs::scale;
            }
            itemButtons.eventPolled(event);
        }
        else if (draggingMode)
        {
            if (event.type == sf::Event::MouseMoved || event.type == sf::Event::TouchMoved)
            {
                if (event.type == sf::Event::MouseMoved) dot = { event.mouseMove.x, event.mouseMove.y };
                else dot = { event.touch.x, event.touch.y };
                draggingSprite.setPosition(dot.x, dot.y);
                if (dropzone.getGlobalBounds().contains(draggingSprite.getPosition().x, draggingSprite.getPosition().y))
                {
                    entity->SendMessage({"Dropping :: Item", std::to_wstring( dot.x ) + L" " + std::to_wstring( dot.y ), (*draggingItem).first});
                    Switch(false); rm::requestCloseButton = false;
                    Inventory::items.Remove((*draggingItem).first->name); itemButtons.anyButtonPressed = false;
                    currentTime = disappearTime; gs::ignoreDraw = false; rm::drawDatePanel = true;
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::TouchEnded) { itemButtons.eventPolled(event); draggingMode = false; }
        }
    }
    void InventoryUI::Resize(unsigned int width, unsigned int height)
    {
        if (!active) return;
        if (spriteLoaded)
        {
            float scaleFactorX, scaleFactorY, scaleFactor;
            scaleFactorX = (float)width / (background.getTexture()->getSize().x);
            scaleFactorY = (float)height / (background.getTexture()->getSize().y);
            
            scaleFactor = (scaleFactorX > scaleFactorY) ? scaleFactorX : scaleFactorY;
            background.setScale(scaleFactor, scaleFactor);
            background.setPosition(width/2, height/2);
            
            itemCountText.setCharacterSize(32*gs::scale);
            itemCountText.setOutlineThickness(gs::scale);
        }
        emptyText.setCharacterSize(90*gs::scale);
        emptyText.setOutlineThickness(2*gs::scale);
        emptyText.setPosition(gs::width/2 - emptyText.getGlobalBounds().width/2, gs::height/2 - emptyText.getGlobalBounds().height/2);
        dropzone.setSize({gs::width/3.f, (float)gs::height});
        dropzone.setPosition(dropzoneOnRight ? (2.f/3 * gs::width) : 0, 0);
        
        slotSprite.setScale(0.5*gs::scale, 0.5*gs::scale);
        if (!isEmpty)
        {
            float xx = 5*gs::scale; bool borderReached{ false }; slotsXXCount = 0;
            for (auto& item : Inventory::items.list) { slotSprite.setPosition(xx, 0); xx += 126*gs::scale; ++slotsXXCount;
                if ((borderReached = (xx > gs::width - (26 + 5)*gs::scale))) { xx -= 126*gs::scale; --slotsXXCount; break; } }
            startSlotsXX = gs::width/2 - xx/2;
        }
    }
    void InventoryUI::Draw(sf::RenderWindow* window)
    {
        if (!active) return;
        if (spriteLoaded)
        {
            window->draw(background);
            if (isEmpty) window->draw(emptyText);
            else
            {
                int xx = startSlotsXX, yy = gs::height/10, inrow = 0, i = 0;
                for (auto& item : Inventory::items.list)
                {
                    slotSprite.setPosition(xx, yy); window->draw(slotSprite);
                    if (ic::FindTexture(utf16("Data/Items/" + item.first->name + ".png")))
                    {
                        if (!draggingMode || (draggingMode && item != (*draggingItem)))
                        {
                            itemButtons.index = i;
                            sf::Texture* texture = ic::FindTexture(utf16("Data/Items/" + item.first->name + ".png"));
                            itemButtons.setTexture(texture);
                            if (texture) {
                                float scaleX = 126.f / texture->getSize().x, scaleY = 126.f / texture->getSize().y;
                                itemButtons.setScale(scaleX > scaleY ? scaleY : scaleX); }
                            itemButtons.setPosition(xx + slotSprite.getGlobalBounds().width/2, yy + slotSprite.getGlobalBounds().height/2);
                            itemButtons.Draw(window);
                            if (item.second > 1)
                            {
                                itemCountText.setString(std::to_string(item.second));
                                itemCountText.setPosition(xx + 124*gs::scale - itemCountText.getGlobalBounds().width, yy + 118*gs::scale - itemCountText.getGlobalBounds().height);
                                window->draw(itemCountText);
                            }
                        }
                        ++i;
                    }
                    ++inrow; if (inrow >= slotsXXCount) { inrow = 0; xx = startSlotsXX; yy += 126*gs::scale; } else xx += 126*gs::scale;
                }
            }
        }
        if (draggingMode) { window->draw(dropzone); window->draw(draggingSprite); }
    }
    void InventoryUI::RecieveMessage(MessageHolder& message)
    {
        if (!active && message.info == "InventoryUI :: Show") Switch(true);
        else if (active && message.info == "InventoryUI :: Close") Switch(false);
        else if (active && nss::Command(message.info, "Request") && message.additional == L"Data/Images/Backgrounds/fridge.jpg")
        {
            sf::Texture* texture = ic::LoadTexture(message.additional);
            if ((spriteLoaded = texture))
            {
                background.setTexture(*texture, true);
                background.setOrigin(texture->getSize().x/2, texture->getSize().y/2);
                Resize(gs::width, gs::height);
            }
        }
        else if (message.info == "InventoryUI :: Switch") Switch(!active);
    }
    void InventoryUI::Switch(const bool& on)
    {
        if (on) rm::drawDatePanel = false; else rm::drawDatePanel = (gs::activeInterfaces.size() == 1);
        if (on)
        {
            isEmpty = Inventory::items.list.empty();
            if (active) mode = appearing;
            else
            {
                gs::PushInterface(this); active = true; mode = appearing; entity->SortAbove(this); rm::requestCloseButton = true;
                for (auto& item : Inventory::items.list) ic::PreloadTexture(utf16("Data/Items/" + item.first->name + ".png"), 2, true, true);
                sf::Texture* texture = ic::RequestHigherTexture(L"Data/Images/Backgrounds/fridge.jpg", entity->system);
                if ((spriteLoaded = texture))
                {
                    background.setTexture(*texture, true);
                    background.setOrigin(texture->getSize().x/2, texture->getSize().y/2);
                }
                texture = ic::LoadTexture(L"Data/Images/UI/inventoryslot.png");
                if ((spriteLoaded = texture)) slotSprite.setTexture(*texture, true);
                Resize(gs::width, gs::height);
            }
        }
        else if (!on)
        {
            if (active) { gs::RemoveInterface(this); gs::ignoreDraw = draggingMode = false; active = true; mode = disappearing; }
            else mode = disappearing;
        }
    }
    void InventoryUI::UpdateAlpha()
    {
        background.setColor({background.getColor().r, background.getColor().g, background.getColor().b, alpha});
        slotSprite.setColor({slotSprite.getColor().r, slotSprite.getColor().g, slotSprite.getColor().b, alpha});
        emptyText.setFillColor({emptyText.getFillColor().r, emptyText.getFillColor().g, emptyText.getFillColor().b, alpha});
        emptyText.setOutlineColor({emptyText.getOutlineColor().r, emptyText.getOutlineColor().g, emptyText.getOutlineColor().b, alpha});
        itemButtons.setAlpha(alpha);
        itemCountText.setFillColor(sf::Color(itemCountText.getFillColor().r, itemCountText.getFillColor().g, itemCountText.getFillColor().b, alpha));
        itemCountText.setOutlineColor(sf::Color(itemCountText.getOutlineColor().r, itemCountText.getOutlineColor().g, itemCountText.getOutlineColor().b, alpha));
    }
    void InventoryUI::Save() { Inventory::SaveInventory(); Inventory::SaveFridge(); Inventory::SaveWardrobe(); }
}
