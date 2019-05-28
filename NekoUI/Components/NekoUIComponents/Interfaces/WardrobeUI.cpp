//
//  WardrobeUI.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 28/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "WardrobeUI.hpp"

namespace NekoUI
{
    void WardrobeUI::Init()
    {
        quitButton.setTexture(L"Data/Images/quit_button.png");
        quitButton.setScale(2);
        nintShape.setFillColor(sf::Color(0,0,0,80));
    }
    void WardrobeUI::Destroy() { if (active) CleanUp(); }
    void WardrobeUI::Update(const sf::Time& elapsedTime)
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
    void WardrobeUI::CleanUp() { ic::DeleteImage(L"Data/Images/wardrobe.jpg"); }
    void WardrobeUI::PollEvent(sf::Event& event)
    {
        if (!active || !gs::isActiveInterface(this)) return;
        
        if (quitButton.PollEvent(event)) entity->SendMessage({"WardrobeUI :: Close"});
    }
    void WardrobeUI::Resize(unsigned int width, unsigned int height)
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
        
        if (gs::verticalOrientation)
        {
            Player::neko.personScale = 1.21 * gs::scScale; Player::neko.ResizePerson();
            nintShape.setSize({gs::width*4.55f/5.f, gs::height*4.3f/5.f});
            nintShape.setPosition((gs::width - nintShape.getSize().x)/2, (gs::height - nintShape.getSize().y)/2);
            
            if (Player::neko.body.person.getGlobalBounds().height + 20*gs::scale > gs::height)
                Player::neko.setPersonPosition(width/2, Player::neko.body.person.getGlobalBounds().height + 20*gs::scale);
            else Player::neko.setPersonPosition(width/2, height);
        }
        else
        {
            Player::neko.personScale = 1.06 * gs::scScale; Player::neko.ResizePerson();
            nintShape.setSize({3*gs::width/5.f, gs::height*4.3f/5.f});
            nintShape.setPosition(gs::width - 3*gs::width/5.f, (gs::height - nintShape.getSize().y)/2);
            
            if (Player::neko.body.person.getGlobalBounds().height + 20*gs::scale > gs::height)
                Player::neko.setPersonPosition(width/5, Player::neko.body.person.getGlobalBounds().height + 20*gs::scale);
            else Player::neko.setPersonPosition(width/5, height);
        }
    }
    void WardrobeUI::Draw(sf::RenderWindow* window)
    {
        if (!active) return;
        
        //window->draw(blackScreenShape);
        if (spriteLoaded) window->draw(background);
        Player::neko.Draw(window, true);
        window->draw(nintShape);
        quitButton.Draw(window);
    }
    void WardrobeUI::RecieveMessage(MessageHolder& message)
    {
        if (!active && message.info == "WardrobeUI :: Show") Switch(true);
        else if (active && message.info == "WardrobeUI :: Close") Switch(false);
        else if (message.info == "NekoUI :: SelectNeko") neko = reinterpret_cast<NekoEntity*>(message.address);
        else if (message.info == "WardrobeUI :: Switch") Switch(!active);
    }
    void WardrobeUI::Switch(const bool& on)
    {
        if (on) rm::drawDatePanel = false; else rm::drawDatePanel = (gs::activeInterfaces.size() == 1);
        if (on)
        {
            if (active) mode = appearing;
            else
            {
                gs::PushInterface(this); active = true; mode = appearing; entity->SortAbove(this);
                sf::Texture* texture = ic::LoadTexture(L"Data/Images/wardrobe.jpg");
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
            if (active) { gs::RemoveInterface(this); gs::ignoreDraw = false; active = true; mode = disappearing; }
            else mode = disappearing;
        }
    }
    void WardrobeUI::UpdateAlpha()
    {
        /*blackScreenShape.setFillColor(sf::Color(blackScreenShape.getFillColor().r, blackScreenShape.getFillColor().g, blackScreenShape.getFillColor().b, 170.f * ((float)alpha/255)));
         blackScreenShape.setOutlineColor(sf::Color(blackScreenShape.getOutlineColor().r, blackScreenShape.getOutlineColor().g, blackScreenShape.getOutlineColor().b, 170.f * ((float)alpha/255.f)));*/
        quitButton.setAlpha(alpha);
        background.setColor({background.getColor().r, background.getColor().g, background.getColor().b, alpha});
        
        Player::neko.setPersonAlpha(alpha);
    }
}
