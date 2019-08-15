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
        occupyText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        occupyText.setOutlineColor(sf::Color::Black);
        occupyActionButton.setFont(L"Pacifica.ttf");
        occupyActionButton.setCharacterSize(32);
        occupyActionButton.valign = Valign::Top;
        
        
        sf::Texture* texture = ic::LoadTexture(L"Data/Images/UI/dialogue1_n.png");
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
            height = texture->getSize().y * 0.54 * relScale;
        }
        
        int i{ 0 }; do {
            x = rand() % Room::width;
            y = rand() % Room::height; ++i;
        } while (Room::mask && Room::mask->getPixel(floor(x/Room::roomScale), floor(y/Room::roomScale)).r && i < 100);
        if (i >= 100) { x = Room::width/2; y = Room::height/2; }
        
        elapsedWaiting = (rand() % 400)/1000.f;
        /* if (elapsedWaiting <= 0.16) { elapsedWaiting = 0.f; doNewEventInstantly = true; }
        Update(sf::seconds(0)); if (occupiedAt == -1) doNewEventInstantly = false; */
    }
    void NekoEntity::Destroy() { ic::DeleteImage(L"Data/Neko/Chibi/body.png"); ic::DeleteImage(L"Data/Images/UI/dialogue1_n.png"); }
    void NekoEntity::Update(const sf::Time& elapsedTime)
    {
        NStat::needHunger -= elapsedTime.asSeconds() * NStat::hungerInSecond;
        NStat::needThirst -= elapsedTime.asSeconds() * NStat::thirstInSecond;
        NStat::needCommunication -= elapsedTime.asSeconds() * NStat::communicationInSecond;
        NStat::needHygiene -= elapsedTime.asSeconds() * NStat::hygieneInSecond;
        NStat::needToilet -= elapsedTime.asSeconds() * NStat::toiletInSecond;
        NStat::needEnergy -= elapsedTime.asSeconds() * NStat::energyInSecond;
        if (NStat::needHunger < 0) NStat::needHunger = 0;
        if (NStat::needThirst < 0) NStat::needThirst = 0;
        if (NStat::needCommunication < 0) NStat::needCommunication = 0;
        if (NStat::needHygiene < 0) NStat::needHygiene = 0;
        if (NStat::needToilet < 0) NStat::needToilet = 0;
        if (NStat::needEnergy < 0) NStat::needEnergy = 0;
        
        if (beingActionedWith) return;
        if (!unlimitedDrawDialogue)
        {
            if (elapsedDialogue > 0) elapsedDialogue -= elapsedTime.asSeconds();
            else
            {
                if (drawDialogue) { drawDialogue = false; if (!gs::ignoreDraw) gs::requestWindowRefresh = true;
                    elapsedDialogue = 3.f + (rand() % 9000) / 1000.f; }
                else if (!activity || activity->doingTheActivity) SetDialogue(GenerateRoomDialogue());
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
                
                if ((moving = (moveLeft || moveRight || moveUp || moveDown)))
                {
                    UpdateDepthPosition(); moving = true; if (!gs::ignoreDraw) gs::requestWindowRefresh = true;
                    sprite.setPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
                    if (inHands) inHandsSprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - 465*sprite.getScale().y);
                    Player::neko.setChibiPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
                    if (drawDialogue) { dialogueSprite.setPosition(sprite.getPosition().x, sprite.getGlobalBounds().top);
                        dialogue.setPosition(dialogueSprite.getPosition()); }
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
                            // т.к. пока что нет поиска пути
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
                if (NStat::needHunger <= 260) { sender->SendMessage({"Apartment :: HungerActivity"}); if (activity) return; }
                if (NStat::needThirst <= 260) { sender->SendMessage({"Apartment :: ThirstActivity"}); if (activity) return; }
                if (NStat::needEnergy <= 260) { activity = adb::activities["GoSleep"]; }
                else if (NStat::needHygiene <= 260) { activity = adb::activities["GoTakeABath"]; }
                
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
        
        if (drawDialogue) { ResizeDialogue();
            dialogue.setOrigin(dialogue.getLocalBounds().width/2, dialogue.getLocalBounds().height + 44*gs::scale); }
        if (beingOccupied) { ResizeOccupied(); occupyText.setOrigin(occupyText.getLocalBounds().width/2, 0); }
        SetOnScreen();
    }
    void NekoEntity::Draw(sf::RenderWindow* window)
    {
        // if (spriteLoaded) window->draw(sprite);
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
            cout << "Activity :: Update" << endl;
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
                    eatOrDrinkAttemts = 0;
                    NStat::needHunger += inHands->calories;
                    NStat::needThirst += inHands->thirstSatisfuction;
                    RemoveItemFromHands(false);
                    if (activity->parameter1) SetDialogue(L"Пасибя, ето было вкусня! :3"); else SetDialogue(L"Йа покушаля! c:");
                    if (NStat::needHunger > NStat::maxNeed) NStat::needHunger = NStat::maxNeed;
                    else if (NStat::needHunger < 0) NStat::needHunger = 0;
                    if (NStat::needThirst > NStat::maxNeed) NStat::needThirst = NStat::maxNeed;
                    else if (NStat::needThirst < 0) NStat::needThirst = 0;
                }
                else if (activity->name == "Drinking" && inHands)
                {
                    eatOrDrinkAttemts = 0;
                    NStat::needHunger += inHands->calories;
                    NStat::needThirst += inHands->thirstSatisfuction;
                    RemoveItemFromHands(false);
                    if (activity->parameter1) SetDialogue(L"Пасибя, ето было освежающе! :3"); else SetDialogue(L"Йа попила! c:");
                    if (NStat::needHunger > NStat::maxNeed) NStat::needHunger = NStat::maxNeed;
                    else if (NStat::needHunger < 0) NStat::needHunger = 0;
                    if (NStat::needThirst > NStat::maxNeed) NStat::needThirst = NStat::maxNeed;
                    else if (NStat::needThirst < 0) NStat::needThirst = 0;
                }
                else if (activity->name == "ReturnToFood") droppedInHands = nullptr;
                else if (activity->name == "Sleeping")
                {
                    // NStat::needEnergy += 300.f;
                    if (NStat::needEnergy > NStat::maxNeed) NStat::needEnergy = NStat::maxNeed;
                    else if (NStat::needEnergy < 0) NStat::needEnergy = 0;
                    InsertActivity(adb::activities["ComeToSensesAfterSleep"], false);
                    SetDialogue(L"*Зевает*");
                    return;
                }
                else if (activity->name == "Bathing")
                {
                    NStat::needHygiene += 300.f;
                    if (NStat::needHygiene > NStat::maxNeed) NStat::needHygiene = NStat::maxNeed;
                    else if (NStat::needHygiene < 0) NStat::needHygiene = 0;
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
                    if (NStat::needEnergy > NStat::maxNeed) NStat::needEnergy = NStat::maxNeed;
                    else if (NStat::needEnergy < 0) NStat::needEnergy = 0;
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
            if (activity) activity->ReceiveMessage(message);
        }
        else if (message.info == "Apartment :: DrinkFridgeSpawned")
        {
            iKnowThereIsNoDrinkInTheFridge = !message.address;
            if (activity) activity->ReceiveMessage(message);
        }
        else if (message.info == "Apartment :: BedInfo" && activity && activity->name == "Sleeping")
        {
            RoomEntity* entity = reinterpret_cast<RoomEntity*>(message.address);
            if (entity)
            {
                x = entity->x;
                y = entity->y + (entity->sprite.getLocalBounds().height/1.3f)*entity->relScale;
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
        
        if (inHands) inHandsSprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - 465*sprite.getScale().y);
        
        if (drawDialogue)
        {
            dialogueSprite.setPosition(sprite.getPosition().x, sprite.getGlobalBounds().top);
            dialogue.setPosition(dialogueSprite.getPosition());
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
    void NekoEntity::SetChibiFacingDirection(bool right)
    {
        if (right != chibiFacingIsRight)
        {
            chibiFacingIsRight = right;
            Player::neko.chibiReversed = !right;
            for (auto& c : Player::neko.cloth) c->chibiReversed = !right;
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
        dialogue.setOrigin(dialogue.getLocalBounds().width/2, dialogue.getLocalBounds().height + 44*gs::scale);
    }
    std::wstring NekoEntity::GenerateRoomDialogue()
    {
        int needsTotal{ 0 };
        if (!activity || !activity->forceActivityReplica)
        {
            if (NStat::needHunger <= 666) ++needsTotal;
            if (NStat::needThirst <= 666) ++needsTotal;
            if (NStat::needCommunication <= 666) ++needsTotal;
            if (NStat::needHygiene <= 666) ++needsTotal;
            if (NStat::needToilet <= 666) ++needsTotal;
            if (NStat::needEnergy <= 666) ++needsTotal;
        }
        if (needsTotal)
        {
            int needArea, chosenArea = 0;
            if (needsTotal == 1)
            {
                if (NStat::needHunger <= 666) chosenArea = 0;
                else if (NStat::needThirst <= 666) chosenArea = 1;
                else if (NStat::needCommunication <= 666) chosenArea = 2;
                else if (NStat::needHygiene <= 666) chosenArea = 3;
                else if (NStat::needToilet <= 666) chosenArea = 4;
                else if (NStat::needEnergy <= 666) chosenArea = 5;
            }
            else
            {
                bool hungerEnabled{ NStat::needHunger <= 666 && previousRandomDialogue != 1000 },
                     thirstEnabled{ NStat::needThirst <= 666 && previousRandomDialogue != 1001 },
                     communicationEnabled{ NStat::needCommunication <= 666 && previousRandomDialogue != 1002 },
                     hygieneEnabled{ NStat::needHygiene <= 666 && previousRandomDialogue != 1003 },
                     toiletEnabled{ NStat::needToilet <= 666 && previousRandomDialogue != 1004 },
                     energyEnabled{ NStat::needEnergy <= 666 && previousRandomDialogue != 1005 };
                needArea = (667 - NStat::needHunger)*hungerEnabled + (667 - NStat::needThirst)*thirstEnabled + (667 - NStat::needCommunication)*communicationEnabled + (667 - NStat::needHygiene)*hygieneEnabled + (667 - NStat::needToilet)*toiletEnabled + (667 - NStat::needEnergy)*energyEnabled;
                int needHungerArea = (667 - NStat::needHunger)*hungerEnabled,
                    needThirstArea = needHungerArea + (667 - NStat::needThirst)*thirstEnabled,
                    needCommunicationArea = needThirstArea + (667 - NStat::needCommunication)*communicationEnabled,
                    needHygieneArea = needCommunicationArea + (667 - NStat::needHygiene)*hygieneEnabled,
                    needToiletArea = needHygieneArea + (667 - NStat::needToilet)*toiletEnabled,
                    needEnergyArea = needToiletArea + (667 - NStat::needEnergy)*energyEnabled;
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
                case 0: if (NStat::needHunger < needRandom) { previousRandomDialogue = 1000; return L"Хотю кушать... т.т"; } break;
                case 1: if (NStat::needThirst < needRandom) { previousRandomDialogue = 1001; return L"Хотю пить... т.т"; } break;
                case 2: if (NStat::needCommunication < needRandom) { previousRandomDialogue = 1002; return L"Одиноко... т.т"; } break;
                case 3: if (NStat::needHygiene < needRandom) { previousRandomDialogue = 1003; return L"Хотю мыться... т.т"; } break;
                case 4: if (NStat::needToilet < needRandom) { previousRandomDialogue = 1004; return L"Хотю в туалет... т.т"; } break;
                case 5: if (NStat::needEnergy < needRandom) { previousRandomDialogue = 1005; return L"Усталя... т.т"; } break;
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
            Neko::eyebrowsEmotion = activity->eyebrowsEmotion;
            Neko::eyesEmotion = activity->eyesEmotion;
            Neko::mouthEmotion = activity->mouthEmotion;
            Player::UpdateNekoEmotion();
        }
    }
    void NekoEntity::FinishCurrentActivity()
    {
        // if (activity && (activity->name == "Sleeping" || activity->name == "ComeToSenses")) Player::SetNekoEmotionTo(Player::Emotion::Smiling);
        Player::NekoEmotionsAccordingToMood();
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
    
    void NekoEntity::Save(std::wofstream &wof) { if (inHands) wof << L"0 " << x << L" " << y << L" 1 " << utf16(inHands->name) << endl; }
}
