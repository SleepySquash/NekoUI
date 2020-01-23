//
//  OffsetHelper.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 20/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "OffsetHelper.hpp"

namespace NekoUI
{
    bool OffsetHelperSettings::disableSmoothing{ true };




    BodyPartHelper::BodyPartHelper(const std::wstring& spritepath, const bool& mode) : spritepath(spritepath), mode(mode) { }
    void BodyPartHelper::Init() { Switch(mode); }
    void BodyPartHelper::Resize(const float& constantScaling, const sf::Vector2f& pos, const float& bodyHeight)
    {
        if (mode) sprite.setScale( ((scaledPerson * bodyHeight) / sprite.getLocalBounds().height),
                                   ((scaledPerson * bodyHeight) / sprite.getLocalBounds().height) );
        else
            sprite.setScale(((scaledChibi * bodyHeight) / sprite.getLocalBounds().height), ((scaledChibi * bodyHeight) / sprite.getLocalBounds().height));
        UpdatePosition(constantScaling, pos);
    }
    void BodyPartHelper::UpdatePosition(const float& constantScaling, const sf::Vector2f& pos)
    {
        if (mode) sprite.setPosition(pos.x + offPerson.first * constantScaling, pos.y + offPerson.second * constantScaling);
        else sprite.setPosition(pos.x + offChibi.first * constantScaling, pos.y + offChibi.second * constantScaling);
    }
    void BodyPartHelper::Draw(sf::RenderWindow* window) { if (spriteLoaded) window->draw(sprite); }
    void BodyPartHelper::Switch(bool moded)
    {
        if (spriteLoaded) ic::DeleteImage(L"Data/Neko/" + std::wstring((mode ? L"Person/" : L"Chibi/")) + spritepath); mode = moded;
        sf::Texture* texture = ic::LoadTexture(L"Data/Neko/" + std::wstring((mode ? L"Person/" : L"Chibi/")) + spritepath);
        if ((spriteLoaded = texture))
        {
            if (OffsetHelperSettings::disableSmoothing)
                texture->setSmooth(false);
            sprite.setTexture(*texture, true);
            sprite.setOrigin(texture->getSize().x/2, 0);
            sprite.setColor(sf::Color(255,255,255, alpha));
        }
    }
    void BodyPartHelper::Load(const std::wstring& path)
    {
        if (spriteLoaded) ic::DeleteImage(L"Data/Neko/" + std::wstring((mode ? L"Person/" : L"Chibi/")) + spritepath);
        spriteLoaded = false; spritepath = path; Switch(mode);
    }
    void BodyPartHelper::SetAlpha(const sf::Uint8& alpha) { this->alpha = alpha; sprite.setColor(sf::Color(255,255,255, alpha)); }
    
    
    
    
    OffsetHelper::OffsetHelper(const std::wstring& spritepath, const bool& mode, const float& requiredScaling) : spritepath(spritepath), mode(mode), requiredScaling(requiredScaling) { }
    void OffsetHelper::Init()
    {
        sf::Texture* texture = ic::LoadTexture(L"Data/Neko/" + std::wstring((mode ? L"Person" : L"Chibi")) + L"/body.png");
        if ((bodyLoaded = texture))
        {
            if (OffsetHelperSettings::disableSmoothing)
                texture->setSmooth(false);
            body.setTexture(*texture, true);
            body.setOrigin(texture->getSize().x/2, texture->getSize().y);
            bodyRelative = (static_cast<float>(gs::relativeHeight)/static_cast<float>(texture->getSize().y));
        }
        
        parts.push_back(new BodyPartHelper(L"else.png", mode));
        parts.back()->offChibi = {-44, -921};    parts.back()->scaledChibi = 1.150638;
        parts.back()->offPerson = {-28, -844}; parts.back()->scaledPerson = 1.064829;
        parts.back()->alpha = 80; parts.back()->Init();
        
        /*parts.push_back(new BodyPartHelper(L"eyes_normal.png", mode));
        parts.back()->offChibi = {-28, -427};  parts.back()->scaledChibi = 0.329595;
        parts.back()->offPerson = {-27, -659}; parts.back()->scaledPerson = 0.455800;
        parts.back()->alpha = 145; parts.back()->Init();*/
        
        spritepath = L"eyes_confused1.png";
        parts.push_back(new BodyPartHelper(spritepath, mode));
        part = parts.back(); part->Init();
        
        offsetText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        offsetText.setOutlineColor(sf::Color::Cyan);
        
        modeText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        modeText.setOutlineColor(sf::Color::Cyan);
        modeText.setString("Zooming the VIEW");
        
        alphaText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        alphaText.setOutlineColor(sf::Color::Cyan);
        alphaText.setString("Alpha: 255");
        
        constrolsText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        constrolsText.setOutlineColor(sf::Color::Cyan);
        constrolsText.setString("Controls:\n1 - Zoom the PART\n2 - Zoom the VIEW\n3 - Change the ALPHA\nLMB - Move the PART\nMMB - Move the VIEW\nScroll - Zoom\nQ - Person/Chibi\nE - Open part");
        
        UpdateInformation();
    }
    void OffsetHelper::PollEvent(sf::Event& event)
    {
        if (event.type == sf::Event::MouseMoved && dxAllowed)
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                if (mode) part->offPerson = { dx + event.mouseMove.x/constantScaling - lastMouse.x,
                                              dy + event.mouseMove.y/constantScaling - lastMouse.y };
                else part->offChibi = { dx + event.mouseMove.x/constantScaling - lastMouse.x,
                                        dy + event.mouseMove.y/constantScaling - lastMouse.y };
                UpdatePosition();
                UpdateInformation();
            }
            else if ((sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) || sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle)))
            {
                x = dx + event.mouseMove.x/gs::scalex - lastMouse.x;
                y = dy + event.mouseMove.y/gs::scaley - lastMouse.y;
                UpdatePosition();
            }
        }
        else if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Button::Left)
            {
                /*if (mode) part->offPerson = { -body.getPosition().x/constantScaling + event.mouseButton.x/constantScaling,
                                              -body.getPosition().y/constantScaling + event.mouseButton.y/constantScaling };
                else part->offChibi = { -body.getPosition().x/constantScaling + event.mouseButton.x/constantScaling,
                                        -body.getPosition().y/constantScaling + event.mouseButton.y/constantScaling };*/
                dxAllowed = true;
                lastMouse = { event.mouseButton.x/constantScaling, event.mouseButton.y/constantScaling };
                if (mode) { dx = part->offPerson.first; dy = part->offPerson.second; }
                else { dx = part->offChibi.first; dy = part->offChibi.second; };
                UpdatePosition();
                UpdateInformation();
            }
            else if (event.mouseButton.button == sf::Mouse::Button::Right || event.mouseButton.button == sf::Mouse::Button::Middle)
            {
                dxAllowed = true; dx = x; dy = y;
                lastMouse = { event.mouseButton.x/gs::scalex, event.mouseButton.y/gs::scaley };
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased) dxAllowed = false;
        else if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.delta != 0 && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
        {
            if (editor == 2)
            {
                int prealpha = part->alpha + (0.4 * event.mouseWheelScroll.delta);
                if (prealpha < 0) prealpha = 0; else if (prealpha > 255) prealpha = 255;
                part->SetAlpha(prealpha);
                alphaText.setString("Alpha: " + std::to_string(prealpha));
                Resize(gs::width, gs::height);
            }
            if (editor == 1)
            {
                requiredScaling += 0.004 * requiredScaling * event.mouseWheelScroll.delta;
                Resize(gs::width, gs::height);
            }
            else if (editor == 0)
            {
                if (mode) part->scaledPerson += 0.004 * part->scaledPerson * event.mouseWheelScroll.delta;
                else part->scaledChibi += 0.004 * part->scaledChibi * event.mouseWheelScroll.delta;
                part->Resize(constantScaling, body.getPosition(), body.getGlobalBounds().height);
                UpdateInformation();
            }
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Key::Q)
            {
                if (mode) { x1 = x; y1 = y; requiredScaling1 = requiredScaling; x = x2; y = y2; requiredScaling = requiredScaling2; }
                else { x2 = x; y2 = y; requiredScaling2 = requiredScaling; x = x1; y = y1; requiredScaling = requiredScaling1; }
                ic::DeleteImage(L"Data/Neko/" + std::wstring((mode ? L"Person" : L"Chibi")) + L"/body.png");
                mode = !mode;
                
                sf::Texture* texture = ic::LoadTexture(L"Data/Neko/" + std::wstring((mode ? L"Person" : L"Chibi")) + L"/body.png");
                if ((bodyLoaded = texture))
                {
                    if (OffsetHelperSettings::disableSmoothing)
                        texture->setSmooth(false);
                    body.setTexture(*texture, true);
                    body.setOrigin(texture->getSize().x/2, texture->getSize().y);
                    bodyRelative = (static_cast<float>(gs::relativeHeight)/static_cast<float>(texture->getSize().y));
                }
                for (auto& p : parts) p->Switch(mode);
                Resize(gs::width, gs::height);
                UpdateInformation();
            }
            else if (event.key.code == sf::Keyboard::Key::E)
            {
                /*std::string p = resourcePath() + "Data/Neko/" + std::string((mode ? "Person/" : "Chibi/"));
                nfdchar_t *outPath = NULL, *defaultPath = strdup(p.c_str());
                nfdresult_t result = NFD_OpenDialog( NULL, defaultPath, &outPath );
                    
                if ( result == NFD_OKAY )
                {
                    spritepath = utf16(std::string(outPath));
                    for (char* c = defaultPath; *c != '\0'; ++c) spritepath.erase(spritepath.begin());
                    
                    part->Load(spritepath);
                    Resize(gs::width, gs::height);
                    UpdateInformation();
                    
                    free(outPath);
                }
                else if ( result == NFD_CANCEL ) puts("User pressed cancel.");
                else printf("Error: %s\n", NFD_GetError() );
                free(defaultPath);*/
            }
            else if (event.key.code == sf::Keyboard::Key::Num1)
            {
                dxAllowed = false; editor = 0;
                modeText.setString("Zooming the PART");
            }
            else if (event.key.code == sf::Keyboard::Key::Num2)
            {
                editor = 1; dxAllowed = false;
                modeText.setString("Zooming the VIEW");
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) { dx = x; dy = y;
                    lastMouse = { sf::Mouse::getPosition(*gs::window).x/gs::scalex, sf::Mouse::getPosition(*gs::window).y/gs::scaley }; }
            }
            else if (event.key.code == sf::Keyboard::Key::Num3)
            {
                editor = 2; dxAllowed = false;
                modeText.setString("Changing the ALPHA");
            }
        }
    }
    void OffsetHelper::Resize(unsigned int width, unsigned int height)
    {
        constantScaling = requiredScaling*gs::scale;
        body.setScale(bodyRelative * constantScaling, bodyRelative * constantScaling);
        body.setPosition(x*gs::scale + gs::width/2, y*gs::scale + gs::height/2 + body.getGlobalBounds().height/2);
        
        offsetText.setOutlineThickness(gs::scale);
        offsetText.setCharacterSize(40*gs::scale);
        
        modeText.setOutlineThickness(gs::scale);
        modeText.setCharacterSize(32*gs::scale);
        modeText.setPosition(0, offsetText.getGlobalBounds().top + offsetText.getGlobalBounds().height*1.1f);
        
        alphaText.setOutlineThickness(gs::scale);
        alphaText.setCharacterSize(32*gs::scale);
        alphaText.setPosition(0, modeText.getGlobalBounds().top + modeText.getGlobalBounds().height*1.1f);
        
        constrolsText.setOutlineThickness(gs::scale);
        constrolsText.setCharacterSize(32*gs::scale);
        constrolsText.setPosition(gs::width - constrolsText.getGlobalBounds().width*1.1, 0);
        
        for (auto& p : parts) p->Resize(constantScaling, body.getPosition(), body.getGlobalBounds().height);
    }
    void OffsetHelper::UpdatePosition()
    {
        body.setPosition(x*gs::scale + gs::width/2, y*gs::scale + gs::height/2 + body.getGlobalBounds().height/2);
        for (auto& p : parts) p->UpdatePosition(constantScaling, body.getPosition());
    }
    void OffsetHelper::Draw(sf::RenderWindow* window)
    {
        if (bodyLoaded) window->draw(body);
        for (auto& p : parts) p->Draw(window);
        window->draw(offsetText);
        window->draw(modeText);
        window->draw(alphaText);
        window->draw(constrolsText);
    }
    void OffsetHelper::UpdateInformation()
    {
        if (!mode) offsetText.setString("(" + std::to_string(part->offChibi.first) + " " + std::to_string(part->offChibi.second) + ")  " + std::to_string(part->scaledChibi));
        else offsetText.setString("(" + std::to_string(part->offPerson.first) + " " + std::to_string(part->offPerson.second) + ")  " + std::to_string(part->scaledPerson));
    }
}
