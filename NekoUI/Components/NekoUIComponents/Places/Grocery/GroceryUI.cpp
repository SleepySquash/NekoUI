//
//  GroceryUI.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 18.01.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#include "GroceryUI.hpp"

namespace NekoUI
{
    namespace Places
    {
        void GroceryUI::Init()
        {
            rm::scrolldownMenuOpened = rm::requestCloseButton = false;
            rm::drawDatePanel = rm::shopMode = true; rm::drawNeeds = rm::drawScrolldownMenu = false;
            rm::canPressDatePanel = false;
            rm::simulationWasAt = rm::simulationWasAtEnum::Grocery;
            
            // background.setFillColor(sf::Color(100, 100, 100));
            sf::Texture* texture = ic::RequestHigherTexture(L"Data/Images/Backgrounds/grocery.jpg", entity->system);
            if ((spriteLoaded = texture))
            {
                background.setTexture(*texture);
                background.setOrigin(texture->getSize().x/2, texture->getSize().y/2);
            }
            
            quitB.halign = Halign::Right; checkoutB.halign = Halign::Left;
            quitB.valign = checkoutB.valign = Valign::Bottom;
            
            quitB.setTexture(L"Data/Images/UI/shopbuttons.png");
            quitB.sprite.setTextureRect({0,0,269,256});
            quitB.setScale(0.55);
            checkoutB.setTexture(L"Data/Images/UI/shopbuttons.png");
            checkoutB.sprite.setTextureRect({323,0,189,256});
            checkoutB.setScale(0.55);
            
            discountsB.setFont(L"Impact.ttf");
            discountsB.setString(L"Акции");
            discountsB.onormalColor = {37, 133, 166};
            foodB.setFont(L"Impact.ttf");
            foodB.setString(L"Продукты");
            foodB.onormalColor = {37, 133, 166};
            drinksB.setFont(L"Impact.ttf");
            drinksB.setString(L"Напитки");
            drinksB.onormalColor = {37, 133, 166};
            householdB.setFont(L"Impact.ttf");
            householdB.setString(L"Хозтовары");
            householdB.onormalColor = {37, 133, 166};
            discountsB.characterSize = foodB.characterSize = drinksB.characterSize = householdB.characterSize = 48;
            discountsB.updateColor(); foodB.updateColor(); drinksB.updateColor(); householdB.updateColor();
            
            UpdateAlpha();
        }
        void GroceryUI::Update(const sf::Time& elapsedTime)
        {
            
        }
        void GroceryUI::Destroy() { ic::DeleteImage(L"Data/Images/Backgrounds/grocery.jpg"); }
        void GroceryUI::PollEvent(sf::Event& event)
        {
            if (!active) return;
            
            if (quitB.PollEvent(event)) { entity->SendMessage({"PlacesUI :: ApartmentUI"}); entity->PopComponent(this); }
            else if (checkoutB.PollEvent(event)) { }
            else if (discountsB.PollEvent(event)) { }
            else if (foodB.PollEvent(event)) { }
            else if (drinksB.PollEvent(event)) { }
            else if (householdB.PollEvent(event)) { }
        }
        void GroceryUI::Resize(unsigned int width, unsigned int height)
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
            
            quitB.Resize(width, height);
            quitB.setPosition(width, height);
            
            checkoutB.Resize(width, height);
            checkoutB.setPosition(0, height);
            
            int zazor = 16;
            
            foodB.setSize({0.85f*gs::width/3.f, gs::height*9.f/10});
            foodB.Resize(width, height);
            foodB.setPosition(gs::width/2 - foodB.shape.getSize().x/2, (gs::height - foodB.shape.getSize().y)/2);
            
            discountsB.setSize({0.75f*gs::width/3.f, gs::height*9.f/20 - zazor/2*gs::scale});
            discountsB.Resize(width, height);
            discountsB.setPosition(foodB.shape.getPosition().x + foodB.shape.getSize().x + zazor*gs::scale, (gs::height - foodB.shape.getSize().y)/2);
            
            drinksB.setSize({0.75f*gs::width/3.f, gs::height*9.f/20 - zazor/2*gs::scale});
            drinksB.Resize(width, height);
            drinksB.setPosition(foodB.shape.getPosition().x + foodB.shape.getSize().x + zazor*gs::scale, gs::height - (gs::height - foodB.shape.getSize().y)/2 - drinksB.shape.getSize().y);
            
            householdB.setSize({0.75f*gs::width/3.f, 0.7f*gs::height*9.f/10});
            householdB.Resize(width, height);
            householdB.setPosition(foodB.shape.getPosition().x - zazor*gs::scale - householdB.shape.getSize().x, gs::height - (gs::height - foodB.shape.getSize().y)/2 - householdB.shape.getSize().y);
        }
        void GroceryUI::Draw(sf::RenderWindow* window)
        {
            if (!active) return;
            
            window->draw(background);
            
            discountsB.Draw(window);
            foodB.Draw(window);
            drinksB.Draw(window);
            householdB.Draw(window);
            
            quitB.Draw(window);
            checkoutB.Draw(window);
        }
        void GroceryUI::RecieveMessage(MessageHolder& message)
        {
            if (active && nss::Command(message.info, "Request") && message.additional == L"Data/Images/Backgrounds/grocery.jpg")
            {
                sf::Texture* texture = ic::LoadTexture(message.additional);
                if ((spriteLoaded = texture))
                {
                    background.setTexture(*texture, true);
                    background.setOrigin(texture->getSize().x/2, texture->getSize().y/2);
                    Resize(gs::width, gs::height);
                }
            }
        }
        void GroceryUI::UpdateAlpha()
        {
            // background.setFillColor({background.getFillColor().r, background.getFillColor().g, background.getFillColor().b, alpha});
            background.setColor({background.getColor().r, background.getColor().g, background.getColor().b, alpha});
            quitB.setAlpha(alpha);
            checkoutB.setAlpha(alpha);
            discountsB.setAlphaEx(alpha, 0.8*alpha);
            foodB.setAlphaEx(alpha, 0.8*alpha);
            drinksB.setAlphaEx(alpha, 0.8*alpha);
            householdB.setAlphaEx(alpha, 0.8*alpha);
        }
    }
}
