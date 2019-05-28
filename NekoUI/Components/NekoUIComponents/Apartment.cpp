//
//  Apartment.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 17/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "Apartment.hpp"

namespace NekoUI
{
    Apartment::Apartment()
    {
        sf::Texture* texture = ic::LoadTexture(L"Data/Images/room.jpg");
        if ((spriteLoaded = texture))
        {
            Room::width = texture->getSize().x * Room::roomScale; Room::height = texture->getSize().y * Room::roomScale;
            sprite.setTexture(*texture, true);
        }
        Room::mask = ic::LoadImage(L"Data/Images/room_mask.png");
    }
    void Apartment::Init()
    {
        neko.Init(); neko.sender = entity;
        Player::neko.Init(); hasFocusOnNeko = true; rm::scale = 1.7f;
        entity->SendMessage({"NekoUI :: SelectNeko", &neko});
        
        sf::Texture* texture = ic::LoadTexture(L"Data/Images/scrolldown rev1.png");
        if (texture) scrolldownMenuOffsetY = texture->getSize().y;
    }
    void Apartment::Update(const sf::Time& elapsedTime)
    {
        if (!active) return;
        neko.Update(elapsedTime);
        
        if (timeSinceFirstPress > 0) { timeSinceFirstPress -= elapsedTime.asSeconds();
            if (timeSinceFirstPress < 0) timeSinceFirstPress = 0; }
        if (timeSinceFirstHold > 0) {
            timeSinceFirstHold -= elapsedTime.asSeconds();
            if (timeSinceFirstHold <= 0)
            {
                if ((sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || sf::Touch::isDown(0)) && neko.sprite.getGlobalBounds().contains(dot.x, dot.y))
                {
                    neko.beingActionedWith = nekoIsBeingMoved = true;
                    /// TODO: Диалог типа "увааа!! >3<" во время перетаскивания.
                    int random = rand() % 5;
                    switch (random)
                    {
                        case 0: neko.SetDialogue(L"Божечки-кошечки!~ >3<", true); break;
                        case 1: neko.SetDialogue(L"Осторожнее, позялуста!! >3<", true); break;
                        default: neko.SetDialogue(L"Уааа!!!! >3<", true); break;
                    }
                    /// TODO: Сброс новой точки и активности после перетаскивания.
                    neko.newPoint = true; neko.elapsedWaiting = neko.waitingTime/2 + (rand() % 1000)/1000.f;
                    if (neko.occupiedAt != -1) { neko.occupiedAt = -1; neko.beingOccupied = neko.drawActionButton = false; }
                    /// TODO: Контроль, чтоб не выходила за пределы комнаты.
                    /// TODO: Скроллинг при поднесении к границам экрана.
                    hasFocusOnNeko = false;
                }
            }
        }
        
        if (hasFocusOnNeko)
        {
            Room::x = gs::width/(2*gs::scale*Room::scale) - neko.x;
            Room::y = gs::height/(2*gs::scale*Room::scale) - neko.y + neko.sprite.getLocalBounds().height/4*neko.relScale;
            CalculateCameraPosition();
        }
    }
    void Apartment::PollEvent(sf::Event& event)
    {
        if (!active || gs::ignoreEvent) return;
        neko.PollEvent(event);
        
        if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) || (event.type == sf::Event::TouchBegan))
        {
            if (event.type == sf::Event::MouseButtonPressed) dot = { event.mouseButton.x, event.mouseButton.y };
            else dot = { event.touch.x, event.touch.y };
            
            if (event.type == sf::Event::MouseButtonPressed || event.touch.finger == 0)
            {
                bool interception = neko.sprite.getGlobalBounds().contains(dot.x, dot.y);
                if (interception) { timeSinceFirstHold = 0.25; pressedNeko = true; pressedNekoPos = { dot.x, dot.y }; }
                
                move_dx = rm::x - dot.x/(rm::scale*gs::scale);
                move_dy = rm::y - dot.y/(rm::scale*gs::scale);
            }
            else if (event.touch.finger == 1)
            {
                float x0{ static_cast<float>(sf::Touch::getPosition(0).x) },
                      y0{ static_cast<float>(sf::Touch::getPosition(0).y) },
                      x1{ static_cast<float>(dot.x) },
                      y1{ static_cast<float>(dot.y) };
                prevDistanceZoom = sqrt((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1));
                prevZoom = rm::scale;
            }
        }
        else if ((event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) || (event.type == sf::Event::TouchEnded && event.touch.finger == 0))
        {
            if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
            else dot = { event.touch.x, event.touch.y };
            
            if (nekoIsBeingMoved) { nekoIsBeingMoved = neko.beingActionedWith = false; neko.SetDialogue(L"Уф~~~ т.т"); }
            else
            {
                bool interception = neko.sprite.getGlobalBounds().contains(dot.x, dot.y);
                if (interception && pressedNeko && sqrt(pow(pressedNekoPos.x - dot.x, 2) + pow(pressedNekoPos.y - dot.y, 2)) < 15)
                {
                    if (timeSinceFirstPress > 0) { entity->SendMessage({"NekoUI :: Show"}); hasFocusOnNeko = true; pressedNeko = false; }
                    else { hasFocusOnNeko = true; timeSinceFirstPress = 0.35f; }
                }
            }
        }
        else if (event.type == sf::Event::TouchMoved || (event.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)))
        {
            if (event.type == sf::Event::MouseMoved) dot = { event.mouseMove.x, event.mouseMove.y };
            else dot = { event.touch.x, event.touch.y };
            
            if (!nekoIsBeingMoved && sf::Touch::isDown(0) && event.touch.finger == 1 && prevDistanceZoom != 0)
            {
                if (hasFocusOnNeko) { neko.beingActionedWith = false; hasFocusOnNeko = false; }
                float x0{ static_cast<float>(sf::Touch::getPosition(0).x) },
                      y0{ static_cast<float>(sf::Touch::getPosition(0).y) },
                      x1{ static_cast<float>(dot.x) },
                      y1{ static_cast<float>(dot.y) };
                float distance = sqrt((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1));
                rm::scale = prevZoom * (distance/prevDistanceZoom);
                if (rm::scale < 0.4) rm::scale = 0.4; else if (rm::scale > 3.4) rm::scale = 3.4;
                rm::xWidth = gs::width/(gs::scale * rm::scale); rm::yHeight = gs::height/(gs::scale * rm::scale);
                
                sprite.setScale(rm::roomScale * gs::scale * rm::scale, rm::roomScale * gs::scale * rm::scale);
                CalculateCameraScale();
            }
            else if (event.type != sf::Event::TouchMoved || event.touch.finger == 0)
            {
                if (nekoIsBeingMoved) {
                    neko.x = -rm::x + dot.x/(gs::scale*rm::scale);// + neko.sprite.getLocalBounds().width/2*neko.relScale;
                    neko.y = -rm::y + dot.y/(gs::scale*rm::scale) + neko.sprite.getGlobalBounds().height/(2*rm::scale*gs::scale);
                    neko.UpdatePosition(); }
                else
                {
                    if (hasFocusOnNeko) { neko.beingActionedWith = false; hasFocusOnNeko = false; }
                    rm::x = move_dx + dot.x/(rm::scale*gs::scale);
                    rm::y = move_dy + dot.y/(rm::scale*gs::scale);
                    CalculateCameraPosition();
                }
            }
        }
        else if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.delta != 0 && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
        {
            float scalePrev = rm::scale;
            rm::scale += scrollSensitivity * event.mouseWheelScroll.delta;
            if (rm::scale < 0.4) rm::scale = 0.4; else if (rm::scale > 3.4) rm::scale = 3.4;
            rm::xWidth = gs::width/(gs::scale * rm::scale); rm::yHeight = gs::height/(gs::scale * rm::scale);
            
            rm::x -= (gs::width/(gs::scale*rm::scale) - gs::width/(gs::scale*rm::scale) * (scalePrev/rm::scale))/2;
            rm::y -= (gs::height/(gs::scale*rm::scale) - gs::height/(gs::scale*rm::scale) * (scalePrev/rm::scale))/2;
            
            sprite.setScale(rm::roomScale * gs::scale * rm::scale, rm::roomScale * gs::scale * rm::scale);
            CalculateCameraScale();
        }
    }
    void Apartment::Resize(unsigned int width, unsigned int height)
    {
        if (!active) return;
        Room::xWidth = gs::width/(gs::scale * Room::scale); Room::yHeight = gs::height/(gs::scale * Room::scale);
        if (spriteLoaded) sprite.setScale(Room::roomScale * gs::scale * Room::scale, Room::roomScale * gs::scale * Room::scale);
        CalculateCameraScale();
    }
    void Apartment::Draw(sf::RenderWindow* window)
    {
        if (!active || gs::ignoreDraw) return;
        if (spriteLoaded) window->draw(sprite);
        neko.Draw(window);
    }
    void Apartment::RecieveMessage(MessageHolder& message)
    {
        if (!active) return;
    }
    void Apartment::CalculateCameraPosition()
    {
        if (sprite.getGlobalBounds().width <= gs::width) rm::x = gs::width/(2*gs::scale*rm::scale) - sprite.getLocalBounds().width*rm::roomScale/2;
        else if (rm::x > 100/(gs::scale*rm::scale)) rm::x = 100/(gs::scale*rm::scale);
        else if (rm::x < -sprite.getLocalBounds().width*rm::roomScale + gs::width/(gs::scale*rm::scale) - 100/(gs::scale*rm::scale))
            rm::x = -sprite.getLocalBounds().width*rm::roomScale + gs::width/(gs::scale*rm::scale) - 100/(gs::scale*rm::scale);
        if (sprite.getGlobalBounds().height <= gs::height) rm::y = gs::height/(2*gs::scale*rm::scale) - sprite.getLocalBounds().height*rm::roomScale/2;
        else if (rm::y > 100/(gs::scale*rm::scale)) rm::y = 100/(gs::scale*rm::scale);
        else if (rm::y < -sprite.getLocalBounds().height*rm::roomScale + gs::height/(gs::scale*rm::scale) - 100/(gs::scale*rm::scale))
            rm::y = -sprite.getLocalBounds().height*rm::roomScale + gs::height/(gs::scale*rm::scale) - 100/(gs::scale*rm::scale);
        
        neko.UpdatePosition();
        sprite.setPosition(rm::x * gs::scale * rm::scale, rm::y * gs::scale * rm::scale);
    }
    void Apartment::CalculateCameraScale()
    {
        if (sprite.getGlobalBounds().width <= gs::width) rm::x = gs::width/(2*gs::scale*rm::scale) - sprite.getLocalBounds().width*rm::roomScale/2;
        else if (rm::x > 100/(gs::scale*rm::scale)) rm::x = 100/(gs::scale*rm::scale);
        else if (rm::x < -sprite.getLocalBounds().width*rm::roomScale + gs::width/(gs::scale*rm::scale) - 100/(gs::scale*rm::scale))
            rm::x = -sprite.getLocalBounds().width*rm::roomScale + gs::width/(gs::scale*rm::scale) - 100/(gs::scale*rm::scale);
        if (sprite.getGlobalBounds().height <= gs::height) rm::y = gs::height/(2*gs::scale*rm::scale) - sprite.getLocalBounds().height*rm::roomScale/2;
        else if (rm::y > 100/(gs::scale*rm::scale)) rm::y = 100/(gs::scale*rm::scale);
        else if (rm::y < -sprite.getLocalBounds().height*rm::roomScale + gs::height/(gs::scale*rm::scale) - 100/(gs::scale*rm::scale))
            rm::y = -sprite.getLocalBounds().height*rm::roomScale + gs::height/(gs::scale*rm::scale) - 100/(gs::scale*rm::scale);
        
        neko.Resize();
        sprite.setPosition(rm::x * gs::scale * rm::scale, rm::y * gs::scale * rm::scale);
    }
}
