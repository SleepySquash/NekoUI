//
//  NekoInterfaceUI.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 17/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "NekoInterfaceUI.hpp"

namespace NekoUI
{
    void NekoInterfaceUI::Init()
    {
        nintText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        nintText.setOutlineColor(sf::Color::Black);
        nintNameText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        nintNameText.setOutlineColor(sf::Color::Black);
        nintShape.setFillColor(sf::Color(0,0,0,80));
        nintCloseButton.setString(L"Закрыть");
        nintCloseButton.setFont(L"Pacifica.ttf");
        nintCloseButton.setCharacterSize(70);
        nintCloseButton.valign = Valign::Bottom;
        
        nintSelectMainButton.setString(L"В отряд");
        nintSelectMainButton.setFont(L"Pacifica.ttf");
        nintSelectMainButton.setCharacterSize(70);
        
        blackScreenShape.setFillColor(sf::Color(0,0,0,170));
        quitButton.setTexture(L"Data/Images/quit_button.png");
        quitButton.setScale(2);
        
        
        sf::Texture* texture = ic::LoadTexture(L"Data/Images/heart.png");
        if (texture)
        {
            nintHeart.setTexture(*texture);
            nintHeart.setTextureRect(sf::IntRect(268, 0, 268, 256));
            nintHeart.setOrigin(texture->getSize().x, 0);
        }
        
        nintTalkButton.setFont(L"Pacifica.ttf");
        nintTalkButton.setCharacterSize(69);
        nintTalkButton.setString(L"Поговорить");
        nintWardrobeButton.setFont(L"Pacifica.ttf");
        nintWardrobeButton.setCharacterSize(54);
        nintWardrobeButton.setString(L"Гардероб");
        nintWardrobeButton.halign = Halign::Left;
        nintGiftButton.setFont(L"Pacifica.ttf");
        nintGiftButton.setCharacterSize(54);
        nintGiftButton.setString(L"Подарок");
        nintGiftButton.halign = Halign::Right;
        nintActionButton.setFont(L"Pacifica.ttf");
        nintActionButton.setString(L"Действие");
        nintActionButton.setCharacterSize(54);
        nintActionButton.valign = nintTalkButton.valign = nintGiftButton.valign = nintWardrobeButton.valign = Valign::Bottom;
    }
    void NekoInterfaceUI::Destroy() { ic::DeleteImage(L"Data/Images/heart.png"); if (active) CleanUp(); }
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
    }
    void NekoInterfaceUI::CleanUp() { /*ic::DeleteImage(L"Data/Neko/Person/Vanilla.png");*/ }
    void NekoInterfaceUI::PollEvent(sf::Event& event)
    {
        if (!active || !gs::isActiveInterface(this)) return;
        
        if (quitButton.PollEvent(event)) entity->SendMessage({"NekoUI :: Close"});
        else if (nintTalkButton.PollEvent(event))
        {
            nintDontDrawPersonNeko = true;
            //nintNeko->info->NovelTalkTo(entity);
        }
        else if (nintActionButton.PollEvent(event)) { }
        else if (nintGiftButton.PollEvent(event)) entity->SendMessage({"Inventory :: Show"});
        else if (nintWardrobeButton.PollEvent(event)) { }
    }
    void NekoInterfaceUI::Resize(unsigned int width, unsigned int height)
    {
        if (!active) return;
        
        blackScreenShape.setSize({(float)gs::width, (float)gs::height});
        quitButton.Resize(width, height); quitButton.setPosition(gs::width - gs::width/12, gs::height/8);
        
        nintCloseButton.Resize(width, height);
        if (gs::verticalOrientation)
        {
            nintBodyNeko.setScale(1.4 * nintBodyNekoScale * gs::scScale, 1.4 * nintBodyNekoScale * gs::scScale);
            Player::neko.personScale = 1.4 * gs::scScale; Player::neko.ResizePerson();
            nintShape.setSize({gs::width*4.55f/5.f, gs::height*4.3f/5.f});
            nintShape.setPosition((gs::width - nintShape.getSize().x)/2, (gs::height - nintShape.getSize().y)/2);
            
            if (nintBodyNeko.getGlobalBounds().height + 20*gs::scale > gs::height)
                nintBodyNeko.setPosition(width/2,nintBodyNeko.getGlobalBounds().height + 20*gs::scale);
            else nintBodyNeko.setPosition(width/2, height);
            
            if (Player::neko.body.person.getGlobalBounds().height + 20*gs::scale > gs::height)
                Player::neko.setPersonPosition(width/2, Player::neko.body.person.getGlobalBounds().height + 20*gs::scale);
            else Player::neko.setPersonPosition(width/2, height);
        }
        else
        {
            nintBodyNeko.setScale(1.3 * nintBodyNekoScale * gs::scScale, 1.3 * nintBodyNekoScale * gs::scScale);
            Player::neko.personScale = 1.3 * gs::scScale; Player::neko.ResizePerson();
            nintShape.setSize({3*gs::width/5.f, gs::height*4.3f/5.f});
            nintShape.setPosition(gs::width - 3*gs::width/5.f, (gs::height - nintShape.getSize().y)/2);
            
            if (nintBodyNeko.getGlobalBounds().height + 20*gs::scale > gs::height)
                nintBodyNeko.setPosition(width/5, nintBodyNeko.getGlobalBounds().height + 20*gs::scale);
            else nintBodyNeko.setPosition(width/5, height);
            
            if (Player::neko.body.person.getGlobalBounds().height + 20*gs::scale > gs::height)
                Player::neko.setPersonPosition(width/5, Player::neko.body.person.getGlobalBounds().height + 20*gs::scale);
            else Player::neko.setPersonPosition(width/5, height);
        }
        
        nintCloseButton.setPosition(nintShape.getPosition().x + nintShape.getSize().x/2, nintShape.getGlobalBounds().top + nintShape.getGlobalBounds().height - 20*gs::scaley);
        nintNameText.setCharacterSize(80 * gs::scale);
        nintNameText.setOrigin(nintNameText.getGlobalBounds().width/2, 0);
        nintNameText.setOutlineThickness(gs::scale);
        nintText.setOutlineThickness(gs::scale);
        
        nintHeart.setScale(0.25*gs::scale, 0.25*gs::scale);
        nintNameText.setScale(1, 1);
        if (nintNameText.getLocalBounds().width + nintHeart.getGlobalBounds().width + 45*gs::scale > nintShape.getSize().x)
            nintNameText.setScale(nintShape.getSize().x/(nintNameText.getLocalBounds().width + nintHeart.getGlobalBounds().width + 45*gs::scale), nintNameText.getScale().y);
        nintNameText.setPosition(nintShape.getPosition().x + nintShape.getSize().x/2, nintShape.getGlobalBounds().top + gs::scaley);
        
        nintHeart.setPosition(nintNameText.getGlobalBounds().left + 45*gs::scale, nintShape.getPosition().y + 25*gs::scale);
        if (nintNameText.getPosition().x < nintShape.getPosition().x) //TODO: add +texture.size().x
            nintHeart.setPosition(nintShape.getPosition().x + 10*gs::scale, nintShape.getPosition().y + 25*gs::scale);
        nintYYInfoStart = nintNameText.getGlobalBounds().top + nintNameText.getGlobalBounds().height;
        
        nintSelectMainButton.Resize(width, height);
        nintSelectMainButton.setPosition(nintShape.getPosition().x + nintShape.getSize().x/2, nintShape.getGlobalBounds().top + nintShape.getGlobalBounds().height/2);
        
        nintTalkButton.Resize(width, height);
        nintWardrobeButton.Resize(width, height);
        nintGiftButton.Resize(width, height);
        nintActionButton.Resize(width, height);
        
        if (nintShape.getSize().x - (10*gs::scale + nintWardrobeButton.text.getGlobalBounds().width + nintGiftButton.text.getGlobalBounds().width) < nintWardrobeButton.text.getGlobalBounds().width)
        {
            nintTalkButton.setPosition(nintShape.getPosition().x + nintShape.getSize().x/2, nintShape.getPosition().y + nintShape.getSize().y - nintWardrobeButton.text.getGlobalBounds().height*2*1.5);
            nintActionButton.setPosition(nintShape.getPosition().x + nintShape.getSize().x/2, nintShape.getPosition().y + nintShape.getSize().y - nintWardrobeButton.text.getGlobalBounds().height*1.4);
        }
        else
        {
            nintTalkButton.setPosition(nintShape.getPosition().x + nintShape.getSize().x/2, nintShape.getPosition().y + nintShape.getSize().y - nintWardrobeButton.text.getGlobalBounds().height*1.7);
            nintActionButton.setPosition(nintShape.getPosition().x + nintShape.getSize().x/2, nintShape.getPosition().y + nintShape.getSize().y - 10*gs::scale);
        }
        nintWardrobeButton.setPosition(nintShape.getPosition().x, nintShape.getPosition().y + nintShape.getSize().y - 10*gs::scale);
        nintGiftButton.setPosition(nintShape.getPosition().x + nintShape.getSize().x, nintShape.getPosition().y + nintShape.getSize().y - 10*gs::scale);
        
        if (neko)
        {
            nintText.setCharacterSize(44 * gs::scale);
            nintDescriptionText = nss::GetStringWithLineBreaks(nintText, L"Пообщайтесь с кошкодевочкой, чтобы узнать её лучше~"/*nintNeko->info->characterDescription*/, nintShape.getSize().x - 10*gs::scalex, 10*gs::scale);
        }
    }
    void NekoInterfaceUI::Draw(sf::RenderWindow* window)
    {
        if (!active || gs::ignoreDraw) return;
        
        window->draw(blackScreenShape);
        Player::neko.Draw(window, true);
        window->draw(nintShape);
        quitButton.Draw(window);
        
        window->draw(nintHeart);
        window->draw(nintNameText);
        
        float yy = nintYYInfoStart;
        nintText.setCharacterSize(30 * gs::scale);
        nintText.setString(L"(Здесь будет мой статус)");
        nintText.setPosition(nintShape.getPosition().x + nintShape.getSize().x/2 - nintText.getGlobalBounds().width/2, yy);
        window->draw(nintText); yy += nintText.getGlobalBounds().height + 10*gs::scale;
        
        nintText.setCharacterSize(50 * gs::scale);
        nintText.setString(nintMoodText);
        nintText.setPosition(nintShape.getPosition().x, yy);
        nintText.setFillColor(nintMoodColor); window->draw(nintText);
        nintText.setString(nintRarityText);
        nintText.setPosition(nintShape.getPosition().x + nintShape.getSize().x - nintText.getGlobalBounds().width, yy);
        nintText.setFillColor(nintRarityColor); window->draw(nintText);
        nintText.setFillColor(sf::Color::White);
        yy += nintText.getGlobalBounds().height + 25*gs::scale;
        
        nintText.setCharacterSize(44 * gs::scale);
        nintText.setString(nintDescriptionText);
        nintText.setPosition(nintShape.getPosition().x + nintShape.getSize().x/2 - nintText.getGlobalBounds().width/2, yy);
        window->draw(nintText); yy += nintText.getGlobalBounds().height + 30*gs::scale;
        
        
        nintTalkButton.Draw(window);
        nintWardrobeButton.Draw(window);
        nintGiftButton.Draw(window);
        nintActionButton.Draw(window);
    }
    void NekoInterfaceUI::RecieveMessage(MessageHolder& message)
    {
        if (!active && message.info == "NekoUI :: Show") Switch(true);
        else if (active && message.info == "NekoUI :: Close") Switch(false);
        else if (message.info == "NekoUI :: SelectNeko") neko = reinterpret_cast<NekoEntity*>(message.address);
        else if (message.info == "NekoUI :: Switch") Switch(!active);
        else if (active && message.info == "NovelComponents :: Novel :: Destroying") nintDontDrawPersonNeko = false;
    }
    void NekoInterfaceUI::Switch(const bool& on)
    {
        if (on) rm::drawDatePanel = false; else rm::drawDatePanel = (gs::activeInterfaces.size() == 1);
        if (on && !active) { gs::PushInterface(this); active = true; mode = appearing; entity->SortAbove(this); OpenNekoInterface(neko); }
        else if (active) { gs::RemoveInterface(this); active = true; mode = disappearing; if (neko) neko->beingActionedWith = false; }
    }
    void NekoInterfaceUI::OpenNekoInterface(NekoEntity* entity)
    {
        entity->beingActionedWith = true; entity->drawDialogue = false;
        nintDontDrawPersonNeko = false;
        nintNameText.setString(Player::display);
        nintNameText.setOrigin(nintNameText.getGlobalBounds().width/2, 0);
        
        nintMoodText = L"Счастье с:"; nintMoodColor = sf::Color::Green;
        Resize(gs::width, gs::height);
    }
    void NekoInterfaceUI::UpdateAlpha()
    {
        nintText.setFillColor(sf::Color(nintText.getFillColor().r, nintText.getFillColor().g, nintText.getFillColor().b, alpha));
        nintText.setOutlineColor(sf::Color(nintText.getOutlineColor().r, nintText.getOutlineColor().g, nintText.getOutlineColor().b, alpha));
        nintNameText.setFillColor(sf::Color(nintNameText.getFillColor().r, nintNameText.getFillColor().g, nintNameText.getFillColor().b, alpha));
        nintNameText.setOutlineColor(sf::Color(nintNameText.getOutlineColor().r, nintNameText.getOutlineColor().g, nintNameText.getOutlineColor().b, alpha));
        
        nintShape.setFillColor(sf::Color(nintShape.getFillColor().r, nintShape.getFillColor().g, nintShape.getFillColor().b, 80.f * ((float)alpha/255.f)));
        nintShape.setOutlineColor(sf::Color(nintShape.getOutlineColor().r, nintShape.getOutlineColor().g, nintShape.getOutlineColor().b, 80.f * ((float)alpha/255.f)));
        
        blackScreenShape.setFillColor(sf::Color(blackScreenShape.getFillColor().r, blackScreenShape.getFillColor().g, blackScreenShape.getFillColor().b, 170.f * ((float)alpha/255)));
        blackScreenShape.setOutlineColor(sf::Color(blackScreenShape.getOutlineColor().r, blackScreenShape.getOutlineColor().g, blackScreenShape.getOutlineColor().b, 170.f * ((float)alpha/255.f)));
        
        nintCloseButton.setAlpha(alpha);
        nintSelectMainButton.setAlpha(alpha);
        quitButton.setAlpha(alpha);
        nintTalkButton.setAlpha(alpha);
        nintWardrobeButton.setAlpha(alpha);
        nintGiftButton.setAlpha(alpha);
        nintActionButton.setAlpha(alpha);
        nintHeart.setColor(sf::Color(nintHeart.getColor().r, nintHeart.getColor().g, nintHeart.getColor().b, alpha));
        
        Player::neko.setPersonAlpha(alpha);
    }
}
