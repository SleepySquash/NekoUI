//
//  NekoEntity.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 17/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "NekoEntity.hpp"

namespace NekoUI
{
    void NekoEntity::Init()
    {
        occupyText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        occupyText.setOutlineColor(sf::Color::Black);
        occupyActionButton.setFont(L"Pacifica.ttf");
        occupyActionButton.setCharacterSize(32);
        occupyActionButton.valign = Valign::Top;
        
        
        sf::Texture* texture = ic::LoadTexture(L"Data/Images/dialogue1_n.png");
        if (texture)
        {
            dialogueSprite.setTexture(*texture);
            dialogueSprite.setOrigin(texture->getSize().x/2, texture->getSize().y);
            dialogue.setFont(*fc::GetFont(L"Pacifica.ttf"));
            dialogue.setFillColor(sf::Color::White);
        }
        
        texture = ic::LoadTexture(L"Data/Neko/Chibi/body.png");
        if ((spriteLoaded = texture))
        {
            sprite.setTexture(*texture);
            sprite.setOrigin(texture->getSize().x/2, texture->getSize().y - texture->getSize().y/15);
            relScale = (static_cast<float>(gs::relativeHeight)/static_cast<float>(texture->getSize().y)) * 0.46;
        }
        
        int i{ 0 }; do {
            x = rand() % Room::width;
            y = rand() % Room::height; ++i;
        } while (Room::mask && Room::mask->getPixel(floor(x/Room::roomScale), floor(y/Room::roomScale)).r && i < 100);
        if (i >= 100) { x = Room::width/2; y = Room::height/2; }
        
        newPoint = true; elapsedWaiting = (rand() % 400)/1000.f;
        if (elapsedWaiting <= 0.16) { elapsedWaiting = 0.f; doNewEventInstantly = true; }
        Update(sf::seconds(0)); if (occupiedAt == -1) doNewEventInstantly = false;
    }
    void NekoEntity::Destroy() { ic::DeleteImage(L"Data/Neko/Chibi/body.png"); ic::DeleteImage(L"Data/Images/dialogue1_n.png"); }
    void NekoEntity::Update(const sf::Time& elapsedTime)
    {
        if (beingActionedWith) return;
        if (!unlimitedDrawDialogue)
        {
            if (elapsedDialogue > 0) elapsedDialogue -= elapsedTime.asSeconds();
            else if (occupiedAt == -1 || beingOccupied)
            {
                if (!gs::ignoreDraw) gs::requestWindowRefresh = true;
                if (drawDialogue) { drawDialogue = false; elapsedDialogue = (rand() % 12000) / 1000.f; }
                else SetDialogue(GenerateRoomDialogue());
            }
        }
        
        if (elapsedWaiting > 0) elapsedWaiting -= elapsedTime.asSeconds();
        else
        {
            if (!newPoint)
            {
                // TODO: Pathfinding algorithm!
                if (x < movingTo_x - 30) { moveRight = true; moveLeft = false; }
                else if (x > movingTo_x + 30) { moveLeft = true; moveRight = false; }
                else { moveLeft = false; moveRight = false; }
                
                if (y < movingTo_y - 30) { moveDown = true; moveUp = false; }
                else if (y > movingTo_y + 30) { moveUp = true; moveDown = false; }
                else { moveDown = false; moveUp = false; }
            }
            
            if (Room::mask)
            {
                float xMove{ 0 }, yMove{ 0 };
                if (moveLeft || moveRight) xMove = xySpd * elapsedTime.asSeconds();
                if (moveUp || moveDown) yMove = xySpd * elapsedTime.asSeconds();
                
                if (moveLeft) {
                    if (!Room::mask->getPixel(floor((x - xMove)/Room::roomScale), floor(y/Room::roomScale)).r)
                        x -= xMove; else moveLeft = false; }
                if (moveRight) {
                    if (!Room::mask->getPixel(floor((x + xMove)/Room::roomScale), floor(y/Room::roomScale)).r)
                        x += xMove; else moveRight = false; }
                if (moveUp) {
                    if (!Room::mask->getPixel(floor(x/Room::roomScale), floor((y - yMove)/Room::roomScale)).r)
                        y -= yMove; else moveUp = false; }
                if (moveDown) {
                    if (!Room::mask->getPixel(floor(x/Room::roomScale), floor((y + yMove)/Room::roomScale)).r)
                        y += yMove; else moveDown = false; }
            }
            else
            {
                if (moveLeft) x -= xySpd * elapsedTime.asSeconds();
                if (moveRight) x += xySpd * elapsedTime.asSeconds();
                if (moveUp) y -= xySpd * elapsedTime.asSeconds();
                if (moveDown) y += xySpd * elapsedTime.asSeconds();
            }
            if (moveLeft || moveRight || moveUp || moveDown)
            {
                UpdateDepthPosition(); if (!gs::ignoreDraw) gs::requestWindowRefresh = true;
                sprite.setPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
                Player::neko.setChibiPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
                if (drawDialogue) { dialogueSprite.setPosition(sprite.getPosition().x, sprite.getGlobalBounds().top);
                    dialogue.setPosition(dialogueSprite.getPosition()); }
            }
            
            if (newPoint)
            {
                if (Room::mask && Room::mask->getPixel(floor(x/Room::roomScale), floor(y/Room::roomScale)).r && Room::mask->getPixel(floor(x/Room::roomScale) - 1, floor(y/Room::roomScale)).r && Room::mask->getPixel(floor(x/Room::roomScale) + 1, floor(y/Room::roomScale)).r && Room::mask->getPixel(floor(x/Room::roomScale), floor(y/Room::roomScale) - 1).r && Room::mask->getPixel(floor(x/Room::roomScale), floor(y/Room::roomScale) + 1).r) { x = Room::width/2; y = Room::height/2; }
                
                if (occupiedAt != -1) { occupiedAt = -1; beingOccupied = drawActionButton = false; }
                int occupyRandom = rand() % 9;
                if (!occupyRandom)
                {
                    do { occupiedAt = rand() % rm::activities.size(); } while (occupiedAt == previousOccupied);
                    previousOccupied = occupiedAt;
                    movingTo_x = rm::activities[occupiedAt].x * Room::roomScale;
                    movingTo_y = rm::activities[occupiedAt].y * Room::roomScale;
                    cout << "I'm occupied at: " << movingTo_x << " " << movingTo_y << endl;
                    if (doNewEventInstantly) { x = movingTo_x; y = movingTo_y; doNewEventInstantly = false; }
                }
                else
                {
                    int i{ 0 }; do {
                        movingTo_x = rand() % Room::width;
                        movingTo_y = rand() % Room::height; ++i;
                    } while (Room::mask && Room::mask->getPixel(floor(movingTo_x/Room::roomScale), floor(movingTo_y/Room::roomScale)).r && i < 100);
                }
                newPoint = false;
            }
            else if (!newPoint && !moveUp && !moveDown && !moveLeft && !moveRight)
            {
                newPoint = true;
                if (occupiedAt != -1 && (x > movingTo_x + 30 || x < movingTo_x - 30 || y > movingTo_y + 30 || y < movingTo_y - 30)) occupiedAt = -1;
                if (occupiedAt == -1) elapsedWaiting = waitingTime + (rand() % 1000)/1000.f;
                else
                {
                    cout << "yeah" << endl;
                    previousRandomDialogue = -1;
                    elapsedWaiting = rm::activities[occupiedAt].duration + (rand() % 5000)/5000.f;
                    occupyText.setString(rm::activities[occupiedAt].occupyString);
                    occupyText.setOrigin(occupyText.getLocalBounds().width/2, 0);
                    if (rm::activities[occupiedAt].actionString != L"") {
                        occupyActionButton.setString(rm::activities[occupiedAt].actionString);
                        drawActionButton = true; }
                    ResizeOccupied(); occupyText.setOrigin(occupyText.getLocalBounds().width/2, 0);
                    if (!gs::ignoreDraw) gs::requestWindowRefresh = beingOccupied = true;
                }
            }
        }
    }
    void NekoEntity::UpdateDepthPosition()
    {
        /*if (positionInArray > 0)
        {
            NekoEntity* prev = rl::neko[positionInArray - 1];
            if (prev->y > y) { rl::neko[positionInArray - 1] = this; rl::neko[positionInArray] = prev;
                --positionInArray; ++prev->positionInArray; }
        }
        if (positionInArray + 1 < rl::neko.size())
        {
            NekoEntity* next = rl::neko[positionInArray + 1];
            if (next->y < y) { rl::neko[positionInArray + 1] = this; rl::neko[positionInArray] = next;
                ++positionInArray; --next->positionInArray; }
        }*/
    }
    void NekoEntity::PollEvent(sf::Event& event)
    {
        if (drawActionButton && occupyActionButton.PollEvent(event)) sender->SendMessage({"NekoUI :: Show"});
    }
    void NekoEntity::Resize()
    {
        sprite.setScale(0.54 * relScale * Room::scale * gs::scale, 0.54 * relScale * Room::scale * gs::scale);
        sprite.setPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
        
        Player::neko.chibiScale = 0.21 * Room::scale * gs::scale;
        Player::neko.ResizeChibi();
        Player::neko.setChibiPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
        
        if (drawDialogue) { ResizeDialogue();
            dialogue.setOrigin(dialogue.getLocalBounds().width/2, dialogue.getLocalBounds().height + 44*gs::scale); }
        if (beingOccupied) { ResizeOccupied(); occupyText.setOrigin(occupyText.getLocalBounds().width/2, 0); }
    }
    void NekoEntity::UpdatePosition()
    {
        sprite.setPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
        Player::neko.setChibiPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
        if (drawDialogue)
        {
            dialogueSprite.setPosition(sprite.getPosition().x, sprite.getGlobalBounds().top);
            dialogue.setPosition(dialogueSprite.getPosition());
        }
        if (beingOccupied)
        {
            occupyText.setPosition(sprite.getPosition().x, sprite.getPosition().y + 6*gs::scale);
            if (drawActionButton)occupyActionButton.setPosition(sprite.getPosition().x, occupyText.getGlobalBounds().top + occupyText.getGlobalBounds().height - 10*Room::scale*gs::scale);
        }
    }
    void NekoEntity::ResizeDialogue()
    {
        dialogue.setCharacterSize(24 * gs::scale);
        dialogue.setScale(Room::scale, Room::scale);
        
        dialogueSprite.setScale(1.1*dialogue.getGlobalBounds().width/dialogueSprite.getLocalBounds().width, 1.5 * Room::scale * gs::scale);
        dialogueSprite.setPosition(sprite.getPosition().x, sprite.getGlobalBounds().top);
        dialogue.setPosition(dialogueSprite.getPosition());
    }
    void NekoEntity::ResizeOccupied()
    {
        occupyText.setCharacterSize(19*gs::scale);
        occupyText.setOutlineThickness(1*gs::scale);
        occupyText.setScale(Room::scale, Room::scale);
        occupyText.setPosition(sprite.getPosition().x, sprite.getPosition().y + 6*gs::scale);
        if (drawActionButton)
        {
            occupyActionButton.Resize(gs::width, gs::height);
            occupyActionButton.text.setScale(Room::scale, Room::scale);
            occupyActionButton.setPosition(sprite.getPosition().x, occupyText.getGlobalBounds().top + occupyText.getGlobalBounds().height - 10*Room::scale*gs::scale);
        }
    }
    void NekoEntity::Draw(sf::RenderWindow* window)
    {
        //if (spriteLoaded) window->draw(sprite);
        Player::neko.Draw(window, false);
        if (drawDialogue) { window->draw(dialogueSprite); window->draw(dialogue); }
        if (beingOccupied)
        {
            window->draw(occupyText);
            if (drawActionButton) occupyActionButton.Draw(window);
        }
    }
    void NekoEntity::SetDialogue(const std::wstring& dialog, bool unlimited)
    {
        drawDialogue = true; unlimitedDrawDialogue = unlimited;
        dialogue.setString(dialog);
        if (!unlimitedDrawDialogue) elapsedDialogue = 2.1f + 0.08 * dialogue.getString().getSize();// + (rand() % 2000) / 2000.f;
        ResizeDialogue();
        dialogue.setOrigin(dialogue.getLocalBounds().width/2, dialogue.getLocalBounds().height + 44*gs::scale);
    }
    std::wstring NekoEntity::GenerateRoomDialogue()
    {
        if (occupiedAt != -1)
        {
            if (rm::activities[occupiedAt].name == "Table")
            {
                int random; do { random = rand() % 3; } while (random == previousRandomDialogue);
                previousRandomDialogue = random;
                switch (random)
                {
                    case 0: return L"Усталя бегать! >3<"; break;
                    case 1: return L"*Залипаеть в стол~*"; break;
                    default: return L"Тяк спокойно и приятно~ :3"; break;
                }
            }
            else if (rm::activities[occupiedAt].name == "Computer")
            {
                int random; do { random = rand() % 3; } while (random == previousRandomDialogue);
                previousRandomDialogue = random;
                switch (random)
                {
                    case 0: return L"Х-хозяин, а что такое \"хентай\"?"; break;
                    case 1: return L"*Смотрит видашки с котиками*"; break;
                    default: return L"САСКЕ, ВЕРНИСЬ В КОНОХУ! т.т"; break;
                }
            }
            else if (rm::activities[occupiedAt].name == "KitchenTable1")
            {
                int random; do { random = rand() % 3; } while (random == previousRandomDialogue);
                previousRandomDialogue = random;
                switch (random)
                {
                    case 0: return L"Эти фруктики выглядят вкусно~"; break;
                    case 1: return L"А хозяин хозяйственный!~ :3"; break;
                    default: return L"Скоро я сама буду готовить! <3"; break;
                }
            }
        }
        int random; do { random = rand() % 4; } while (random == previousRandomDialogue);
        previousRandomDialogue = random;
        switch (random)
        {
            case 0: return L"Бегаю тудя-сюда! >3<"; break;
            case 1: return L"М-мяу!!~ с:"; break;
            case 2: return L"Здесь так приятно~ <3"; break;
            default: return L"Хозяин? Ти де, хозяин? >3<"; break;
        }
    }
}
