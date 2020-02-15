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
        blackScreenShape.setFillColor(sf::Color(0,0,0,170)); // TODO: Добавить "скейлинг относительно столько-то пикселей", на который будет просто делиться реальный размер, чтобы скейл не зависел от ресолюшинкласса
        groceryButton.setTexture(L"Data/Images/UI/map_Grocery.png"); groceryButton.setScale(0.5f);
        shopkeeperButton.setTexture(L"Data/Images/UI/map_Shopkeeper.png"); shopkeeperButton.setScale(0.5f);
        homeButton.setTexture(L"Data/Images/UI/map_Home.png"); homeButton.setScale(0.5f);
        parkButton.setTexture(L"Data/Images/UI/map_Park.png"); parkButton.setScale(0.5f);
        currentPosition.setFillColor({32, 129, 215});
        
        scrolldownMenu.setTexture(L"Data/Images/UI/ScrolldownButton_exit.png");
        scrolldownMenu.setScale(0.6f);
        scrolldownMenu.halign = Halign::Right;
        scrolldownMenu.valign = Valign::Bottom;
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
    void MapUI::CleanUp() { ic::DeleteImage(L"Data/Images/Backgrounds/map.png"); }
    void MapUI::PollEvent(sf::Event& event)
    {
        if (!active || !gs::isActiveInterface(this) || !clickable) return;
        
        if (scrolldownMenu.PollEvent(event)) { rm::requestCloseButton = clickable = false; Switch(false); }
        else if (groceryButton.PollEvent(event))
        {
            rm::requestCloseButton = clickable = false;
            entity->SendMessage({"PlacesUI :: GroceryUI"});
            Switch(false);
        }
        else if (shopkeeperButton.PollEvent(event))
        {
            rm::requestCloseButton = clickable = false;
            entity->SendMessage({"PlacesUI :: ShopkeeperUI"});
            Switch(false);
        }
        else if (homeButton.PollEvent(event))
        {
            rm::requestCloseButton = clickable = false;
            entity->SendMessage({"PlacesUI :: ApartmentUI"});
            Switch(false);
        }
        else if (parkButton.PollEvent(event))
        {
            rm::requestCloseButton = clickable = false;
            entity->SendMessage({"PlacesUI :: TestNovel"});
            Switch(false);
        }
    }
    void MapUI::Resize(const unsigned int& width, const unsigned int& height)
    {
        if (!active) return;
        
        blackScreenShape.setSize({(float)gs::width, (float)gs::height});
        groceryButton.Resize(width, height); groceryButton.setPosition(gs::width/2, gs::height/2);
        shopkeeperButton.Resize(width, height); shopkeeperButton.setPosition(gs::width/2 - 200*gs::scalex, gs::height/2 - 200*gs::scaley);
        homeButton.Resize(width, height); homeButton.setPosition(gs::width/2 + 200*gs::scalex, gs::height/2 + 200*gs::scaley);
        parkButton.Resize(width, height); parkButton.setPosition(gs::width/2 - 200*gs::scalex, gs::height/2 + 200*gs::scaley);
        scrolldownMenu.Resize(width, height); scrolldownMenu.setPosition(width - 10*gs::scalex, height - 10*gs::scaley);
        if (spriteLoaded)
        {
            float scaleFactorX, scaleFactorY, scaleFactor;
            scaleFactorX = (float)width / (background.getTexture()->getSize().x);
            scaleFactorY = (float)height / (background.getTexture()->getSize().y);
            
            scaleFactor = (scaleFactorX > scaleFactorY) ? scaleFactorX : scaleFactorY;
            background.setScale(scaleFactor, scaleFactor);
            background.setPosition(width/2, height/2);
        }
        
        if (buttonPosition)
        {
            currentPosition.setRadius(0.65*buttonPosition->getGlobalBounds().width);
            currentPosition.setPosition(buttonPosition->getPosition().x - currentPosition.getGlobalBounds().width/2, buttonPosition->getPosition().y - currentPosition.getGlobalBounds().height/2);
        }
    }
    void MapUI::Draw(sf::RenderWindow* window)
    {
        if (!active) return;
        window->draw(background);
        window->draw(currentPosition);
        groceryButton.draw(window);
        shopkeeperButton.draw(window);
        homeButton.draw(window);
        parkButton.draw(window);
        scrolldownMenu.draw(window);
    }
    void MapUI::ReceiveMessage(MessageHolder& message)
    {
        if (!active && message.info == "MapUI :: Show") Switch(true);
        else if (active && message.info == "MapUI :: Close") Switch(false);
        else if (active && nss::Command(message.info, "Request") && message.additional == L"Data/Images/Backgrounds/map.png")
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
                homeButton.active = groceryButton.active = shopkeeperButton.active = true;
                switch (rm::location)
                {
                    case rm::Location::Apartment: buttonPosition = &homeButton; break;
                    case rm::Location::Grocery: buttonPosition = &groceryButton; break;
                    case rm::Location::Shopkeeper: buttonPosition = &shopkeeperButton; break;
                    default: buttonPosition = nullptr; break;
                }
                if (buttonPosition) buttonPosition->active = false;
                sf::Texture* texture = ic::RequestHigherTexture(L"Data/Images/Backgrounds/map.png", ic::globalRequestSender);
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
        homeButton.setAlpha(alpha);
        scrolldownMenu.setAlpha(alpha);
        background.setColor({background.getColor().r, background.getColor().g, background.getColor().b, alpha});
        currentPosition.setFillColor({currentPosition.getFillColor().r, currentPosition.getFillColor().g, currentPosition.getFillColor().b, alpha});
    }
}
