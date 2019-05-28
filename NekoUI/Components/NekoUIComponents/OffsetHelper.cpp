//
//  OffsetHelper.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 20/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "OffsetHelper.hpp"

namespace NekoUI
{
    OffsetHelper::OffsetHelper(const std::wstring& spritepath, const bool& mode, const float& requiredScaling) : spritepath(spritepath), mode(mode), requiredScaling(requiredScaling) { }
    void OffsetHelper::Init()
    {
        sf::Texture* texture = ic::LoadTexture(L"Data/Neko/" + std::wstring((mode ? L"Person" : L"Chibi")) + L"/body.png");
        if ((bodyLoaded = texture))
        {
            body.setTexture(*texture, true);
            body.setOrigin(texture->getSize().x/2, texture->getSize().y);
            bodyRelative = (static_cast<float>(gs::relativeHeight)/static_cast<float>(texture->getSize().y));
        }
        
        texture = ic::LoadTexture(L"Data/Neko/" + std::wstring((mode ? L"Person/" : L"Chibi/")) + spritepath);
        if ((spriteLoaded = texture))
        {
            sprite.setTexture(*texture, true);
            sprite.setOrigin(texture->getSize().x/2, 0);
        }
        
        text.setFont(*fc::GetFont(L"Pacifica.ttf"));
        text.setOutlineColor(sf::Color::Cyan);
    }
    void OffsetHelper::PollEvent(sf::Event& event)
    {
        if (event.type == sf::Event::MouseMoved)
        {
            mousex = event.mouseMove.x/constantScaling; mousey = event.mouseMove.y/constantScaling;
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                sprite.setPosition(mousex * constantScaling, mousey * constantScaling);
                text.setString("(" + std::to_string(mousex - body.getPosition().x/constantScaling) + " " + std::to_string(mousey - body.getPosition().y/constantScaling) + ")  " + std::to_string(sprite.getGlobalBounds().height/body.getGlobalBounds().height));
            }
        }
        else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left)
        {
            mousex = event.mouseButton.x/gs::scalex; mousey = event.mouseButton.y/gs::scaley;
            sprite.setPosition(mousex * gs::scalex, mousey * gs::scaley);
        }
        else if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.delta != 0 && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
        {
            scaled += 0.003 * event.mouseWheelScroll.delta;
            sprite.setScale(gs::scale * scaled, gs::scale * scaled);
            text.setString("(" + std::to_string(mousex - body.getPosition().x/constantScaling) + " " + std::to_string(mousey - body.getPosition().y/constantScaling) + ")  " + std::to_string(sprite.getGlobalBounds().height/body.getGlobalBounds().height));
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Key::K)
            {
                ic::DeleteImage(L"Data/Neko/" + std::wstring((mode ? L"Person" : L"Chibi")) + L"/body.png");
                ic::DeleteImage(L"Data/Neko/" + std::wstring((mode ? L"Person/" : L"Chibi/")) + spritepath);
                mode = !mode;
                
                sf::Texture* texture = ic::LoadTexture(L"Data/Neko/" + std::wstring((mode ? L"Person" : L"Chibi")) + L"/body.png");
                if ((bodyLoaded = texture))
                {
                    body.setTexture(*texture, true);
                    body.setOrigin(texture->getSize().x/2, texture->getSize().y);
                    bodyRelative = (static_cast<float>(gs::relativeHeight)/static_cast<float>(texture->getSize().y));
                }
                texture = ic::LoadTexture(L"Data/Neko/" + std::wstring((mode ? L"Person/" : L"Chibi/")) + spritepath);
                if ((spriteLoaded = texture))
                {
                    sprite.setTexture(*texture, true);
                    sprite.setOrigin(texture->getSize().x/2, 0);
                }
                Resize(gs::width, gs::height);
            }
            else if (event.key.code == sf::Keyboard::Key::D)
            {
                std::string str;
                char c; while ((c = getchar()) != '\n') str += c;
                std::wstring wstr = utf16(str);
                if (base::FileExists(utf16(resourcePath()) + L"Data/Neko/" + std::wstring((mode ? L"Person/" : L"Chibi/")) + wstr))
                {
                    ic::DeleteImage(L"Data/Neko/" + std::wstring((mode ? L"Person/" : L"Chibi/")) + spritepath);
                    spritepath = wstr;
                    
                    sf::Texture* texture = ic::LoadTexture(L"Data/Neko/" + std::wstring((mode ? L"Person/" : L"Chibi/")) + spritepath);
                    if ((spriteLoaded = texture))
                    {
                        sprite.setTexture(*texture, true);
                        sprite.setOrigin(texture->getSize().x/2, 0);
                    }
                    Resize(gs::width, gs::height);
                }
            }
        }
    }
    void OffsetHelper::Resize(unsigned int width, unsigned int height)
    {
        /*float scale = 0.6f * height/body.getLocalBounds().height;
        body.setScale(scale, scale);
        body.setPosition(width/2, height/2 + body.getGlobalBounds().height/2);
        realBodyScale = body.getGlobalBounds().height/static_cast<float>(gs::relativeHeight);*/
        constantScaling = requiredScaling*gs::scale;
        body.setScale(bodyRelative * constantScaling, bodyRelative * constantScaling);
        body.setPosition(width/2, height/2 + body.getGlobalBounds().height/2);
        
        sprite.setPosition(mousex * constantScaling, mousey * constantScaling);
        sprite.setScale(gs::scale * scaled, gs::scale * scaled);
        
        text.setOutlineThickness(gs::scale);
        text.setCharacterSize(50*gs::scale);
    }
    void OffsetHelper::Draw(sf::RenderWindow* window)
    {
        if (bodyLoaded) window->draw(body);
        if (spriteLoaded) window->draw(sprite);
        window->draw(text);
    }
}
