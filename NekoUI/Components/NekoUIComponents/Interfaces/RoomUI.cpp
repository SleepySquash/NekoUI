//
//  RoomUI.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 23/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "RoomUI.hpp"

namespace NekoUI
{
    void RoomUI::Init()
    {
        nekoMoodText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        nekoMoodText.setString(L"Счастье");
        dayText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        monthText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        tm* timeinfo = std::localtime(&now_c);
        dateDay = timeinfo->tm_mday; dayText.setString(std::to_string(dateDay));
        dateMonth = timeinfo->tm_mon; monthText.setString(GetMonthStringFromItsNumber(dateMonth));
        int hoursUntilNextDay = 24 - timeinfo->tm_hour;
        int minutesUntilNextDay = 60 - timeinfo->tm_min;
        int secsUntilNextDay = 60 - timeinfo->tm_sec;
        secondsUntilNextDay = secsUntilNextDay + 60*minutesUntilNextDay + 3600*hoursUntilNextDay;
        newDayIsComing = (secondsUntilNextDay < 60);
        entity->system->SendMessage({"Time", timeinfo});
        
        sf::Texture* texture = ic::LoadTexture(L"Data/Images/UI/sun.png");
        if ((spriteLoaded = texture)) dateSprite.setTexture(*texture);
        dateSprite.setColor(sf::Color(231, 152, 36));
        
        
        texture = ic::LoadTexture(L"Data/Images/UI/need.png");
        if ((spriteLoaded = texture)) {
            needbaseSprite.setTexture(*texture);
            needbaseSprite.setOrigin(0, texture->getSize().y); }
        texture = ic::LoadTexture(L"Data/Images/UI/needs.png");
        if ((spriteLoaded = texture)) needSprite.setTexture(*texture);
        
        
        // scrolldownMenu.setTexture(L"Data/Images/scrolldown rev1.png");
        scrolldownTexture_normal = ic::LoadTexture(L"Data/Images/UI/scrolldown rev1.png");
        scrolldownTexture_close = ic::LoadTexture(L"Data/Images/UI/ScrolldownButton_exit.png");
        scrolldownMenu.setTexture(scrolldownTexture_normal);
        scrolldownMenu.setScale(0.6f);
        scrolldownMenu.halign = Halign::Right;
        scrolldownMenu.valign = Valign::Bottom;
        
        scrolldownButtons.setTexture(L"Data/Images/UI/scrolldown.png");
        scrolldownButtons.sprite.setTextureRect({0, 0, 108, 125});
        scrolldownButtons.halign = Halign::Right;
        scrolldownButtons.valign = Valign::Center;
        
        active = true;
        mode = appearing;
    }
    void RoomUI::Destroy() { if (active) CleanUp(); }
    void RoomUI::Update(const sf::Time& elapsedTime)
    {
        if (newDayIsComing)
        {
            secondsUntilNextDay -= elapsedTime.asSeconds();
            if (secondsUntilNextDay <= 0.f)
            {
                auto now = std::chrono::system_clock::now();
                std::time_t now_c = std::chrono::system_clock::to_time_t(now);
                tm* timeinfo = std::localtime(&now_c);
                dateDay = timeinfo->tm_mday; dayText.setString(std::to_string(dateDay));
                dateMonth = timeinfo->tm_mon; monthText.setString(GetMonthStringFromItsNumber(dateMonth));
                int hoursUntilNextDay = 24 - timeinfo->tm_hour;
                int minutesUntilNextDay = 60 - timeinfo->tm_min;
                int secsUntilNextDay = 60 - timeinfo->tm_sec;
                secondsUntilNextDay = secsUntilNextDay + 60*minutesUntilNextDay + 3600*hoursUntilNextDay;
                newDayIsComing = (secondsUntilNextDay < 60);
                Player::UpdateDaysTogether();
                entity->system->SendMessage({"Time", timeinfo});
            }
        }
        secondsPassed = elapsedTime.asSeconds();
        if (secondsPassed >= 30.f)
        {
            // TODO: Some simulation as quite a time has passed.
            secondsUntilNextDay -= secondsPassed;
            newDayIsComing = (secondsUntilNextDay <= 30);
            Player::SaveCurrentDT(); dt = 0.f;
        }
        else
        {
            dt += secondsPassed;
            if (dt >= 30.f)
            {
                secondsUntilNextDay -= dt;
                newDayIsComing = (secondsUntilNextDay <= 30);
                Player::SaveCurrentDT(); dt = 0.f;
                
                if (shouldDoBackupSaving)
                {
                    Inventory::SaveInventory(); Inventory::SaveFridge(); Inventory::SaveWardrobe();
                    Player::SaveData(); Player::SavePersona(); cout << "Backup saving... Success!" << endl;
                    entity->system->SendMessage({"Apartment :: Save"});
                }
                shouldDoBackupSaving = !shouldDoBackupSaving;
            }
        }
        
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
        if (rm::requestCloseButton != pressWillCloseInterfaces)
        {
            pressWillCloseInterfaces = rm::requestCloseButton;
            if (pressWillCloseInterfaces) scrolldownMenu.setTexture(scrolldownTexture_close);
            else scrolldownMenu.setTexture(scrolldownTexture_normal);
        }
    }
    void RoomUI::CleanUp()
    {
        ic::DeleteImage(L"Data/Images/UI/sun.png");
        ic::DeleteImage(L"Data/Images/UI/need.png");
        ic::DeleteImage(L"Data/Images/UI/needs.png");
        ic::DeleteImage(L"Data/Images/UI/scrolldown rev1.png");
        ic::DeleteImage(L"Data/Images/UI/ScrolldownButton_exit.png");
    }
    void RoomUI::PollEvent(sf::Event& event)
    {
        if (!active || !rm::drawScrolldownMenu) return;
        
        if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::TouchBegan)
        {
            sf::Vector2i dot; if (event.type == sf::Event::MouseButtonPressed) dot = {event.mouseButton.x, event.mouseButton.y}; else dot = {event.touch.x, event.touch.y};
            if (!(ignoreEventMove = (rm::drawDatePanel && dateSprite.getGlobalBounds().contains(dot.x, dot.y))))
            {
                if (rm::scrolldownMenuOpened) ignoreEventMove = !availableToTouchZone.contains(dot);
                else ignoreEventMove = rm::scrolldownMenuBounds.contains(dot.x, dot.y);
            }
        } else if (event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::TouchEnded) { ignoreEventMove = false;
            if (rm::drawDatePanel && ((event.type == sf::Event::MouseButtonReleased && dateSprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) || (event.type == sf::Event::TouchEnded && dateSprite.getGlobalBounds().contains(event.touch.x, event.touch.y)))) entity->system->SendMessage({"CalendarUI :: Switch"});
        } else if (ignoreEventMove && (event.type == sf::Event::MouseMoved || event.type == sf::Event::TouchMoved)) event = sf::Event();
        
        if (scrolldownMenu.PollEvent(event))
        {
            if (pressWillCloseInterfaces)
            {
                rm::scrolldownMenuOpened = rm::requestCloseButton = false;
                entity->system->SendMessage({"CalendarUI :: Close"});
                entity->system->SendMessage({"NekoUI :: Close"});
                entity->system->SendMessage({"InventoryUI :: Close"});
                entity->system->SendMessage({"DetailsUI :: Close"});
                entity->system->SendMessage({"WardrobeUI :: Close"});
                entity->system->SendMessage({"JobInterfaceUI :: Close"});
                entity->system->SendMessage({"MapUI :: Close"});
            }
            else rm::scrolldownMenuOpened = !rm::scrolldownMenuOpened;
        }
        if (rm::scrolldownMenuOpened && ((event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::TouchEnded || event.type == sf::Event::TouchBegan)))
        {
            float xx = gs::verticalOrientation ? scrolldownMenu.sprite.getPosition().x : scrolldownMenu.sprite.getGlobalBounds().left - 5*scrolldownButtonsScaling*gs::scalex;
            float yy = gs::verticalOrientation ? scrolldownMenu.sprite.getGlobalBounds().top - 5*scrolldownButtonsScaling*gs::scaley : scrolldownMenu.sprite.getPosition().y;
            bool anyPressed{ event.type != sf::Event::TouchEnded && event.type != sf::Event::MouseButtonReleased };
            for (int i = 0; i < 8; ++i)
            {
                scrolldownButtons.index = i;
                switch (i)
                {
                    case 0: scrolldownButtons.sprite.setTextureRect({0, 0, 108, 125});
                        scrolldownButtons.setPosition(xx, yy - 62*gs::scale);
                        if (scrolldownButtons.PollEvent(event)) {
                            entity->system->SendMessage({"CalendarUI :: Close"});
                            entity->system->SendMessage({"NekoUI :: Switch"});
                            entity->system->SendMessage({"InventoryUI :: Close"});
                            entity->system->SendMessage({"WardrobeUI :: Close"});
                            entity->system->SendMessage({"JobInterfaceUI :: Close"});
                            entity->system->SendMessage({"MapUI :: Close"});
                            rm::requestCloseButton = true;
                            rm::scrolldownMenuOpened = false; }
                        break;
                    case 1: scrolldownButtons.sprite.setTextureRect({124, 0, 97, 126});
                        scrolldownButtons.setPosition(xx, yy - 62*gs::scale);
                        if (scrolldownButtons.PollEvent(event)) {
                            entity->system->SendMessage({"CalendarUI :: Close"});
                            entity->system->SendMessage({"NekoUI :: Close"});
                            entity->system->SendMessage({"InventoryUI :: Switch"});
                            entity->system->SendMessage({"WardrobeUI :: Close"});
                            entity->system->SendMessage({"JobInterfaceUI :: Close"});
                            entity->system->SendMessage({"MapUI :: Close"});
                            rm::requestCloseButton = true;
                            rm::scrolldownMenuOpened = false; }
                        break;
                    case 2: scrolldownButtons.sprite.setTextureRect({230, 5, 128, 123});
                        scrolldownButtons.setPosition(xx, yy - 62*gs::scale);
                        if (scrolldownButtons.PollEvent(event)) {
                            entity->system->SendMessage({"CalendarUI :: Close"});
                            entity->system->SendMessage({"NekoUI :: Close"});
                            entity->system->SendMessage({"InventoryUI :: Close"});
                            entity->system->SendMessage({"WardrobeUI :: Switch"});
                            entity->system->SendMessage({"JobInterfaceUI :: Close"});
                            entity->system->SendMessage({"MapUI :: Close"});
                            rm::requestCloseButton = true;
                            rm::scrolldownMenuOpened = false; }
                        break;
                    case 3: scrolldownButtons.sprite.setTextureRect({368, 0, 130, 117});
                        scrolldownButtons.setPosition(xx, yy - 62*gs::scale);
                        if (scrolldownButtons.PollEvent(event)) {
                            entity->system->SendMessage({"CalendarUI :: Close"});
                            entity->system->SendMessage({"NekoUI :: Close"});
                            entity->system->SendMessage({"InventoryUI :: Close"});
                            entity->system->SendMessage({"WardrobeUI :: Close"});
                            entity->system->SendMessage({"JobInterfaceUI :: Switch"});
                            entity->system->SendMessage({"MapUI :: Close"});
                            rm::requestCloseButton = true;
                            rm::scrolldownMenuOpened = false; }
                        break;
                    case 4: scrolldownButtons.sprite.setTextureRect({507, 0, 120, 124});
                        scrolldownButtons.setPosition(xx, yy - 62*gs::scale); break;
                    case 5: scrolldownButtons.sprite.setTextureRect({637, 2, 136, 116});
                        scrolldownButtons.setPosition(xx, yy - 62*gs::scale);
                        if (scrolldownButtons.PollEvent(event)) {
                            entity->system->SendMessage({"CalendarUI :: Close"});
                            entity->system->SendMessage({"NekoUI :: Close"});
                            entity->system->SendMessage({"InventoryUI :: Close"});
                            entity->system->SendMessage({"WardrobeUI :: Close"});
                            entity->system->SendMessage({"JobInterfaceUI :: Close"});
                            entity->system->SendMessage({"MapUI :: Switch"});
                            rm::requestCloseButton = true;
                            rm::scrolldownMenuOpened = false; }
                        break;
                    case 6: scrolldownButtons.sprite.setTextureRect({780, 3, 123, 114});
                        scrolldownButtons.setPosition(xx, yy - 62*gs::scale); break;
                    case 7: scrolldownButtons.sprite.setTextureRect({908, 14, 120, 80});
                        scrolldownButtons.setPosition(xx, yy - 62*gs::scale); break;
                    default: break;
                }
                if (!rm::scrolldownMenuOpened) { anyPressed = true; break; }
                if (!gs::verticalOrientation) xx -= scrolldownButtons.sprite.getGlobalBounds().width - 5*scrolldownButtonsScaling*gs::scalex;
                else yy -= scrolldownButtons.sprite.getGlobalBounds().height - 5*scrolldownButtonsScaling*gs::scaley;
            }
            scrolldownButtons.eventPolled(event);
            if (!anyPressed) rm::scrolldownMenuOpened = false;
        }
        if (ignoreEventMove && (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::TouchBegan)) event = sf::Event();
    }
    void RoomUI::Resize(unsigned int width, unsigned int height)
    {
        if (!active) return;
        
        dateSprite.setScale(0.65f * gs::scale, 0.65f * gs::scale);
        dayText.setCharacterSize(70*gs::scale);
        dayText.setPosition(80*gs::scale, 20*gs::scale);
        dayText.setOrigin(dayText.getLocalBounds().width/2, 0);
        monthText.setCharacterSize(45*gs::scale);
        monthText.setPosition(80*gs::scale, 30*gs::scale + dayText.getGlobalBounds().height);
        monthText.setOrigin(monthText.getLocalBounds().width/2, 0);
        nekoMoodText.setCharacterSize(40*gs::scale);
        nekoMoodText.setPosition(dateSprite.getGlobalBounds().left + dateSprite.getGlobalBounds().width + 15*gs::scale, 6*gs::scale + gs::screenOffsetTop);
        
        
        needbaseSprite.setScale(0.75f*gs::scale, 0.75f*gs::scale);
        needSprite.setScale(0.75f*gs::scale, 0.75f*gs::scale);
        needbaseSprite.setPosition(0, height - 5*gs::scale);
        
        
        scrolldownMenu.Resize(width, height);
        scrolldownMenu.setPosition(width - 10*gs::scalex, height - 10*gs::scaley);
        scrolldownButtons.Resize(width, height);
        rm::scrolldownMenuBounds = scrolldownMenu.sprite.getGlobalBounds();
        
        availableToTouchZone.width = width;
        availableToTouchZone.height = height;
        if (gs::verticalOrientation) availableToTouchZone.width -= rm::scrolldownMenuBounds.width;
        else availableToTouchZone.height -= rm::scrolldownMenuBounds.height;
        
        float xx = gs::verticalOrientation ? scrolldownMenu.sprite.getPosition().x : scrolldownMenu.sprite.getGlobalBounds().left - 5*gs::scalex;
        float yy = gs::verticalOrientation ? scrolldownMenu.sprite.getGlobalBounds().top - 5*gs::scalex : scrolldownMenu.sprite.getPosition().y;
        scrolldownButtons.setScale(1.f);
        
        for (int i = 0; i < 9; ++i)
        {
            switch (i)
            {
                case 0: scrolldownButtons.sprite.setTextureRect({0, 0, 108, 125}); break;
                case 1: scrolldownButtons.sprite.setTextureRect({124, 0, 97, 126}); break;
                case 2: scrolldownButtons.sprite.setTextureRect({230, 5, 128, 123}); break;
                case 3: scrolldownButtons.sprite.setTextureRect({368, 0, 130, 117}); break;
                case 4: scrolldownButtons.sprite.setTextureRect({507, 0, 120, 124}); break;
                case 5: scrolldownButtons.sprite.setTextureRect({637, 2, 136, 116}); break;
                case 6: scrolldownButtons.sprite.setTextureRect({780, 3, 123, 114}); break;
                case 7: scrolldownButtons.sprite.setTextureRect({908, 14, 120, 80}); break;
                default: break;
            }
            if (!gs::verticalOrientation) xx -= scrolldownButtons.sprite.getGlobalBounds().width + 5*gs::scalex;
            else yy -= scrolldownButtons.sprite.getGlobalBounds().height + 5*gs::scaley;
        }
        if (!gs::verticalOrientation && xx < 0) scrolldownButtonsScaling = (scrolldownMenu.sprite.getGlobalBounds().left)/((scrolldownMenu.sprite.getGlobalBounds().left) - xx);
        else if (gs::verticalOrientation && yy < 0) scrolldownButtonsScaling = (scrolldownMenu.sprite.getGlobalBounds().top)/((scrolldownMenu.sprite.getGlobalBounds().top) - yy);
        else scrolldownButtonsScaling = 1.f;
        if (scrolldownButtonsScaling != 1.f) scrolldownButtons.setScale(1.f * scrolldownButtonsScaling);
    }
    void RoomUI::Draw(sf::RenderWindow* window)
    {
        if (!active) return;
        
        if (rm::drawDatePanel)
        {
            window->draw(dateSprite);
            window->draw(nekoMoodText);
            window->draw(dayText);
            window->draw(monthText);
        }
        if (rm::drawNeeds)
        {
            float xx = 5*gs::scale;
            for (int i = 0; i < 7; ++i)
            {
                needbaseSprite.setPosition(xx, needbaseSprite.getPosition().y);
                switch (i)
                {
                    case 0: needSprite.setTextureRect({0, 0, 33, 34}); needSprite.setOrigin(16, 17);
                        if (NStat::needHunger > 666) needbaseSprite.setColor(sf::Color(95, 151, 62));
                        else if (NStat::needHunger > 333) needbaseSprite.setColor(sf::Color(240, 240, 0));
                        else needbaseSprite.setColor(sf::Color(255, 61, 0)); break;
                    case 1: needSprite.setTextureRect({38, 4, 27, 29}); needSprite.setOrigin(13, 14);
                        if (NStat::needThirst > 666) needbaseSprite.setColor(sf::Color(95, 151, 62));
                        else if (NStat::needThirst > 333) needbaseSprite.setColor(sf::Color(240, 240, 0));
                        else needbaseSprite.setColor(sf::Color(255, 61, 0)); break;
                    case 2: needSprite.setTextureRect({72, 7, 34, 22}); needSprite.setOrigin(17, 11);
                        if (NStat::needCommunication > 666) needbaseSprite.setColor(sf::Color(95, 151, 62));
                        else if (NStat::needCommunication > 333) needbaseSprite.setColor(sf::Color(240, 240, 0));
                        else needbaseSprite.setColor(sf::Color(255, 61, 0)); break;
                    case 3: needSprite.setTextureRect({115, 6, 32, 27}); needSprite.setOrigin(16, 13);
                        if (NStat::needToilet > 666) needbaseSprite.setColor(sf::Color(95, 151, 62));
                        else if (NStat::needToilet > 333) needbaseSprite.setColor(sf::Color(240, 240, 0));
                        else needbaseSprite.setColor(sf::Color(255, 61, 0)); break;
                    case 4: needSprite.setTextureRect({155, 0, 30, 32}); needSprite.setOrigin(15, 16);
                        if (NStat::needHygiene > 666) needbaseSprite.setColor(sf::Color(95, 151, 62));
                        else if (NStat::needHygiene > 333) needbaseSprite.setColor(sf::Color(240, 240, 0));
                        else needbaseSprite.setColor(sf::Color(255, 61, 0)); break;
                    case 5: needSprite.setTextureRect({195, 5, 27, 25}); needSprite.setOrigin(13, 12);
                        if (NStat::needEnergy > 666) needbaseSprite.setColor(sf::Color(95, 151, 62));
                        else if (NStat::needEnergy > 333) needbaseSprite.setColor(sf::Color(240, 240, 0));
                        else needbaseSprite.setColor(sf::Color(255, 61, 0)); break;
                    case 6: needSprite.setTextureRect({226, 0, 16, 34}); needSprite.setOrigin(8, 16);
                        if (NStat::needWarmth > 666) needbaseSprite.setColor(sf::Color(95, 151, 62));
                        else if (NStat::needWarmth > 333) needbaseSprite.setColor(sf::Color(240, 240, 0));
                        else needbaseSprite.setColor(sf::Color(255, 61, 0)); break;
                    default: break;
                }
                if (needbaseSpriteTransparent) needbaseSprite.setColor({needbaseSprite.getColor().r, needbaseSprite.getColor().g, needbaseSprite.getColor().b, 100});
                needSprite.setPosition(xx + needbaseSprite.getGlobalBounds().width/2, needbaseSprite.getPosition().y - needbaseSprite.getGlobalBounds().height/2);
                window->draw(needbaseSprite); window->draw(needSprite);
                xx += needbaseSprite.getGlobalBounds().width + 5*gs::scale;
            }
        }
        if (rm::drawScrolldownMenu)
        {
            scrolldownMenu.Draw(window);
            if (rm::scrolldownMenuOpened)
            {
                float xx = gs::verticalOrientation ? scrolldownMenu.sprite.getPosition().x : scrolldownMenu.sprite.getGlobalBounds().left - 5*scrolldownButtonsScaling*gs::scalex;
                float yy = gs::verticalOrientation ? scrolldownMenu.sprite.getGlobalBounds().top - 5*scrolldownButtonsScaling*gs::scaley : scrolldownMenu.sprite.getPosition().y;
                for (int i = 0; i < 8; ++i)
                {
                    scrolldownButtons.index = i;
                    switch (i)
                    {
                        case 0: scrolldownButtons.sprite.setTextureRect({0, 0, 108, 125}); break;
                        case 1: scrolldownButtons.sprite.setTextureRect({124, 0, 97, 126}); break;
                        case 2: scrolldownButtons.sprite.setTextureRect({230, 5, 128, 123}); break;
                        case 3: scrolldownButtons.sprite.setTextureRect({368, 0, 130, 117}); break;
                        case 4: scrolldownButtons.sprite.setTextureRect({507, 0, 120, 124}); break;
                        case 5: scrolldownButtons.sprite.setTextureRect({637, 2, 136, 116}); break;
                        case 6: scrolldownButtons.sprite.setTextureRect({780, 3, 123, 114}); break;
                        case 7: scrolldownButtons.sprite.setTextureRect({908, 14, 120, 80}); break;
                        default: break;
                    }
                    scrolldownButtons.setPosition(xx, yy - 62*gs::scale); scrolldownButtons.Draw(window);
                    if (!gs::verticalOrientation) xx -= scrolldownButtons.sprite.getGlobalBounds().width + 5*scrolldownButtonsScaling*gs::scalex;
                    else yy -= scrolldownButtons.sprite.getGlobalBounds().height + 5*scrolldownButtonsScaling*gs::scaley;
                }
            }
        }
    }
    void RoomUI::RecieveMessage(MessageHolder& message)
    {
        if (!active && message.info == "RoomUI :: Show") Switch(true);
        else if (active && message.info == "RoomUI :: Close") Switch(false);
        else if (message.info == "RoomUI :: Switch") Switch(!active);
        else if (message.info == "RoomUI :: StatusTransparent") { needbaseSpriteTransparent = true;
            needSprite.setColor({needSprite.getColor().r, needSprite.getColor().g, needSprite.getColor().b, 100}); }
        else if (message.info == "RoomUI :: StatusVivid") { needbaseSpriteTransparent = false;
            needSprite.setColor({needSprite.getColor().r, needSprite.getColor().g, needSprite.getColor().b, 255}); }
    }
    void RoomUI::Switch(const bool& on)
    {
        if (on)
        {
            if (active) mode = appearing;
            else { active = true; mode = appearing; Resize(gs::width, gs::height); }
        }
        else if (!on) mode = disappearing;
    }
    void RoomUI::UpdateAlpha()
    {
        /*quitButton.setAlpha(alpha);
        background.setColor({background.getColor().r, background.getColor().g, background.getColor().b, alpha});*/
    }
}
