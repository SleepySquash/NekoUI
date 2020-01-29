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
            rm::drawDatePanel = true; rm::drawNeeds = rm::drawScrolldownMenu = false;
            rm::canPressDatePanel = rm::allowDTSaving = false;
            rm::simulationWasAt = rm::simulationWasAtEnum::Grocery;
            entity->system->SendMessage({"RoomUI :: Update"});
            
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
            foodB.setFont(L"Impact.ttf");
            foodB.setString(L"Продукты");
            drinksB.setFont(L"Impact.ttf");
            drinksB.setString(L"Напитки");
            householdB.setFont(L"Impact.ttf");
            householdB.setString(L"Хозтовары");
            discountsB.snormalColor = foodB.snormalColor = drinksB.snormalColor = householdB.snormalColor = {37, 133, 166};
            discountsB.sonormalColor = foodB.sonormalColor = drinksB.sonormalColor = householdB.sonormalColor = {37, 133, 166};
            discountsB.characterSize = foodB.characterSize = drinksB.characterSize = householdB.characterSize = 48;
            discountsB.radius = foodB.radius = drinksB.radius = householdB.radius = 20.f;
            discountsB.ralpha = foodB.ralpha = drinksB.ralpha = householdB.ralpha = 0.8f * 255;
            discountsB.updateColor(); foodB.updateColor(); drinksB.updateColor(); householdB.updateColor();
            
            texture = ic::LoadTexture(L"Data/Images/UI/ShopSlotConcept.png");
            if (texture) shelfSprite.setTexture(*texture);
            
            UpdateAlpha();
        }
        void renderGroceryShelvesSprite(sf::RenderTexture* texture, sf::Sprite* sprite, sf::Sprite* render)
        {
            sf::Context context;
            gs::requestWindowRefresh = true;
            
            texture->create(gs::width, gs::height);
            sprite->setScale(gs::width/sprite->getLocalBounds().width, gs::scale);
            int count = floor(gs::height/sprite->getGlobalBounds().height) - 1; if (count < 1) count = 1;
            // gs::scale*count = gs::height/(sprite->getLocalBounds().height)
            sprite->setScale(gs::width/sprite->getLocalBounds().width, gs::height/(sprite->getLocalBounds().height * count));
            float yy = 0, yyuntil = gs::height;
            while (yy < yyuntil)
            {
                sprite->setPosition(0, yy);
                texture->draw(*sprite);
                yy += sprite->getGlobalBounds().height;
            }
            texture->display();
            render->setTexture(texture->getTexture(), true);
        }
        void GroceryUI::Destroy() { ic::DeleteImage(L"Data/Images/Backgrounds/grocery.jpg"); ic::DeleteImage(L"Data/Images/UI/ShopSlotConcept.png"); }
        void GroceryUI::PollEvent(sf::Event& event)
        {
            if (!active) return;
            
            switch (mode)
            {
                case Mode::Food: case Mode::Drinks:
                    if (quitB.PollEvent(event))
                    {
                        mode = Mode::Main;
                        for (auto i : shelfFood) if (i) ic::DeleteImage(L"Data/Items/" + utf16(i->name) + L".png");
                        shelfFood.clear(); shelfFoodTexture.clear();
                    }
                    break;
                default:
                    if (quitB.PollEvent(event)) { entity->SendMessage({"PlacesUI :: ApartmentUI"}); active = false; }
                    else if (checkoutB.PollEvent(event)) { }
                    else if (discountsB.PollEvent(event)) { }
                    else if (foodB.PollEvent(event))
                    {
                        mode = Mode::Food;
                        for (auto i : shelfFood) if (i) ic::DeleteImage(L"Data/Items/" + utf16(i->name) + L".png");
                        shelfFood.clear(); shelfFoodTexture.clear();
                        shelfFood.push_back(nullptr); shelfFoodTexture.push_back(nullptr);
                        for (auto i : Inventory::map)
                            if (i.second->type == ItemType::Food)
                            {
                                shelfFood.push_back(i.second);
                                shelfFoodTexture.push_back(ic::LoadTexture(L"Data/Items/" + utf16(i.second->name) + L".png"));
                            }
                    }
                    else if (drinksB.PollEvent(event))
                    {
                        mode = Mode::Drinks;
                        for (auto i : shelfFood) if (i) ic::DeleteImage(L"Data/Items/" + utf16(i->name) + L".png");
                        shelfFood.clear(); shelfFoodTexture.clear();
                        shelfFood.push_back(nullptr); shelfFoodTexture.push_back(nullptr);
                        for (auto i : Inventory::map)
                            if (i.second->type == ItemType::Drink)
                            {
                                shelfFood.push_back(i.second);
                                shelfFoodTexture.push_back(ic::LoadTexture(L"Data/Items/" + utf16(i.second->name) + L".png"));
                            }
                    }
                    else if (householdB.PollEvent(event)) { }
                    break;
            }
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
            
            std::thread(renderGroceryShelvesSprite, &shelvesTexture, &shelfSprite, &shelvesSprite).join();
            shelfCount = floor(gs::height/shelfSprite.getGlobalBounds().height + 0.1f);
            shelfItemWidth = 200*gs::scale;
            shelfColumnR = floor(gs::width/shelfItemWidth) + 1;
            pricetagShape.setSize({55*gs::scale, 23*shelfSprite.getScale().y});
            pricetagShape.setOrigin(pricetagShape.getSize().x/2, 0);
        }
        void GroceryUI::Draw(sf::RenderWindow* window)
        {
            switch (mode)
            {
                case Mode::Food: case Mode::Drinks:
                    window->draw(shelvesSprite);
                    
                    xx = 0; yy = 0; done = false;
                    for (int i = shelfColumnL; i < shelfColumnR && !done; ++i)
                    {
                        for (int j = 0; j < shelfCount && !done; ++j)
                        {
                            xy = i*shelfCount + j;
                            if (shelfFoodTexture[xy])
                            {
                                pricetagShape.setPosition(xx + shelfItemWidth/2, yy + (128 - 23)*shelfSprite.getScale().y);
                                window->draw(pricetagShape);
                                
                                shelfItem.setTexture(*shelfFoodTexture[xy], true);
                                shelfItem.setScale(0.85*gs::scale*shelfFood[xy]->scale, 0.85*gs::scale*shelfFood[xy]->scale);
                                if (shelfItem.getGlobalBounds().width > shelfItemWidth)
                                    shelfItem.setScale(shelfItemWidth/shelfItem.getLocalBounds().width, shelfItemWidth/shelfItem.getLocalBounds().width);
                                if (shelfItem.getGlobalBounds().height > (128 - 23)*shelfSprite.getScale().y)
                                    shelfItem.setScale((128 - 23)*shelfSprite.getScale().y/shelfItem.getLocalBounds().height, (128 - 23)*shelfSprite.getScale().y/shelfItem.getLocalBounds().height);
                                shelfItem.setPosition(xx + shelfItemWidth/2 - shelfItem.getGlobalBounds().width/2, pricetagShape.getPosition().y - shelfItem.getGlobalBounds().height);
                                window->draw(shelfItem);
                            }
                            
                            yy += shelfSprite.getGlobalBounds().height;
                            done = (xy >= shelfFood.size() - 1);
                        }
                        yy = 0; xx += shelfItemWidth;
                    }
                    
                    quitB.Draw(window);
                    break;
                default:
                    window->draw(background);
                    
                    discountsB.Draw(window);
                    foodB.Draw(window);
                    drinksB.Draw(window);
                    householdB.Draw(window);
                    
                    quitB.Draw(window);
                    checkoutB.Draw(window);
                    break;
            }
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
            else if (message.info == "PlacesUI :: ApartmentUI Destroy") entity->PopComponent(this);
        }
        void GroceryUI::UpdateAlpha()
        {
            // background.setFillColor({background.getFillColor().r, background.getFillColor().g, background.getFillColor().b, alpha});
            background.setColor({background.getColor().r, background.getColor().g, background.getColor().b, alpha});
            quitB.setAlpha(alpha);
            checkoutB.setAlpha(alpha);
            discountsB.setAlpha(alpha);
            foodB.setAlpha(alpha);
            drinksB.setAlpha(alpha);
            householdB.setAlpha(alpha);
        }
    }
}
