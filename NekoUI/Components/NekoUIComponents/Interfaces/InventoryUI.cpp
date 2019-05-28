//
//  InventoryUI.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 28/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "InventoryUI.hpp"

namespace NekoUI
{
    void InventoryUI::Init()
    {
        quitButton.setTexture(L"Data/Images/quit_button.png");
        quitButton.setScale(2);
    }
    void InventoryUI::Destroy() { if (active) CleanUp(); }
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
    }
    void InventoryUI::CleanUp() { ic::DeleteImage(L"Data/Images/fridge.jpg"); }
    void InventoryUI::PollEvent(sf::Event& event)
    {
        if (!active || !gs::isActiveInterface(this)) return;
        
        if (quitButton.PollEvent(event)) entity->SendMessage({"InventoryUI :: Close"});
    }
    void InventoryUI::Resize(unsigned int width, unsigned int height)
    {
        if (!active) return;
        
        quitButton.Resize(width, height); quitButton.setPosition(gs::width - gs::width/12, gs::height/8);
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
    void InventoryUI::Draw(sf::RenderWindow* window)
    {
        if (!active) return;
        
        //window->draw(blackScreenShape);
        if (spriteLoaded) window->draw(background);
        quitButton.Draw(window);
    }
    void InventoryUI::RecieveMessage(MessageHolder& message)
    {
        if (!active && message.info == "InventoryUI :: Show") Switch(true);
        else if (active && message.info == "InventoryUI :: Close") Switch(false);
        else if (message.info == "InventoryUI :: Switch") Switch(!active);
    }
    void InventoryUI::Switch(const bool& on)
    {
        if (on) rm::drawDatePanel = false; else rm::drawDatePanel = (gs::activeInterfaces.size() == 1);
        if (on)
        {
            if (active) mode = appearing;
            else
            {
                gs::PushInterface(this); active = true; mode = appearing; entity->SortAbove(this);
                sf::Texture* texture = ic::LoadTexture(L"Data/Images/fridge.jpg");
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
            if (active) { gs::RemoveInterface(this); gs::ignoreDraw = false; active = true; mode = disappearing; Resize(gs::width, gs::height); }
            else mode = disappearing;
        }
    }
    void InventoryUI::UpdateAlpha()
    {
        /*blackScreenShape.setFillColor(sf::Color(blackScreenShape.getFillColor().r, blackScreenShape.getFillColor().g, blackScreenShape.getFillColor().b, 170.f * ((float)alpha/255)));
         blackScreenShape.setOutlineColor(sf::Color(blackScreenShape.getOutlineColor().r, blackScreenShape.getOutlineColor().g, blackScreenShape.getOutlineColor().b, 170.f * ((float)alpha/255.f)));*/
        quitButton.setAlpha(alpha);
        background.setColor({background.getColor().r, background.getColor().g, background.getColor().b, alpha});
    }
}
