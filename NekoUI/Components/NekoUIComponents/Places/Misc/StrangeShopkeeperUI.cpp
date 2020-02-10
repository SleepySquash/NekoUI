//
//  StrangeShopkeeperUI.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 25.01.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#include "StrangeShopkeeperUI.hpp"

namespace NekoUI
{
    namespace Places
    {
        void StrangeShopkeeperUI::Init()
        {
            rm::scrolldownMenuOpened = rm::requestCloseButton = false;
            rm::drawDatePanel = true; rm::drawNeeds = rm::drawScrolldownMenu = false;
            rm::canPressDatePanel = rm::allowDTSaving = false;
            rm::location = rm::Location::Shopkeeper;
            
            // background.setFillColor(sf::Color(100, 100, 100));
            sf::Texture* texture = ic::RequestHigherTexture(L"Data/Images/Backgrounds/strangeshopkeeper.jpg", entity->system);
            if ((spriteLoaded = texture))
            {
                background.setTexture(*texture);
                background.setOrigin(texture->getSize().x/2, texture->getSize().y/2);
            }
            
            shopButton.setFont(L"Noteworthy-Bold.ttf");
            shopButton.setCharacterSize(62);
            shopButton.setString(L"Купить");
            talkButton.halign = Halign::Right;
            talkButton.setFont(L"Noteworthy-Bold.ttf");
            talkButton.setCharacterSize(62/*80*/);
            talkButton.setString(L"Поговорить");
            goButton.setFont(L"Noteworthy-Bold.ttf");
            goButton.setCharacterSize(62);
            goButton.setString(L"Уйти");
            goButton.halign = shopButton.halign = Halign::Left;
            talkButton.onormalColor = goButton.onormalColor = shopButton.onormalColor = sf::Color(99, 70, 70);
            talkButton.thickness = goButton.thickness = shopButton.thickness = 4.f;
            talkButton.updateColor(); goButton.updateColor(); shopButton.updateColor();
            
            UpdateAlpha();
        }
        void StrangeShopkeeperUI::Update(const sf::Time& elapsedTime)
        {
            
        }
        void StrangeShopkeeperUI::Destroy() { ic::DeleteImage(L"Data/Images/Backgrounds/strangeshopkeeper.jpg"); }
        void StrangeShopkeeperUI::PollEvent(sf::Event& event)
        {
            if (!active) return;
            
            if (goButton.PollEvent(event))
            {
                if (Inventory::transition.list.empty()) entity->system->SendMessage({"MapUI :: Show"});
                else { entity->SendMessage({"PlacesUI :: ApartmentUI"}); active = false; }
            }
            else if (shopButton.PollEvent(event)) { }
            else if (talkButton.PollEvent(event)) { }
        }
        void StrangeShopkeeperUI::Resize(const unsigned int& width, const unsigned int& height)
        {
            // background.setSize({(float)width, (float)height});
            if (spriteLoaded)
            {
                float scaleFactorX, scaleFactorY, scaleFactor;
                scaleFactorX = (float)width / (background.getTexture()->getSize().x);
                scaleFactorY = (float)height / (background.getTexture()->getSize().y);
                
                scaleFactor = (scaleFactorX > scaleFactorY) ? scaleFactorX : scaleFactorY;
                background.setScale(scaleFactor, scaleFactor);
                background.setPosition(width/2, height/2);
            }
            
            talkButton.Resize(width, height);
            goButton.Resize(width, height);
            shopButton.Resize(width, height);
            float yyAround{ gs::height/2.f }; if (gs::trueVerticalOrientation) yyAround = 2.2f*gs::height/3.f;
            talkButton.setPosition(19.f*gs::width/20, yyAround);
            shopButton.setPosition(talkButton.text.getGlobalBounds().left + 35*gs::scale, talkButton.text.getGlobalBounds().top - 50*gs::scale/* + 5*gs::scale*/);
            goButton.setPosition(talkButton.text.getGlobalBounds().left + 35*gs::scale, talkButton.text.getGlobalBounds().top + talkButton.text.getGlobalBounds().height + 30*gs::scale /* + 8*gs::scale*/);
        }
        void StrangeShopkeeperUI::Draw(sf::RenderWindow* window)
        {
            window->draw(background);
            
            shopButton.draw(window);
            talkButton.draw(window);
            goButton.draw(window);
        }
        void StrangeShopkeeperUI::ReceiveMessage(MessageHolder& message)
        {
            if (active && nss::Command(message.info, "Request") && message.additional == L"Data/Images/Backgrounds/strangeshopkeeper.jpg")
            {
                sf::Texture* texture = ic::LoadTexture(message.additional);
                if ((spriteLoaded = texture))
                {
                    background.setTexture(*texture, true);
                    background.setOrigin(texture->getSize().x/2, texture->getSize().y/2);
                    Resize(gs::width, gs::height);
                }
            }
            else if (message.info == "PlacesUI :: ApartmentUI Destroy") entity->PopComponent(this);
        }
        void StrangeShopkeeperUI::UpdateAlpha()
        {
            // background.setFillColor({background.getFillColor().r, background.getFillColor().g, background.getFillColor().b, alpha});
            background.setColor({background.getColor().r, background.getColor().g, background.getColor().b, alpha});
            shopButton.setAlpha(alpha);
            talkButton.setAlpha(alpha);
            shopButton.setAlpha(alpha);
        }
    }
}
