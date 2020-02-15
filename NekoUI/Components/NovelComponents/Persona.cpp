//
//  Persona.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 13.02.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#include "Persona.hpp"

namespace ns
{
    namespace NovelComponents
    {
        Persona::Persona(NekoUI::Persona* persona) : persona(persona) { }
        void Persona::Resize(const unsigned int& width, const unsigned int& height)
        {
            if (persona)
            {
                // sprite.setScale(scaleX * (doParallax ? (1 + parallaxPower) : 1) * gs::scScale, scaleY * (doParallax ? (1 + parallaxPower) : 1) * gs::scScale);
                // sprite.setOrigin(sprite.getLocalBounds().width/2, (sprite.getLocalBounds().height > height/sprite.getScale().y ? height/sprite.getScale().y : (sprite.getLocalBounds().height - (doParallax ? (sprite.getLocalBounds().height*parallaxPower) : 0))));
                
                if (gs::verticalOrientation) { persona->personScale = 1.4 * gs::scScale; persona->ResizePerson(); }
                else { persona->personScale = 1.3 * gs::scScale; persona->ResizePerson(); }
                
                float posx, posy{ 0 };
                switch (position)
                {
                    case Position::Left: posx = width/6.f; break;
                    case Position::CLeft: posx = width/3.f; break;
                    case Position::Center: posx = width/2.f; break;
                    case Position::CRight: posx = width - width/3.f; break;
                    case Position::Right: posx = width - width/6.f; break;
                    default: posx = customX; posy = customY; break;
                }
                if (persona->body.person.getGlobalBounds().height + 20*gs::scale > gs::height)
                    persona->setPersonPosition(posx, persona->body.person.getGlobalBounds().height + 20*gs::scale + gs::screenOffsetTop);
                else persona->setPersonPosition(posx, height + gs::screenOffsetTop);
                
                defaultPositionX = persona->body.person.getPosition().x;
                defaultPositionY = persona->body.person.getPosition().y;
                if (doParallax && !(gs::isPauseEnabled && gs::isPause))
                    CalculateParallax(sf::Mouse::getPosition(*ns::gs::window).x, sf::Mouse::getPosition(*ns::gs::window).y);
            }
        }
        void Persona::Update(const sf::Time& elapsedTime)
        {
            switch (mode)
            {
                case Mode::Appear: gs::requestWindowRefresh = true;
                    if (currentTime < appearTime) currentTime += elapsedTime.asSeconds();
                    if (currentTime >= appearTime) { alpha = maxAlpha; currentTime = disappearTime; mode = switchTo;
                        if (messageBack == MessageBack::AtAppearance) novelSystem->SendMessage({"UnHold", this}); }
                    else alpha = (sf::Uint8)(maxAlpha * (currentTime / appearTime));
                    persona->setPersonAlpha(alpha);
                    break;
                    
                case Mode::Disapper: gs::requestWindowRefresh = true;
                    if (currentTime > 0) currentTime -= elapsedTime.asSeconds();
                    if (currentTime <= 0) { alpha = 0; currentTime = 0.f; mode = Mode::Deprecate;
                        if (messageBack == MessageBack::AtDeprecated) novelSystem->SendMessage({"UnHold", this}); }
                    else alpha = (sf::Uint8)(maxAlpha * (currentTime / disappearTime));
                    persona->setPersonAlpha(alpha);
                    break;
                case Mode::Deprecate: gs::requestWindowRefresh = true; novelSystem->PopComponent(this); break;
                default: break;
            }
        }
        void Persona::Draw(sf::RenderWindow* window) { if (persona && visible) persona->Draw(window, true); }
        void Persona::Destroy() { novelSystem->SendMessage({"Destroy", this}); }
        void Persona::PollEvent(sf::Event& event)
        {
            if (event.type == sf::Event::MouseMoved && mode != Mode::Deprecate && visible && doParallax && parallaxPower > 0)
                CalculateParallax(event.mouseMove.x, event.mouseMove.y);
        }
        void Persona::CalculateParallax(int mouseX, int mouseY)
        {
            if (mouseX >= 0 && mouseY >= 0 && mouseX <= ns::gs::width && mouseY <= ns::gs::height)
            {
                float posX = defaultPositionX + (int)(mouseX - ns::gs::width/2) * parallaxPower;
                float posY = defaultPositionY + (int)(mouseY - ns::gs::height/2) * parallaxPower;
                // sprite.setPosition(posX, posY);
            }
        }
        void Persona::SetStateMode(const Mode& newMode)
        {
            if (mode != newMode && mode != Mode::Deprecate)
            {
                if (newMode == Mode::Disapper) currentTime = disappearTime; else currentTime = 0.f; mode = newMode;
                if ((newMode == Mode::Disapper && messageBack == MessageBack::AtDisappearance) ||
                    (newMode == Mode::Deprecate && messageBack == MessageBack::AtDeprecated))
                    novelSystem->SendMessage({"UnHold", this});
            }
        }
        void Persona::SetPosition(const Position& pos, float x, float y) { position = pos; customX = x; customY = y; }
    }
}
