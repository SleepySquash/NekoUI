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
        nekoMoodText.setFont(*fc::GetFont(L"Noteworthy-Bold.ttf"));
        nekoMoodText.setString(L"Счастье"); nekoMoodText.setOutlineColor(sf::Color::Black);
        dayText.setFont(*fc::GetFont(L"Noteworthy-Bold.ttf"));
        monthText.setFont(*fc::GetFont(L"Noteworthy-Bold.ttf"));
        
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
#ifdef _MSC_VER
        tm* timeinfo = new tm(); localtime_s(timeinfo, &now_c);
#else
        tm* timeinfo = std::localtime(&now_c);
#endif
        dateDay = timeinfo->tm_mday; dayText.setString(std::to_string(dateDay));
        dateMonth = timeinfo->tm_mon; monthText.setString(GetMonthStringFromItsNumber(dateMonth));
        int hoursUntilNextDay = 24 - timeinfo->tm_hour;
        int minutesUntilNextDay = 60 - timeinfo->tm_min;
        int secsUntilNextDay = 60 - timeinfo->tm_sec;
        secondsUntilNextDay = secsUntilNextDay + 60*minutesUntilNextDay + 3600*hoursUntilNextDay;
        newDayIsComing = (secondsUntilNextDay < 60);
        entity->system->SendMessage({"Time", timeinfo});
        
        /*sf::Texture* texture = ic::LoadTexture(L"Data/Images/UI/sun.png");
        if ((spriteLoaded = texture)) dateSprite.setTexture(*texture);*/
        dateSprite.setColor(sf::Color(231, 152, 36));
        
        
        /*texture = ic::LoadTexture(L"Data/Images/UI/need.png");
        if ((spriteLoaded = texture)) {
            needbaseSprite.setTexture(*texture);
            needbaseSprite.setOrigin(0, texture->getSize().y); }
        texture = ic::LoadTexture(L"Data/Images/UI/needs.png");
        if ((spriteLoaded = texture)) needSprite.setTexture(*texture); */
        
        
        // scrolldownTexture_normal = ic::LoadTexture(L"Data/Images/UI/scrolldown rev1.png");
        // scrolldownTexture_close = ic::LoadTexture(L"Data/Images/UI/ScrolldownButton_exit.png");
        // scrolldownMenu.setTexture(scrolldownTexture_normal);
        scrolldownMenu.setScale(0.6f);
        scrolldownMenu.halign = Halign::Right;
        scrolldownMenu.valign = Valign::Bottom;
        
        scrolldownButtons.setTexture(L"Data/Images/UI/scrolldown.png");
        scrolldownButtons.sprite.setTextureRect({0, 0, 108, 125});
        scrolldownButtons.halign = Halign::Right;
        scrolldownButtons.valign = Valign::Center;
        
        
        attentionShape.setCornerPointCount(5);
        attentionShape.setFillColor(sf::Color::Red);
        attentionShape.setOutlineColor(sf::Color::White);
        attentionText.setFillColor(sf::Color::White);
        attentionText.setFont(*fc::GetFont(L"Noteworthy-Bold.ttf"));
        attentionText.setString(L"Нет еды!");
        
        
        Switch(true);
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
#ifdef _MSC_VER
                tm* timeinfo = new tm(); localtime_s(timeinfo, &now_c);
#else
                tm* timeinfo = std::localtime(&now_c);
#endif
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
            secondsUntilNextDay -= secondsPassed; dt = 0.f;
            newDayIsComing = (secondsUntilNextDay <= 30);
            if (rm::allowDTSaving) Player::SaveCurrentDT();
        }
        else
        {
            dt += secondsPassed;
            if (dt >= 30.f)
            {
                secondsUntilNextDay -= dt; dt = 0.f;
                newDayIsComing = (secondsUntilNextDay <= 30);
                if (rm::allowDTSaving)
                {
                    Player::SaveCurrentDT();
                    if (shouldDoBackupSaving)
                    {
                        Inventory::SaveInventory(); Inventory::SaveFridge(); Inventory::SaveWardrobe();
                        Player::SaveData(); Player::SavePersona(); cout << "Backup saving... Success!" << endl;
                        entity->system->SendMessage({"Apartment :: Save"});
                    }
                    shouldDoBackupSaving = !shouldDoBackupSaving;
                }
            }
        }
        
        if (!active) return;
        switch (mode)
        {
            case Mode::appearing:      //currentTime = appearTime;
                if (currentTime < appearTime) currentTime += elapsedTime.asSeconds();
                if (currentTime >= appearTime) { alpha = 255; currentTime = disappearTime; mode = Mode::existing; }
                else alpha = (sf::Uint8)(255 * (currentTime / appearTime));
                UpdateAlpha(); gs::requestWindowRefresh = true;
                break;
                
            case Mode::disappearing:     //currentTime = disappearTime;
                if (currentTime > 0) currentTime -= elapsedTime.asSeconds();
                if (currentTime <= 0) { active = false; CleanUp(); alpha = 0; currentTime = 0.f; mode = Mode::existing; }
                else alpha = (sf::Uint8)(255 * (currentTime / disappearTime));
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
        switch (scrolldown)
        {
            case Mode::appearing:      //currentTime = appearTime;
                if (scrollcurrentTime < scrollappearTime) scrollcurrentTime += elapsedTime.asSeconds();
                if (scrollcurrentTime >= scrollappearTime) { scrolldownButtonsOffset = 1;
                    scrollcurrentTime = scrolldisappearTime; scrolldown = Mode::existing; }
                else scrolldownButtonsOffset = (scrollcurrentTime / scrollappearTime); gs::requestWindowRefresh = true;
                break;
                
            case Mode::disappearing:     //currentTime = disappearTime;
                if (scrollcurrentTime > 0) scrollcurrentTime -= elapsedTime.asSeconds();
                if (scrollcurrentTime <= 0) { scrolldownButtonsOffset = 0; scrollcurrentTime = 0.f; scrolldown = Mode::existing; }
                else scrolldownButtonsOffset = (scrollcurrentTime / scrolldisappearTime); gs::requestWindowRefresh = true;
                break;
            default: break;
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
        if (!active || !rm::drawScrolldownMenu || (gs::forceIgnoreEvent)) return;
        
        if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::TouchBegan)
        {
            sf::Vector2i dot; if (event.type == sf::Event::MouseButtonPressed) dot = {event.mouseButton.x, event.mouseButton.y}; else dot = {event.touch.x, event.touch.y};
            if (!(ignoreEventMove = (rm::drawDatePanel && dateSprite.getGlobalBounds().contains(dot.x, dot.y))))
            {
                if (rm::scrolldownMenuOpened) ignoreEventMove = !availableToTouchZone.contains(dot);
                else ignoreEventMove = rm::scrolldownMenuBounds.contains(dot.x, dot.y);
            }
        } else if (event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::TouchEnded) { ignoreEventMove = false;
            if (rm::drawDatePanel && rm::canPressDatePanel && ((event.type == sf::Event::MouseButtonReleased && dateSprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) || (event.type == sf::Event::TouchEnded && dateSprite.getGlobalBounds().contains(event.touch.x, event.touch.y)))) entity->system->SendMessage({"CalendarUI :: Switch"});
        } else if (ignoreEventMove && (event.type == sf::Event::MouseMoved || event.type == sf::Event::TouchMoved)) event = sf::Event();
        
        if (scrolldownMenu.PollEvent(event) && rm::canPressScrolldownMenu)
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
            if (rm::scrolldownMenuOpened) scrolldown = Mode::appearing; else scrolldown = Mode::disappearing;
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
                            rm::requestCloseButton = true; scrolldown = Mode::disappearing;
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
                            rm::requestCloseButton = true; scrolldown = Mode::disappearing;
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
                            rm::requestCloseButton = true; scrolldown = Mode::disappearing;
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
                            rm::requestCloseButton = true; scrolldown = Mode::disappearing;
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
                            rm::requestCloseButton = true; scrolldown = Mode::disappearing;
                            rm::scrolldownMenuOpened = false; }
                        break;
                    case 6: scrolldownButtons.sprite.setTextureRect({780, 3, 123, 114});
                        scrolldownButtons.setPosition(xx, yy - 62*gs::scale); break;
                    case 7: scrolldownButtons.sprite.setTextureRect({908, 14, 120, 80});
                        scrolldownButtons.setPosition(xx, yy - 62*gs::scale); break;
                    default: break;
                }
                if (!rm::scrolldownMenuOpened) { anyPressed = true; break; }
                if (!gs::verticalOrientation) xx -= scrolldownButtons.sprite.getGlobalBounds().width + 5*scrolldownButtonsScaling*gs::scalex;
                else yy -= scrolldownButtons.sprite.getGlobalBounds().height + 5*scrolldownButtonsScaling*gs::scaley;
            }
            scrolldownButtons.eventPolled(event);
            if (!anyPressed) { rm::scrolldownMenuOpened = false; scrolldown = Mode::disappearing; }
        }
        if (ignoreEventMove && (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::TouchBegan)) event = sf::Event();
    }
    void RoomUI::Resize(const unsigned int& width, const unsigned int& height)
    {
        if (!active) return;
        UpdateAccordingToMode();
        
        needbaseSprite.setScale(0.75f*gs::scale, 0.75f*gs::scale);
        needSprite.setScale(0.75f*gs::scale, 0.75f*gs::scale);
        needbaseSprite.setPosition(0, height - 5*gs::scale);
        
        
        scrolldownMenu.Resize(width, height);
        scrolldownMenu.setPosition(width - 10*gs::scalex, height - 10*gs::scaley);
        scrolldownButtons.Resize(width, height);
        // sf::FloatRect rect = scrolldownMenu.sprite.getGlobalBounds();
        // rect.left -= 60*gs::scale; rect.top -= 60*gs::scale; rect.width += 60*gs::scale; rect.height += 60*gs::scale;
        rm::scrolldownMenuBounds = scrolldownMenu.sprite.getGlobalBounds(); // rect;
        
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
        
        
        attentionText.setCharacterSize(25*gs::scale);
        attentionText.setPosition(20*gs::scale, 0);
        attentionText.setOrigin(0, attentionText.getGlobalBounds().height);
        attentionStartY = needbaseSprite.getGlobalBounds().top - 5*gs::scale;
        attentionShape.setSize({10.f, needbaseSprite.getGlobalBounds().height * 1.1f});
        attentionShape.setOrigin(0, attentionShape.getSize().y);
        attentionShape.setPosition(5*gs::scale, 0);
        attentionShape.setCornersRadius(20*gs::scale);
    }
    void RoomUI::UpdateAccordingToMode()
    {
        switch (rm::location)
        {
            default:
                dateSprite.setScale(0.65f * gs::scale, 0.65f * gs::scale);
                dayText.setCharacterSize(70*gs::scale);
                dayText.setPosition(80*gs::scale, 20*gs::scale);
                dayText.setOrigin(dayText.getLocalBounds().width/2, 0);
                monthText.setCharacterSize(45*gs::scale);
                monthText.setPosition(80*gs::scale, 30*gs::scale + dayText.getGlobalBounds().height);
                monthText.setOrigin(monthText.getLocalBounds().width/2, 0);
                nekoMoodText.setCharacterSize(40*gs::scale);
                nekoMoodText.setPosition(dateSprite.getGlobalBounds().left + dateSprite.getGlobalBounds().width + 15*gs::scale, 6*gs::scale + gs::screenOffsetTop);
                nekoMoodText.setOutlineThickness(gs::scale);
                break;
        }
    }
    void RoomUI::Draw(sf::RenderWindow* window)
    {
        if (!active) return;
        
        if (rm::drawDatePanel)
        {
            switch (rm::location)
            {
                default:
                    window->draw(dateSprite);
                    window->draw(nekoMoodText);
                    window->draw(dayText);
                    window->draw(monthText);
                    break;
            }
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
                        if (NekoS::needHunger > 666) needbaseSprite.setColor(sf::Color(95, 151, 62));
                        else if (NekoS::needHunger > 333) needbaseSprite.setColor(sf::Color(240, 240, 0));
                        else needbaseSprite.setColor(sf::Color(255, 61, 0)); break;
                    case 1: needSprite.setTextureRect({38, 4, 27, 29}); needSprite.setOrigin(13, 14);
                        if (NekoS::needThirst > 666) needbaseSprite.setColor(sf::Color(95, 151, 62));
                        else if (NekoS::needThirst > 333) needbaseSprite.setColor(sf::Color(240, 240, 0));
                        else needbaseSprite.setColor(sf::Color(255, 61, 0)); break;
                    case 2: needSprite.setTextureRect({72, 7, 34, 22}); needSprite.setOrigin(17, 11);
                        if (NekoS::needCommunication > 666) needbaseSprite.setColor(sf::Color(95, 151, 62));
                        else if (NekoS::needCommunication > 333) needbaseSprite.setColor(sf::Color(240, 240, 0));
                        else needbaseSprite.setColor(sf::Color(255, 61, 0)); break;
                    case 3: needSprite.setTextureRect({115, 6, 32, 27}); needSprite.setOrigin(16, 13);
                        if (NekoS::needToilet > 666) needbaseSprite.setColor(sf::Color(95, 151, 62));
                        else if (NekoS::needToilet > 333) needbaseSprite.setColor(sf::Color(240, 240, 0));
                        else needbaseSprite.setColor(sf::Color(255, 61, 0)); break;
                    case 4: needSprite.setTextureRect({155, 0, 30, 32}); needSprite.setOrigin(15, 16);
                        if (NekoS::needHygiene > 666) needbaseSprite.setColor(sf::Color(95, 151, 62));
                        else if (NekoS::needHygiene > 333) needbaseSprite.setColor(sf::Color(240, 240, 0));
                        else needbaseSprite.setColor(sf::Color(255, 61, 0)); break;
                    case 5: needSprite.setTextureRect({195, 5, 27, 25}); needSprite.setOrigin(13, 12);
                        if (NekoS::needEnergy > 666) needbaseSprite.setColor(sf::Color(95, 151, 62));
                        else if (NekoS::needEnergy > 333) needbaseSprite.setColor(sf::Color(240, 240, 0));
                        else needbaseSprite.setColor(sf::Color(255, 61, 0)); break;
                    case 6: needSprite.setTextureRect({226, 0, 16, 34}); needSprite.setOrigin(8, 16);
                        if (NekoS::needWarmth > 666) needbaseSprite.setColor(sf::Color(95, 151, 62));
                        else if (NekoS::needWarmth > 333) needbaseSprite.setColor(sf::Color(240, 240, 0));
                        else needbaseSprite.setColor(sf::Color(255, 61, 0)); break;
                    default: break;
                }
                if (needbaseSpriteTransparent) needbaseSprite.setColor({needbaseSprite.getColor().r, needbaseSprite.getColor().g, needbaseSprite.getColor().b, 100});
                needSprite.setPosition(xx + needbaseSprite.getGlobalBounds().width/2, needbaseSprite.getPosition().y - needbaseSprite.getGlobalBounds().height/2);
                window->draw(needbaseSprite); window->draw(needSprite);
                xx += needbaseSprite.getGlobalBounds().width + 5*gs::scale;
            }
            
            if (!needbaseSpriteTransparent && (Player::noFood || Player::noDrink))
            {
                unsigned int yy = attentionStartY;
                if (Player::noDrink)
                {
                    attentionText.setPosition(attentionText.getPosition().x, yy - 9*gs::scale);
                    attentionText.setString(L"Нет воды!");
                    attentionShape.setSize({attentionText.getGlobalBounds().width + 30*gs::scale, attentionShape.getSize().y});
                    attentionShape.setPosition(attentionShape.getPosition().x, yy);
                    
                    window->draw(attentionShape);
                    window->draw(attentionText);
                    yy -= attentionShape.getSize().y + 5*gs::scale;
                }
                if (Player::noFood)
                {
                    attentionText.setPosition(attentionText.getPosition().x, yy - 9*gs::scale);
                    attentionText.setString(L"Нет еды!");
                    attentionShape.setSize({attentionText.getGlobalBounds().width + 30*gs::scale, attentionShape.getSize().y});
                    attentionShape.setPosition(attentionShape.getPosition().x, yy);
                    
                    window->draw(attentionShape);
                    window->draw(attentionText);
                    yy -= attentionShape.getSize().y + 5*gs::scale;
                }
            }
        }
        if (rm::drawScrolldownMenu)
        {
            if (rm::scrolldownMenuOpened || scrolldown != Mode::existing)
            {
                float xx = gs::verticalOrientation ? scrolldownMenu.sprite.getPosition().x : scrolldownMenu.sprite.getPosition().x - scrolldownMenu.sprite.getGlobalBounds().width/2 - (scrolldownMenu.sprite.getGlobalBounds().width/2 + 5*scrolldownButtonsScaling*gs::scalex) * scrolldownButtonsOffset;
                float yy = gs::verticalOrientation ? scrolldownMenu.sprite.getPosition().y - scrolldownMenu.sprite.getGlobalBounds().height/2 - (scrolldownMenu.sprite.getGlobalBounds().height/2 + 5*scrolldownButtonsScaling*gs::scaley) * scrolldownButtonsOffset : scrolldownMenu.sprite.getPosition().y;
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
                    scrolldownButtons.setPosition(xx, yy - 62*gs::scale); scrolldownButtons.draw(window);
                    if (!gs::verticalOrientation) xx -= (scrolldownButtons.sprite.getGlobalBounds().width + 5*scrolldownButtonsScaling*gs::scalex) * scrolldownButtonsOffset;
                    else yy -= (scrolldownButtons.sprite.getGlobalBounds().height + 5*scrolldownButtonsScaling*gs::scaley) * scrolldownButtonsOffset;
                }
            }
            scrolldownMenu.draw(window);
        }
    }
    void RoomUI::ReceiveMessage(MessageHolder& message)
    {
        if (!active && message.info == "RoomUI :: Show") Switch(true);
        else if (active && message.info == "RoomUI :: Close") Switch(false);
        else if (message.info == "RoomUI :: Switch") Switch(!active);
        else if (message.info == "RoomUI :: StatusTransparent") { needbaseSpriteTransparent = true;
            needSprite.setColor({needSprite.getColor().r, needSprite.getColor().g, needSprite.getColor().b, 100}); }
        else if (message.info == "RoomUI :: StatusVivid") { needbaseSpriteTransparent = false;
            needSprite.setColor({needSprite.getColor().r, needSprite.getColor().g, needSprite.getColor().b, 255}); }
        else if (message.info == "RoomUI :: Update")
        {
            switch (rm::location)
            {
                case rm::Location::Apartment:
                    nekoMoodText.setString(L"Счастье");
                    break;
                case rm::Location::Grocery: case rm::Location::Shopkeeper:
                    nekoMoodText.setString(std::to_string(NekoS::money) + " P");
                    break;
                default: break;
            }
            if (lastLocation != rm::location) { lastLocation = rm::location; UpdateAccordingToMode(); }
        }
    }
    void RoomUI::Switch(const bool& on)
    {
        if (on)
        {
            if (active) mode = Mode::appearing;
            else
            {
                sf::Texture* texture = ic::LoadTexture(L"Data/Images/UI/sun.png");
                if ((spriteLoaded = texture)) dateSprite.setTexture(*texture);
                
                texture = ic::LoadTexture(L"Data/Images/UI/need.png");
                if ((spriteLoaded = texture)) {
                    needbaseSprite.setTexture(*texture);
                    needbaseSprite.setOrigin(0, texture->getSize().y); }
                texture = ic::LoadTexture(L"Data/Images/UI/needs.png");
                if ((spriteLoaded = texture)) needSprite.setTexture(*texture);
                
                scrolldownTexture_normal = ic::LoadTexture(L"Data/Images/UI/scrolldown rev1.png");
                scrolldownTexture_close = ic::LoadTexture(L"Data/Images/UI/ScrolldownButton_exit.png");
                scrolldownMenu.setTexture(scrolldownTexture_normal);
                active = true; mode = Mode::appearing; Resize(gs::width, gs::height);
            }
        }
        else if (!on) mode = Mode::disappearing;
    }
    void RoomUI::UpdateAlpha()
    {
        /*quitButton.setAlpha(alpha);
        background.setColor({background.getColor().r, background.getColor().g, background.getColor().b, alpha});*/
    }
}
