//
//  CalendarUI.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 28/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "CalendarUI.hpp"

namespace NekoUI
{
    void CalendarUI::Init()
    {
        dateText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        dateText.setOutlineColor(sf::Color::Black);
        dateText.setString(L"13 ИЮНЯ");
        closeButton.setString(L"Закрыть");
        closeButton.setFont(L"Pacifica.ttf");
        closeButton.setCharacterSize(84);
        closeButton.valign = Valign::Bottom;
        
        blackScreenShape.setFillColor(sf::Color(0,0,0,170));
        quitButton.setTexture(L"Data/Images/quit_button.png");
        quitButton.setScale(2);
    }
    void CalendarUI::Destroy() { if (active) CleanUp(); }
    void CalendarUI::Update(const sf::Time& elapsedTime)
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
    void CalendarUI::CleanUp() { /*ic::DeleteImage(L"Data/Neko/Person/Vanilla.png");*/ }
    void CalendarUI::PollEvent(sf::Event& event)
    {
        if (!active || !gs::isActiveInterface(this)) return;
        
        if (quitButton.PollEvent(event) || closeButton.PollEvent(event)) entity->SendMessage({"CalendarUI :: Close"});
    }
    void CalendarUI::Resize(unsigned int width, unsigned int height)
    {
        if (!active) return;
        
        blackScreenShape.setSize({(float)gs::width, (float)gs::height});
        quitButton.Resize(width, height); quitButton.setPosition(gs::width - gs::width/12, gs::height/8);
        
        closeButton.Resize(width, height);
        closeButton.setPosition(gs::width/2, 3.f*gs::height/4);
        dateText.setCharacterSize(120 * gs::scale);
        dateText.setOutlineThickness(gs::scale);
        dateText.setOrigin(dateText.getLocalBounds().width/2, dateText.getLocalBounds().height/1.3);
        dateText.setPosition(gs::width/2, gs::height/2);
    }
    void CalendarUI::Draw(sf::RenderWindow* window)
    {
        if (!active || gs::ignoreDraw) return;
        
        window->draw(blackScreenShape);
        quitButton.draw(window);
        window->draw(dateText);
        closeButton.draw(window);
    }
    void CalendarUI::RecieveMessage(MessageHolder& message)
    {
        if (message.info == "Time")
        {
            tm* timeinfo = reinterpret_cast<tm*>(message.address);
            if (timeinfo)
            {
                dateText.setString(std::to_wstring(timeinfo->tm_mday) + L" " + GetMonthStringFromItsNumber(timeinfo->tm_mon));
                dateText.setOrigin(dateText.getLocalBounds().width/2, dateText.getLocalBounds().height/1.3);
            }
        }
        else if (!active && message.info == "CalendarUI :: Show") Switch(true);
        else if (active && message.info == "CalendarUI :: Close") Switch(false);
        else if (message.info == "CalendarUI :: Switch") Switch(!active);
    }
    void CalendarUI::Switch(const bool& on)
    {
        if (on && !active) { gs::PushInterface(this); active = true; mode = appearing; entity->SortAbove(this); Resize(gs::width, gs::height); }
        else if (active) { gs::RemoveInterface(this); mode = disappearing; }
    }
    void CalendarUI::UpdateAlpha()
    {
        dateText.setFillColor(sf::Color(dateText.getFillColor().r, dateText.getFillColor().g, dateText.getFillColor().b, alpha));
        dateText.setOutlineColor(sf::Color(dateText.getOutlineColor().r, dateText.getOutlineColor().g, dateText.getOutlineColor().b, alpha));
        blackScreenShape.setFillColor(sf::Color(blackScreenShape.getFillColor().r, blackScreenShape.getFillColor().g, blackScreenShape.getFillColor().b, 170.f * ((float)alpha/255)));
        blackScreenShape.setOutlineColor(sf::Color(blackScreenShape.getOutlineColor().r, blackScreenShape.getOutlineColor().g, blackScreenShape.getOutlineColor().b, 170.f * ((float)alpha/255.f)));
        
        quitButton.setAlpha(alpha);
        closeButton.setAlpha(alpha);
    }
}
