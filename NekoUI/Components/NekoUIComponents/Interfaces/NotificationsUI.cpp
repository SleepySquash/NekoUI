//
//  NotificationsUI.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 15.01.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#include "NotificationsUI.hpp"

namespace NekoUI
{
    PopupMessage::PopupMessage(const std::wstring& message) { text.setString(message); }
    void PopupMessage::Init()
    {
        shape.setFillColor(sf::Color(0,0,0,170));
        quitButton.setTexture(L"Data/Images/UI/quit_button.png");
        quitButton.setScale(2);
        
        if (fc::GetFont(L"Noteworthy-Bold.ttf"))
            text.setFont(*fc::GetFont(L"Noteworthy-Bold.ttf"));
        
        gs::PushInterface(this);
        entity->SortAbove(this);
    }
    void PopupMessage::Update(const sf::Time& elapsedTime)
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
                if (currentTime >= disappearTime) { entity->PopComponent(this); return; }
                else alpha = (sf::Uint8)(255 - (255 * (currentTime / disappearTime)));
                UpdateAlpha(); gs::requestWindowRefresh = true;
                break;
            default: break;
        }
    }
    void PopupMessage::PollEvent(sf::Event& event)
    {
        if (!active || !gs::isActiveInterface(this)) return;
        if (quitButton.PollEvent(event)) Close();
    }
    void PopupMessage::Resize(unsigned int width, unsigned int height)
    {
        if (!active) return;
        
        shape.setSize({(float)gs::width/3, (float)gs::height/3});
        shape.setPosition(gs::width/2 - shape.getSize().x/2, gs::height/2 - shape.getSize().y/2);
        quitButton.Resize(width, height); quitButton.setPosition(shape.getPosition().x + shape.getSize().x/2, shape.getPosition().y + shape.getSize().y/2);
        
        text.setCharacterSize(20*gs::scale);
        text.setPosition(shape.getPosition().x + shape.getSize().x/2, shape.getPosition().y + shape.getSize().y/10);
    }
    void PopupMessage::Draw(sf::RenderWindow* window)
    {
        if (!active) return;
        window->draw(shape);
        window->draw(text);
        quitButton.Draw(window);
    }
    void PopupMessage::RecieveMessage(MessageHolder& message) { if (active && message.info == "Popup :: Close") Close(); }
    void PopupMessage::Close()
    {
        if (active) { gs::RemoveInterface(this); gs::ignoreDraw = false; active = true; mode = disappearing; }
        else mode = disappearing;
    }
    void PopupMessage::UpdateAlpha()
    {
        quitButton.setAlpha(alpha);
        text.setFillColor({text.getFillColor().r, text.getFillColor().g, text.getFillColor().b, alpha});
        shape.setFillColor({shape.getFillColor().r, shape.getFillColor().g, shape.getFillColor().b, static_cast<unsigned char>(0.5f*alpha)});
    }


    void NotificationsUI::AddPopupMessage(const std::wstring& message) { entity->AddComponent<PopupMessage>(message); }
    void NotificationsUI::RecieveMessage(MessageHolder& message)
    {
        if (active && message.info == "NotUI :: Popup") AddPopupMessage(message.additional);
    }
}
