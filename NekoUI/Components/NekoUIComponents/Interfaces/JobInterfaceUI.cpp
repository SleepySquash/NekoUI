//
//  JobInterfaceUI.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 23/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "JobInterfaceUI.hpp"

namespace NekoUI
{
    void JobInterfaceUI::Init()
    {
        blackScreenShape.setFillColor(sf::Color(0,0,0,170));
        jobs.setCharacterSize(54);
        jobs.setFont(L"Impact.ttf");
        jobs.halign = Halign::Center;
        jobs.valign = Valign::Bottom;
        jobs.ralpha = 255;
        
        moneyText.setFont(*fc::GetFont(L"Noteworthy-Bold.ttf"));
        moneyText.setString(std::to_string(NekoS::money) + " P");
        moneyText.setOutlineColor(sf::Color::Black);
    }
    void JobInterfaceUI::Destroy() { if (active) CleanUp(); }
    void JobInterfaceUI::Update(const sf::Time& elapsedTime)
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
    void JobInterfaceUI::CleanUp() { ic::DeleteImage(L"Data/Images/Backgrounds/job_background1.jpg"); jobs.setTexture(L""); }
    void JobInterfaceUI::PollEvent(sf::Event& event)
    {
        if (!active || !gs::isActiveInterface(this) || !clickable) return;
        
        if (event.type == sf::Event::MouseMoved && gs::buttonHovering)
        {
            float xx{ jobsStartXX }, yy{ yyScroll + 200*gs::scale }; int currentButton{ 0 };
            for (int i = 0; i < 4; ++i)
            {
                jobs.index = i; jobs.setPosition(xx, yy); jobs.PollEvent(event);
                xx += jobs.shape.getSize().x + 10*gs::scale;
                if (++currentButton >= jobsButtonsCount) { currentButton = 0;
                    xx = jobsStartXX; yy += jobs.shape.getSize().y + 10*gs::scale; }
            }
        }
        else if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::TouchBegan || event.type == sf::Event::TouchEnded)
        {
            float xx{ jobsStartXX }, yy{ yyScroll + 200*gs::scale }; int currentButton{ 0 };
            for (int i = 0; i < 4; ++i)
            {
                jobs.index = i; jobs.setPosition(xx, yy);
                if (jobs.PollEvent(event))
                {
                    switch (i)
                    {
                        case 1: rm::requestCloseButton = clickable = false;
                            entity->SendMessage({"PlacesUI :: JobFNAF"});
                            Switch(false); break;
                        default: break;
                    }
                }
                xx += jobs.shape.getSize().x + 10*gs::scale;
                if (++currentButton >= jobsButtonsCount) { currentButton = 0;
                    xx = jobsStartXX; yy += jobs.shape.getSize().y + 10*gs::scale; }
            }
            jobs.eventPolled(event);
        }
    }
    void JobInterfaceUI::Resize(const unsigned int& width, const unsigned int& height)
    {
        if (!active) return;
        
        blackScreenShape.setSize({(float)gs::width, (float)gs::height});
        quitButton.Resize(width, height); quitButton.setPosition(gs::width - gs::width/12, gs::height/8);
        if (spriteLoaded)
        {
            float scaleFactorX, scaleFactorY, scaleFactor;
            scaleFactorX = (float)width / background.getLocalBounds().width;
            scaleFactorY = (float)height / background.getLocalBounds().height;
            
            scaleFactor = (scaleFactorX > scaleFactorY) ? scaleFactorX : scaleFactorY;
            background.setScale(scaleFactor, scaleFactor);
            background.setPosition(width/2, height/2);
        }
        
        jobs.Resize(width, height);
        jobs.setSize({450*gs::scale, 230*gs::scale});
        jobsButtonsCount = floor((gs::width - 200*gs::scalex - (jobs.shape.getSize().x + 10*gs::scale))/(jobs.shape.getSize().x + 10*gs::scale)) + 1;
        if (jobsButtonsCount > 4) jobsButtonsCount = 4;
        jobsStartXX = gs::width/2 - (jobs.shape.getSize().x + 10*gs::scale)*jobsButtonsCount/2;
        
        moneyText.setCharacterSize(40*gs::scale);
        moneyText.setPosition(15*gs::scale, 6*gs::scale + gs::screenOffsetTop);
        moneyText.setOutlineThickness(gs::scale);
    }
    void JobInterfaceUI::Draw(sf::RenderWindow* window)
    {
        if (!active) return;
        if (spriteLoaded) window->draw(background);
        
        float xx{ jobsStartXX }, yy{ yyScroll + 200*gs::scale };
        int currentButton{ 0 };
        for (int i = 0; i < 4; ++i)
        {
            jobs.index = i;
            switch (i) {
                case 0: jobs.setString(L"Официант"); jobs.shape.setTextureRect({0,0,jobsBackgroundW,jobsBackgroundH}); break;
                case 1: jobs.setString(L"FNAF"); jobs.shape.setTextureRect({jobsBackgroundW,0,jobsBackgroundW,jobsBackgroundH}); break;
                case 2: jobs.setString(L"NekoExpress"); jobs.shape.setTextureRect({0,jobsBackgroundH,jobsBackgroundW,jobsBackgroundH}); break;
                case 3: jobs.setString(L"Защита башни"); jobs.shape.setTextureRect({jobsBackgroundW,jobsBackgroundH,jobsBackgroundW,jobsBackgroundH}); break;
                default: break;
            }
            jobs.setPosition(xx, yy);
            jobs.draw(window);
            xx += jobs.shape.getSize().x + 10*gs::scale;
            if (++currentButton >= jobsButtonsCount) { currentButton = 0;
                xx = jobsStartXX; yy += jobs.shape.getSize().y + 10*gs::scale; }
        }
        
        window->draw(moneyText);
    }
    void JobInterfaceUI::ReceiveMessage(MessageHolder& message)
    {
        if (!active && message.info == "JobInterfaceUI :: Show") Switch(true);
        else if (active && message.info == "JobInterfaceUI :: Close") Switch(false);
        else if (active && nss::Command(message.info, "Request") && message.additional == L"Data/Images/Backgrounds/job_background1.jpg")
        {
            sf::Texture* texture = ic::LoadTexture(message.additional);
            if ((spriteLoaded = texture))
            {
                background.setTexture(*texture, true);
                background.setOrigin(texture->getSize().x/2, texture->getSize().y/2);
                Resize(gs::width, gs::height);
            }
        }
        else if (message.info == "JobInterfaceUI :: Switch") Switch(!active);
        else if (active && nss::Command(message.info, "Request") && message.additional == jobs.textureName)
        {
            jobs.ReceiveMessage(message);
            const sf::Texture* texture = jobs.shape.getTexture();
            if (texture)
            {
                jobsBackgroundW = texture->getSize().x/2;
                jobsBackgroundH = texture->getSize().y/2;
                jobs.shape.setTextureRect({0,0,jobsBackgroundW,jobsBackgroundH});
            }
        }
    }
    void JobInterfaceUI::Switch(const bool& on)
    {
        rm::drawNeeds = !on;
        if (on) rm::drawDatePanel = false; else rm::drawDatePanel = (gs::activeInterfaces.size() == 1);
        if (on)
        {
            if (active) mode = appearing;
            else
            {
                gs::PushInterface(this); active = clickable = true; mode = appearing; entity->SortAbove(this);
                sf::Texture* texture = ic::RequestHigherTexture(L"Data/Images/Backgrounds/job_background1.jpg", entity->system);
                moneyText.setString(std::to_string(NekoS::money) + " P");
                if ((spriteLoaded = texture))
                {
                    background.setTexture(*texture);
                    background.setOrigin(texture->getSize().x/2, texture->getSize().y/2);
                }
                jobs.setTexture(L"Data/Images/UI/jobButtonBackgrounds.jpg", ic::globalRequestSender);
                const sf::Texture* ttexture = jobs.shape.getTexture();
                if (ttexture)
                {
                    jobsBackgroundW = ttexture->getSize().x/2;
                    jobsBackgroundH = ttexture->getSize().y/2;
                    jobs.shape.setTextureRect({0,0,jobsBackgroundW,jobsBackgroundH});
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
    void JobInterfaceUI::UpdateAlpha()
    {
        jobs.setAlpha(alpha);
        background.setColor({background.getColor().r, background.getColor().g, background.getColor().b, alpha});
    }
}
