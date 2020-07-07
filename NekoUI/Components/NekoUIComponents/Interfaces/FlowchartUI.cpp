//
//  FlowchartUI.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 16.02.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#include "FlowchartUI.hpp"

namespace NekoUI
{
    void FlowchartUI::Init()
    {
        skillsButton.setString(lang::get("Skills"));
        skillsButton.setCharacterSize(40);
        skillsButton.setFont(L"Impact.ttf");
        interestsButton.setString(lang::get("Interests"));
        interestsButton.setCharacterSize(40);
        interestsButton.setFont(L"Impact.ttf");
        biographyButton.setString(lang::get("Biography"));
        biographyButton.setCharacterSize(40);
        biographyButton.setFont(L"Impact.ttf");
        
        skillsButton.halign = Halign::Right; biographyButton.halign = Halign::Left;
        skillsButton.valign = interestsButton.valign = biographyButton.valign = Valign::Top;
    }
    void FlowchartUI::Destroy() { if (active) CleanUp(); }
    void FlowchartUI::Update(const sf::Time& elapsedTime)
    {
        if (!active) return;
        switch (mode)
        {
            case appearing:      //currentTime = appearTime;
                if (currentTime < appearTime) currentTime += elapsedTime.asSeconds();
                if (currentTime >= appearTime) { gs::ignoreDraw = true; alpha = 255; currentTime = disappearTime; mode = existing; }
                else alpha = (sf::Uint8)(255 * (currentTime / appearTime));
                UpdateAlpha(); gs::requestWindowRefresh = true;
                break;
                
            case disappearing:     //currentTime = disappearTime;
                if (currentTime > 0) currentTime -= elapsedTime.asSeconds();
                if (currentTime <= 0) { active = false; CleanUp(); alpha = 0; currentTime = 0.f; mode = existing; }
                else alpha = (sf::Uint8)(255 * (currentTime / disappearTime));
                UpdateAlpha(); gs::requestWindowRefresh = true;
                break;
            default: break;
        }
    }
    void FlowchartUI::CleanUp() { ic::DeleteImage(L"Data/Images/Backgrounds/flowchart1.png"); }
    void FlowchartUI::PollEvent(sf::Event& event)
    {
        if (!active || !gs::isActiveInterface(this)) return;
        
        if (skillsButton.PollEvent(event)) SwitchScreen(Screen::Skill);
        else if (interestsButton.PollEvent(event)) SwitchScreen(Screen::Interest);
        else if (biographyButton.PollEvent(event)) SwitchScreen(Screen::Biography);
        
        switch (screen)
        {
            default: break;
        }
    }
    void FlowchartUI::Resize(const unsigned int& width, const unsigned int& height)
    {
        if (!active) return;
        
        interestsButton.Resize(width, height); interestsButton.setPosition(gs::width/2, 10*gs::scale);
        skillsButton.Resize(width, height); skillsButton.setPosition(gs::width/2 - interestsButton.getGlobalBounds().width/2 - 10*gs::scale, 10*gs::scale);
        biographyButton.Resize(width, height); biographyButton.setPosition(gs::width/2 + interestsButton.getGlobalBounds().width/2 + 10*gs::scale, 10*gs::scale);
        if (loaded)
        {
            float scaleFactorX, scaleFactorY, scaleFactor;
            scaleFactorX = (float)width / (background.getTexture()->getSize().x);
            scaleFactorY = (float)height / (background.getTexture()->getSize().y);
            
            scaleFactor = (scaleFactorX > scaleFactorY) ? scaleFactorX : scaleFactorY;
            background.setScale(scaleFactor, scaleFactor);
            background.setPosition(width/2, height/2);
        }
    }
    void FlowchartUI::Draw(sf::RenderWindow* window)
    {
        if (!active) return;
        window->draw(background);
        skillsButton.draw(window);
        interestsButton.draw(window);
        biographyButton.draw(window);
        
        switch (screen)
        {
            default: break;
        }
    }
    void FlowchartUI::ReceiveMessage(MessageHolder& message)
    {
        if (!active && message.info == "FlowchartUI :: Show") Switch(true);
        else if (active && message.info == "FlowchartUI :: Close") Switch(false);
        else if (active && nss::Command(message.info, "Request") && message.additional == L"Data/Images/Backgrounds/flowchart1.png")
        {
            sf::Texture* texture = ic::LoadTexture(message.additional);
            if ((loaded = texture))
            {
                background.setTexture(*texture, true);
                background.setOrigin(texture->getSize().x/2, texture->getSize().y/2);
                Resize(gs::width, gs::height);
            }
        }
        else if (message.info == "FlowchartUI :: Switch") Switch(!active);
    }
    void FlowchartUI::SwitchScreen(const Screen& scr)
    {
        if (screen == scr) return;
        
        screen = scr;
        switch (screen)
        {
            default: break;
        }
    }
    void FlowchartUI::Switch(const bool& on)
    {
        if (on) rm::drawDatePanel = false; else rm::drawDatePanel = (gs::activeInterfaces.size() == 1);
        if (on)
        {
            if (active) mode = appearing;
            else
            {
                gs::PushInterface(this); active = true; mode = appearing; entity->SortAbove(this);
                sf::Texture* texture = ic::RequestHigherTexture(L"Data/Images/Backgrounds/flowchart1.png", ic::globalRequestSender);
                if ((loaded = texture))
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
    void FlowchartUI::UpdateAlpha()
    {
        skillsButton.setAlpha(alpha);
        biographyButton.setAlpha(alpha);
        interestsButton.setAlpha(alpha);
        background.setColor({background.getColor().r, background.getColor().g, background.getColor().b, alpha});
    }
}
