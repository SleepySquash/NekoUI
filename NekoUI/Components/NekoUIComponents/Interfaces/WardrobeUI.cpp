//
//  WardrobeUI.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 28/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "WardrobeUI.hpp"

namespace NekoUI
{
    void WardrobeUI::Init()
    {
        nintShape.setFillColor(sf::Color(0,0,0,80));
        nintShape.setOutlineColor(sf::Color::White);
        sf::Font* font = fc::GetFont(L"Impact.ttf");
        if (font) emptyText.setFont(*font);
        emptyText.setOutlineColor(sf::Color::Black);
        emptyText.setString(L"Empty");
        
        slotUndressButton.setTexture(L"Data/Images/UI/select_none.png");
        categoriesButtons.setTexture(L"Data/Images/UI/wardrobe_category.png");
        categoriesButtons.setScale(.45);
        categoriesButtons.halign = Halign::Left;
        categoriesButtons.valign = Valign::Top;
        
        sf::Texture* texture = ic::LoadTexture(L"Data/Images/UI/wardrobe_category.png");
        if (texture) selectedCategorySprite.setTexture(*texture, true);
        selectedCategorySprite.setColor(sf::Color::Green);
        
        texture = ic::LoadTexture(L"Data/Images/UI/inventoryslot.png");
        if (texture) selectedClothSprite.setTexture(*texture, true);
        selectedClothSprite.setColor(sf::Color::Green);
        
        texture = ic::LoadTexture(L"Data/Images/UI/wardrobe_categories.png");
        if (texture) categoriesSprite.setTexture(*texture, true);
        
        filterButtons.setFont(L"Impact.ttf");
        filterButtons.setCharacterSize(48);
        filterButtons.setString(L"All");
        filterButtons.halign = Halign::Left;
        filterButtons.valign = Valign::Top;
    }
    void WardrobeUI::Destroy()
    {
        Player::SavePersona();
        ic::DeleteImage(L"Data/Images/UI/wardrobe_category.png");
        ic::DeleteImage(L"Data/Images/UI/select_none.png");
        ic::DeleteImage(L"Data/Images/UI/inventoryslot.png");
        ic::DeleteImage(L"Data/Images/UI/wardrobe_categories.png");
        if (active) CleanUp();
    }
    void WardrobeUI::Update(const sf::Time& elapsedTime)
    {
        if (!active) return;
        if (updateWindowRefresh) { updateWindowRefresh = false; gs::requestWindowRefresh = true; }
        switch (mode)
        {
            case appearing:      //currentTime = appearTime;
                if (currentTime < appearTime) currentTime += elapsedTime.asSeconds();
                if (currentTime >= appearTime) { gs::ignoreDraw = true; alpha = 255; currentTime = 0.f; mode = existing; }
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
    void WardrobeUI::CleanUp()
    {
        ic::DeleteImage(L"Data/Images/Backgrounds/wardrobe.jpg");
        ic::DeleteImage(L"Data/Images/UI/inventoryslot.png");
        if (inventory) for (auto& item : inventory->list) ic::DeleteImage(utf16("Data/Items/" + item.first->name + ".png"));
    }
    void WardrobeUI::PollEvent(sf::Event& event)
    {
        if (event.type == sf::Event::Closed || event.type == sf::Event::LostFocus) Player::SavePersona();
        if (!active || !gs::isActiveInterface(this)) return;
        
        if (filterButtons.PollEvent(event)) { }
        else if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::TouchBegan || event.type == sf::Event::TouchEnded)
        {
            float xx = categoriesStartXX;
            for (int i = 0; i < 9; ++i)
            {
                categoriesButtons.index = i;
                categoriesButtons.setPosition(xx, categoriesButtons.sprite.getPosition().y);
                if (categoriesButtons.PollEvent(event))
                {
                    bool update{ false };
                    switch (i)
                    {
                        case 0: update = (category != Category::Head); category = Category::Head; break;
                        case 1: update = (category != Category::Top); category = Category::Top; break;
                        case 2: update = (category != Category::Bottom); category = Category::Bottom; break;
                        case 3: update = (category != Category::Onepiece); category = Category::Onepiece; break;
                        case 4: update = (category != Category::Underwear); category = Category::Underwear; break;
                        case 5: update = (category != Category::Socks); category = Category::Socks; break;
                        case 6: update = (category != Category::Shoes); category = Category::Shoes; break;
                        case 7: update = (category != Category::Accessories); category = Category::Accessories; break;
                        default: break;
                    }
                    if (update) UpdateCategory(); return;
                }
                xx += categoriesButtons.sprite.getGlobalBounds().width + 2*gs::scale;
            }
            
            if (!isEmpty && inventory)
            {
                xx = startSlotsXX; int yy = nintShape.getGlobalBounds().top + 10*gs::scale, inrow = 0, i = 0;
                
                slotSprite.setPosition(xx, yy); ++inrow;
                slotUndressButton.setPosition(xx + slotSprite.getGlobalBounds().width/2, yy + slotSprite.getGlobalBounds().height/2);
                if (slotUndressButton.PollEvent(event))
                {
                    switch (category)
                    {
                        case Category::Head: Player::neko.Undress(ClothType::Head); break;
                        case Category::Top: Player::neko.Undress(ClothType::Top); break;
                        case Category::Bottom: Player::neko.Undress(ClothType::Bottom); break;
                        case Category::Onepiece: Player::neko.Undress(ClothType::Onepiece); break;
                        case Category::Underwear: Player::neko.Undress(ClothType::Bra); Player::neko.Undress(ClothType::Pantsu); break;
                        case Category::Socks: Player::neko.Undress(ClothType::Socks); break;
                        case Category::Shoes: Player::neko.Undress(ClothType::Legwear); break;
                        case Category::Accessories: Player::neko.Undress(ClothType::Accessory); Player::neko.Undress(ClothType::Gloves); break;
                        default: break;
                    }
                    return;
                }
                if (inrow >= slotsXXCount) { inrow = 0; xx = startSlotsXX; yy += 126*gs::scale; } else xx += 126*gs::scale;
                
                for (auto& item : inventory->list)
                {
                    slotSprite.setPosition(xx, yy);
                    if (ic::FindTexture(utf16("Data/Items/" + item.first->name + ".png")))
                    {
                        itemButtons.index = i;
                        sf::Texture* texture = ic::FindTexture(utf16("Data/Items/" + item.first->name + ".png"));
                        itemButtons.setTexture(texture);
                        if (texture) {
                            float scaleX = 126.f / texture->getSize().x, scaleY = 126.f / texture->getSize().y;
                            itemButtons.setScale(scaleX > scaleY ? scaleY : scaleX); }
                        itemButtons.setPosition(xx + slotSprite.getGlobalBounds().width/2, yy + slotSprite.getGlobalBounds().height/2);
                        if (itemButtons.PollEvent(event))
                        {
                            if (item.first->dependencies)
                            {
                                bool dressing{ item.first->dressed };
                                if (!item.first->dependencies->calculated) Inventory::CalculateWearset(item.first);
                                for (auto it : item.first->dependencies->set)
                                    if ((!dressing && !it->dressed) || (dressing && it->dressed))
                                        DressUndressItem(it, false);
                                DressUndressItem(item.first, true);
                            }
                            else DressUndressItem(item.first, true);
                            Player::neko.savingIsRequired = true;
                            return;
                        }
                        ++i;
                    }
                    ++inrow; if (inrow >= slotsXXCount) { inrow = 0; xx = startSlotsXX; yy += 126*gs::scale; } else xx += 126*gs::scale;
                }
            }
            itemButtons.eventPolled(event);
            categoriesButtons.eventPolled(event);
        }
    }
    void WardrobeUI::DressUndressItem(Wearable* item, bool sort)
    {
        if (item->dressed) Player::neko.Undress(item);
        else
        {
            switch (category)
            {
                case Category::Head: UndressByCategory(ClothType::Head); break;
                case Category::Top: UndressByCategory(ClothType::Onepiece); UndressByCategory(ClothType::Top); break;
                case Category::Bottom: UndressByCategory(ClothType::Onepiece); UndressByCategory(ClothType::Bottom); break;
                case Category::Onepiece: UndressByCategory(ClothType::Onepiece); UndressByCategory(ClothType::Top); UndressByCategory(ClothType::Bottom); break;
                case Category::Underwear: if (item->clothing == ClothType::Bra) UndressByCategory(ClothType::Bra); else if (item->clothing == ClothType::Pantsu) UndressByCategory(ClothType::Pantsu); break;
                case Category::Socks: UndressByCategory(ClothType::Socks); break;
                case Category::Shoes: UndressByCategory(ClothType::Legwear); break;
                default: break;
            }
            Player::neko.Dress(item, sort);
        }
    }
    void WardrobeUI::UndressByCategory(const ClothType& category)
    {
        vector<Cloth*>::iterator it = Player::neko.cloth.begin();
        while (it != Player::neko.cloth.end())
            if ((*it)->item && (*it)->item->clothing == category)
            {
                if ((*it)->offline) { delete (*it); it = Player::neko.cloth.erase(it); }
                else
                {
                    if ((*it)->item->dependencies)
                    {
                        Cloth* cl = (*it);
                        if (!cl->item->dependencies->calculated) Inventory::CalculateWearset(cl->item);
                        for (auto iter : cl->item->dependencies->set) Player::neko.Undress(iter);
                        cl->item->dressed = false; cl->Destroy(); it = Player::neko.cloth.begin();
                    }
                    else { (*it)->item->dressed = false; (*it)->Destroy(); delete (*it); it = Player::neko.cloth.erase(it); }
                }
            }
            else ++it;
    }
    void WardrobeUI::Resize(const unsigned int& width, const unsigned int& height)
    {
        if (!active) return;
        
        // quitButton.Resize(width, height); quitButton.setPosition(gs::width - gs::width/12, gs::height/8);
        if (spriteLoaded)
        {
            float scaleFactorX, scaleFactorY, scaleFactor;
            scaleFactorX = (float)width / (background.getTexture()->getSize().x);
            scaleFactorY = (float)height / (background.getTexture()->getSize().y);
            
            scaleFactor = (scaleFactorX > scaleFactorY) ? scaleFactorX : scaleFactorY;
            background.setScale(scaleFactor, scaleFactor);
            background.setPosition(width/2, height/2);
        }
        
        Player::neko.personScale = 0.96f*((gs::height - gs::screenOffsetTop)/((float)gs::relativeHeight)); Player::neko.ResizePerson();
        if (gs::verticalOrientation)
        {
            // Player::neko.personScale = 1.21 * gs::scScale; Player::neko.ResizePerson();
            nintShape.setSize({gs::width*4.6f/5.f, gs::height*4.f/5.f});
            nintShape.setPosition((gs::width - nintShape.getSize().x)/2, 0);
            
            if (Player::neko.body.person.getGlobalBounds().height + 20*gs::scale > gs::height)
                Player::neko.setPersonPosition(width/2, Player::neko.body.person.getGlobalBounds().height + 20*gs::scale);
            else Player::neko.setPersonPosition(width/2, height);
        }
        else
        {
            // Player::neko.personScale = 1.01 * gs::scScale; Player::neko.ResizePerson();
            nintShape.setSize({3.4f*gs::width/5.f, gs::height*4.f/5.f});
            nintShape.setPosition(gs::width - nintShape.getSize().x - 20*gs::scale, 0);
            
            if (Player::neko.body.person.getGlobalBounds().height + 20*gs::scale > gs::height)
                Player::neko.setPersonPosition(0.83*width/5, Player::neko.body.person.getGlobalBounds().height + 20*gs::scale);
            else Player::neko.setPersonPosition(0.83*width/5, height);
        }
        nintShape.setOutlineThickness(2*gs::scale);
        
        categoriesButtons.setPosition(0, 20*gs::scale + gs::screenOffsetTop); categoriesButtons.setScale(0.42f); categoriesButtonsScaling = 1.f;
        float xx = gs::verticalOrientation ? 0 : nintShape.getGlobalBounds().left; categoriesStartXX = xx;
        
        xx += (categoriesButtons.sprite.getGlobalBounds().width + 2*gs::scale)*9;
        if (!gs::verticalOrientation && xx > nintShape.getGlobalBounds().left + nintShape.getGlobalBounds().width) categoriesButtonsScaling = nintShape.getGlobalBounds().width/(xx - nintShape.getGlobalBounds().left);
        else if (!gs::verticalOrientation) categoriesStartXX = nintShape.getGlobalBounds().left + nintShape.getGlobalBounds().width/2 - (xx - nintShape.getGlobalBounds().left)/2;
        else if (gs::verticalOrientation && xx > gs::width) categoriesButtonsScaling = gs::width/xx;
        else if (gs::verticalOrientation) categoriesStartXX = gs::width/2 - xx/2;
        if (categoriesButtonsScaling != 1.f) categoriesButtons.setScale(0.42f * categoriesButtonsScaling);
        categoriesSprite.setScale(256.f/114.f * categoriesButtons.sprite.getScale().x, 256.f/114.f * categoriesButtons.sprite.getScale().y);
        selectedCategorySprite.setScale(categoriesButtons.sprite.getScale().x, categoriesButtons.sprite.getScale().y);
        
        filterButtons.Resize(width, height);
        filterButtons.setPosition(nintShape.getPosition().x, categoriesButtons.sprite.getGlobalBounds().top + categoriesButtons.sprite.getGlobalBounds().height);
        
        float remainingSpace = gs::height - categoriesButtons.sprite.getGlobalBounds().height - categoriesButtons.sprite.getGlobalBounds().top - filterButtons.text.getGlobalBounds().height - 30*gs::scale;
        nintShape.setSize({nintShape.getSize().x, remainingSpace});
        nintShape.setPosition(nintShape.getPosition().x, -remainingSpace + gs::height - 15*gs::scale);
        
        slotSprite.setScale(0.5*gs::scale, 0.5*gs::scale); ResizeSlots();
        selectedClothSprite.setScale(slotSprite.getScale().x, slotSprite.getScale().y);
        float scaleX = 110.f / slotUndressButton.sprite.getLocalBounds().width, scaleY = 110.f / slotUndressButton.sprite.getLocalBounds().height;
        slotUndressButton.setScale((scaleX > scaleY ? scaleY : scaleX));
        
        emptyText.setCharacterSize(90*gs::scale);
        emptyText.setOutlineThickness(2*gs::scale);
        emptyText.setPosition(nintShape.getGlobalBounds().left + nintShape.getGlobalBounds().width/2 - emptyText.getGlobalBounds().width/2, nintShape.getGlobalBounds().top + nintShape.getGlobalBounds().height/2 - emptyText.getGlobalBounds().height/2);
    }
    void WardrobeUI::Draw(sf::RenderWindow* window)
    {
        if (!active) return;
        
        //window->draw(blackScreenShape);
        if (spriteLoaded) window->draw(background);
        Player::neko.Draw(window, true);
        window->draw(nintShape);
        
        float xx = categoriesStartXX;
        for (int i = 0; i < 9; ++i)
        {
            categoriesButtons.index = i;
            categoriesButtons.setPosition(xx, categoriesButtons.sprite.getPosition().y);
            categoriesButtons.draw(window);
            switch (i)
            {
                case 0: categoriesSprite.setTextureRect({0, 0, 111, 114}); break;
                case 1: categoriesSprite.setTextureRect({111, 0, 111, 114}); break;
                case 2: categoriesSprite.setTextureRect({222, 0, 111, 114}); break;
                case 3: categoriesSprite.setTextureRect({333, 0, 111, 114}); break;
                case 4: categoriesSprite.setTextureRect({444, 0, 111, 114}); break;
                case 5: categoriesSprite.setTextureRect({555, 0, 111, 114}); break;
                case 6: categoriesSprite.setTextureRect({666, 0, 111, 114}); break;
                case 7: categoriesSprite.setTextureRect({777, 0, 111, 114}); break;
                case 8: categoriesSprite.setTextureRect({888, 0, 111, 114}); break;
                default: break;
            }
            if ((int)category == i) { selectedCategorySprite.setPosition(xx, categoriesButtons.sprite.getPosition().y);
                window->draw(selectedCategorySprite); }
            categoriesSprite.setPosition(xx, categoriesButtons.sprite.getPosition().y);
            window->draw(categoriesSprite);
            xx += categoriesButtons.sprite.getGlobalBounds().width + 2*gs::scale;
        }
        filterButtons.draw(window);
        
        if (isEmpty) window->draw(emptyText);
        else if (inventory)
        {
            int xx = startSlotsXX, yy = nintShape.getGlobalBounds().top + 10*gs::scale, inrow = 0, i = 0;
            
            slotSprite.setPosition(xx, yy); window->draw(slotSprite);
            slotUndressButton.setPosition(xx + slotSprite.getGlobalBounds().width/2, yy + slotSprite.getGlobalBounds().height/2);
            slotUndressButton.draw(window); ++inrow;
            if (inrow >= slotsXXCount) { inrow = 0; xx = startSlotsXX; yy += 126*gs::scale; } else xx += 126*gs::scale;
            
            for (auto& item : inventory->list)
            {
                slotSprite.setPosition(xx, yy); window->draw(slotSprite);
                if (item.first->dressed) { selectedClothSprite.setPosition(xx, yy); window->draw(selectedClothSprite); }
                if (ic::FindTexture(utf16("Data/Items/" + item.first->name + ".png")))
                {
                    itemButtons.index = i;
                    sf::Texture* texture = ic::FindTexture(utf16("Data/Items/" + item.first->name + ".png"));
                    itemButtons.setTexture(texture);
                    if (texture) {
                        float scaleX = 126.f / texture->getSize().x, scaleY = 126.f / texture->getSize().y;
                        itemButtons.setScale(scaleX > scaleY ? scaleY : scaleX); }
                    itemButtons.setPosition(xx + slotSprite.getGlobalBounds().width/2, yy + slotSprite.getGlobalBounds().height/2);
                    itemButtons.draw(window);
                    ++i;
                }
                ++inrow; if (inrow >= slotsXXCount) { inrow = 0; xx = startSlotsXX; yy += 126*gs::scale; } else xx += 126*gs::scale;
            }
        }
    }
    void WardrobeUI::ReceiveMessage(MessageHolder& message)
    {
        if (!active && message.info == "WardrobeUI :: Show") Switch(true);
        else if (active && message.info == "WardrobeUI :: Close") Switch(false);
        else if (active && nss::Command(message.info, "Request"))
        {
            updateWindowRefresh = true; // cout << message.info << "  " << utf8(message.additional) << endl;
            if (message.additional == L"Data/Images/Backgrounds/wardrobe.jpg")
            {
                sf::Texture* texture = ic::LoadTexture(message.additional);
                if ((spriteLoaded = texture))
                {
                    background.setTexture(*texture, true);
                    background.setOrigin(texture->getSize().x/2, texture->getSize().y/2);
                    
                    float scaleFactorX, scaleFactorY, scaleFactor;
                    scaleFactorX = (float)gs::width / (texture->getSize().x);
                    scaleFactorY = (float)gs::height / (texture->getSize().y);
                    
                    scaleFactor = (scaleFactorX > scaleFactorY) ? scaleFactorX : scaleFactorY;
                    background.setScale(scaleFactor, scaleFactor);
                    background.setPosition(gs::width/2, gs::height/2);
                }
            }
        }
        else if (message.info == "NekoUI :: SelectNeko") neko = reinterpret_cast<NekoEntity*>(message.address);
        else if (message.info == "WardrobeUI :: Switch") Switch(!active);
    }
    void WardrobeUI::Switch(const bool& on)
    {
        if (on) rm::drawDatePanel = false; else rm::drawDatePanel = (gs::activeInterfaces.size() == 1);
        if (on)
        {
            if (active) mode = appearing;
            else
            {
                /* category = Category::Head; */ inventory = nullptr; UpdateCategory();
                entity->system->SendMessage({"RoomUI :: StatusTransparent"});
                gs::PushInterface(this); active = true; mode = appearing; entity->SortAbove(this);
                sf::Texture* texture = ic::RequestHigherTexture(L"Data/Images/Backgrounds/wardrobe.jpg", entity->system);
                if ((spriteLoaded = texture))
                {
                    background.setTexture(*texture);
                    background.setOrigin(texture->getSize().x/2, texture->getSize().y/2);
                }
                texture = ic::LoadTexture(L"Data/Images/UI/inventoryslot.png");
                if ((spriteLoaded = texture)) slotSprite.setTexture(*texture, true);
                Resize(gs::width, gs::height);
            }
        }
        else if (!on)
        {
            if (active) { gs::RemoveInterface(this); gs::ignoreDraw = false; active = true; mode = disappearing;
                entity->system->SendMessage({"RoomUI :: StatusVivid"}); Player::SavePersona(); }
            else mode = disappearing;
        }
    }
    void WardrobeUI::UpdateAlpha()
    {
        background.setColor({background.getColor().r, background.getColor().g, background.getColor().b, alpha});
        nintShape.setFillColor(sf::Color(nintShape.getFillColor().r, nintShape.getFillColor().g, nintShape.getFillColor().b, 80.f * ((float)alpha/255)));
        nintShape.setOutlineColor(sf::Color(nintShape.getOutlineColor().r, nintShape.getOutlineColor().g, nintShape.getOutlineColor().b, alpha));
        selectedCategorySprite.setColor({selectedCategorySprite.getColor().r, selectedCategorySprite.getColor().g, selectedCategorySprite.getColor().b, alpha});
        selectedClothSprite.setColor({selectedClothSprite.getColor().r, selectedClothSprite.getColor().g, selectedClothSprite.getColor().b, alpha});
        categoriesSprite.setColor({categoriesSprite.getColor().r, categoriesSprite.getColor().g, categoriesSprite.getColor().b, alpha});
        categoriesButtons.setAlpha(alpha);
        filterButtons.setAlpha(alpha);
        
        slotSprite.setColor({slotSprite.getColor().r, slotSprite.getColor().g, slotSprite.getColor().b, alpha});
        slotUndressButton.setAlpha(alpha);
        emptyText.setFillColor({emptyText.getFillColor().r, emptyText.getFillColor().g, emptyText.getFillColor().b, alpha});
        emptyText.setOutlineColor({emptyText.getOutlineColor().r, emptyText.getOutlineColor().g, emptyText.getOutlineColor().b, alpha});
        itemButtons.setAlpha(alpha);
        
        Player::neko.setPersonAlpha(alpha);
    }
    void WardrobeUI::UpdateCategory()
    {
        if (inventory) for (auto& item : inventory->list) ic::DeleteImage(utf16("Data/Items/" + item.first->name + ".png"));
        switch (category)
        {
            case Category::Head: inventory = &Inventory::wardrobeHead; break;
            case Category::Top: inventory = &Inventory::wardrobeTop; break;
            case Category::Bottom: inventory = &Inventory::wardrobeBottom; break;
            case Category::Onepiece: inventory = &Inventory::wardrobeOnepiece; break;
            case Category::Underwear: inventory = &Inventory::wardrobeUnderwear; break;
            case Category::Socks: inventory = &Inventory::wardrobeSocks; break;
            case Category::Shoes: inventory = &Inventory::wardrobeShoes; break;
            case Category::Accessories: inventory = &Inventory::wardrobeAccessories; break;
            default: break;
        }
        isEmpty = inventory->list.empty();
        if (inventory) for (auto& item : inventory->list) ic::PreloadTexture(utf16("Data/Items/" + item.first->name + ".png"), 2, true, true, this);
        ResizeSlots();
    }
    void WardrobeUI::ResizeSlots()
    {
        if (!isEmpty && inventory)
        {
            float xx = nintShape.getGlobalBounds().left + 10*gs::scale; startSlotsXX = xx; bool borderReached{ false }; slotsXXCount = 0;
            
            xx += 126*gs::scale; ++slotsXXCount;
            if ((borderReached = (xx > nintShape.getGlobalBounds().left + nintShape.getGlobalBounds().width - (10)*gs::scale)))
            {
                xx -= 126*gs::scale; --slotsXXCount;
                startSlotsXX = nintShape.getGlobalBounds().left + nintShape.getGlobalBounds().width/2 - (xx - nintShape.getGlobalBounds().left)/2 + 5*gs::scale; return;
            }
            
            for (auto& item : inventory->list)
            {
                slotSprite.setPosition(xx, 0); xx += 126*gs::scale; ++slotsXXCount;
                if ((borderReached = (xx > nintShape.getGlobalBounds().left + nintShape.getGlobalBounds().width - (10)*gs::scale)))
                {
                    xx -= 126*gs::scale; --slotsXXCount;
                    startSlotsXX = nintShape.getGlobalBounds().left + nintShape.getGlobalBounds().width/2 - (xx - nintShape.getGlobalBounds().left)/2 + 5*gs::scale; return;
                }
            }
        }
    }
}
