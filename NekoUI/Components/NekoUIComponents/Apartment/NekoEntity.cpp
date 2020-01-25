//
//  NekoEntity.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 17/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "NekoEntity.hpp"

namespace NekoUI
{
    void NekoEntity::Init()
    {
        movable = true; type = Type::Neko;
        occupyText.setFont(*fc::GetFont(L"Noteworthy-Bold.ttf"));
        occupyText.setOutlineColor(sf::Color(99, 70, 70));
        occupyActionButton.setFont(L"Noteworthy-Bold.ttf");
        occupyActionButton.setCharacterSize(32);
        occupyActionButton.valign = Valign::Top;
        occupyActionButton.thickness = 2.f;
        occupyActionButton.onormalColor = sf::Color(99, 70, 70);
        occupyActionButton.updateColor();
        
        
        sf::Texture* texture = ic::LoadTexture(L"Data/Images/UI/dialogue1_n.png");
        if (texture)
        {
            dialogueSprite.setTexture(*texture);
            dialogueSprite.setOrigin(texture->getSize().x/2, texture->getSize().y);
            dialogue.setFont(*fc::GetFont(L"Noteworthy-Bold.ttf"));
            dialogue.setFillColor(sf::Color::White);
        }
        
        texture = ic::LoadTexture(L"Data/Neko/Chibi/body.png");
        if ((spriteLoaded = texture))
        {
            sprite.setTexture(*texture);
            sprite.setOrigin(texture->getSize().x/2, texture->getSize().y - texture->getSize().y/15);
            relScale = (static_cast<float>(gs::relativeHeight)/static_cast<float>(texture->getSize().y)) * 0.46;
            height = texture->getSize().y * 0.54 * relScale;
        }
        
        texture = ic::LoadTexture(L"Data/Neko/Chibi/shadow.png");
        if (texture)
        {
            shadow.setTexture(*texture);
            shadowOffsetR = { texture->getSize().x/2.f + texture->getSize().x/20, texture->getSize().y - texture->getSize().y/2.4f };
            shadowOffsetL = { texture->getSize().x/2.f - texture->getSize().x/20, texture->getSize().y - texture->getSize().y/2.4f };
            shadow.setOrigin(shadowOffsetR);
        }
        
        int i{ 0 }; do {
            x = rand() % Room::width;
            y = rand() % Room::height; ++i;
        } while (Room::mask && Room::mask->getPixel(floor(x/Room::roomScale), floor(y/Room::roomScale)).r && i < 100);
        if (i >= 100) { x = Room::width/2; y = Room::height/2; }
        
        elapsedWaiting = (rand() % 400)/1000.f;
    }
    void NekoEntity::Destroy() { ic::DeleteImage(L"Data/Neko/Chibi/body.png"); ic::DeleteImage(L"Data/Neko/Chibi/shadow.png"); ic::DeleteImage(L"Data/Images/UI/dialogue1_n.png"); }
    void NekoEntity::Update(const sf::Time& elapsedTime)
    {
        if (gs::isPause) return;
        NekoS::needHunger -= elapsedTime.asSeconds() * NekoS::hungerInSecond;
        NekoS::needThirst -= elapsedTime.asSeconds() * NekoS::thirstInSecond;
        NekoS::needCommunication -= elapsedTime.asSeconds() * NekoS::communicationInSecond;
        NekoS::needHygiene -= elapsedTime.asSeconds() * NekoS::hygieneInSecond;
        NekoS::needToilet -= elapsedTime.asSeconds() * NekoS::toiletInSecond;
        NekoS::needEnergy -= elapsedTime.asSeconds() * NekoS::energyInSecond;
        if (NekoS::needHunger < 0) NekoS::needHunger = 0;
        if (NekoS::needThirst < 0) NekoS::needThirst = 0;
        if (NekoS::needCommunication < 0) NekoS::needCommunication = 0;
        if (NekoS::needHygiene < 0) NekoS::needHygiene = 0;
        if (NekoS::needToilet < 0) NekoS::needToilet = 0;
        if (NekoS::needEnergy < 0) NekoS::needEnergy = 0;
        
        if (blinking)
        {
            if (elapsedBlinking > 0.f) elapsedBlinking -= elapsedTime.asSeconds();
            else
            {
                if (!itisblink)
                {
                    blinkEmotion = NekoP::eyesEmotion;
                    NekoP::eyesEmotion = NekoS::EyesEmotion::Closed;
                    if (NekoP::eyesEmotion != Player::eyesEmotion) gs::requestWindowRefresh = true;
                    Player::UpdateNekoEmotion();
                    elapsedBlinking = closedBlinkDuration + (rand() % 4)/100.f;
                }
                else
                {
                    if (NekoP::eyesEmotion == NekoS::EyesEmotion::Closed)
                        NekoP::eyesEmotion = blinkEmotion;
                    if (NekoP::eyesEmotion != Player::eyesEmotion) gs::requestWindowRefresh = true;
                    Player::UpdateNekoEmotion();
                    elapsedBlinking = blinkingFrequency + (rand() % 6)/10.f;
                    if (allowRandomBlink)
                    {
                        int chance = rand() % 100;
                        if (chance < 20) elapsedBlinking = 0.1 + (rand() % 6)/10.f;
                    }
                }
                itisblink = !itisblink;
            }
        }
        
        if (beingActionedWith && !ignoreBeingActionedWith) return;
        if (!unlimitedDrawDialogue)
        {
            if (elapsedDialogue > 0) elapsedDialogue -= elapsedTime.asSeconds();
            else
            {
                if (drawDialogue) { drawDialogue = false; if (!gs::ignoreDraw) gs::requestWindowRefresh = true;
                    elapsedDialogue = 3.f + (rand() % 9000) / 1000.f; }
                else if (!activity || activity->doingTheActivity)
                {
                    if (beingActionedWith) sender->SendMessage({"NekoUI :: DDialogue", GenerateRoomDialogue()});
                    else SetDialogue(GenerateRoomDialogue());
                }
                else elapsedDialogue = 1.f;
            }
        }
        
        if (activity)
        {
            activity->Update(elapsedTime);
            if (moveTo)
            {
                // TODO: Pathfinding algorithm!
                if (x < movingTo_x - 20) { moveRight = true; moveLeft = false; SetChibiFacingDirection(true); }
                else if (x > movingTo_x + 20) { moveLeft = true; moveRight = false; SetChibiFacingDirection(false); }
                else { moveLeft = false; moveRight = false; }
                
                if (y < movingTo_y - 20) { moveDown = true; moveUp = false; }
                else if (y > movingTo_y + 20) { moveUp = true; moveDown = false; }
                else { moveDown = false; moveUp = false; }
                
                if (Room::mask)
                {
                    float xMove{ 0 }, yMove{ 0 };
                    if (moveLeft || moveRight) xMove = xySpd * elapsedTime.asSeconds();
                    if (moveUp || moveDown) yMove = xySpd * elapsedTime.asSeconds();
                    
                    if (moveLeft) { if (!Room::Collision(x - xMove, y)) x -= xMove; else moveLeft = false; }
                    if (moveRight) { if (!Room::Collision(x + xMove, y)) x += xMove; else moveRight = false; }
                    if (moveUp) { if (!Room::Collision(x, y - yMove)) y -= yMove; else moveUp = false; }
                    if (moveDown) { if (!Room::Collision(x, y + yMove)) y += yMove; else moveDown = false; }
                }
                else
                {
                    if (moveLeft) x -= xySpd * elapsedTime.asSeconds();
                    if (moveRight) x += xySpd * elapsedTime.asSeconds();
                    if (moveUp) y -= xySpd * elapsedTime.asSeconds();
                    if (moveDown) y += xySpd * elapsedTime.asSeconds();
                }
                
                if ((moving = (moveLeft || moveRight || moveUp || moveDown)))
                {
                    UpdateDepthPosition(); moving = true; if (!gs::ignoreDraw) gs::requestWindowRefresh = true;
                    sprite.setPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
                    if (inHands) inHandsSprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - 465*sprite.getScale().y);
                    Player::neko.setChibiPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
                    shadow.setPosition(Player::neko.body.chibi.getPosition());
                    if (drawDialogue) { dialogueSprite.setPosition(sprite.getPosition().x, sprite.getGlobalBounds().top);
                        dialogue.setPosition(dialogueSprite.getPosition().x, dialogueSprite.getPosition().y - dialogueSprite.getGlobalBounds().height/2 - 4*dialogueSprite.getScale().y); }
                    if (beingOccupied)
                    {
                        occupyText.setPosition(sprite.getPosition().x, sprite.getPosition().y + 6*gs::scale);
                        if (drawActionButton) occupyActionButton.setPosition(sprite.getPosition().x, occupyText.getGlobalBounds().top + occupyText.getGlobalBounds().height - 10*Room::scale*gs::scale);
                    }
                    SetOnScreen();
                }
                
                if (!moveUp && !moveDown && !moveLeft && !moveRight)
                {
                    moveTo = moving = false;
                    if (randomMoving) elapsedWaiting = waitingTime + (rand() % 1000)/1000.f;
                    else
                    {
                        if ((x > movingTo_x + 20 || x < movingTo_x - 20 || y > movingTo_y + 20 || y < movingTo_y - 20))
                        {
                            // activity->Abort();
                            // Но т.к. пока что нет поиска пути:
                            x = movingTo_x; y = movingTo_y; UpdatePosition(); UpdateDepthContinuously();
                            InsertActivity(adb::activities["TeleportationCuzStuck"]);
                            SetDialogue(L"Телепортация! >3<"); elapsedDialogue = 0.7f;
                        }
                        else activity->NextTask();
                    }
                }
            }
            else if (randomMoving)
            {
                if (elapsedWaiting > 0.f) elapsedWaiting -= elapsedTime.asSeconds();
                else
                {
                    if (Room::mask && Room::mask->getPixel(floor(x/Room::roomScale), floor(y/Room::roomScale)).r && Room::mask->getPixel(floor(x/Room::roomScale) - 1, floor(y/Room::roomScale)).r && Room::mask->getPixel(floor(x/Room::roomScale) + 1, floor(y/Room::roomScale)).r && Room::mask->getPixel(floor(x/Room::roomScale), floor(y/Room::roomScale) - 1).r && Room::mask->getPixel(floor(x/Room::roomScale), floor(y/Room::roomScale) + 1).r)
                    {
                        x = Room::width/2; y = Room::height/2; UpdatePosition(); UpdateDepthContinuously();
                        InsertActivity(adb::activities["TeleportationCuzStuck"]);
                        SetDialogue(L"Телепортация! >3<"); elapsedDialogue = 0.7f;
                    }
                    
                    int i{ 0 }; do { moveTo = true;
                        movingTo_x = rand() % Room::width;
                        movingTo_y = rand() % Room::height; ++i;
                    } while (Room::mask && Room::mask->getPixel(floor(movingTo_x/Room::roomScale), floor(movingTo_y/Room::roomScale)).r && i < 100);
                }
            }
        }
        else
        {
            randomMoving = moveTo = false;
            if (itsTheDroppedFood) itsTheDroppedFood = droppedInHands;
            if (itsTheDroppedFood)
            {
                tasks.insert(tasks.begin(), adb::activities["ReturnToFood"]);
                adb::activities["ReturnToFood"]->roomEntity = droppedInHands;
                itsTheDroppedFood = false;
            }
            
            if (tasks.empty())
            {
                if (NekoS::needHunger <= 260) { sender->SendMessage({"Apartment :: HungerActivity"}); if (activity) return; }
                if (NekoS::needThirst <= 260) { sender->SendMessage({"Apartment :: ThirstActivity"}); if (activity) return; }
                if (NekoS::needEnergy <= 260) { activity = adb::activities["GoSleep"]; }
                else if (NekoS::needHygiene <= 260) { activity = adb::activities["GoTakeABath"]; }
                // HungerActivity или ThirstActivity - ломают игру, когда в холодильнике ничего нет?
                // ИТОГ: ломал игру MovingTo в Init()'е, когда он сбрасывал movingObject в L""?
                
                if (!activity)
                {
                    int occupiedAt = -1; do { occupiedAt = rand() % adb::availableActivities.size(); } while (occupiedAt == previousOccupied);
                    auto occupiedIt = adb::availableActivities.begin(); std::advance(occupiedIt, occupiedAt);
                    previousOccupied = (adb::availableActivities.size() == 1) ? -1 : occupiedAt;
                    activity = (*occupiedIt);
                }
            }
            else
            {
                activity = tasks.front();
                tasks.pop_front(); // previousOccupied = -1;
            }
            cout << "I'm occupied at: " << activity->name << endl;
            ExecuteCurrentActivity();
        }
    }
    void NekoEntity::PollEvent(sf::Event& event)
    {
        if (drawActionButton && occupyActionButton.PollEvent(event))
        {
            if (drawDialogue) sender->SendMessage({"NekoUI :: DDialogue", dialogue.getString()});
            sender->SendMessage({"NekoUI :: Show"});
        }
    }
    void NekoEntity::Resize()
    {
        sprite.setScale(0.54 * relScale * Room::scale * gs::scale, 0.54 * relScale * Room::scale * gs::scale);
        sprite.setPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
        
        if (inHands) { inHandsSprite.setScale(inHandsScale * Room::scale * gs::scale, inHandsScale * Room::scale * gs::scale);
            inHandsSprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - 465*sprite.getScale().y); }
        
        Player::neko.chibiScale = 0.21 * Room::scale * gs::scale;
        Player::neko.ResizeChibi();
        Player::neko.setChibiPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
        
        shadow.setScale(Player::neko.chibiScale * 1.6f, Player::neko.chibiScale * 1.6f);
        shadow.setPosition(Player::neko.body.chibi.getPosition());
        
        if (drawDialogue) { ResizeDialogue();
            dialogue.setOrigin(dialogue.getLocalBounds().width/2, dialogue.getLocalBounds().height/2); }
        if (beingOccupied) { ResizeOccupied(); occupyText.setOrigin(occupyText.getLocalBounds().width/2, 0); }
        SetOnScreen();
    }
    void NekoEntity::Draw(sf::RenderWindow* window)
    {
        // if (spriteLoaded) window->draw(sprite);
        if (drawShadow) window->draw(shadow);
        Player::neko.Draw(window, false);
        if (inHands) window->draw(inHandsSprite);
        if (drawDialogue) { window->draw(dialogueSprite); window->draw(dialogue); }
        if (beingOccupied)
        {
            window->draw(occupyText);
            if (drawActionButton) occupyActionButton.Draw(window);
        }
    }
    void NekoEntity::ReceiveMessage(MessageHolder& message)
    {
        if (message.info == "Apartment :: RequestPosition" || message.info == "Apartment :: RequestFoodFromFridge" || message.info == "Apartment :: RequestDrinkFromFridge") sender->SendMessage(message);
        else if (message.info == "Activity :: Update" && activity)
        {
            cout << "Activity :: Update (" << (int)((*(activity->task))->type) << ")" << endl;
            if ((*(activity->task))->facingSet) SetChibiFacingDirection((*(activity->task))->facing);
            if ((*(activity->task))->occupyString.length())
            {
                occupyText.setString((*(activity->task))->occupyString);
                occupyText.setOrigin(occupyText.getLocalBounds().width/2, 0);
                if ((drawActionButton = (*(activity->task))->actionString.length())) occupyActionButton.setString((*(activity->task))->actionString);
                ResizeOccupied(); occupyText.setOrigin(occupyText.getLocalBounds().width/2, 0);
                if (!gs::ignoreDraw) gs::requestWindowRefresh = true; beingOccupied = true;
            }
            else beingOccupied = drawActionButton = false;
            moveTo = randomMoving = false;
            
            if ((*(activity->task))->type == ActivityTask::TaskType::randommoving) randomMoving = true;
            else if ((*(activity->task))->type == ActivityTask::TaskType::moving)
            {
                moveTo = true;
                movingTo_x = (*(activity->task))->x;
                movingTo_y = (*(activity->task))->y;
                movingToEntity = nullptr;
                if (x <= movingTo_x + 20 && x >= movingTo_x - 20 && y <= movingTo_y + 20 && y >= movingTo_y - 20) activity->NextTask();
            }
            else if ((*(activity->task))->type == ActivityTask::TaskType::movetoentity)
            {
                if (!(*(activity->task))->moveToEntity) { activity->Abort(); SetDialogue(L"Куда пропало..?"); }
                else
                {
                    moveTo = true;
                    movingTo_x = (*(activity->task))->x;
                    movingTo_y = (*(activity->task))->y;
                    movingToEntity = (*(activity->task))->moveToEntity;
                    if (x <= movingTo_x + 20 && x >= movingTo_x - 20 && y <= movingTo_y + 20 && y >= movingTo_y - 20) activity->NextTask();
                }
            }
            else if ((*(activity->task))->type == ActivityTask::TaskType::pickupitem)
            {
                sender->SendMessage({"Apartment :: PutInHands", (*(activity->task))->moveToEntity });
                if (inHands) activity->NextTask(); else activity->Abort();
            }
            else if ((*(activity->task))->type == ActivityTask::TaskType::consumefromhands)
            {
                if (inHands)
                {
                    activity->NextTask();
                    if (inHands->type == ItemType::Food)
                    {
                        InsertActivity(adb::activities["Eating"]);
                        SetDialogue(L"*Ом-ном-ном*");
                    }
                    else if (inHands->type == ItemType::Drink)
                    {
                        InsertActivity(adb::activities["Drinking"]);
                        SetDialogue(L"*Бульк-бульк*");
                    }
                }
                else activity->Abort();
            }
            else if ((*(activity->task))->type == ActivityTask::TaskType::insertactivity)
            {
                auto it = adb::activities.find((*(activity->task))->activityToInsert);
                if (it != adb::activities.end()) { activity->NextTask(); InsertActivity((*it).second); } else activity->Abort();
            }
            else if ((*(activity->task))->type == ActivityTask::TaskType::hidedressed)
            {
                int type = base::atoi(message.additional);
                (*(activity->task))->isSuccess = ShowOrHidePersonaCloth(type, true);
                activity->NextTask();
            }
            else if ((*(activity->task))->type == ActivityTask::TaskType::dresshidden)
            {
                int type = base::atoi(message.additional);
                (*(activity->task))->isSuccess = ShowOrHidePersonaCloth(type, false);
                activity->NextTask();
            }
            else if ((*(activity->task))->type == ActivityTask::TaskType::bathing) sender->SendMessage({"Apartment :: GimmeBathInfo"});
        }
        else if ((message.info == "Activity :: Abort" || message.info == "Activity :: Done") && activity)
        {
            if (message.info == "Activity :: Done")
            {
                abortedActivity = nullptr; abortedCount = 0;
                if (activity->name == "Eating" && inHands)
                {
                    ConsumeItem(inHands);
                    if (activity->parameter1) SetDialogue(L"Пасибя, ето было вкусня! :3");
                    else SetDialogue(L"Йа покушаля! c:");
                }
                else if (activity->name == "Drinking" && inHands)
                {
                    ConsumeItem(inHands);
                    if (activity->parameter1) SetDialogue(L"Пасибя, ето было освежающе! :3");
                    else SetDialogue(L"Йа попила! c:");
                }
                else if (activity->name == "ReturnToFood") droppedInHands = nullptr;
                else if (activity->name == "Sleeping")
                {
                    // NekoS::needEnergy += 300.f;
                    if (NekoS::needEnergy > NekoS::maxNeed) NekoS::needEnergy = NekoS::maxNeed;
                    else if (NekoS::needEnergy < 0) NekoS::needEnergy = 0;
                    InsertActivity(adb::activities["ComeToSensesAfterSleep"], false);
                    SetDialogue(L"*Зевает*");
                    return;
                }
                else if (activity->name == "Bathing")
                {
                    NekoS::needHygiene += 300.f;
                    if (NekoS::needHygiene > NekoS::maxNeed) NekoS::needHygiene = NekoS::maxNeed;
                    else if (NekoS::needHygiene < 0) NekoS::needHygiene = 0;
                    SetDialogue(L"Йа чистенькая!~ :з");
                }
            }
            else
            {
                if (abortedActivity == activity) ++abortedCount; else { abortedCount = 0; abortedActivity = activity; }
                if (inHands)
                {
                    if (activity->name == "Eating") SetDialogue(L"Уааа, йа не доела! >.<");
                    else if (activity->name == "Drinking") SetDialogue(L"Уааа, йа не допила! >.<");
                    itsTheDroppedFood = true; RemoveItemFromHands(true, true);
                }
                if (activity->name == "ReturnToFood") droppedInHands = nullptr;
                else if (activity->name == "Sleeping")
                {
                    if (NekoS::needEnergy > NekoS::maxNeed) NekoS::needEnergy = NekoS::maxNeed;
                    else if (NekoS::needEnergy < 0) NekoS::needEnergy = 0;
                    SetDialogue(L"А-а..? *зевает* т.т");
                }
                waitTilEntityStopsBeingMoved = false;
            }
            cout << message.info << endl; FinishCurrentActivity(); activity = nullptr;
        }
        else if (message.info == "Activity :: ClothCheck" && activity)
        {
            int type = base::atoi(message.additional);
            if (ShowOrHidePersonaCloth(type, true, true)) activity->SendMessage({"ClothCheck :: Yes"});
            else activity->SendMessage({"ClothCheck :: No"});
        }
        else if (message.info == "MB :: SpawnItem" && itsTheDroppedFood) droppedInHands = reinterpret_cast<RoomEntity*>(message.address);
        else if (message.info == "Apartment :: FoodFridgeSpawned")
        {
            iKnowThereIsNoFoodInTheFridge = !message.address;
            if (iKnowThereIsNoFoodInTheFridge) SetDialogue(L"Туть нет еды! т.т");
            if (activity) activity->ReceiveMessage(message);
        }
        else if (message.info == "Apartment :: DrinkFridgeSpawned")
        {
            iKnowThereIsNoDrinkInTheFridge = !message.address;
            if (iKnowThereIsNoFoodInTheFridge) SetDialogue(L"Туть нет воды! т.т");
            if (activity) activity->ReceiveMessage(message);
        }
        else if (message.info == "Apartment :: BedInfo" && activity && activity->name == "Sleeping")
        {
            RoomEntity* entity = reinterpret_cast<RoomEntity*>(message.address);
            if (entity)
            {
                x = entity->x;
                y = entity->y + (entity->sprite.getLocalBounds().height/2.0f)*entity->relScale;
                UpdatePosition(); UpdateDepthContinuously();
            }
        }
        else if (message.info == "Apartment :: BathInfo" && activity && activity->name == "Bathing")
        {
            RoomEntity* entity = reinterpret_cast<RoomEntity*>(message.address);
            if (entity)
            {
                x = entity->x - (entity->sprite.getLocalBounds().width/2)*entity->relScale;
                y = entity->y + 1;
                UpdatePosition(); UpdateDepthContinuously();
            }
        }
        else if (activity) activity->ReceiveMessage(message);
    }
    void NekoEntity::SendMessage(MessageHolder message) { ReceiveMessage(message); }
    
    
    
    void NekoEntity::UpdatePosition()
    {
        sprite.setPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
        Player::neko.setChibiPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
        shadow.setPosition(Player::neko.body.chibi.getPosition());
        
        if (inHands) inHandsSprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - 465*sprite.getScale().y);
        
        if (drawDialogue)
        {
            dialogueSprite.setPosition(sprite.getPosition().x, sprite.getGlobalBounds().top);
            dialogue.setPosition(dialogueSprite.getPosition().x, dialogueSprite.getPosition().y - dialogueSprite.getGlobalBounds().height/2 - 4*dialogueSprite.getScale().y);
        }
        if (beingOccupied)
        {
            occupyText.setPosition(sprite.getPosition().x, sprite.getPosition().y + 6*gs::scale);
            if (drawActionButton) occupyActionButton.setPosition(sprite.getPosition().x, occupyText.getGlobalBounds().top + occupyText.getGlobalBounds().height - 10*Room::scale*gs::scale);
        }
        if (!gs::ignoreDraw) gs::requestWindowRefresh = true;
        SetOnScreen();
    }
    void NekoEntity::ResizeDialogue()
    {
        dialogue.setCharacterSize(24 * gs::scale);
        dialogue.setScale(Room::scale, Room::scale);
        
        dialogueSprite.setScale(1.1*dialogue.getGlobalBounds().width/dialogueSprite.getLocalBounds().width, 1.2 * Room::scale * gs::scale);
        dialogueSprite.setPosition(sprite.getPosition().x, sprite.getGlobalBounds().top);
        dialogue.setPosition(dialogueSprite.getPosition().x, dialogueSprite.getPosition().y - dialogueSprite.getGlobalBounds().height/2 - 4*dialogueSprite.getScale().y);
    }
    void NekoEntity::ResizeOccupied()
    {
        occupyText.setCharacterSize(19*gs::scale);
        occupyText.setOutlineThickness(2*gs::scale);
        occupyText.setScale(Room::scale, Room::scale);
        occupyText.setPosition(sprite.getPosition().x, sprite.getPosition().y + 6*gs::scale);
        if (drawActionButton)
        {
            occupyActionButton.Resize(gs::width, gs::height);
            occupyActionButton.text.setScale(Room::scale, Room::scale);
            occupyActionButton.setPosition(sprite.getPosition().x, occupyText.getGlobalBounds().top + occupyText.getGlobalBounds().height - 10*Room::scale*gs::scale);
        }
    }
    void NekoEntity::SetChibiFacingDirection(bool right)
    {
        if (right != chibiFacingIsRight)
        {
            chibiFacingIsRight = right;
            Player::neko.chibiReversed = !right;
            for (auto& c : Player::neko.cloth) c->chibiReversed = !right;
            if (right) shadow.setOrigin(shadowOffsetR); else shadow.setOrigin(shadowOffsetL);
            Player::neko.ResizeChibi();
        }
    }
    bool NekoEntity::ShowOrHidePersonaCloth(const int& type, const bool& hidden, const bool& justCheck)
    {
        bool head{ false }, top{ false }, bottom{ false }, onepiece{ false }, bra{ false }, pantsu{ false }, socks{ false }, gloves{ false }, legwear{ false }, accessories{ false };
        /// { All, Accessory, Head, Top, Bottom, Onepiece, Gloves, Bra, Pantsu, Socks, Legwear, Outerwear, Underwear }
        switch (type)
        {
            case 0: head = top = bottom = onepiece = bra = pantsu = socks = gloves = legwear = accessories = true; break;
            case 1: accessories = true; break;
            case 2: head = true; break;
            case 3: top = onepiece = true; break;
            case 4: bottom = onepiece = true; break;
            case 5: top = bottom = onepiece = true; break;
            case 6: gloves = true; break;
            case 7: bra = true; break;
            case 8: pantsu = true; break;
            case 9: socks = true; break;
            case 10: legwear = true; break;
            case 11: head = top = bottom = onepiece = gloves = legwear = accessories = true; break;
            case 12: bra = pantsu = socks = true; break;
            default: break;
        }
        /// { Accessory, Head, Top, Bottom, Onepiece, Gloves, Bra, Pantsu, Socks, Legwear };
        bool anyClothesTaken{ false };
        for (auto& c : Player::neko.cloth) if (c->item && ((accessories && c->item->clothing == ClothType::Accessory) || (head && c->item->clothing == ClothType::Head) || (top && c->item->clothing == ClothType::Top) || (bottom && c->item->clothing == ClothType::Accessory) || (onepiece && c->item->clothing == ClothType::Onepiece) || (bra && c->item->clothing == ClothType::Bra) || (pantsu && c->item->clothing == ClothType::Pantsu) || (socks && c->item->clothing == ClothType::Socks) || (gloves && c->item->clothing == ClothType::Gloves) || (legwear && c->item->clothing == ClothType::Legwear)) ) { if (justCheck) return true; c->hidden = hidden; anyClothesTaken = true; }
        
        return anyClothesTaken;
    }
    
    
    
    void NekoEntity::SetDialogue(const std::wstring& dialog, bool unlimited)
    {
        if (!gs::ignoreDraw) gs::requestWindowRefresh = true;
        drawDialogue = true; unlimitedDrawDialogue = unlimited;
        dialogue.setString(dialog);
        if (!unlimitedDrawDialogue) elapsedDialogue = 2.1f + 0.08 * dialogue.getString().getSize();// + (rand() % 2000) / 2000.f;
        ResizeDialogue();
        dialogue.setOrigin(dialogue.getLocalBounds().width/2, dialogue.getLocalBounds().height/2);
    }
    std::wstring NekoEntity::GenerateRoomDialogue()
    {
        int needsTotal{ 0 };
        if (!activity || !activity->forceActivityReplica)
        {
            if (NekoS::needHunger <= 666) ++needsTotal;
            if (NekoS::needThirst <= 666) ++needsTotal;
            if (NekoS::needCommunication <= 666) ++needsTotal;
            if (NekoS::needHygiene <= 666) ++needsTotal;
            if (NekoS::needToilet <= 666) ++needsTotal;
            if (NekoS::needEnergy <= 666) ++needsTotal;
        }
        if (needsTotal)
        {
            int needArea, chosenArea = 0;
            if (needsTotal == 1)
            {
                if (NekoS::needHunger <= 666) chosenArea = 0;
                else if (NekoS::needThirst <= 666) chosenArea = 1;
                else if (NekoS::needCommunication <= 666) chosenArea = 2;
                else if (NekoS::needHygiene <= 666) chosenArea = 3;
                else if (NekoS::needToilet <= 666) chosenArea = 4;
                else if (NekoS::needEnergy <= 666) chosenArea = 5;
            }
            else
            {
                bool hungerEnabled{ NekoS::needHunger <= 666 && previousRandomDialogue != 1000 },
                     thirstEnabled{ NekoS::needThirst <= 666 && previousRandomDialogue != 1001 },
                     communicationEnabled{ NekoS::needCommunication <= 666 && previousRandomDialogue != 1002 },
                     hygieneEnabled{ NekoS::needHygiene <= 666 && previousRandomDialogue != 1003 },
                     toiletEnabled{ NekoS::needToilet <= 666 && previousRandomDialogue != 1004 },
                     energyEnabled{ NekoS::needEnergy <= 666 && previousRandomDialogue != 1005 };
                needArea = (667 - NekoS::needHunger)*hungerEnabled + (667 - NekoS::needThirst)*thirstEnabled + (667 - NekoS::needCommunication)*communicationEnabled + (667 - NekoS::needHygiene)*hygieneEnabled + (667 - NekoS::needToilet)*toiletEnabled + (667 - NekoS::needEnergy)*energyEnabled;
                int needHungerArea = (667 - NekoS::needHunger)*hungerEnabled,
                    needThirstArea = needHungerArea + (667 - NekoS::needThirst)*thirstEnabled,
                    needCommunicationArea = needThirstArea + (667 - NekoS::needCommunication)*communicationEnabled,
                    needHygieneArea = needCommunicationArea + (667 - NekoS::needHygiene)*hygieneEnabled,
                    needToiletArea = needHygieneArea + (667 - NekoS::needToilet)*toiletEnabled,
                    needEnergyArea = needToiletArea + (667 - NekoS::needEnergy)*energyEnabled;
                int needRandomArea = rand() % needArea;
                if (needRandomArea >= 0 && needRandomArea < needHungerArea && hungerEnabled) chosenArea = 0;
                else if (needRandomArea >= needHungerArea && needRandomArea < needThirstArea && thirstEnabled) chosenArea = 1;
                else if (needRandomArea >= needThirstArea && needRandomArea < needCommunicationArea && communicationEnabled) chosenArea = 2;
                else if (needRandomArea >= needCommunicationArea && needRandomArea < needHygieneArea && hygieneEnabled) chosenArea = 3;
                else if (needRandomArea >= needHygieneArea && needRandomArea < needToiletArea && toiletEnabled) chosenArea = 4;
                else if (needRandomArea >= needToiletArea && needRandomArea < needEnergyArea && energyEnabled) chosenArea = 5;
            }
            
            int needRandom = rand() % 667;
            switch (chosenArea)
            {
                case 0: if (NekoS::needHunger < needRandom) { previousRandomDialogue = 1000; return L"Хотю кушать... т.т"; } break;
                case 1: if (NekoS::needThirst < needRandom) { previousRandomDialogue = 1001; return L"Хотю пить... т.т"; } break;
                case 2: if (NekoS::needCommunication < needRandom) { previousRandomDialogue = 1002; return L"Одиноко... т.т"; } break;
                case 3: if (NekoS::needHygiene < needRandom) { previousRandomDialogue = 1003; return L"Хотю мыться... т.т"; } break;
                case 4: if (NekoS::needToilet < needRandom) { previousRandomDialogue = 1004; return L"Хотю в туалет... т.т"; } break;
                case 5: if (NekoS::needEnergy < needRandom) { previousRandomDialogue = 1005; return L"Усталя... т.т"; } break;
                default: break;
            }
        }
        if (activity && activity->doingTheActivity)
        {
            if (activity->name == "SittingAtTable")
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
            else if (activity->name == "SittingAtComputer")
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
            else if (activity->name == "LookingAtKitchen")
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
            else if (activity->name == "Sleeping")
            {
                int random = rand() % 2;
                switch (random)
                {
                    case 0: return L"*мило посапывает*"; break;
                    case 1: return L"М-мур... *спит*"; break;
                }
            }
            else if (activity->name == "Bathing")
            {
                int random; do { random = rand() % 3; } while (random == previousRandomDialogue);
                previousRandomDialogue = random;
                switch (random)
                {
                    case 0: return L"Потри мне спинку! >3<"; break;
                    case 1: return L"Ваа, так приятно~~"; break;
                    default: return L"Не подсматривай!! >:3"; break;
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
    void NekoEntity::SetOnScreen()
    {
        float actualWidth = Player::neko.chibiWidth * abs(Player::neko.eyes.chibi.getScale().x)/2.f; int addToTop{ 0 }, addToBottom{ 0 };
        if (drawDialogue) { addToTop = dialogueSprite.getGlobalBounds().height;
            if (dialogueSprite.getGlobalBounds().width > actualWidth) actualWidth = dialogueSprite.getGlobalBounds().width; }
        if (beingOccupied) { if (occupyActionButton.text.getGlobalBounds().width > actualWidth)
            actualWidth = occupyActionButton.text.getGlobalBounds().width;
            addToBottom = occupyText.getGlobalBounds().height + occupyActionButton.text.getGlobalBounds().height + 6*gs::scale; }
        onScreen = (sprite.getPosition().x + actualWidth/2 > 0 && sprite.getPosition().y + addToBottom > 0 && sprite.getPosition().x - actualWidth/2 < gs::width && sprite.getPosition().y - sprite.getGlobalBounds().height - addToTop < gs::height);
    }
    
    
    
    void NekoEntity::ExecuteCurrentActivity()
    {
        if (!activity) return;
        activity->sender = this;
        if ((sleeping = (activity->name == "Sleeping")))
            sender->SendMessage({"Apartment :: GimmeBedInfo"});
        activity->OnStart();
        
        if (activity)
        {
            rm::canOpenNekoUI = activity->canNekoUI;
            NekoP::eyebrowsEmotion = activity->eyebrowsEmotion;
            NekoP::eyesEmotion = activity->eyesEmotion;
            NekoP::mouthEmotion = activity->mouthEmotion;
            blinking = activity->blinking; itisblink = false;
            drawShadow = activity->drawShadow;
            ignoreBeingActionedWith = activity->ignoreBeingActionedWith;
            Player::UpdateNekoEmotion();
        }
    }
    void NekoEntity::FinishCurrentActivity()
    {
        // if (activity && (activity->name == "Sleeping" || activity->name == "ComeToSenses")) Player::SetNekoEmotionTo(Player::Emotion::Smiling);
        Player::NekoEmotionsAccordingToMood(); blinking = drawShadow = true; itisblink = ignoreBeingActionedWith = false;
        if (previousRandomDialogue < 1000 || previousRandomDialogue > 1005) previousRandomDialogue = -1;
        beingOccupied = drawActionButton = randomMoving = moveTo = sleeping = false; rm::canOpenNekoUI = true;
    }
    void NekoEntity::InsertActivity(Activity* insertActivity, bool delayCurrent)
    {
        if (activity) { FinishCurrentActivity(); if (delayCurrent) tasks.insert(tasks.begin(), activity); }
        if (insertActivity->name == "ComeToSenses")
        {
            if (activity && activity->name == "ReturnToFood" && droppedInHands)
            {
                float newdistance = sqrt(pow((droppedInHands->x - x), 2) + pow((droppedInHands->y - y), 2));
                if (newdistance > distanceToMovingEntity) { SetDialogue(L"Ну и ладно, бука! т.т"); droppedInHands = nullptr; itsTheDroppedFood = false; eatOrDrinkAttemts = 0; activity->Abort(); tasks.pop_front(); }
            }
            else if (eatOrDrinkAttemts >= 3) { SetDialogue(L"Ну и ладно, бука! т.т");
                for (auto& t : tasks) t->OnEnd(); tasks.clear(); droppedInHands = nullptr; itsTheDroppedFood = false;
                eatOrDrinkAttemts = 0; }
        }
        else if (insertActivity->name == "Eating" || insertActivity->name == "Drinking") ++eatOrDrinkAttemts;
        
        activity = insertActivity;
        cout << "Inserted activity: " << activity->name << endl;
        ExecuteCurrentActivity();
    }
    void NekoEntity::DropCurrentActivity(bool moveToTheNextOneImmediately)
    {
        if (activity) { FinishCurrentActivity(); activity->Abort(); }
        if (moveToTheNextOneImmediately && !tasks.empty())
        {
            activity = tasks.front(); tasks.pop_front();
            cout << "Dropped to activity: " << activity->name << endl;
            ExecuteCurrentActivity();
        }
    }
    
    
    
    void NekoEntity::PutItemInHands(Item* item)
    {
        RemoveItemFromHands(); inHands = item;
        if (item)
        {
            sf::Texture* texture = ic::LoadTexture(utf16("Data/Items/" + item->name + ".png"));
            if ((spriteLoaded = texture))
            {
                inHandsSprite.setTexture(*texture, true);
                inHandsSprite.setOrigin(texture->getSize().x/2, texture->getSize().y);
                inHandsScale = 0.24 * (static_cast<float>(gs::relativeHeight)/static_cast<float>(texture->getSize().y)) * 0.15 * item->scale;
                inHandsSprite.setScale(inHandsScale * Room::scale * gs::scale, inHandsScale * Room::scale * gs::scale);
                inHandsSprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - 465*sprite.getScale().y);
            }
        }
    }
    void NekoEntity::RemoveItemFromHands(bool drop, bool messageBackTheEntity)
    {
        if (inHands)
        {
            if (drop && messageBackTheEntity) sender->SendMessage({"SpawnMB :: Item", std::to_wstring(x) + L" " + std::to_wstring(y), inHands});
            else if (drop) sender->SendMessage({"Spawn :: Item", std::to_wstring(x) + L" " + std::to_wstring(y), inHands});
            ic::DeleteImage(utf16("Data/Items/" + inHands->name + ".png"));
            inHands = nullptr;
        }
        // else if (droppedInHands) distanceToMovingEntity = sqrt(pow((droppedInHands->x - x), 2) + pow((droppedInHands->y - y), 2));
    }
    void NekoEntity::ConsumeItem(Item* item)
    {
        if (item->type == ItemType::Food)
        {
            eatOrDrinkAttemts = 0;
            NekoS::needHunger += item->calories;
            NekoS::needThirst += item->thirstSatisfuction;
            if (inHands && inHands == item) RemoveItemFromHands(false);
            if (NekoS::needHunger > NekoS::maxNeed) NekoS::needHunger = NekoS::maxNeed;
            else if (NekoS::needHunger < 0) NekoS::needHunger = 0;
            if (NekoS::needThirst > NekoS::maxNeed) NekoS::needThirst = NekoS::maxNeed;
            else if (NekoS::needThirst < 0) NekoS::needThirst = 0;
        }
        else if (item->type == ItemType::Drink)
        {
            eatOrDrinkAttemts = 0;
            NekoS::needHunger += item->calories;
            NekoS::needThirst += item->thirstSatisfuction;
            if (inHands && inHands == item) RemoveItemFromHands(false);
            if (NekoS::needHunger > NekoS::maxNeed) NekoS::needHunger = NekoS::maxNeed;
            else if (NekoS::needHunger < 0) NekoS::needHunger = 0;
            if (NekoS::needThirst > NekoS::maxNeed) NekoS::needThirst = NekoS::maxNeed;
            else if (NekoS::needThirst < 0) NekoS::needThirst = 0;
        }
    }
    
    
    
    void NekoEntity::Save(std::wofstream &wof) { if (inHands) wof << L"0 " << x << L" " << y << L" 1 " << utf16(inHands->name) << endl; }
    void NekoEntity::SaveActivity(std::wofstream &wof)
    {
        if (activity)
        {
            wof << L"name " << utf16(activity->name) << endl;
            if (activity->name == "ReturnToFood" && movingToEntity) { wof << movingToEntity->x << " " << movingToEntity->y << endl; }
        }
        wof << "position " << x << " " << y << endl;
    }
}
