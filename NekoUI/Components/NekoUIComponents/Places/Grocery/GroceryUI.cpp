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
        ShelfItemStruct::ShelfItemStruct(Item* item, int qty) : item(item), count(qty) { if (item) { texture = ic::LoadTexture(utf16("Data/Items/" + item->name + ".png")); } }
        ShelfItemStruct::~ShelfItemStruct() { if (texture && item) { ic::DeleteImage(utf16("Data/Items/" + item->name + ".png")); } }
        ShelfItemStruct::ShelfItemStruct(ShelfItemStruct&& b) : item(b.item), texture(b.texture) { b.item = nullptr; b.texture = nullptr; }
        void GroceryUI::Init()
        {
            rm::scrolldownMenuOpened = rm::requestCloseButton = false;
            rm::drawDatePanel = true; rm::drawNeeds = rm::drawScrolldownMenu = false;
            rm::canPressDatePanel = rm::allowDTSaving = false;
            rm::location = rm::Location::Grocery;
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
            pricetagText.setFont(*fc::GetFont(L"Pacifica.ttf"));
            pricetagText.setOutlineColor(sf::Color::Black);
            
            
            checkShape.setFillColor({101, 94, 152});
            checkShape.setOutlineColor(sf::Color::White);
            checkShape.setCornerPointCount(14);
            checkCaption.setFont(*fc::GetFont(L"Impact.ttf"));
            checkCaption.setString(L"Чек");
            checkBalance.setFont(*fc::GetFont(L"Impact.ttf"));
            checkBalance.setString(L"Баланс: " + std::to_wstring(NekoS::money) + L" P");
            checkName.setFont(*fc::GetFont(L"Impact.ttf"));
            checkQty.setFont(*fc::GetFont(L"Impact.ttf"));
            checkPriceT.setFont(*fc::GetFont(L"Impact.ttf"));
            payB.setFont(L"Impact.ttf");
            payB.setString(L"       Pay");
            payB.setCharacterSize(40);
            resetB.setFont(L"Impact.ttf");
            resetB.setString(L"Очистить");
            resetB.setCharacterSize(25);
            payB.radius = resetB.radius = 9.f;
            payB.snormalColor = resetB.snormalColor = sf::Color::Black;
            payB.drawOutline = resetB.drawOutline = payB.drawOutlineShape = resetB.drawOutlineShape = false;
            payB.ralpha = resetB.ralpha = 100.f;
            payB.valign = resetB.valign = Valign::Center;
            payB.updateColor(); resetB.updateColor();
            
            texture = ic::LoadTexture(L"Data/Images/UI/nekopay.png");
            if (texture) { nekopaySprite.setTexture(*texture); nekopaySprite.setOrigin(texture->getSize().x, texture->getSize().y/2); }
            
            
            UpdateAlpha();
        }
        void GroceryUI::Destroy() { ic::DeleteImage(L"Data/Images/Backgrounds/grocery.jpg"); ic::DeleteImage(L"Data/Images/UI/ShopSlotConcept.png"); }
        void GroceryUI::SwitchMode(const Mode& to)
        {
            prev = mode; mode = to;
            switch (mode)
            {
                case Mode::Check: shelfFood.clear(); ResizeCheck(); break;
                case Mode::Food:
                    shelfFood.clear();
                    shelfFood.emplace_back();
                    for (auto i : Inventory::map)
                        if (i.second->type == ItemType::Food) {
                            int qty = 0; auto it = checkList.find(i.second);
                            if (it != checkList.end()) qty = it->second;
                            shelfFood.emplace_back(i.second, qty); }
                    break;
                case Mode::Drinks:
                    shelfFood.clear();
                    shelfFood.emplace_back();
                    for (auto i : Inventory::map)
                        if (i.second->type == ItemType::Drink) {
                            int qty = 0; auto it = checkList.find(i.second);
                            if (it != checkList.end()) qty = it->second;
                            shelfFood.emplace_back(i.second, qty); }
                    break;
                default: shelfFood.clear(); break;
            }
        }
        void GroceryUI::PollEvent(sf::Event& event)
        {
            if (!active || gs::ignoreEvent) return;
            switch (mode)
            {
                case Mode::Food: case Mode::Drinks:
                    if (quitB.PollEvent(event)) SwitchMode(Mode::Main);
                    // else if (checkoutB.PollEvent(event)) SwitchMode(Mode::Check);
                    else if (event.type == sf::Event::TouchEnded || event.type == sf::Event::MouseButtonReleased)
                    {
                        xx = 0; yy = 0; done = false;
                        for (int i = shelfColumnL; i < shelfColumnR && !done; ++i)
                        {
                            for (int j = 0; j < shelfCount && !done; ++j)
                            {
                                xy = i*shelfCount + j;
                                if (shelfFood[xy].texture)
                                {
                                    sf::Vector2i dot; if (event.type == sf::Event::TouchEnded)
                                        dot = { event.touch.x, event.touch.y }; else dot = { event.mouseButton.x, event.mouseButton.y };
                                    if (dot.x > xx && dot.x < xx + shelfItemWidth && dot.y > yy && dot.y < yy + shelfSprite.getGlobalBounds().height)
                                    {
                                        selectedxy = xy; entity->AddComponent<ItemDetails>(shelfFood[xy].item, shelfFood[xy].count); return;
                                        // Добавлять нужно в некий список Transition, который будет опустощён по прибытию уже в апартаменты (т.е. когда ты якобы кладёшь вещи в холодос). Это обусловлено тем, что не в холодильник же сразу вещи-то добавляются.
                                    }
                                }
                                
                                yy += shelfSprite.getGlobalBounds().height;
                                done = (xy >= shelfFood.size() - 1);
                            }
                            yy = 0; xx += shelfItemWidth;
                        }
                    }
                    break;
                case Mode::Check:
                    if (quitB.PollEvent(event)) SwitchMode(Mode::Main);
                    else if (payB.PollEvent(event))
                    {
                        if (!checkList.empty() && NekoS::money >= checkPrice)
                        {
                            for (auto i : checkList)
                                Inventory::transition.Add(i.first, i.second, false);
                            NekoS::money -= checkPrice;
                            checkList.clear(); checkPrice = 0;
                            checkName.setString(L"Куплено!");
                            checkName.setPosition(gs::width/2 - checkName.getGlobalBounds().width/2,
                                                  gs::height/2 - checkName.getGlobalBounds().height/2);
                            checkBalance.setString(L"Баланс: " + std::to_wstring(NekoS::money) + L" Р");
                            entity->system->SendMessage({"RoomUI :: Update"});
                        }
                    }
                    else if (resetB.PollEvent(event))
                    {
                        checkList.clear(); checkPrice = 0;
                        checkName.setString(L"Пусто");
                        checkName.setPosition(gs::width/2 - checkName.getGlobalBounds().width/2,
                                              gs::height/2 - checkName.getGlobalBounds().height/2);
                    }
                    break;
                default:
                    if (quitB.PollEvent(event))
                    {
                        if (Inventory::transition.list.empty()) entity->system->SendMessage({"MapUI :: Show"});
                        else { entity->SendMessage({"PlacesUI :: ApartmentUI"}); active = false; }
                    }
                    else if (checkoutB.PollEvent(event)) SwitchMode(Mode::Check);
                    else if (discountsB.PollEvent(event)) { }
                    else if (foodB.PollEvent(event)) SwitchMode(Mode::Food);
                    else if (drinksB.PollEvent(event)) SwitchMode(Mode::Drinks);
                    else if (householdB.PollEvent(event)) { }
                    break;
            }
        }
        void GroceryUI::ResizeCheck()
        {
            if (checkList.empty()) { checkName.setString(L"Пусто"); checkName.setPosition(gs::width/2 - checkName.getGlobalBounds().width/2, gs::height/2 - checkName.getGlobalBounds().height/2); }
            else
            {
                checkLinesXX1 = checkShape.getPosition().x + 20*gs::scale;
                checkLinesXX2 = checkShape.getPosition().x + 0.7*checkShape.getSize().x;
                checkLinesXX3 = checkShape.getPosition().x + checkShape.getSize().x - 20*gs::scale;
                checkName.setString(L"Наименование");
                checkLinesYY = checkShape.getPosition().y + checkShape.getSize().y/2 - (checkName.getGlobalBounds().height + 10*gs::scale - 14*gs::scale + checkName.getGlobalBounds().height + 20*gs::scale + (checkName.getGlobalBounds().height + 10*gs::scale)*checkList.size() - 14*gs::scale + checkName.getGlobalBounds().height + 20*gs::scale + checkName.getGlobalBounds().height)/2;
                if (checkLinesYY < checkCaption.getPosition().y + checkCaption.getGlobalBounds().height)
                    checkLinesYY = checkCaption.getPosition().y + checkCaption.getGlobalBounds().height;
            }
        }
        void GroceryUI::Resize(const unsigned int& width, const unsigned int& height)
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
            
            std::thread([this]
            {
                sf::Context context;
                gs::requestWindowRefresh = true;
                
                shelvesTexture.create(gs::width, gs::height);
                shelfSprite.setScale(gs::width/shelfSprite.getLocalBounds().width, gs::scale);
                int count = floor(gs::height/shelfSprite.getGlobalBounds().height) - 1;
                if (gs::trueVerticalOrientation) count -= 1;
                if (count < 1) count = 1;
                shelfSprite.setScale(gs::width/shelfSprite.getLocalBounds().width,
                                     gs::height/(shelfSprite.getLocalBounds().height * count));
                float yy = 0, yyuntil = gs::height;
                while (yy < yyuntil)
                {
                    shelfSprite.setPosition(0, yy);
                    shelvesTexture.draw(shelfSprite);
                    yy += shelfSprite.getGlobalBounds().height;
                }
                shelvesTexture.display();
                shelvesSprite.setTexture(shelvesTexture.getTexture(), true);
            }).join();
            shelfCount = floor(gs::height/shelfSprite.getGlobalBounds().height + 0.1f);
            shelfItemWidth = 200*gs::scale;
            shelfColumnR = floor(gs::width/shelfItemWidth) + 1;
            pricetagShape.setSize({55*gs::scale, 23*shelfSprite.getScale().y});
            pricetagShape.setOrigin(pricetagShape.getSize().x/2, 0);
            pricetagText.setCharacterSize(30*gs::scale);
            pricetagText.setOutlineThickness(gs::scale);
            
            
            checkShape.setCornersRadius(170*gs::scale);
            checkShape.setOutlineThickness(10 * gs::scale);
            if (gs::trueVerticalOrientation) checkShape.setSize({1000*gs::scale, 650*gs::scaley});
            else checkShape.setSize({1000*gs::scale, 700*gs::scale});
            if (checkShape.getGlobalBounds().width > gs::width) checkShape.setSize({(float)gs::width - 20*gs::scale, checkShape.getSize().y});
            checkShape.setPosition(gs::width/2 - checkShape.getSize().x/2, gs::height/2 - checkShape.getSize().y/2);
            
            checkCaption.setCharacterSize(48*gs::scale);
            checkCaption.setPosition(gs::width/2 - checkCaption.getGlobalBounds().width/2, checkShape.getPosition().y);
            
            payB.Resize(width, height); resetB.Resize(width, height);
            payB.setSize({0.28f*checkShape.getSize().x, payB.text.getGlobalBounds().height + 20*gs::scale});
            resetB.setSize({0.22f*checkShape.getSize().x, resetB.text.getGlobalBounds().height + 10*gs::scale});
            payB.setPosition(gs::width/2 - payB.shape.getGlobalBounds().width/2, checkShape.getPosition().y + checkShape.getSize().y - payB.shape.getGlobalBounds().height - 15*gs::scale);
            nekopaySprite.setScale(0.18*gs::scale, 0.18*gs::scale);
            nekopaySprite.setPosition(payB.shape.getPosition().x + payB.shape.getSize().x/2 - 10*gs::scale, payB.shape.getPosition().y + payB.shape.getSize().y/2 - 2.5f*gs::scale);
            resetB.setPosition(payB.shape.getPosition().x + payB.shape.getGlobalBounds().width + 10*gs::scale, checkShape.getPosition().y + checkShape.getSize().y - resetB.shape.getGlobalBounds().height - 15*gs::scale);
            checkBalance.setCharacterSize(25*gs::scale);
            checkBalance.setPosition(checkShape.getPosition().x + checkBalance.getGlobalBounds().width, checkShape.getPosition().y + checkShape.getSize().y - resetB.shape.getGlobalBounds().height - 15*gs::scale);
            
            checkName.setCharacterSize(32*gs::scale);
            checkQty.setCharacterSize(32*gs::scale);
            checkPriceT.setCharacterSize(32*gs::scale);
            
            ResizeCheck();
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
                            if (shelfFood[xy].texture)
                            {
                                pricetagShape.setPosition(xx + shelfItemWidth/2, yy + (128 - 23)*shelfSprite.getScale().y);
                                window->draw(pricetagShape); pricetagText.setString(std::to_string(shelfFood[xy].item->price));
                                pricetagText.setPosition(xx + shelfItemWidth/2 - pricetagText.getGlobalBounds().width/2, yy + (128 - 23)*shelfSprite.getScale().y - 3*gs::scale);
                                if (pricetagText.getGlobalBounds().width > shelfItemWidth)
                                    pricetagText.setScale(shelfItemWidth/pricetagText.getLocalBounds().width, 1.f);
                                window->draw(pricetagText);
                                
                                shelfItem.setTexture(*shelfFood[xy].texture, true);
                                shelfItem.setScale(0.85*gs::scale*shelfFood[xy].item->scale, 0.85*gs::scale*shelfFood[xy].item->scale);
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
                    
                    quitB.draw(window);
                    // checkoutB.Draw(window);
                    break;
                case Mode::Check:
                    window->draw(background);
                    
                    window->draw(checkShape);
                    window->draw(checkCaption);
                    window->draw(checkBalance);
                    if (checkList.empty()) window->draw(checkName);
                    else
                    {
                        float xx1 = checkLinesXX1, xx2 = checkLinesXX2, xx3 = checkLinesXX3, yy = checkLinesYY;
                        checkName.setString(L"Наименование");
                        checkQty.setString(L"Кол-во");
                        checkPriceT.setString(L"Цена");
                        checkQty.setOrigin(checkQty.getLocalBounds().width, 0);
                        checkPriceT.setOrigin(checkPriceT.getLocalBounds().width, 0);
                        checkName.setPosition(xx1, yy);
                        checkQty.setPosition(xx2, yy);
                        checkPriceT.setPosition(xx3, yy);
                        window->draw(checkName);
                        window->draw(checkQty);
                        window->draw(checkPriceT);
                        yy += checkName.getGlobalBounds().height + 10*gs::scale;
                        
                        yy -= 14*gs::scale;
                        checkName.setString(L"------------------------------------");
                        checkQty.setString(L"------------------------------------");
                        checkPriceT.setString(L"------------------------------------");
                        checkQty.setOrigin(checkQty.getLocalBounds().width, 0);
                        checkPriceT.setOrigin(checkPriceT.getLocalBounds().width, 0);
                        checkName.setPosition(xx1, yy);
                        checkQty.setPosition(xx2, yy);
                        checkPriceT.setPosition(xx3, yy);
                        window->draw(checkName);
                        window->draw(checkQty);
                        window->draw(checkPriceT);
                        yy += checkName.getGlobalBounds().height + 20*gs::scale;
                        
                        for (auto i : checkList)
                        {
                            checkName.setString(i.first->display);
                            checkQty.setString(std::to_string(i.second));
                            checkPriceT.setString(std::to_string((int)(i.first->price * i.second)));
                            checkQty.setOrigin(checkQty.getLocalBounds().width, 0);
                            checkPriceT.setOrigin(checkPriceT.getLocalBounds().width, 0);
                            checkName.setPosition(xx1, yy);
                            checkQty.setPosition(xx2, yy);
                            checkPriceT.setPosition(xx3, yy);
                            window->draw(checkName);
                            window->draw(checkQty);
                            window->draw(checkPriceT);
                            yy += checkName.getGlobalBounds().height + 10*gs::scale;
                        }
                        
                        yy -= 14*gs::scale;
                        checkName.setString(L"------------------------------------");
                        checkQty.setString(L"------------------------------------");
                        checkPriceT.setString(L"------------------------------------");
                        checkQty.setOrigin(checkQty.getLocalBounds().width, 0);
                        checkPriceT.setOrigin(checkPriceT.getLocalBounds().width, 0);
                        checkName.setPosition(xx1, yy);
                        checkQty.setPosition(xx2, yy);
                        checkPriceT.setPosition(xx3, yy);
                        window->draw(checkName);
                        window->draw(checkQty);
                        window->draw(checkPriceT);
                        yy += checkName.getGlobalBounds().height + 20*gs::scale;
                        
                        checkName.setString(L"    ИТОГО: " + std::to_wstring(checkPrice) + L" Р");
                        checkName.setPosition(xx1, yy);
                        window->draw(checkName);
                    }
                    payB.draw(window);
                    window->draw(nekopaySprite);
                    resetB.draw(window);
                    quitB.draw(window);
                    break;
                default:
                    window->draw(background);
                    
                    discountsB.draw(window);
                    foodB.draw(window);
                    drinksB.draw(window);
                    householdB.draw(window);
                    
                    quitB.draw(window);
                    checkoutB.draw(window);
                    break;
            }
        }
        void GroceryUI::ReceiveMessage(MessageHolder& message)
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
            else if (active && message.info == "++count") { ++shelfFood[selectedxy].count; checkList[shelfFood[selectedxy].item] = shelfFood[selectedxy].count; checkPrice += shelfFood[selectedxy].item->price; }
            else if (active && message.info == "count += 10") { shelfFood[selectedxy].count += 10; checkList[shelfFood[selectedxy].item] = shelfFood[selectedxy].count; checkPrice += 10*shelfFood[selectedxy].item->price; }
            else if (active && message.info == "--count")
            {
                --shelfFood[selectedxy].count;
                if (shelfFood[selectedxy].count <= 0) { shelfFood[selectedxy].count = 0; checkList.erase(shelfFood[selectedxy].item); }
                else { checkList[shelfFood[selectedxy].item] = shelfFood[selectedxy].count; }
                checkPrice -= shelfFood[selectedxy].item->price;
            }
            else if (active && message.info == "count -= 10")
            {
                shelfFood[selectedxy].count -= 10; int actualCount = 10;
                if (shelfFood[selectedxy].count <= 0) { actualCount = 10 + shelfFood[selectedxy].count; shelfFood[selectedxy].count = 0; checkList.erase(shelfFood[selectedxy].item); }
                else checkList[shelfFood[selectedxy].item] = shelfFood[selectedxy].count;
                checkPrice -= actualCount*shelfFood[selectedxy].item->price;
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
            
            checkShape.setFillColor({checkShape.getFillColor().r, checkShape.getFillColor().g, checkShape.getFillColor().b, static_cast<sf::Uint8>(0.93f*alpha)});
            checkShape.setOutlineColor({checkShape.getOutlineColor().r, checkShape.getOutlineColor().g, checkShape.getOutlineColor().b, static_cast<sf::Uint8>(0.5f*alpha)});
            checkCaption.setFillColor({checkCaption.getFillColor().r, checkCaption.getFillColor().g, checkCaption.getFillColor().b, alpha});
            checkName.setFillColor({checkName.getFillColor().r, checkName.getFillColor().g, checkName.getFillColor().b, alpha});
            checkBalance.setFillColor({checkBalance.getFillColor().r, checkBalance.getFillColor().g, checkBalance.getFillColor().b, alpha});
            checkQty.setFillColor({checkQty.getFillColor().r, checkQty.getFillColor().g, checkQty.getFillColor().b, alpha});
            checkPriceT.setFillColor({checkPriceT.getFillColor().r, checkPriceT.getFillColor().g, checkPriceT.getFillColor().b, alpha});
            payB.setAlpha(alpha);
            resetB.setAlpha(alpha);
        }
    }
}
