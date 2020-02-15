//
//  Dialogue.cpp
//  NovelSome
//
//  Created by Никита Исаенко on 11/09/2018.
//  Copyright © 2018 Melancholy Hill. All rights reserved.
//

#include "Dialogue.hpp"

namespace ns
{
    namespace NovelComponents
    {
        void Dialogue::Init()
        {
            appearTime = .4f;
            disappearTime = .4f;
            maxAlpha = 255;
            characterSize = 34;
            characterInSecond = 0.03;
            fontName = L"Noteworthy-Bold.ttf";
            
            afterRedLineShift = -30;
            
            textAppearMode = textAppearModeEnum::printing;
            forcePressInsideDialogue = gs::forcePressInsideDialogue;
            
            text.setFillColor(sf::Color::White);
            // text.setOutlineColor(sf::Color::Black);
            text.setFont(*fc::GetFont(fontName));
            text.setLineSpacing(0.6f);
            fontLoaded = text.getFont();
            charText.setFont(*fc::GetFont(fontName));
            charText.setFillColor(sf::Color::White);
            // charText.setOutlineColor(sf::Color::Black);
            
            sf::Texture* texture = ic::LoadTexture(L"Data/Images/UI/dialogue1.png");
            if (texture)
            {
                sprite.setTexture(*texture);
                sprite.setOrigin(texture->getSize().x/2, texture->getSize().y);
            }
            
            texture = ic::LoadTexture(L"Data/Images/UI/dialogue_n.png");
            if (texture)
            {
                nameSprite.setTexture(*texture);
                nameSprite.setOrigin(0, texture->getSize().y);
            }
        }
        void Dialogue::Update(const sf::Time& elapsedTime)
        {
            if (mode != Mode::Deprecate && visible && textAppearMode == textAppearModeEnum::printing && textAppearPos < textAppearMax)
            {
                elapsedCharacterSum += elapsedTime.asSeconds();
                while (elapsedCharacterSum > characterInSecond && textAppearPos < textAppearMax)
                {
                    gs::requestWindowRefresh = true;
                    ++textAppearPos; elapsedCharacterSum -= characterInSecond;
                    
                    while (printingString[textAppearI] == L'\n' || printingString[textAppearI] == L' ')
                        currentString += printingString[textAppearI++];
                    currentString += printingString[textAppearI++];
                    text.setString(currentString);
                }
            }
            switch (mode)
            {
                case Mode::Appear: gs::requestWindowRefresh = true;
                    if (currentTime < appearTime) currentTime += elapsedTime.asSeconds();
                    if (currentTime >= appearTime) {
                        alpha = maxAlpha; currentTime = 0.f; mode = switchTo;
                        if (messageBack == MessageBack::AtAppearance) novelSystem->SendMessage({"UnHold", this});
                    } else alpha = (sf::Uint8)(maxAlpha * (currentTime / appearTime));
                    UpdateAlpha(true);
                    break;
                    
                case Mode::Disapper: gs::requestWindowRefresh = true;
                    if (currentTime < disappearTime) currentTime += elapsedTime.asSeconds();
                    if (currentTime >= disappearTime) {
                        alpha = 0; currentTime = 0.f; mode = Mode::Deprecate;
                        if (messageBack == MessageBack::AtDeprecated) novelSystem->SendMessage({"UnHold", this});
                    } else alpha = (sf::Uint8)(maxAlpha - (maxAlpha * (currentTime / disappearTime)));
                    UpdateAlpha();
                    break;
                case Mode::Deprecate: gs::requestWindowRefresh = true; novelSystem->PopComponent(this); break;
                case Mode::WaitingForTime:
                    if (currentTime < waitingTime) currentTime += elapsedTime.asSeconds();
                    if (currentTime >= waitingTime)
                    {
                        currentTime = 0.f; mode = Mode::Disapper;
                        if (messageBack == MessageBack::AtDisappearance) novelSystem->SendMessage({"UnHold", this});
                    }
                    break;
                case Mode::WaitingForChoose: /*if (novel == nullptr || novel->chooseGroup.size() == 0) mode = disappearing;*/ break;
                default: break;
            }
        }
        void Dialogue::PollEvent(sf::Event& event)
        {
            if (mode == Mode::WaitingForChoose || mode == Mode::WaitingForInput)
            {
                // if (guiSystem && visible) guiSystem->PollEvent(event);
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) visible = !visible;
            }
            if (mode == Mode::WaitingForInput && visible)
            {
                if (wasPressed && (event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::TouchEnded))
                {
                    wasPressed = workingArea.contains(gs::lastMousePos.first, gs::lastMousePos.second);
                    if (wasPressed)
                    {
                        bool fadeAway{ true }; wasPressed = false;
                        if (textAppearMode == textAppearModeEnum::printing && textAppearPos != textAppearMax)
                            currentString = printingString, text.setString(currentString), textAppearPos = textAppearMax, fadeAway = false;
                        
                        event = sf::Event();
                        if (fadeAway)
                        {
                            mode = Mode::Disapper;
                            if (messageBack == MessageBack::AtDisappearance) novelSystem->SendMessage({"UnHold", this});
                        }
                    }
                }
                else if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::TouchBegan)
                    wasPressed = workingArea.contains(gs::lastMousePos.first, gs::lastMousePos.second);
            }
            else if (mode == Mode::WaitingForChoose)
            {
                if (visible && (event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::TouchEnded) && textAppearMode == textAppearModeEnum::printing && textAppearPos != textAppearMax)
                {
                    bool pressed{ ((event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Button::Left && workingArea.contains(event.mouseButton.x, event.mouseButton.y)) || (event.type == sf::Event::TouchEnded && workingArea.contains(event.touch.x, event.touch.y))) };
                    if (pressed)
                    {
                        currentString = printingString; text.setString(currentString);
                        textAppearPos = textAppearMax; event = sf::Event();
                    }
                }
            }
        }
        void Dialogue::Draw(sf::RenderWindow* window)
        {
            if (visible)
            {
                // if (guiSystem) guiSystem->Draw(window);
                window->draw(sprite);
                window->draw(nameSprite);
                if (fontLoaded)
                {
                    window->draw(text);
                    if (drawCharacterName)
                        window->draw(charText);
                }
            }
        }
        void Dialogue::Destroy() { novelSystem->SendMessage({"Destroy", this}); }
        void Dialogue::Resize(const unsigned int& width, const unsigned int& height)
        {
            charText.setCharacterSize((unsigned int)(characterSize * gs::scale));
            text.setCharacterSize((unsigned int)(characterSize * gs::scale));
            
            sprite.setScale(1.3*gs::scale, 1.3*gs::scale);
            if (sprite.getGlobalBounds().width > gs::width) sprite.setScale(gs::width/sprite.getLocalBounds().width, gs::width/sprite.getLocalBounds().width);
            sprite.setPosition(gs::width/2, gs::height - 10*gs::scale);
            
            if (drawCharacterName)
            {
                nameSprite.setScale(0.8*gs::scale, 0.8*gs::scale);
                nameSprite.setPosition(sprite.getGlobalBounds().left + sprite.getGlobalBounds().width/9.f, sprite.getPosition().y - sprite.getGlobalBounds().height + nameSprite.getGlobalBounds().height/2 - 15*gs::scale);
            }
            
            if (fontLoaded)
            {
                text.setOutlineThickness(1 * gs::scale);
                if (drawCharacterName && charThickness)
                    charText.setOutlineThickness(charThickness * gs::scale);
                
                int textWidth = sprite.getGlobalBounds().width*0.85;
                int textXOffset = sprite.getGlobalBounds().left + 70*gs::scale;
                if (textString != L"" && afterRedLineShift < 0) textXOffset += afterRedLineShift*gs::scale;
                int textYOffset = sprite.getGlobalBounds().top + 24*gs::scale;
                
                workingArea.left = sprite.getGlobalBounds().left;
                workingArea.width = sprite.getGlobalBounds().width;
                workingArea.top = sprite.getGlobalBounds().top;
                workingArea.height = sprite.getGlobalBounds().height;
                
                int nameTextWidth, nameTextXOffset, nameTextYOffset;
                if (charString != L"")
                {
                    nameTextWidth = nameSprite.getGlobalBounds().width;
                    charText.setString(nss::GetStringWithLineBreaks(charText, charString, nameTextWidth, 0, afterRedLineShift*gs::scale));
                    nameTextXOffset = nameSprite.getGlobalBounds().left + nameSprite.getGlobalBounds().width/2 - charText.getGlobalBounds().width/2;
                    if (charString != L"" && afterRedLineShift < 0) nameTextXOffset += afterRedLineShift*gs::scale;
                    nameTextYOffset = nameSprite.getGlobalBounds().top + 10*gs::scale;
                }
                if (textString != L"")
                {
                    if (textAppearMode ==  textAppearModeEnum::printing)
                    {
                        currentString = L""; textAppearI = 0;
                        printingString = nss::GetStringWithLineBreaks(text, textString, textWidth, 0, afterRedLineShift*gs::scale);
                        for (size_t i = 0; i < textAppearPos; ++i)
                        {
                            while (printingString[textAppearI] == L'\n' || printingString[textAppearI] == L' ')
                                currentString += printingString[textAppearI++];
                            currentString += printingString[textAppearI++];
                        }
                        text.setString(currentString);
                    }
                    else text.setString(nss::GetStringWithLineBreaks(text, textString, textWidth, 0, afterRedLineShift*gs::scale));
                }
            
                text.setPosition(textXOffset, textYOffset);
                if (drawCharacterName) charText.setPosition(nameTextXOffset, nameTextYOffset);
            }
        }
        void Dialogue::SetCharacter(CharacterData* character)
        {
            if (character)
            {
                this->character = character;
                charText.setOutlineThickness(1*gs::scale);
                charText.setFillColor(sf::Color(character->fillColor.r, character->fillColor.g, character->fillColor.b, alpha));
                charText.setOutlineColor(sf::Color(character->outlineColor.r, character->outlineColor.g, character->outlineColor.b, alpha));
            }
        }
        void Dialogue::SetCharacterName(const sf::String& characterName)
        {
            charString = characterName;
            drawCharacterName = true;
            
            charText.setString(charString);
            
            charText.setFillColor(sf::Color(charText.getFillColor().r, charText.getFillColor().g, charText.getFillColor().b, alpha));
            charText.setOutlineColor(sf::Color(charText.getOutlineColor().r, charText.getOutlineColor().g, charText.getOutlineColor().b, alpha));
        }
        void Dialogue::SetDialogue(const sf::String& dialogue)
        {
            textString = dialogue;
            if (charString.length() != 0)
            {
                if (leftSpeechAddition != 0) textString.insert(textString.begin(), leftSpeechAddition);
                if (rightSpeechAddition != 0) textString += rightSpeechAddition;
            }
            
            printingString = textString;
            if (textAppearMode == textAppearModeEnum::printing)
            {
                currentString = L""; textAppearMax = base::GetLengthWONewLinesAndSpaces(textString);
                text.setString(currentString);
            }
            else text.setString(printingString);
            
            Resize(gs::width, gs::height);
        }
        void Dialogue::SetStateMode(const Mode& newMode)
        {
            gs::requestWindowRefresh = true;
            if (mode != newMode) { currentTime = 0.f; mode = newMode; }
        }
        void Dialogue::ReceiveMessage(MessageHolder &message) { if (message.info == "Show/Hide Interface") visible = !visible; }
        void Dialogue::UpdateAlpha(bool mode)
        {
            text.setFillColor(sf::Color(text.getFillColor().r, text.getFillColor().g, text.getFillColor().b, alpha));
            if (text.getOutlineThickness() != 0)
                text.setOutlineColor(sf::Color(text.getOutlineColor().r, text.getOutlineColor().g, text.getOutlineColor().b, alpha));
            if (drawCharacterName)
            {
                charText.setFillColor(sf::Color(charText.getFillColor().r, charText.getFillColor().g, charText.getFillColor().b, alpha));
                charText.setOutlineColor(sf::Color(charText.getOutlineColor().r, charText.getOutlineColor().g, charText.getOutlineColor().b, alpha));
            }
            sprite.setColor(sf::Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, alpha));
            nameSprite.setColor(sf::Color(nameSprite.getColor().r, nameSprite.getColor().g, nameSprite.getColor().b, 0.8f*alpha));
        }
    }
}
