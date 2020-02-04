//
//  NotificationsUI.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 15.01.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#include "Notifications.hpp"

namespace ns
{
    PopupMessage::PopupMessage(const std::wstring& caption, const std::wstring& message) : textStr(message) { this->caption.setString(caption); }
    void PopupMessage::Init()
    {
        gs::ignoreEvent = true;
        
        shape.setFillColor({0,0,0});
        shape.setCornerPointCount(4);
        shape.setOutlineColor(sf::Color::White);
        backshape.setFillColor({0,0,0});
        okButton.setFont(L"Impact.ttf");
        okButton.setString(L"OK");
        okButton.setCharacterSize(55);
        okButton.valign = Valign::Bottom;
        
        if (fc::GetFont(L"Noteworthy-Bold.ttf")) {
            text.setFont(*fc::GetFont(L"Noteworthy-Bold.ttf"));
            caption.setFont(*fc::GetFont(L"Noteworthy-Bold.ttf")); }
        
        gs::forceIgnoreEvent = true;
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
                if (currentTime >= appearTime) { alpha = 255; currentTime = 0.f; mode = existing; }
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
        if (okButton.PollEvent(event)) Close();
    }
    void PopupMessage::Resize(unsigned int width, unsigned int height)
    {
        if (!active) return;
        
        backshape.setSize({(float)gs::width, (float)gs::height});
        shape.setSize({560*gs::scale, 360*gs::scale});
        shape.setCornersRadius(20*gs::scale); shape.setOutlineThickness(2*gs::scale);
        shape.setPosition(gs::width/2 - shape.getSize().x/2, gs::height/2 - shape.getSize().y/2);
        okButton.Resize(width, height);
        okButton.setPosition(shape.getPosition().x + shape.getSize().x/2, shape.getPosition().y + shape.getSize().y - 10*gs::scale);
        
        caption.setCharacterSize(50*gs::scale);
        caption.setOrigin(caption.getLocalBounds().width/2, 0);
        caption.setPosition(shape.getPosition().x + shape.getSize().x/2, shape.getPosition().y);
        
        float space = shape.getSize().y - caption.getGlobalBounds().height - okButton.text.getGlobalBounds().height - 10*gs::scale;
        text.setCharacterSize(24*gs::scale);
        text.setString(nss::GetStringWithLineBreaks(text, textStr, shape.getSize().x));
        text.setOrigin(text.getLocalBounds().width/2, 0);
        text.setPosition(shape.getPosition().x + shape.getSize().x/2, shape.getPosition().y + caption.getGlobalBounds().height + space/2 - text.getGlobalBounds().height/2);
    }
    void PopupMessage::Draw(sf::RenderWindow* window)
    {
        if (!active) return;
        window->draw(backshape);
        window->draw(shape);
        window->draw(caption);
        window->draw(text);
        okButton.draw(window);
    }
    void PopupMessage::RecieveMessage(MessageHolder& message) { if (active && message.info == "Popup :: Close") Close(); }
    void PopupMessage::Close()
    {
        if (active) { gs::RemoveInterface(this); gs::ignoreEvent = gs::forceIgnoreEvent = false; mode = disappearing; }
        else mode = disappearing;
    }
    void PopupMessage::UpdateAlpha()
    {
        okButton.setAlpha(alpha);
        text.setFillColor({text.getFillColor().r, text.getFillColor().g, text.getFillColor().b, alpha});
        caption.setFillColor({caption.getFillColor().r, caption.getFillColor().g, caption.getFillColor().b, alpha});
        backshape.setFillColor({shape.getFillColor().r, shape.getFillColor().g, shape.getFillColor().b, static_cast<unsigned char>(0.2f*alpha)});
        shape.setFillColor({shape.getFillColor().r, shape.getFillColor().g, shape.getFillColor().b, static_cast<unsigned char>(0.78f*alpha)});
        shape.setOutlineColor({shape.getOutlineColor().r, shape.getOutlineColor().g, shape.getOutlineColor().b, static_cast<unsigned char>(0.78f*alpha)});
    }


    void NotificationsUI::RecieveMessage(MessageHolder& message)
    {
        if (active && message.info == "NotUI :: Popup")
        {
            NotificationHolder* holder = reinterpret_cast<NotificationHolder*>(message.address);
            if (holder)
            {
                entity->AddComponent<PopupMessage>(holder->caption, holder->text);
                delete holder;
            }
        }
    }
}
