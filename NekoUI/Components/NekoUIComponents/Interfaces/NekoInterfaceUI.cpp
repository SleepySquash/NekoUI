//
//  NekoInterfaceUI.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 17/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "NekoInterfaceUI.hpp"

namespace NekoUI
{
    void NekoInterfaceUI::Init()
    {
        nameText.setFont(*fc::GetFont(L"Noteworthy-Bold.ttf"));
        nameText.setOutlineColor(sf::Color::Black);
        blackScreenShape.setFillColor(sf::Color(0,0,0,170));
        choiceBackShape.setFillColor(sf::Color(0,0,0,100));
        
        sf::Texture* texture = ic::LoadTexture(L"Data/Images/UI/heart.png");
        if (texture)
        {
            heartSprite.setTexture(*texture);
            heartSprite.setTextureRect(sf::IntRect(268, 0, 268, 256));
            heartSprite.setOrigin(texture->getSize().x, 0);
        }
        texture = ic::LoadTexture(L"Data/Images/UI/speech_bubble.png");
        if (texture)
        {
            speechBubbleSprite.setTexture(*texture);
            speechBubbleSprite.setOrigin(texture->getSize().x/2, 0);
            speechBubbleSprite.setColor(sf::Color(255,182,193));
        }
        speechBubbleText.setFont(*fc::GetFont(L"Noteworthy-Bold.ttf"));
        speechBubbleText.setOutlineColor(sf::Color::Black);
        speechBubbleText.setString("...");
        
        talkButton.setFont(L"Noteworthy-Bold.ttf");
        talkButton.setCharacterSize(62/*80*/);
        talkButton.setString(L"Поговорить");
        talkButton.halign = Halign::Right;
        requestButton.setFont(L"Noteworthy-Bold.ttf");
        requestButton.setCharacterSize(62);
        requestButton.setString(L"Просьба");
        engageButton.setFont(L"Noteworthy-Bold.ttf");
        engageButton.setCharacterSize(62);
        engageButton.setString(L"Занятие");
        actionButton.setFont(L"Noteworthy-Bold.ttf");
        actionButton.setCharacterSize(62);
        actionButton.setString(L"Действие");
        requestButton.valign = Valign::Bottom;
        engageButton.valign = actionButton.valign = Valign::Top;
        requestButton.halign = engageButton.halign = actionButton.halign = Halign::Left;
        talkButton.onormalColor = requestButton.onormalColor = engageButton.onormalColor = actionButton.onormalColor = sf::Color(99, 70, 70);
        talkButton.thickness = requestButton.thickness = engageButton.thickness = actionButton.thickness = 4.f;
        talkButton.updateColor(); requestButton.updateColor(); engageButton.updateColor(); actionButton.updateColor();
        
        
        talkChooseCircle.setTexture(L"Data/Images/UI/talkChooseCircle.png");
        talkChooseCircle.setScale(0.7f);
        
        talkNekoButton.setFont(L"Noteworthy-Bold.ttf");
        talkNekoButton.setCharacterSize(75);
        talkNekoButton.setString(L"О тебе");
        talkSelfButton.setFont(L"Noteworthy-Bold.ttf");
        talkSelfButton.setCharacterSize(75);
        talkSelfButton.setString(L"Обо мне");
        talkInterestButton.setFont(L"Noteworthy-Bold.ttf");
        talkInterestButton.setCharacterSize(75);
        talkInterestButton.setString(L"Об интересе");
        talkSkillButton.setFont(L"Noteworthy-Bold.ttf");
        talkSkillButton.setCharacterSize(75);
        talkSkillButton.setString(L"Об умении");
        talkNekoButton.halign = talkSelfButton.halign = talkInterestButton.halign = talkSkillButton.halign = Halign::Right;
        choiceButtons.onormalColor = talkNekoButton.onormalColor = talkSelfButton.onormalColor = talkInterestButton.onormalColor = talkSkillButton.onormalColor = sf::Color(99, 70, 70);
        choiceButtons.thickness = talkNekoButton.thickness = talkSelfButton.thickness = talkInterestButton.thickness = talkSkillButton.thickness = 4.f;
        choiceButtons.updateColor(); talkNekoButton.updateColor(); talkSelfButton.updateColor(); talkInterestButton.updateColor(); talkSkillButton.updateColor();
        
        choiceButtons.setFont(L"Noteworthy-Bold.ttf");
        choiceButtons.setCharacterSize(60);
    }
    void NekoInterfaceUI::Destroy() { ic::DeleteImage(L"Data/Images/UI/heart.png");
        ic::DeleteImage(L"Data/Images/UI/speech_bubble.png"); if (active) CleanUp(); }
    void NekoInterfaceUI::Update(const sf::Time& elapsedTime)
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
        if (drawSpeechBubble && elapsedSpeechBubble > 0.f)
        {
            elapsedSpeechBubble -= elapsedTime.asSeconds();
            if (elapsedSpeechBubble <= 0.f)
            {
                drawSpeechBubble = false;
                speechBubbleLine = L"..."; gs::requestWindowRefresh = true;
                speechBubbleText.setString(speechBubbleLine);
                speechBubbleText.setPosition(speechBubbleSprite.getPosition().x - speechBubbleText.getGlobalBounds().width/2, speechBubbleSprite.getPosition().y + speechBubbleSprite.getGlobalBounds().height/2 - speechBubbleText.getGlobalBounds().height/2);
            }
        }
    }
    void NekoInterfaceUI::CleanUp() { speechBubbleLine = L"..."; elapsedSpeechBubble = 0.f; nekoIsSleeping = false; }
    void NekoInterfaceUI::PollEvent(sf::Event& event)
    {
        if (!active || !gs::isActiveInterface(this)) return;
        
        switch (screen)
        {
            case Screen::main:
                if (talkButton.PollEvent(event)) { screen = Screen::talk; }
                else if (requestButton.PollEvent(event)) { }
                else if (engageButton.PollEvent(event)) { }
                else if (actionButton.PollEvent(event)) { }
                break;
            case Screen::talk:
                if (talkChooseCircle.PollEvent(event)) { screen = Screen::main; }
                else if (talkNekoButton.PollEvent(event)) { }
                else if (talkSelfButton.PollEvent(event)) { }
                else if (talkInterestButton.PollEvent(event)) { }
                else if (talkSkillButton.PollEvent(event)) { }
                break;
            default: break;
        }
    }
    void NekoInterfaceUI::Resize(unsigned int width, unsigned int height)
    {
        if (!active) return;
        
        blackScreenShape.setSize({(float)gs::width, (float)gs::height});
        choiceBackShape.setSize({(float)gs::width, (float)gs::height});
        if (gs::verticalOrientation) { Player::neko.personScale = 1.4 * gs::scScale; Player::neko.ResizePerson(); }
        else { Player::neko.personScale = 1.3 * gs::scScale; Player::neko.ResizePerson(); }
        if (Player::neko.body.person.getGlobalBounds().height + 20*gs::scale > gs::height)
            Player::neko.setPersonPosition(width/2, Player::neko.body.person.getGlobalBounds().height + 20*gs::scale + gs::screenOffsetTop);
        else Player::neko.setPersonPosition(width/2, height + gs::screenOffsetTop);
        
        nameText.setCharacterSize(80 * gs::scale);
        nameText.setOrigin(nameText.getGlobalBounds().width/2, 0);
        nameText.setOutlineThickness(gs::scale);
        
        heartSprite.setScale(0.25*gs::scale, 0.25*gs::scale);
        nameText.setScale(1, 1);
        // if (nameText.getLocalBounds().width + heartSprite.getGlobalBounds().width + 45*gs::scale > nintShape.getSize().x) nameText.setScale(nintShape.getSize().x/(nintNameText.getLocalBounds().width + nintHeart.getGlobalBounds().width + 45*gs::scale), nintNameText.getScale().y);
        nameText.setPosition(gs::width/4.25f, 5*gs::scale + gs::screenOffsetTop);
        heartSprite.setPosition(nameText.getGlobalBounds().left + 40*gs::scale, 23*gs::scale + gs::screenOffsetTop);
        
        talkButton.Resize(width, height);
        requestButton.Resize(width, height);
        engageButton.Resize(width, height);
        actionButton.Resize(width, height);
        float yyAround{ gs::height/2.f }; if (gs::trueVerticalOrientation) yyAround = 2.2f*gs::height/3.f;
        talkButton.setPosition(19.f*gs::width/20, yyAround);
        requestButton.setPosition(talkButton.text.getGlobalBounds().left + 35*gs::scale, talkButton.text.getGlobalBounds().top - 6*gs::scale/* - 25*gs::scale*/);
        engageButton.setPosition(talkButton.text.getGlobalBounds().left + 35*gs::scale, talkButton.text.getGlobalBounds().top + talkButton.text.getGlobalBounds().height - 3*gs::scale/* + 5*gs::scale*/);
        actionButton.setPosition(engageButton.text.getGlobalBounds().left + 35*gs::scale, engageButton.text.getGlobalBounds().top + engageButton.text.getGlobalBounds().height/* + 8*gs::scale*/);
        
        if (drawSpeechBubble)
        {
            speechBubbleSprite.setScale(0.7*gs::scale, 0.7*gs::scale);
            speechBubbleSprite.setPosition(3.1f*gs::width/4, gs::screenOffsetTop + 20*gs::scale);
            speechBubbleText.setCharacterSize(28*gs::scale);
            speechBubbleText.setOutlineThickness(gs::scale);
            speechBubbleText.setString(nss::GetStringWithLineBreaks(speechBubbleText, speechBubbleLine, speechBubbleSprite.getGlobalBounds().width - 10*gs::scale));
            speechBubbleText.setPosition(speechBubbleSprite.getPosition().x - speechBubbleText.getGlobalBounds().width/2, speechBubbleSprite.getPosition().y + speechBubbleSprite.getGlobalBounds().height/2 - speechBubbleText.getGlobalBounds().height/2);
        }
        
        
        choiceButtons.Resize(width, height);
        talkChooseCircle.Resize(width, height);
        talkChooseCircle.setPosition(talkButton.text.getGlobalBounds().left + talkButton.text.getGlobalBounds().width/2, talkButton.text.getGlobalBounds().top + talkButton.text.getGlobalBounds().height/2);
        
        talkNekoButton.Resize(width, height);
        talkSelfButton.Resize(width, height);
        talkInterestButton.Resize(width, height);
        talkSkillButton.Resize(width, height);
        float buttonHeight = talkNekoButton.text.getGlobalBounds().height + 14*gs::scale,
              xxCorrecting = talkChooseCircle.sprite.getGlobalBounds().left,
              yyCorrecting = talkChooseCircle.sprite.getPosition().y - 15*gs::scale;
        float widestButtonWidth = std::max(std::max(talkNekoButton.text.getGlobalBounds().width + 20*gs::scale, talkSelfButton.text.getGlobalBounds().width + 50*gs::scale), std::max(talkInterestButton.text.getGlobalBounds().width + 50*gs::scale, talkSkillButton.text.getGlobalBounds().width + 20*gs::scale));
        if (xxCorrecting < widestButtonWidth) { talkChooseCircle.setPosition(talkChooseCircle.sprite.getPosition().x + (widestButtonWidth - xxCorrecting), talkChooseCircle.sprite.getPosition().y); xxCorrecting = talkChooseCircle.sprite.getGlobalBounds().left; }
        talkNekoButton.setPosition(xxCorrecting - 20*gs::scale, yyCorrecting - 3*buttonHeight/2);
        talkSelfButton.setPosition(xxCorrecting - 50*gs::scale, yyCorrecting - buttonHeight/2);
        talkInterestButton.setPosition(xxCorrecting - 50*gs::scale, yyCorrecting + buttonHeight/2);
        talkSkillButton.setPosition(xxCorrecting - 20*gs::scale, yyCorrecting + 3*buttonHeight/2 + 6*gs::scale);
        /*talkChooseCircle.Resize(width, height);
        talkChooseCircle.setPosition(gs::width/2, gs::height/2);
        
        talkNekoButton.Resize(width, height);
        talkSelfButton.Resize(width, height);
        talkInterestButton.Resize(width, height);
        talkSkillButton.Resize(width, height);
        talkNekoButton.setPosition(gs::width/2 + 100*gs::scale, gs::height/2);
        talkSelfButton.setPosition(gs::width/2 - 100*gs::scale, gs::height/2);
        talkInterestButton.setPosition(gs::width/2, gs::height/2 - 90*gs::scale);
        talkSkillButton.setPosition(gs::width/2, gs::height/2 + 90*gs::scale);*/
    }
    void NekoInterfaceUI::Draw(sf::RenderWindow* window)
    {
        if (!active || gs::ignoreDraw) return;
        
        window->draw(blackScreenShape);
        Player::neko.Draw(window, true);
        
        switch (screen)
        {
            case Screen::main:
                window->draw(heartSprite);
                window->draw(nameText);
                
                if (drawSpeechBubble) {
                    window->draw(speechBubbleSprite);
                    window->draw(speechBubbleText); }
                
                talkButton.Draw(window);
                requestButton.Draw(window);
                engageButton.Draw(window);
                actionButton.Draw(window);
                break;
            case Screen::talk:
                window->draw(heartSprite);
                window->draw(nameText);
                
                if (drawSpeechBubble) {
                    window->draw(speechBubbleSprite);
                    window->draw(speechBubbleText); }
                
                window->draw(choiceBackShape);
                talkChooseCircle.Draw(window);
                talkNekoButton.Draw(window);
                talkSelfButton.Draw(window);
                talkInterestButton.Draw(window);
                talkSkillButton.Draw(window);
                break;
            default: break;
        }
    }
    void NekoInterfaceUI::RecieveMessage(MessageHolder& message)
    {
        if (!active && message.info == "NekoUI :: Show") Switch(true);
        else if (active && message.info == "NekoUI :: Close") Switch(false);
        else if (message.info == "NekoUI :: SelectNeko") neko = reinterpret_cast<NekoEntity*>(message.address);
        else if (message.info == "NekoUI :: Switch") Switch(!active);
        else if (message.info == "NekoUI :: DDialogue")
        {
            if (nekoIsSleeping) return; speechBubbleLine = message.additional;
            drawSpeechBubble = true; elapsedSpeechBubble = 1.f + speechBubbleLine.length() * 0.09f;
            speechBubbleText.setString(nss::GetStringWithLineBreaks(speechBubbleText, speechBubbleLine, speechBubbleSprite.getGlobalBounds().width - 10*gs::scale));
            speechBubbleText.setPosition(speechBubbleSprite.getPosition().x - speechBubbleText.getGlobalBounds().width/2, speechBubbleSprite.getPosition().y + speechBubbleSprite.getGlobalBounds().height/2 - speechBubbleText.getGlobalBounds().height/2);
        }
        else if (message.info == "NekoUI :: Sleeping")
        {
            nekoIsSleeping = true; speechBubbleLine = L"*Спит*";
            drawSpeechBubble = true; elapsedSpeechBubble = 1.f + speechBubbleLine.length() * 0.09f;
            speechBubbleText.setString(nss::GetStringWithLineBreaks(speechBubbleText, speechBubbleLine, speechBubbleSprite.getGlobalBounds().width - 10*gs::scale));
            speechBubbleText.setPosition(speechBubbleSprite.getPosition().x - speechBubbleText.getGlobalBounds().width/2, speechBubbleSprite.getPosition().y + speechBubbleSprite.getGlobalBounds().height/2 - speechBubbleText.getGlobalBounds().height/2);
        }
        else if (active && message.info == "NovelComponents :: Novel :: Destroying") nintDontDrawPersonNeko = false;
    }
    void NekoInterfaceUI::Switch(const bool& on)
    {
        if (rm::canOpenNekoUI) { if (on) rm::drawDatePanel = false; else rm::drawDatePanel = (gs::activeInterfaces.size() == 1); }
        if (on && !active && rm::canOpenNekoUI) { screen = Screen::main;
            gs::PushInterface(this); active = true; mode = appearing; entity->SortAbove(this); OpenNekoInterface(neko); }
        else if (active) { gs::RemoveInterface(this); mode = disappearing; if (neko) neko->beingActionedWith = false; }
    }
    void NekoInterfaceUI::OpenNekoInterface(NekoEntity* entity)
    {
        nekoIsSleeping = entity->sleeping;
        entity->beingActionedWith = true; entity->drawDialogue = false;
        nintDontDrawPersonNeko = false; rm::requestCloseButton = true;
        nameText.setString(Player::display);
        nameText.setOrigin(nameText.getGlobalBounds().width/2, 0);
        if (speechBubbleLine == L"..." && !nekoIsSleeping)
        {
            drawSpeechBubble = true;
            int random = rand() % 3;
            switch (random)
            {
                case 0: speechBubbleLine = L"Мм, тиво тякое? :з"; break;
                case 1: speechBubbleLine = L"Хозяииин! Ето ти! С:"; break;
                case 2: speechBubbleLine = L"Няя, йа скучаля! >3<"; break;
                default: speechBubbleLine = L"..."; break;
            }
        } else if (nekoIsSleeping) speechBubbleLine = L"*Спит*";
        elapsedSpeechBubble = 1.f + speechBubbleLine.length() * 0.09f;
        
        // nintMoodText = L"Счастье с:"; nintMoodColor = sf::Color::Green;
        Resize(gs::width, gs::height);
    }
    void NekoInterfaceUI::UpdateAlpha()
    {
        nameText.setFillColor(sf::Color(nameText.getFillColor().r, nameText.getFillColor().g, nameText.getFillColor().b, alpha));
        nameText.setOutlineColor(sf::Color(nameText.getOutlineColor().r, nameText.getOutlineColor().g, nameText.getOutlineColor().b, alpha));
        
        blackScreenShape.setFillColor(sf::Color(blackScreenShape.getFillColor().r, blackScreenShape.getFillColor().g, blackScreenShape.getFillColor().b, 170.f * ((float)alpha/255)));
        blackScreenShape.setOutlineColor(sf::Color(blackScreenShape.getOutlineColor().r, blackScreenShape.getOutlineColor().g, blackScreenShape.getOutlineColor().b, 170.f * ((float)alpha/255.f)));
        choiceBackShape.setFillColor(sf::Color(choiceBackShape.getFillColor().r, choiceBackShape.getFillColor().g, choiceBackShape.getFillColor().b, 100.f * ((float)alpha/255)));
        
        talkButton.setAlpha(alpha);
        requestButton.setAlpha(alpha);
        engageButton.setAlpha(alpha);
        actionButton.setAlpha(alpha);
        heartSprite.setColor(sf::Color(heartSprite.getColor().r, heartSprite.getColor().g, heartSprite.getColor().b, alpha));
        
        choiceButtons.setAlpha(alpha);
        talkChooseCircle.setAlpha(alpha);
        talkSkillButton.setAlpha(alpha);
        talkInterestButton.setAlpha(alpha);
        talkSelfButton.setAlpha(alpha);
        talkNekoButton.setAlpha(alpha);
        
        speechBubbleText.setFillColor(sf::Color(speechBubbleText.getFillColor().r, speechBubbleText.getFillColor().g, speechBubbleText.getFillColor().b, alpha));
        speechBubbleText.setOutlineColor(sf::Color(speechBubbleText.getOutlineColor().r, speechBubbleText.getOutlineColor().g, speechBubbleText.getOutlineColor().b, alpha));
        speechBubbleSprite.setColor(sf::Color(speechBubbleSprite.getColor().r, speechBubbleSprite.getColor().g, speechBubbleSprite.getColor().b, alpha));
        
        Player::neko.setPersonAlpha(alpha);
    }
}
