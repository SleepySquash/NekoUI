//
//  MapUI.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 28/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "MapUI.hpp"

namespace NekoUI
{
    void MapUI::Init()
    {
        blackScreenShape.setFillColor(sf::Color(0,0,0,170));
        groceryButton.setTexture(L"Data/Images/UI/quit_button.png"); groceryButton.setScale(2);
        shopkeeperButton.setTexture(L"Data/Images/UI/quit_button.png"); shopkeeperButton.setScale(2);
    }
    void MapUI::Destroy() { if (active) CleanUp(); }
    void MapUI::Update(const sf::Time& elapsedTime)
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
    }
    void MapUI::CleanUp() { ic::DeleteImage(L"Data/Images/Backgrounds/park.jpg"); }
    void MapUI::PollEvent(sf::Event& event)
    {
        if (!active || !gs::isActiveInterface(this) || !clickable) return;
        
        if (groceryButton.PollEvent(event))
        {
            rm::requestCloseButton = clickable = false;
            entity->SendMessage({"Apartment :: Destroy"});
            entity->SendMessage({"PlacesUI :: GroceryUI"});
            Switch(false);
        }
        else if (shopkeeperButton.PollEvent(event))
        {
            rm::requestCloseButton = clickable = false;
            entity->SendMessage({"Apartment :: Destroy"});
            entity->SendMessage({"PlacesUI :: ShopkeeperUI"});
            Switch(false);
        }
    }
    void MapUI::Resize(unsigned int width, unsigned int height)
    {
        if (!active) return;
        
        blackScreenShape.setSize({(float)gs::width, (float)gs::height});
        groceryButton.Resize(width, height); groceryButton.setPosition(gs::width/2, gs::height/2);
        shopkeeperButton.Resize(width, height); shopkeeperButton.setPosition(gs::width/2 - 200*gs::scalex, gs::height/2 - 200*gs::scaley);
        if (spriteLoaded)
        {
            float scaleFactorX, scaleFactorY, scaleFactor;
            scaleFactorX = (float)width / (background.getTexture()->getSize().x);
            scaleFactorY = (float)height / (background.getTexture()->getSize().y);
            
            scaleFactor = (scaleFactorX > scaleFactorY) ? scaleFactorX : scaleFactorY;
            background.setScale(scaleFactor, scaleFactor);
            background.setPosition(width/2, height/2);
        }
    }
    void MapUI::Draw(sf::RenderWindow* window)
    {
        if (!active) return;
        window->draw(background);
        groceryButton.Draw(window);
        shopkeeperButton.Draw(window);
    }
    void MapUI::RecieveMessage(MessageHolder& message)
    {
        if (!active && message.info == "MapUI :: Show") Switch(true);
        else if (active && message.info == "MapUI :: Close") Switch(false);
        else if (active && nss::Command(message.info, "Request") && message.additional == L"Data/Images/Backgrounds/park.jpg")
        {
            sf::Texture* texture = ic::LoadTexture(message.additional);
            if ((spriteLoaded = texture))
            {
                background.setTexture(*texture, true);
                background.setOrigin(texture->getSize().x/2, texture->getSize().y/2);
                Resize(gs::width, gs::height);
            }
        }
        else if (message.info == "MapUI :: Switch") Switch(!active);
    }
    void MapUI::Switch(const bool& on)
    {
        if (on) rm::drawDatePanel = false; else rm::drawDatePanel = (gs::activeInterfaces.size() == 1);
        if (on)
        {
            if (active) mode = appearing;
            else
            {
                gs::PushInterface(this); active = clickable = true; mode = appearing; entity->SortAbove(this);
                sf::Texture* texture = ic::RequestHigherTexture(L"Data/Images/Backgrounds/park.jpg", entity->system);
                if ((spriteLoaded = texture))
                {
                    background.setTexture(*texture);
                    background.setOrigin(texture->getSize().x/2, texture->getSize().y/2);
                }
                Resize(gs::width, gs::height);
            }
        }
        else if (!on)
        {
            if (active) { gs::RemoveInterface(this); gs::ignoreDraw = false; mode = disappearing; }
            else mode = disappearing;
        }
    }
    void MapUI::UpdateAlpha()
    {
        groceryButton.setAlpha(alpha);
        shopkeeperButton.setAlpha(alpha);
        background.setColor({background.getColor().r, background.getColor().g, background.getColor().b, alpha});
    }
}
