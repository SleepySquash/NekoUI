//
//  Apartment.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 17/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "Apartment.hpp"

namespace NekoUI
{
    Apartment::Apartment()
    {
        sf::Texture* texture = ic::LoadTexture(L"Data/Apartment/Background/room.png");
        if ((spriteLoaded = texture)) { texture->setSmooth(false);
            Room::width = texture->getSize().x * Room::roomScale;
            Room::height = texture->getSize().y * Room::roomScale;
            sprite.setTexture(*texture, true); }
        Room::mask = ic::LoadImage(L"Data/Apartment/Background/room_mask.png");
        
        texture = ic::LoadTexture(L"Data/Images/UI/PointerToNeko.png");
        if (texture) { nekoPtrSprite.setTexture(*texture); nekoPtrSprite.setOrigin(texture->getSize().x/2, texture->getSize().y/2); }
        texture = ic::LoadTexture(L"Data/Images/UI/PointerToNeko_arrow.png");
        if (texture) { nekoArrowSprite.setTexture(*texture); nekoArrowSprite.setOrigin(0, texture->getSize().y/2); }
        texture = ic::LoadTexture(L"Data/Images/UI/scrolldown.png");
        if (texture) { inventoryMovingSprite.setTexture(*texture); inventoryMovingSprite.setTextureRect({124, 0, 97, 126}); }
        texture = ic::LoadTexture(L"Data/Apartment/Background/" + Player::backgroundCover);
        if ((spriteLoaded = texture)) { backgroundSprite.setTexture(*texture); }
    }
    void Apartment::Init()
    {
        neko.Init(); Player::neko.OccupyMemory(); neko.sender = entity;
        neko.positionInArray = 0; neko.vector = &entities; entities.push_back(&neko);
        hasFocusOnNeko = true; rm::scale = 1.7f;
        entity->SendMessage({"NekoUI :: SelectNeko", &neko});
        
        rm::scrolldownMenuOpened = rm::requestCloseButton = false;
        rm::drawDatePanel = rm::drawNeeds = rm::drawScrolldownMenu = rm::allowDTSaving = true;
        rm::canPressScrolldownMenu = rm::canPressDatePanel = rm::canOpenNekoUI = true;
        
        Player::eyebrowsEmotion = NekoS::EyebrowsEmotion::DEFAULT;
        Player::eyesEmotion = NekoS::EyesEmotion::DEFAULT;
        Player::mouthEmotion = NekoS::MouthEmotion::DEFAULT;
        
        sf::Texture* texture = ic::LoadTexture(L"Data/Images/UI/scrolldown rev1.png");
        if (texture) scrolldownMenuOffsetY = texture->getSize().y;
        
        sf::Font* font = fc::GetFont(L"Impact.ttf");
        if (font) hoverText.setFont(*font);
        
        FurnitureEntity* bed = new FurnitureEntity(); bed->id = "Bed";
        bed->Init(); bed->furnitureScale = 0.66f; bed->LoadSprite(L"bed1.png");
        bed->sprite.setOrigin(bed->sprite.getOrigin().x, 28);
        bed->x = 63 * Room::roomScale; bed->y = 202 * Room::roomScale;
        RegisterEntity(bed); RegisterFurniture(bed); bed->canDropNekoOn = true;
        
        FurnitureEntity* toilet = new FurnitureEntity(); toilet->id = "Toilet";
        toilet->Init(); toilet->furnitureScale = 0.32f; toilet->LoadSprite(L"toilet1.png");
        toilet->sprite.setOrigin(toilet->sprite.getOrigin().x, 38);
        toilet->x = 161 * Room::roomScale; toilet->y = 99 * Room::roomScale;
        RegisterEntity(toilet); RegisterFurniture(toilet); toilet->canDropNekoOn = true;
        
        FurnitureEntity* bathtub = new FurnitureEntity(); bathtub->id = "Bathtub";
        bathtub->Init(); bathtub->furnitureScale = 0.27f; bathtub->LoadSprite(L"bathtub1.png");
        bathtub->sprite.setOrigin(bathtub->sprite.getLocalBounds().width, 29);
        bathtub->x = 426 * Room::roomScale; bathtub->y = 214 * Room::roomScale;
        RegisterEntity(bathtub); RegisterFurniture(bathtub); bathtub->canDropNekoOn = true;
        
        FurnitureEntity* fridge = new FurnitureEntity(); fridge->id = "Fridge";
        fridge->Init(); fridge->furnitureScale = 0.58f; fridge->LoadSprite(L"fridge1.png");
        fridge->sprite.setOrigin(2, 67); fridge->x = 299 * Room::roomScale; fridge->y = 128 * Room::roomScale;
        RegisterEntity(fridge); RegisterFurniture(bed);
        
        LoadApartment();
        
        
        
        // Player::timePassed = 172800.f;
        // "DressOrUndress" не выполняется, если нечего возвращать к одежде (т.е. ничего не hidden).
        
        
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
#ifdef _MSC_VER
        tm* timeinfo = new tm(); localtime_s(timeinfo, &now_c);
#else
        tm* timeinfo = std::localtime(&now_c);
#endif
        if (rm::simulationWasAt != rm::simulationWasAtEnum::Non) Player::UpdateCurrentDT();
        
        std::list<std::wstring> nekoDidList;
        
        bool doAnyMoreSimulation{ true }, forceNewCoodrinates{ true };
        if ((forceNewCoodrinates = !(loadedActivity != "ReturnToFood" && Player::timePassed <= 4.5f))) { neko.x = loadedNeko_x; neko.y = loadedNeko_y; }
        if (loadedActivity.length())
        {
            if (loadedActivity == "Sleeping")
            {
                float energyGain = 0.04629f * Player::timePassed;
                if (timeinfo->tm_hour > 7 && timeinfo->tm_hour < 23)
                {
                    if ((NekoS::needEnergy + energyGain) < 1000.f)
                    {
                        neko.InsertActivity(adb::activities["Sleeping"]);
                        ActivityTasks::Sleeping* task = reinterpret_cast<ActivityTasks::Sleeping*>(*(neko.activity->task));
                        if (task) task->energyCap = 1000.f;
                        doAnyMoreSimulation = false;
                    }
                    else nekoDidList.push_back(L"поспала");
                }
                NekoS::needEnergy += energyGain;
                if (NekoS::needEnergy > NekoS::maxNeed) NekoS::needEnergy = NekoS::maxNeed;
            }
            else if (loadedActivity == "ReturnToFood")
            {
                if (loaded_ReturnToFood_entity && loaded_ReturnToFood_entity->type == RoomEntity::Type::Item)
                {
                    ItemEntity* ient = reinterpret_cast<ItemEntity*>(loaded_ReturnToFood_entity);
                    if (ient)
                    {
                        if (Player::timePassed >= 30.f) { nekoDidList.push_back(L"скушала " + ient->item->display);
                            neko.ConsumeItem(ient->item); UnregisterEntity(loaded_ReturnToFood_entity); }
                        else
                        {
                            float distance = sqrt( pow(loadedNeko_x - ient->x, 2) + pow(loadedNeko_y - ient->y, 2) );
                            float timeEstimation = distance/neko.xySpd;
                            if (timeEstimation < Player::timePassed) {
                                neko.x = ient->x; neko.y = ient->y;
                                neko.PutItemInHands(ient->item);
                                UnregisterEntity(loaded_ReturnToFood_entity);
                                neko.InsertActivity(adb::activities["Eating"]);
                                doAnyMoreSimulation = false; }
                            else { nekoDidList.push_back(L"скушала " + ient->item->display);
                                neko.ConsumeItem(ient->item); UnregisterEntity(loaded_ReturnToFood_entity); }
                        }
                    }
                }
            }
            else if (loadedActivity == "Bathing")
            {
                if (Player::timePassed >= 16.f) { NekoS::needHygiene += 300.f; nekoDidList.push_back(L"помылась"); }
                else
                {
                    neko.InsertActivity(adb::activities["Bathing"]);
                    if ((*(neko.activity->task))->type != ActivityTask::TaskType::bathing) neko.activity->NextTask();
                    neko.ShowOrHidePersonaCloth(0, true);
                    doAnyMoreSimulation = false;
                }
            }
        }
        
        if (doAnyMoreSimulation)
        {
            if (timeinfo->tm_hour >= 23 || timeinfo->tm_hour <= 7)
            {
                // TODO: chance she might be doing something else (maybe naughty if she's that type of character)
                float beenSleepingFor = NekoHasBeenSleepingFor(timeinfo);
                if (Player::timePassed >= 28800.f) NekoS::needEnergy = (beenSleepingFor/28800.f) * NekoS::maxNeed;
                if (NekoS::needEnergy <= 920)
                {
                    neko.InsertActivity(adb::activities["Sleeping"]);
                    ActivityTasks::Sleeping* task = reinterpret_cast<ActivityTasks::Sleeping*>(*(neko.activity->task));
                    task->energyCap = 1000.f;
                    doAnyMoreSimulation = (Player::timePassed > beenSleepingFor);
                }
                else if (loadedActivity == "Sleeping") nekoDidList.push_back(L"поспала");
            }
            else if (Player::timePassed >= 28800.f) {
                NekoS::needEnergy = 4.174f/(timeinfo->tm_hour - 7) * NekoS::maxNeed; nekoDidList.push_back(L"поспала"); }
            /// ((23 - 7)*261)/1000 = 17.f;
        }
        
        if (doAnyMoreSimulation)
        {
            bool noFood{ false }, noDrink{ false };
            std::list<ItemEntity*> foodInRoom, drinkInRoom;
            for (auto& i : entities)
                if (i->type == RoomEntity::Type::Item)
                {
                    ItemEntity* ient = reinterpret_cast<ItemEntity*>(i);
                    if (ient->item->type == ItemType::Food) foodInRoom.push_back(ient);
                    else if (ient->item->type == ItemType::Drink) drinkInRoom.push_back(ient);
                }
            bool enablePreciseSimulation{ rm::simulationWasAt != rm::simulationWasAtEnum::Non };
            float simulationSpd = 300; if (enablePreciseSimulation) simulationSpd = 5.f;
            float timePassed = std::min(Player::timePassed, 172800.f);
            while (timePassed > 0)
            {
                NekoS::needHunger -= 5.f * NekoS::hungerInSecond;
                NekoS::needThirst -= 5.f * NekoS::thirstInSecond;
                NekoS::needCommunication -= 5.f * NekoS::communicationInSecond;
                NekoS::needHygiene -= 5.f * NekoS::hygieneInSecond;
                /// NekoS::needToilet -= 5.f * NekoS::toiletInSecond;
                // NekoS::needEnergy -= 300.f * NekoS::energyInSecond;
                if (NekoS::needHunger < 0) NekoS::needHunger = 0;
                if (NekoS::needThirst < 0) NekoS::needThirst = 0;
                if (NekoS::needCommunication < 0) NekoS::needCommunication = 0;
                if (NekoS::needHygiene < 0) NekoS::needHygiene = 0;
                /// if (NekoS::needToilet < 0) NekoS::needToilet = 0;
                // if (NekoS::needEnergy < 0) NekoS::needEnergy = 0;
                
                if (NekoS::needHunger <= NekoS::autoCapHunger)
                {
                    if (!foodInRoom.empty())
                    {
                        ItemEntity* ient = foodInRoom.front(); foodInRoom.pop_front();
                        nekoDidList.push_back(L"скушала " + ient->item->display);
                        neko.ConsumeItem(ient->item); UnregisterEntity(ient);
                    }
                    else if (!noFood)
                    {
                        noFood = true;
                        for (auto& i : Inventory::items.list)
                            if (i.first && i.first->type == ItemType::Food)
                            {
                                Inventory::items.savingIsRequired = true; cout << "Ate " << i.first->name << endl;
                                nekoDidList.push_back(L"скушала " + i.first->display);
                                neko.ConsumeItem(i.first); noFood = false;
                                Inventory::items.Remove(i.first->name);
                                break;
                            }
                    }
                }
                if (NekoS::needThirst <= NekoS::autoCapThirst)
                {
                    if (!drinkInRoom.empty())
                    {
                        ItemEntity* ient = drinkInRoom.front(); drinkInRoom.pop_front();
                        nekoDidList.push_back(L"выпила " + ient->item->display);
                        neko.ConsumeItem(ient->item); UnregisterEntity(ient);
                    }
                    else if (!noDrink)
                    {
                        noDrink = true;
                        for (auto& i : Inventory::items.list)
                            if (i.first && i.first->type == ItemType::Drink)
                            {
                                Inventory::items.savingIsRequired = true; cout << "Drank " << i.first->name << endl;
                                nekoDidList.push_back(L"выпила " + i.first->display);
                                neko.ConsumeItem(i.first); noDrink = false;
                                Inventory::items.Remove(i.first->name);
                                break;
                            }
                    }
                }
                if (NekoS::needHygiene <= NekoS::autoCapHygiene) NekoS::needHygiene += 300.f;
                
                timePassed -= simulationSpd;
            }
            neko.iKnowThereIsNoFoodInTheFridge = noFood;
            neko.iKnowThereIsNoDrinkInTheFridge = noDrink;
            
            if (!neko.activity)
            {
                int needsTotal{ 0 };
                if (NekoS::needHunger <= NekoS::autoCapHunger) ++needsTotal;
                if (NekoS::needThirst <= NekoS::autoCapThirst) ++needsTotal;
                // if (NekoS::needCommunication <= NekoS::autoCapCommunication) ++needsTotal; // <- Internet communication possible activity?
                if (NekoS::needHygiene <= NekoS::autoCapHygiene) ++needsTotal;
                // if (NekoS::needToilet <= NekoS::autoCapToilet) ++needsTotal;
                if (NekoS::needEnergy <= NekoS::autoCapEnergy) ++needsTotal;
            
                int needArea, chosenArea = -1;
                if (needsTotal == 1)
                {
                    if (NekoS::needHunger <= NekoS::autoCapHunger && (!foodInRoom.empty() || !noFood)) chosenArea = 0;
                    else if (NekoS::needThirst <= NekoS::autoCapThirst && (!drinkInRoom.empty() || !noDrink)) chosenArea = 1;
                    else if (NekoS::needCommunication <= NekoS::autoCapCommunication) chosenArea = 2;
                    else if (NekoS::needHygiene <= NekoS::autoCapHygiene) chosenArea = 3;
                    else if (NekoS::needToilet <= NekoS::autoCapToilet) chosenArea = 4;
                    else if (NekoS::needEnergy <= NekoS::autoCapEnergy) chosenArea = 5;
                }
                else
                {
                    bool hungerEnabled{ NekoS::needHunger <= NekoS::autoCapHunger && (!foodInRoom.empty() || !noFood) },
                         thirstEnabled{ NekoS::needThirst <= NekoS::autoCapThirst && (!drinkInRoom.empty() || !noDrink) },
                         communicationEnabled{ false && NekoS::needCommunication <= NekoS::autoCapCommunication },
                         hygieneEnabled{ NekoS::needHygiene <= NekoS::autoCapHygiene },
                         toiletEnabled{ false && NekoS::needToilet <= NekoS::autoCapToilet },
                         energyEnabled{ NekoS::needEnergy <= NekoS::autoCapEnergy };
                    needArea = (NekoS::autoCapHunger + 1 - NekoS::needHunger)*hungerEnabled + (NekoS::autoCapThirst + 1 - NekoS::needThirst)*thirstEnabled + (NekoS::autoCapCommunication + 1 - NekoS::needCommunication)*communicationEnabled + (NekoS::autoCapHygiene + 1 - NekoS::needHygiene)*hygieneEnabled + (NekoS::autoCapToilet + 1 - NekoS::needToilet)*toiletEnabled + (NekoS::autoCapEnergy + 1 - NekoS::needEnergy)*energyEnabled;
                    int needHungerArea = (NekoS::autoCapHunger + 1 - NekoS::needHunger)*hungerEnabled,
                        needThirstArea = needHungerArea + (NekoS::autoCapThirst + 1 - NekoS::needThirst)*thirstEnabled,
                        needCommunicationArea = needThirstArea + (NekoS::autoCapCommunication + 1 - NekoS::needCommunication)*communicationEnabled,
                        needHygieneArea = needCommunicationArea + (NekoS::autoCapHygiene + 1 - NekoS::needHygiene)*hygieneEnabled,
                        needToiletArea = needHygieneArea + (NekoS::autoCapToilet + 1 - NekoS::needToilet)*toiletEnabled,
                        needEnergyArea = needToiletArea + (NekoS::autoCapEnergy + 1 - NekoS::needEnergy)*energyEnabled;
                    if (needArea != 0)
                    {
                        int needRandomArea = rand() % needArea;
                        if (needRandomArea >= 0 && needRandomArea < needHungerArea && hungerEnabled) chosenArea = 0;
                        else if (needRandomArea >= needHungerArea && needRandomArea < needThirstArea && thirstEnabled) chosenArea = 1;
                        else if (needRandomArea >= needThirstArea && needRandomArea < needCommunicationArea && communicationEnabled) chosenArea = 2;
                        else if (needRandomArea >= needCommunicationArea && needRandomArea < needHygieneArea && hygieneEnabled) chosenArea = 3;
                        else if (needRandomArea >= needHygieneArea && needRandomArea < needToiletArea && toiletEnabled) chosenArea = 4;
                        else if (needRandomArea >= needToiletArea && needRandomArea < needEnergyArea && energyEnabled) chosenArea = 5;
                    }
                }
                
                // int needRandom = rand() % 261;
                switch (chosenArea)
                {
                    case 0:
                        if (!foodInRoom.empty())
                        {
                            neko.x = foodInRoom.front()->x; neko.y = foodInRoom.front()->y;
                            adb::activities["ReturnToFood"]->roomEntity = foodInRoom.front();
                            neko.InsertActivity(adb::activities["ReturnToFood"]);
                        }
                        else if (!noFood)
                        {
                            noFood = true;
                            for (auto& i : Inventory::items.list)
                                if (i.first && i.first->type == ItemType::Food) { noFood = false; break; }
                            neko.iKnowThereIsNoFoodInTheFridge = noFood;
                            if (!noFood) {
                                neko.InsertActivity(adb::activities["EatFromFridge"]);
                                neko.x = fridge->x; neko.y = fridge->y + 20; }
                        }
                        break;
                    case 1:
                        if (!drinkInRoom.empty())
                        {
                            neko.x = drinkInRoom.front()->x; neko.y = drinkInRoom.front()->y;
                            adb::activities["ReturnToFood"]->roomEntity = drinkInRoom.front();
                            neko.InsertActivity(adb::activities["ReturnToFood"]);
                        }
                        else if (!noDrink)
                        {
                            noDrink = true;
                            for (auto& i : Inventory::items.list)
                                if (i.first && i.first->type == ItemType::Drink) { noDrink = false; break; }
                            neko.iKnowThereIsNoDrinkInTheFridge = noDrink;
                            if (!noDrink) {
                                neko.InsertActivity(adb::activities["DrinkFromFridge"]);
                                neko.x = fridge->x; neko.y = fridge->y + 20; }
                        }
                        break;
                    case 2: neko.SetDialogue(L"СКАЖИ DEV'у: НЕВОЗМОЖНОЕ ПРОИЗОШЛО!"); break;
                    case 3:
                        neko.InsertActivity(adb::activities["Bathing"]);
                        if ((*(neko.activity->task))->type != ActivityTask::TaskType::bathing) neko.activity->NextTask();
                        neko.ShowOrHidePersonaCloth(0, true);
                        break;
                    case 4: neko.SetDialogue(L"СКАЖИ DEV'у: НЕВОЗМОЖНОЕ ПРОИЗОШЛО!"); break;
                    case 5: neko.InsertActivity(adb::activities["Sleeping"]); break;
                    default: break;
                }
            }
            if (!neko.activity)
            {
                int randomActivity = rand() % adb::availableActivities.size();
                auto occupiedIt = adb::availableActivities.begin(); std::advance(occupiedIt, randomActivity);
                neko.previousOccupied = (adb::availableActivities.size() == 1) ? -1 : randomActivity;
                neko.activity = (*occupiedIt);
                cout << "I'm selected to be occupied at: " << neko.activity->name << endl;
                neko.ExecuteCurrentActivity();
                
                if (forceNewCoodrinates)
                {
                    int chanceShesAlreadyDoingIt = rand() % 1000;
                    if (chanceShesAlreadyDoingIt < 600)
                    {
                        if (neko.activity->name == "SittingAtComputer") { neko.x = 72*Room::roomScale; neko.y = 148*Room::roomScale; }
                        else if (neko.activity->name == "SittingAtTable") { neko.x = 227*Room::roomScale; neko.y = 122*Room::roomScale; }
                        else if (neko.activity->name == "SittingAtComputer") { neko.x = 363*Room::roomScale; neko.y = 130*Room::roomScale; }
                    }
                }
                
                if (rm::simulationWasAt == rm::simulationWasAtEnum::Grocery)
                {
                    int random = rand() % 4;
                    switch (random)
                    {
                        case 0: neko.SetDialogue(L"Ваа, уже вернулься?~"); break;
                        case 1: neko.SetDialogue(L"А хозяин купиль вкусняшек??"); break;
                        case 2: case 3: neko.SetDialogue(L"С возвращением, хозяин! <3"); break;
                        default: break;
                    }
                }
                else
                {
                    if (timeinfo->tm_hour >= 23 || timeinfo->tm_hour <= 1)
                    {
                        int random = rand() % 3;
                        switch (random)
                        {
                            case 0: neko.SetDialogue(L"Хозяин?~ Тоже не спится?"); break;
                            case 1: neko.SetDialogue(L"А я чуть позже лягу!! >3<"); break;
                            case 2: neko.SetDialogue(L"Доброй ночи, хозяин!~~"); break;
                            default: break;
                        }
                    }
                    else if (timeinfo->tm_hour <= 5)
                    {
                        int random = rand() % 3;
                        switch (random)
                        {
                            case 0: neko.SetDialogue(L"Хозяин?~ А ты тиво не спишь?"); break;
                            case 1: neko.SetDialogue(L"Уф, дя, совсем не спится! >3<"); break;
                            case 2: neko.SetDialogue(L"А? Хозяин? Д-доброй ночки, дя~"); break;
                            default: break;
                        }
                    }
                    else if (timeinfo->tm_hour <= 7)
                    {
                        int random = rand() % 3;
                        switch (random)
                        {
                            case 0: neko.SetDialogue(L"Доброе утро, хозяин!~"); break;
                            case 1: neko.SetDialogue(L"У-у, ты так рано! >3<"); break;
                            case 2: neko.SetDialogue(L"Мм? Ты ведь выспалься, надеюсь? >3<"); break;
                            default: break;
                        }
                    }
                    else if (timeinfo->tm_hour >= 20)
                    {
                        int random = rand() % 3;
                        switch (random)
                        {
                            case 0: neko.SetDialogue(L"Доброго вечера, хозяин!~"); break;
                            case 1: neko.SetDialogue(L"Приветь, как твой денёк?~~"); break;
                            case 2: neko.SetDialogue(L"Весь день уже делаю скучашки!! >3<"); break;
                            default: break;
                        }
                    }
                    else
                    {
                        int random = rand() % 3;
                        switch (random)
                        {
                            case 0: neko.SetDialogue(L"Хозяин!!! Приветь! :3?"); break;
                            case 1: neko.SetDialogue(L"Доброго дня, лапка~ c:"); break;
                            case 2: neko.SetDialogue(L"Я скучала по тебе!! >3<"); break;
                            default: break;
                        }
                    }
                }
            }
        }
        
        if (!nekoDidList.empty())
        {
            std::wstring finalStr = L"Кошечка: ";
            for (auto it : nekoDidList) finalStr += it + L", ";
            finalStr.pop_back(); finalStr.pop_back(); finalStr += L"!";
            entity->system->SendMessage({"NotUI :: Popup", new NotificationHolder(L"Пока тебя не было...", finalStr)});
        }
        
        rm::simulationWasAt = rm::simulationWasAtEnum::Non;
        entity->system->SendMessage({"RoomUI :: Update"});
        
        savingIsRequired = true;
        SortEntities();
    }
    void Apartment::Destroy() { Player::SaveData(); SaveApartment(); CleanUp(); Player::neko.FreeMemory(); }
    void Apartment::CleanUp()
    {
        ic::DeleteImage(L"Data/Apartment/Background/room.png");
        ic::DeleteImage(L"Data/Apartment/Background/room_mask.png"); Room::mask = nullptr;
        ic::DeleteImage(L"Data/Apartment/Background/" + Player::backgroundCover);
        ic::DeleteImage(L"Data/Images/UI/PointerToNeko.png");
        ic::DeleteImage(L"Data/Images/UI/PointerToNeko_arrow.png");
        ic::DeleteImage(L"Data/Images/UI/scrolldown.png");
        ic::DeleteImage(L"Data/Images/UI/scrolldown rev1.png");
    }
    void renderBackgroundSprite(sf::RenderTexture* backgroundTexture, sf::Sprite* backgroundSprite, sf::Sprite* backgroundRenderSprite)
    {
        sf::Context context;
        gs::requestWindowRefresh = true;
        
        backgroundTexture->create(gs::width + 2*backgroundSprite->getGlobalBounds().width, gs::height + 2*backgroundSprite->getGlobalBounds().height);
        float yy = 0, yyuntil = gs::height + 2*backgroundSprite->getGlobalBounds().height;
        while (yy < yyuntil)
        {
            float xx = 0, xxuntil = gs::width + 2*backgroundSprite->getGlobalBounds().width;
            while (xx < xxuntil)
            {
                backgroundSprite->setPosition(xx, yy);
                backgroundTexture->draw(*backgroundSprite);
                xx += backgroundSprite->getGlobalBounds().width;
            }
            yy += backgroundSprite->getGlobalBounds().height;
        }
        backgroundTexture->display();
        backgroundRenderSprite->setTexture(backgroundTexture->getTexture(), true);
        backgroundRenderSprite->setOrigin(backgroundSprite->getGlobalBounds().width, backgroundSprite->getGlobalBounds().height);
        
        gs::requestWindowRefresh = true;
    }
    void Apartment::Update(const sf::Time& elapsedTime)
    {
        if (!active) return;
        vector<RoomEntity*>::iterator it = entities.begin();
        unsigned long sizeRemembered = entities.size();
        while (it != entities.end())
        {
            if ((*it)->offline)
            {
                unsigned long pos = (*it)->positionInArray;
                for (unsigned long i = pos + 1; i < entities.size(); ++i) --entities[i]->positionInArray;
                if ((*it)->type == RoomEntity::Type::Furniture) UnregisterFurniture(reinterpret_cast<FurnitureEntity*>(*it), false);
                (*it)->Destroy(); delete (*it); it = entities.erase(it); break;
            }
            else { (*it)->Update(elapsedTime); ++it; }
            if (entities.size() != sizeRemembered) break; // it crashes when Neko's Update() makes Apartment spawn an item (increasing entities.size()), but this Update() skips the item that has been spawned and tries to cover the size of entities, therefore moving through the entities.end() to the undefined memory.
        }
        if (neko.onScreen != lastMistake) { lastMistake = neko.onScreen; drawPointer = !lastMistake; }
        if (drawPointer && (requestPointerUpdate || neko.moving)) UpdateNekoPointer();
        
        if (timeSinceFirstPress > 0) { timeSinceFirstPress -= elapsedTime.asSeconds();
            if (timeSinceFirstPress < 0) timeSinceFirstPress = 0; }
        if (timeSinceFirstHold > 0) {
            timeSinceFirstHold -= elapsedTime.asSeconds();
            if (timeSinceFirstHold <= 0 && ((sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || sf::Touch::isDown(0)) &&
                                            pressedEntity->sprite.getGlobalBounds().contains(dot.x, dot.y))) { RegisterMovingEntity(pressedEntity); pressedEntity = nullptr; }
        }
        
        if (hasFocusOnNeko)
        {
            Room::x = gs::width/(2*gs::scale*Room::scale) - neko.x;
            Room::y = gs::height/(2*gs::scale*Room::scale) - neko.y + neko.sprite.getLocalBounds().height/4*neko.relScale;
            CalculateCameraPosition();
        }
        else if (entityIsBeingMoved)
        {
            if (movedEntity == &neko && checkNekoHovering)
            {
                if (elapsedNekoHovering > 0.f) elapsedNekoHovering -= elapsedTime.asSeconds();
                else
                {
                    drawHoverText = false;
                    for (auto& e : base::reverse(furnitures))
                        if (e->canDropNekoOn && e->sprite.getGlobalBounds().contains(neko.sprite.getPosition().x, neko.sprite.getPosition().y - neko.sprite.getGlobalBounds().height/2))
                        {
                            e->highlighted = drawHoverText = true;
                            if (e != nekoHoveringEntity)
                            {
                                if (nekoHoveringEntity) nekoHoveringEntity->highlighted = false;
                                nekoHoveringEntity = e;
                                if (e->id == "Bed") hoverText.setString(L"Уложить спать");
                                else if (e->id == "Bathtub") hoverText.setString(L"Помыть в ванной");
                                else if (e->id == "Toilet") hoverText.setString(L"Справить нужду");
                                else hoverText.setString(e->id);
                                hoverText.setFillColor(sf::Color::White);
                                hoverText.setOutlineColor(sf::Color::Black);
                                hoverText.setOrigin(hoverText.getLocalBounds().width/2, 0);
                                hoverText.setPosition(nekoHoveringEntity->sprite.getGlobalBounds().left + nekoHoveringEntity->sprite.getGlobalBounds().width/2, nekoHoveringEntity->sprite.getGlobalBounds().top + nekoHoveringEntity->sprite.getGlobalBounds().height);
                            }
                            break;
                        }
                    if (!drawHoverText && nekoHoveringEntity) { nekoHoveringEntity->highlighted = false; nekoHoveringEntity = nullptr; }
                    checkNekoHovering = false;
                }
            }
            if ((neko.moving || requestHoverUpdate) && hoverIsPossible && neko.onScreen && movedEntity != &neko)
            {
                // if (sf::Touch::isDown(0)) dot = sf::Touch::getPosition(0, *gs::window); else dot = sf::Mouse::getPosition(*gs::window);
                drawHoverText = !drawInventoryButton && neko.sprite.getGlobalBounds().contains(gs::lastMousePos.first, gs::lastMousePos.second);
                if (lastDrawHoverText != drawHoverText) {
                    hoverText.setPosition(gs::lastMousePos.first + 30*gs::scale, gs::lastMousePos.second); lastDrawHoverText = drawHoverText; }
            }
            if (inAreaToMoveView)
            {
                // if (sf::Touch::isDown(0)) dot = sf::Touch::getPosition(0, *gs::window); else dot = sf::Mouse::getPosition(*gs::window);
                dot = { gs::lastMousePos.first, gs::lastMousePos.second };
                if ((dot.x > 220*gs::scalex && dot.y > 220*gs::scaley && dot.x < gs::width - 220*gs::scalex && dot.y < gs::height - 220*gs::scaley) || drawInventoryButton) inAreaToMoveView = false;
                else
                {
                    float spdx{ 270 * gs::scalex }, spdy{ 270 * gs::scaley }, sclx{ 16 * gs::scalex }, scly{ 16 * gs::scaley };
                    if (dot.x > gs::width - spdx) rm::x += (-spdx + gs::width - dot.x)/sclx   * 90.f * elapsedTime.asSeconds();
                    if (dot.x < spdx) rm::x += (spdx - dot.x)/sclx                            * 90.f * elapsedTime.asSeconds();
                    if (dot.y > gs::height - spdy) rm::y += (-spdy + gs::height - dot.y)/scly * 90.f * elapsedTime.asSeconds();
                    if (dot.y < spdy) rm::y += (spdy - dot.y)/scly                            * 90.f * elapsedTime.asSeconds();
                    gs::requestWindowRefresh = true; MoveEntityTo(dot.x, dot.y); movedEntity->UpdateDepthPosition(); CalculateCameraPosition();
                }
            }
        }
    }
    void Apartment::PollEvent(sf::Event& event)
    {
        if (!active || gs::ignoreEvent) return;
        neko.PollEvent(event);
        
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Key::K) for (auto& item : Inventory::map) Inventory::items.Add(item.first);
            else if (event.key.code == sf::Keyboard::Key::D) Player::SaveCurrentDT();
        }
        else if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) || (event.type == sf::Event::TouchBegan))
        {
            if (event.type == sf::Event::MouseButtonPressed) dot = { event.mouseButton.x, event.mouseButton.y };
            else dot = { event.touch.x, event.touch.y };
            
            if (event.type == sf::Event::MouseButtonPressed || event.touch.finger == 0)
            {
                bool interception = false;
                for (auto& e : base::reverse(entities))
                    if (!e->offline && e->movable)
                    {
                        interception = e->sprite.getGlobalBounds().contains(dot.x, dot.y);
                        if (interception)
                        {
                            if (e == &neko) { timeSinceFirstHold = 0.25; pressedNeko = true; pressedNekoPos = { dot.x, dot.y }; }
                            else timeSinceFirstHold = 0.15f; pressedEntity = e; lastValidDot = { e->x, e->y }; break;
                        }
                    }
                
                canMoveAround = true;
                move_dx = rm::x - dot.x/(rm::scale*gs::scale);
                move_dy = rm::y - dot.y/(rm::scale*gs::scale);
            }
            else if (event.touch.finger == 1)
            {
                float x0{ static_cast<float>(sf::Touch::getPosition(0).x) },
                      y0{ static_cast<float>(sf::Touch::getPosition(0).y) },
                      x1{ static_cast<float>(dot.x) },
                      y1{ static_cast<float>(dot.y) };
                prevDistanceZoom = sqrt((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1));
                prevZoom = rm::scale;
            }
        }
        else if ((event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) || (event.type == sf::Event::TouchEnded && event.touch.finger == 0))
        {
            canMoveAround = false;
            if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
            else dot = { event.touch.x, event.touch.y };
            
            if (entityIsBeingMoved) ReleaseMovingEntity();
            else
            {
                bool interception = neko.sprite.getGlobalBounds().contains(dot.x, dot.y);
                if (interception && pressedNeko && sqrt(pow(pressedNekoPos.x - dot.x, 2) + pow(pressedNekoPos.y - dot.y, 2)) < 15)
                {
                    if (timeSinceFirstPress > 0) {
                        if (neko.drawDialogue) entity->SendMessage({"NekoUI :: DDialogue", neko.dialogue.getString()});
                        entity->SendMessage({"NekoUI :: Show"}); hasFocusOnNeko = true; pressedNeko = false; }
                    else { hasFocusOnNeko = true; timeSinceFirstPress = 0.35f; }
                }
                else if (drawPointer && nekoPtrSprite.getGlobalBounds().contains(dot.x, dot.y)) { hasFocusOnNeko = true; drawPointer = false; }
                else
                {
                    // Клик по мебели здесь.
                    // TODO: if furniture was clicked elapsedTime ago and this is the second time the same entity was clicked, then if entity->interactive do the action it was intented to so. E.g. if Fridge then open the fridge.
                }
            }
        }
        else if (event.type == sf::Event::TouchMoved || (event.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)))
        {
            if (event.type == sf::Event::MouseMoved) dot = { event.mouseMove.x, event.mouseMove.y };
            else dot = { event.touch.x, event.touch.y };
            
            if (!entityIsBeingMoved && sf::Touch::isDown(0) && event.touch.finger == 1 && prevDistanceZoom != 0)
            {
                if (hasFocusOnNeko) { neko.beingActionedWith = false; hasFocusOnNeko = false; }
                float x0{ static_cast<float>(sf::Touch::getPosition(0).x) },
                      y0{ static_cast<float>(sf::Touch::getPosition(0).y) },
                      x1{ static_cast<float>(dot.x) },
                      y1{ static_cast<float>(dot.y) };
                float distance = sqrt((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1));
                rm::scale = prevZoom * (distance/prevDistanceZoom);
                if (rm::scale < 0.4) rm::scale = 0.4; else if (rm::scale > 3.4) rm::scale = 3.4;
                rm::xWidth = gs::width/(gs::scale * rm::scale); rm::yHeight = gs::height/(gs::scale * rm::scale);
                
                sprite.setScale(rm::roomScale * gs::scale * rm::scale, rm::roomScale * gs::scale * rm::scale);
                CalculateCameraScale();
            }
            else if (event.type != sf::Event::TouchMoved || event.touch.finger == 0)
            {
                if (entityIsBeingMoved)
                {
                    if (movedEntity == &neko && !checkNekoHovering) { elapsedNekoHovering = 0.1f; checkNekoHovering = true; }
                    requestHoverUpdate = true;
                    MoveEntityTo(dot.x, dot.y); movedEntity->UpdatePosition(); movedEntity->UpdateDepthPosition();
                    inAreaToMoveView = (dot.x > gs::width - 220*gs::scalex || dot.x < 220*gs::scalex || dot.y > gs::height - 220*gs::scaley || dot.y < 220*gs::scaley);
                    drawInventoryButton = inAreaToMoveView && (movedEntity != &neko && dot.x > gs::width - 150*gs::scale && dot.y > gs::height - 150*gs::scale);
                    if (lastDrawInventoryButton != drawInventoryButton)
                    {
                        lastDrawInventoryButton = drawInventoryButton;
                        rm::drawScrolldownMenu = !drawInventoryButton;
                    }
                }
                else if (canMoveAround)
                {
                    if (hasFocusOnNeko) { neko.beingActionedWith = false; hasFocusOnNeko = false; }
                    rm::x = move_dx + dot.x/(rm::scale*gs::scale);
                    rm::y = move_dy + dot.y/(rm::scale*gs::scale);
                    CalculateCameraPosition();
                }
                if (drawHoverText && !nekoHoveringEntity) hoverText.setPosition(dot.x + 30*gs::scale, dot.y);
            }
        }
        else if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.delta != 0 && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
        {
            float scalePrev = rm::scale;
            rm::scale += scrollSensitivity * event.mouseWheelScroll.delta;
            if (rm::scale < 0.4) rm::scale = 0.4; else if (rm::scale > 3.4) rm::scale = 3.4;
            rm::xWidth = gs::width/(gs::scale * rm::scale); rm::yHeight = gs::height/(gs::scale * rm::scale);
            
            rm::x -= (gs::width/(gs::scale*rm::scale) - gs::width/(gs::scale*rm::scale) * (scalePrev/rm::scale))/2;
            rm::y -= (gs::height/(gs::scale*rm::scale) - gs::height/(gs::scale*rm::scale) * (scalePrev/rm::scale))/2;
            
            sprite.setScale(rm::roomScale * gs::scale * rm::scale, rm::roomScale * gs::scale * rm::scale);
            CalculateCameraScale();
        }
        else if (event.type == sf::Event::Closed || event.type == sf::Event::LostFocus) { Player::SaveData(); Player::SaveCurrentDT();
            if (savingIsRequired) SaveApartment(); }
    }
    void Apartment::Resize(unsigned int width, unsigned int height)
    {
        if (!active) return;
        Room::xWidth = gs::width/(gs::scale * Room::scale); Room::yHeight = gs::height/(gs::scale * Room::scale);
        if (spriteLoaded) sprite.setScale(Room::roomScale * gs::scale * Room::scale, Room::roomScale * gs::scale * Room::scale);
        CalculateCameraScale();
        
        if (backgroundSprite.getGlobalBounds().width > 0 && backgroundSprite.getGlobalBounds().height > 0)
            std::thread(renderBackgroundSprite, &backgroundTexture, &backgroundSprite, &backgroundRenderSprite).join();
        
        nekoPtrSprite.setScale(0.5 * gs::scale, 0.5 * gs::scale);
        nekoArrowSprite.setScale(0.5 * gs::scale, 0.5 * gs::scale);
        inventoryMovingSprite.setScale(1.2*gs::scale, 1.2*gs::scale);
        inventoryMovingSprite.setPosition(gs::width - inventoryMovingSprite.getGlobalBounds().width - 20*gs::scale, gs::height - inventoryMovingSprite.getGlobalBounds().height - 10*gs::scale);
        
        hoverText.setCharacterSize(50 * gs::scale);
        hoverText.setOutlineThickness(2 * gs::scale);
    }
    void Apartment::Draw(sf::RenderWindow* window)
    {
        if (!active || gs::ignoreDraw) return;
        if (spriteLoaded) { window->draw(backgroundRenderSprite); window->draw(sprite); }
        for (auto& e : entities) if (!e->offline && e->onScreen && e != movedEntity) e->Draw(window);
        if (drawInventoryButton) window->draw(inventoryMovingSprite);
        if (entityIsBeingMoved && movedEntity) movedEntity->Draw(window);
        if (drawHoverText)
        {
            // if (neko.activity && !neko.activity->canFeed) hoverText.setFillColor(sf::Color(180, 180, 180));
            window->draw(hoverText);
        }
        if (drawPointer) { window->draw(nekoArrowSprite); window->draw(nekoPtrSprite); }
    }
    void Apartment::RecieveMessage(MessageHolder& message)
    {
        if (!active) return;
        
        if ((message.info == "Spawn :: Item" || message.info == "SpawnMB :: Item") && message.address)
        {
            nss::CommandSettings command; command.Command(message.additional);
            float x = nss::ParseAsFloat(command); nss::SkipSpaces(command);
            float y = nss::ParseAsFloat(command);
            ItemEntity* item = RegisterItemEntity(reinterpret_cast<Item*>(message.address), x, y);
            /*ItemEntity* item = new ItemEntity();
            item->x = x; item->y = y;
            item->Init(reinterpret_cast<Item*>(message.address));
            item->Resize(); RegisterEntity(item, true);*/
            if (message.info == "SpawnMB :: Item") neko.SendMessage({"MB :: SpawnItem", item});
        }
        else if ((message.info == "Dropping :: Item" || message.info == "DroppingAt :: Item") && message.address)
        {
            nss::CommandSettings command; command.Command(message.additional);
            float x = nss::ParseAsFloat(command); nss::SkipSpaces(command);
            float y = nss::ParseAsFloat(command);
            if (message.info == "Dropping :: Item") { x = -rm::x + x/(gs::scale*rm::scale); y = -rm::y + y/(gs::scale*rm::scale); }
            ItemEntity* item = RegisterItemEntity(reinterpret_cast<Item*>(message.address), x, y);
            /*Item* itembase = reinterpret_cast<Item*>(message.address);
            ItemEntity* item = new ItemEntity();
            if (message.info == "Dropping :: Item")
            {
                item->x = -rm::x + x/(gs::scale*rm::scale);
                item->y = -rm::y + y/(gs::scale*rm::scale);
                if (itembase->type == ItemType::Food) neko.iKnowThereIsNoFoodInTheFridge = false;
                else if (itembase->type == ItemType::Drink) neko.iKnowThereIsNoDrinkInTheFridge = false;
            }
            else { item->x = x; item->y = y; }
            item->Init(itembase); item->Resize();
            RegisterEntity(item, true);*/
            
            lastValidDot = { Room::width/2.f, Room::height/2.f }; RegisterMovingEntity(item, true);
        }
        else if (message.info == "NekoUI :: Switch" || message.info == "NekoUI :: Show") { hasFocusOnNeko = true;
            if (neko.drawDialogue) entity->SendMessage({"NekoUI :: DDialogue", neko.dialogue.getString()}); }
        else if (message.info == "Apartment :: Save") SaveApartment();
        else if (message.info == "Apartment :: RequestPosition")
        {
            if (message.address)
            {
                for (auto& e : entities)
                    if (e == message.address && !e->offline)
                    {
                        if (movedEntity == e) {
                            neko.SendMessage({"Apartment :: ReceivePosition", L"0 0"}); neko.moveTo = false;
                            neko.distanceToMovingEntity = std::numeric_limits<float>::infinity();
                            neko.waitTilEntityStopsBeingMoved = true; neko.SetDialogue(L"Дяй1!!!! >.<"); }
                        else neko.SendMessage({"Apartment :: ReceivePosition", std::to_wstring(e->x) + L" " + std::to_wstring(e->y)});
                        return;
                    }
                neko.SendMessage({"Apartment :: ReceivePosition", L"0"});
            }
            else if (message.additional == L"Computer") { neko.SendMessage({"Apartment :: ReceivePosition", std::to_wstring(72*Room::roomScale) + L" " + std::to_wstring(148*Room::roomScale)}); }
            else if (message.additional == L"Table") { neko.SendMessage({"Apartment :: ReceivePosition", std::to_wstring(227*Room::roomScale) + L" " + std::to_wstring(122*Room::roomScale)}); }
            else if (message.additional == L"KitchenTable") { neko.SendMessage({"Apartment :: ReceivePosition", std::to_wstring(363*Room::roomScale) + L" " + std::to_wstring(130*Room::roomScale)}); }
            else
            {
                RoomEntity* found{ nullptr }; std::string id = utf8(message.additional);
                for (auto& e : entities) if ((e->type == RoomEntity::Type::Furniture && e->id == id && !e->offline)) { found = e; break; }
                if (found)
                {
                    if (id == "Bed") neko.SendMessage({"Apartment :: ReceivePosition", std::to_wstring(found->x) + L" " + std::to_wstring(found->y + found->sprite.getLocalBounds().height/1.3f * found->relScale)});
                    else if (id == "Bathtub") neko.SendMessage({"Apartment :: ReceivePosition", std::to_wstring(found->x - (found->sprite.getLocalBounds().width/2)*found->relScale) + L" " + std::to_wstring(found->y + 1)});
                    else neko.SendMessage({"Apartment :: ReceivePosition", std::to_wstring(found->x) + L" " + std::to_wstring(found->y)});
                }
                else neko.SendMessage({"Apartment :: ReceivePosition", L"0"});
            }
        }
        else if (message.info == "Apartment :: PutInHands")
        {
            if (message.address)
                for (auto& e : entities)
                    if (message.address == e && e != &neko && !e->offline)
                    {
                        ItemEntity* ient = reinterpret_cast<ItemEntity*>(e);
                        if (ient) { neko.PutItemInHands(ient->item); DestroyItemEntity(e, false); return; }
                    }
            neko.RemoveItemFromHands();
        }
        else if (message.info == "Apartment :: HungerActivity")
        {
            Player::noFood = !Inventory::FridgeContainsAnyOfType(ItemType::Food);
            RoomEntity* closestEntity{ nullptr }; float shortest{ std::numeric_limits<float>::infinity() };
            for (auto& e : entities)
            {
                Item* reitem{ nullptr }; if (e->type == RoomEntity::Type::Item) reitem = reinterpret_cast<ItemEntity*>(e)->item;
                if (((e->type == RoomEntity::Type::Item && reitem->type == ItemType::Food) || (e->type == RoomEntity::Type::Furniture && e->id == "Fridge" && !neko.iKnowThereIsNoFoodInTheFridge)) && movedEntity != e && !e->offline)
                {
                    float distance = sqrt(pow((neko.x - e->x), 2) + pow((neko.y - e->y), 2));
                    if (distance < shortest) { shortest = distance; closestEntity = e; }
                }
            }
            if (closestEntity)
            {
                if (closestEntity->type == RoomEntity::Type::Item) {
                    adb::activities["ReturnToFood"]->roomEntity = closestEntity;
                    neko.InsertActivity(adb::activities["ReturnToFood"]); }
                else neko.InsertActivity(adb::activities["EatFromFridge"]);
            }
        }
        else if (message.info == "Apartment :: ThirstActivity")
        {
            RoomEntity* closestEntity{ nullptr }; float shortest{ std::numeric_limits<float>::infinity() };
            for (auto& e : entities)
            {
                Item* reitem{ nullptr }; if (e->type == RoomEntity::Type::Item) reitem = reinterpret_cast<ItemEntity*>(e)->item;
                if (((e->type == RoomEntity::Type::Item && reitem->type == ItemType::Drink) || (e->type == RoomEntity::Type::Furniture && e->id == "Fridge" && !neko.iKnowThereIsNoDrinkInTheFridge)) && movedEntity != e && !e->offline)
                {
                    float distance = sqrt(pow((neko.x - e->x), 2) + pow((neko.y - e->y), 2));
                    if (distance < shortest) { shortest = distance; closestEntity = e; }
                }
            }
            if (closestEntity)
            {
                if (closestEntity->type == RoomEntity::Type::Item) {
                    adb::activities["ReturnToFood"]->roomEntity = closestEntity;
                    neko.InsertActivity(adb::activities["ReturnToFood"]); }
                else neko.InsertActivity(adb::activities["DrinkFromFridge"]);
            }
        }
        else if (message.info == "Apartment :: RequestFoodFromFridge")
        {
            for (auto& i : Inventory::items.list)
                if (i.first->type == ItemType::Food)
                {
                    savingIsRequired = Inventory::items.savingIsRequired = true;
                    ItemEntity* item = new ItemEntity();
                    item->x = neko.x; item->y = neko.y;
                    item->Init(i.first); item->Resize();
                    RegisterEntity(item, true);
                    Inventory::items.Remove(i.first->name);
                    neko.SendMessage({"Apartment :: FoodFridgeSpawned", item});
                    return;
                }
            Player::noFood = !Inventory::FridgeContainsAnyOfType(ItemType::Food);
            neko.SendMessage({"Apartment :: FoodFridgeSpawned"});
        }
        else if (message.info == "Apartment :: RequestDrinkFromFridge")
        {
            for (auto& i : Inventory::items.list)
                if (i.first->type == ItemType::Drink)
                {
                    savingIsRequired = Inventory::items.savingIsRequired = true;
                    ItemEntity* item = new ItemEntity();
                    item->x = neko.x; item->y = neko.y;
                    item->Init(i.first); item->Resize();
                    RegisterEntity(item, true);
                    Inventory::items.Remove(i.first->name);
                    neko.SendMessage({"Apartment :: DrinkFridgeSpawned", item});
                    return;
                }
            Player::noDrink = !Inventory::FridgeContainsAnyOfType(ItemType::Drink);
            neko.SendMessage({"Apartment :: DrinkFridgeSpawned"});
        }
        else if (message.info == "Apartment :: GimmeBedInfo") {
            for (auto& e : entities) if ((e->type == RoomEntity::Type::Furniture && e->id == "Bed" && !e->offline)) {
                neko.SendMessage({"Apartment :: BedInfo", e}); return; } }
        else if (message.info == "Apartment :: GimmeBathInfo") {
            for (auto& e : entities) if ((e->type == RoomEntity::Type::Furniture && e->id == "Bathtub" && !e->offline)) {
                neko.SendMessage({"Apartment :: BathInfo", e}); return; } }
        else if (message.info == "Apartment :: Destroy") { Player::SaveCurrentDT(); entity->PopComponent(this); }
        
    }
    
    
    
    void Apartment::CalculateCameraPosition()
    {
        if (sprite.getGlobalBounds().width <= gs::width)
            rm::x = gs::width/(2*gs::scale*rm::scale) - sprite.getLocalBounds().width*rm::roomScale/2;
        else if (rm::x > 0) rm::x = 0;
        else if (rm::x < -sprite.getLocalBounds().width*rm::roomScale + gs::width/(gs::scale*rm::scale))
            rm::x = -sprite.getLocalBounds().width*rm::roomScale + gs::width/(gs::scale*rm::scale);
        
        if (sprite.getGlobalBounds().height <= gs::height)
            rm::y = gs::height/(2*gs::scale*rm::scale) - sprite.getLocalBounds().height*rm::roomScale/2;
        else if (rm::y > 0) rm::y = 0;
        else if (rm::y < -sprite.getLocalBounds().height*rm::roomScale + gs::height/(gs::scale*rm::scale))
            rm::y = -sprite.getLocalBounds().height*rm::roomScale + gs::height/(gs::scale*rm::scale);
        
        if (sprite.getGlobalBounds().width <= gs::width) backgroundXX = 0;
        else backgroundXX = (int)(rm::x) % (int)backgroundSprite.getGlobalBounds().width;
        if (sprite.getGlobalBounds().height <= gs::height) backgroundYY = 0;
        else backgroundYY = (int)(rm::y) % (int)backgroundSprite.getGlobalBounds().height;
        backgroundRenderSprite.setPosition(backgroundXX, backgroundYY);
        
        for (auto& e : entities) if (!e->offline) e->UpdatePosition(); requestPointerUpdate = true;
        sprite.setPosition(rm::x * gs::scale * rm::scale, rm::y * gs::scale * rm::scale);
        if (drawHoverText && nekoHoveringEntity) hoverText.setPosition(nekoHoveringEntity->sprite.getGlobalBounds().left + nekoHoveringEntity->sprite.getGlobalBounds().width/2, nekoHoveringEntity->sprite.getGlobalBounds().top + nekoHoveringEntity->sprite.getGlobalBounds().height);
    }
    void Apartment::CalculateCameraScale()
    {
        if (sprite.getGlobalBounds().width <= gs::width)
            rm::x = gs::width/(2*gs::scale*rm::scale) - sprite.getLocalBounds().width*rm::roomScale/2;
        else if (rm::x > 0) rm::x = 0;
        else if (rm::x < -sprite.getLocalBounds().width*rm::roomScale + gs::width/(gs::scale*rm::scale))
            rm::x = -sprite.getLocalBounds().width*rm::roomScale + gs::width/(gs::scale*rm::scale);
        
        if (sprite.getGlobalBounds().height <= gs::height)
            rm::y = gs::height/(2*gs::scale*rm::scale) - sprite.getLocalBounds().height*rm::roomScale/2;
        else if (rm::y > 0) rm::y = 0;
        else if (rm::y < -sprite.getLocalBounds().height*rm::roomScale + gs::height/(gs::scale*rm::scale))
            rm::y = -sprite.getLocalBounds().height*rm::roomScale + gs::height/(gs::scale*rm::scale);
        
        if (sprite.getGlobalBounds().width <= gs::width) backgroundXX = 0;
        else backgroundXX = (int)(rm::x) % (int)backgroundSprite.getGlobalBounds().width;
        if (sprite.getGlobalBounds().height <= gs::height) backgroundYY = 0;
        else backgroundYY = (int)(rm::y) % (int)backgroundSprite.getGlobalBounds().height;
        backgroundRenderSprite.setPosition(backgroundXX, backgroundYY);
        
        for (auto& e : entities) if (!e->offline) e->Resize(); requestPointerUpdate = !hasFocusOnNeko;
        sprite.setPosition(rm::x * gs::scale * rm::scale, rm::y * gs::scale * rm::scale);
        if (drawHoverText && nekoHoveringEntity) hoverText.setPosition(nekoHoveringEntity->sprite.getGlobalBounds().left + nekoHoveringEntity->sprite.getGlobalBounds().width/2, nekoHoveringEntity->sprite.getGlobalBounds().top + nekoHoveringEntity->sprite.getGlobalBounds().height);
    }
    void Apartment::MoveEntityTo(int x, int y)
    {
        movedEntity->x = -rm::x + x/(gs::scale*rm::scale);
        movedEntity->y = -rm::y + y/(gs::scale*rm::scale) + movedEntity->height/2;
        if (Room::mask && !Room::Collision(movedEntity->x, movedEntity->y)) lastValidDot = { movedEntity->x, movedEntity->y };
    }
    void Apartment::UpdateNekoPointer()
    {
        float cathetY = neko.sprite.getPosition().y - neko.height/2 - gs::height/2;
        float cathetX = neko.sprite.getPosition().x - gs::width/2;
        float theta = atan(cathetY/cathetX); if (cathetX < 0) theta += 3.14;
        nekoArrowSprite.setRotation(theta * 57.3f);
        
        sf::Vector2f edge = GetEdgeOfView(theta);
        nekoArrowSprite.setPosition(edge.x, gs::height - edge.y);
        nekoPtrSprite.setPosition(edge.x, gs::height - edge.y);
    }
    sf::Vector2f Apartment::GetEdgeOfView(float theta)
    {
        ///////////////////////////////////////////////////////////
        /// This function belongs to: https://stackoverflow.com/users/34820/olie
        ///////////////////////////////////////////////////////////
#ifndef M_PI
#define M_PI 3.1415926535
#endif
        const double twoPI = M_PI * 2.;
        while (theta < -M_PI) theta += twoPI;
        while (theta > M_PI) theta -= twoPI;
        
        // Ref: http://stackoverflow.com/questions/4061576/finding-points-on-a-rectangle-at-a-given-angle
        // float aa = 8*gs::width/10.f, bb = 7*gs::height/10.f;
        float aa = gs::width - 240*gs::scale, bb = gs::height - 240*gs::scale;
        float rectAtan = atan2f(bb, aa), tanTheta = tan(theta);
        
        int region;
        if ((theta > -rectAtan) && (theta <= rectAtan)) region = 1;
        else if ((theta > rectAtan) && (theta <= (M_PI - rectAtan))) region = 2;
        else if ((theta > (M_PI - rectAtan)) || (theta <= -(M_PI - rectAtan))) region = 3;
        else region = 4;
        
        sf::Vector2f edgePoint = { gs::width/2.f, gs::height/2.f };
        float xFactor = 1, yFactor = 1;
        
        switch (region)
        {
            case 1: yFactor = -1; break;
            case 2: yFactor = -1; break;
            case 3: xFactor = -1; break;
            case 4: xFactor = -1; break;
        }
        
        if ((region == 1) || (region == 3)) {
            edgePoint.x += xFactor * (aa / 2.);
            edgePoint.y += yFactor * (aa / 2.) * tanTheta;
        } else {
            edgePoint.x += xFactor * (bb / (2. * tanTheta));
            edgePoint.y += yFactor * (bb /  2.); }
        
        return edgePoint;
    }
    float Apartment::NekoHasBeenSleepingFor(const tm* timeinfo)
    {
        if (timeinfo->tm_hour >= 23) return (60 - timeinfo->tm_min)*60 + (60 - timeinfo->tm_sec);
        else return 3600 + (7 - timeinfo->tm_hour)*3600 + (60 - timeinfo->tm_min)*60 + (60 - timeinfo->tm_sec);
    }
    
    
    
    
    
    void Apartment::RegisterMovingEntity(RoomEntity *entity, bool fromInterface)
    {
        if (entity == &neko && neko.activity && !neko.activity->canMove) { return; }
        movedEntity = entity; entityIsBeingMoved = true;
        entityFromInterface = fromInterface; hasFocusOnNeko = false;
        if (!fromInterface) { MoveEntityTo(gs::lastMousePos.first, gs::lastMousePos.second);
            entity->UpdatePosition(); entity->UpdateDepthContinuously(); }
        if (movedEntity == &neko)
        {
            neko.beingActionedWith = true;
            int random = rand() % 5;
            switch (random)
            {
                case 0: neko.SetDialogue(L"Божечки-кошечки!~ >3<", true); break;
                case 1: neko.SetDialogue(L"Осторожнее, позялуста!! >3<", true); break;
                default: neko.SetDialogue(L"Уааа!!!! >3<", true); break;
            }
            if (neko.activity)
            {
                if (neko.movingToEntity)
                    neko.distanceToMovingEntity = sqrt(pow((neko.movingToEntity->x - neko.x), 2) + pow((neko.movingToEntity->y - neko.y), 2));
                if (neko.activity->name != "ReturnToFood") neko.activity->Abort();
                /* if (neko.activity->name == "ComeToSenses" || neko.activity->name == "Eating" || neko.activity->name == "Drinking") neko.activity->Abort();
                else if (neko.activity->name == "ReturnToFood") neko.RemoveItemFromHands(); */
            }
            NekoP::eyesEmotion = NekoS::EyesEmotion::Confused; Player::UpdateNekoEmotion();
        }
        else
        {
            savingIsRequired = true;
            if (neko.activity && neko.moveTo && neko.movingToEntity == entity) {
                neko.distanceToMovingEntity = sqrt(pow((entity->x - neko.x), 2) + pow((entity->y - neko.y), 2));
                neko.moveTo = false; neko.waitTilEntityStopsBeingMoved = true; neko.SetDialogue(L"Дяй!! >.<"); }
            ItemEntity* item = reinterpret_cast<ItemEntity*>(entity);
            if ((hoverIsPossible = item))
            {
                hoverText.setOrigin(0, 0);
                if (item->item->type == ItemType::Food)
                {
                    hoverText.setString(L"Покормить");
                    hoverText.setFillColor(sf::Color(100, 180, 100));
                    hoverText.setOutlineColor(sf::Color::White);
                }
                else if (item->item->type == ItemType::Drink)
                {
                    hoverText.setString(L"Дать попить");
                    hoverText.setFillColor(sf::Color(120, 120, 255));
                    hoverText.setOutlineColor(sf::Color::White);
                }
                else
                {
                    hoverText.setString(L"Дать");
                    hoverText.setFillColor(sf::Color(100, 100, 180));
                    hoverText.setOutlineColor(sf::Color::White);
                }
            }
        }
    }
    void Apartment::ReleaseMovingEntity()
    {
        if (entityIsBeingMoved)
        {
            bool inAreaToDropToInventory{ false }, yeahItIs{ neko.movingToEntity == movedEntity };
            entityIsBeingMoved = drawInventoryButton = lastDrawInventoryButton = false;
            rm::drawScrolldownMenu = true; bool performEntityReposition{ true };
            if (movedEntity == &neko)
            {
                bool actionTaken{ false }; neko.beingActionedWith = false;
                if (nekoHoveringEntity)
                {
                    if (nekoHoveringEntity->sprite.getGlobalBounds().contains(gs::lastMousePos.first, gs::lastMousePos.second))
                    {
                        actionTaken = true; neko.drawDialogue = neko.unlimitedDrawDialogue = false;
                        if (nekoHoveringEntity->id == "Bed") { neko.InsertActivity(adb::activities["Sleeping"]); performEntityReposition = false; }
                        else if (nekoHoveringEntity->id == "Bathtub") neko.InsertActivity(adb::activities["Bathing"]);
                    }
                    nekoHoveringEntity->highlighted = drawHoverText = false; nekoHoveringEntity = nullptr;
                }
                if (!actionTaken) { neko.SetDialogue(L"Уф~~~ т.т"); neko.InsertActivity(adb::activities["ComeToSenses"]); }
            }
            else
            {
                inAreaToDropToInventory = (dot.x > gs::width - 150*gs::scale && dot.y > gs::height - 150*gs::scale);
                if (inAreaToDropToInventory) { DestroyItemEntity(movedEntity, true); movedEntity = nullptr; }
                else if (drawHoverText)
                {
                    ItemEntity* itementity = reinterpret_cast<ItemEntity*>(movedEntity);
                    bool removeFromInventory{ false }, removeFromWorld{ true };
                    if (itementity)
                    {
                        Item* item = itementity->item;
                        if (item->type == ItemType::Food)
                        {
                            if (neko.activity && !neko.activity->canFeed) { removeFromWorld = false;
                                if (neko.activity->name != "Sleeping") neko.SetDialogue(L"Ни сийчас! >3<"); }
                            else
                            {
                                float requiredHunger = 9*NekoS::maxNeed/10;
                                if (item->calories < 100) requiredHunger = NekoS::maxNeed - item->calories/2;
                                removeFromInventory = (NekoS::needHunger <= requiredHunger);
                                if (removeFromInventory)
                                {
                                    /// if (neko.activity && (neko.activity->name == "ComeToSenses")) neko.DropCurrentActivity();
                                    if (neko.activity && (neko.activity->name == "ReturnToFood" || neko.activity->name == "EatFromFridge")) neko.activity->Abort();
                                    auto it = neko.tasks.begin(); while (it != neko.tasks.end())
                                        if ((*it)->name == "ReturnToFood" || (*it)->name == "EatFromFridge") {
                                            (*it)->OnEnd(); it = neko.tasks.erase(it); } else ++it;
                                    neko.PutItemInHands(item); savingIsRequired = true;
                                    neko.InsertActivity(adb::activities["Eating"]);
                                    if (neko.activity) neko.activity->parameter1 = 1;
                                    neko.SetDialogue(L"*Ом-ном-ном*");
                                }
                                else { neko.SetDialogue(L"Ни хотю куфать! >3<"); removeFromWorld = false; }
                            }
                        }
                        else if (item->type == ItemType::Drink)
                        {
                            if (neko.activity && !neko.activity->canFeed) { removeFromWorld = false;
                                if (neko.activity->name != "Sleeping") neko.SetDialogue(L"Ни сийчас! >3<"); }
                            else
                            {
                                float requiredThirst = 9*NekoS::maxNeed/10;
                                if (item->thirstSatisfuction < 100) requiredThirst = NekoS::maxNeed - item->thirstSatisfuction/2;
                                removeFromInventory = (NekoS::needThirst <= requiredThirst);
                                if (removeFromInventory)
                                {
                                    /// if (neko.activity && (neko.activity->name == "ComeToSenses")) neko.DropCurrentActivity();
                                    if (neko.activity && (neko.activity->name == "ReturnToFood" || neko.activity->name == "DrinkFromFridge")) neko.activity->Abort();
                                    auto it = neko.tasks.begin(); while (it != neko.tasks.end())
                                        if ((*it)->name == "ReturnToFood" || (*it)->name == "DrinkFromFridge") {
                                            (*it)->OnEnd(); it = neko.tasks.erase(it); } else ++it;
                                    neko.PutItemInHands(item); savingIsRequired = true;
                                    neko.InsertActivity(adb::activities["Drinking"]);
                                    if (neko.activity) neko.activity->parameter1 = 1;
                                    neko.SetDialogue(L"*Бульк-бульк*");
                                }
                                else { neko.SetDialogue(L"Ни хотю пить! >3<"); removeFromWorld = false; }
                            }
                        }
                        else neko.SetDialogue(L"Ого, какая штука~~ ^o^");
                    }
                    if (removeFromWorld) { entityFromInterface = false;
                        DestroyItemEntity(movedEntity, !removeFromInventory); movedEntity = nullptr; }
                }
            }
            
            if (performEntityReposition && movedEntity && Room::mask && Room::Collision(movedEntity->x, movedEntity->y))
            {
                if (!Room::Collision(lastValidDot.x, lastValidDot.y)) { movedEntity->x = lastValidDot.x; movedEntity->y = lastValidDot.y; }
                else { movedEntity->x = rm::width/2; movedEntity->y = rm::height/2; }
                movedEntity->UpdatePosition(); movedEntity->UpdateDepthPosition();
            }
            
            if ( (inAreaToDropToInventory || movedEntity != &neko) && neko.activity && neko.waitTilEntityStopsBeingMoved && yeahItIs)
            {
                neko.waitTilEntityStopsBeingMoved = false; float distance{ 0 };
                if (!inAreaToDropToInventory && movedEntity) distance = sqrt(pow((movedEntity->x - neko.x), 2) + pow((movedEntity->y - neko.y), 2));
                if (distance > neko.distanceToMovingEntity || inAreaToDropToInventory)
                {
                    neko.SetDialogue(L"Ну и ладно, бука! т.т"); neko.droppedInHands = nullptr;
                    neko.itsTheDroppedFood = false; neko.eatOrDrinkAttemts = 0; neko.activity->Abort();
                    neko.movingToEntity = nullptr;
                }
                else
                {
                    neko.distanceToMovingEntity = distance; neko.moveTo = true;
                    neko.moveUp = neko.moveDown = neko.moveLeft = neko.moveRight = false;
                    neko.movingTo_x = movedEntity->x; neko.movingTo_y = movedEntity->y;
                }
            }
            
            movedEntity = nullptr; drawHoverText = false;
        }
    }
    ItemEntity* Apartment::RegisterItemEntity(Item* itembase, float x, float y, bool removeFromInventory)
    {
        savingIsRequired = Inventory::items.savingIsRequired = true;
        ItemEntity* item = new ItemEntity();
        item->x = x; item->y = y;
        if (removeFromInventory) {
            if (itembase->type == ItemType::Food) neko.iKnowThereIsNoFoodInTheFridge = false;
            else if (itembase->type == ItemType::Drink) neko.iKnowThereIsNoDrinkInTheFridge = false; }
        item->Init(itembase); item->Resize();
        RegisterEntity(item, true);
        
        return item;
    }
    void Apartment::DestroyItemEntity(RoomEntity* entity, bool addToInventory)
    {
        savingIsRequired = true;
        if (addToInventory) { ItemEntity* ent = reinterpret_cast<ItemEntity*>(entity); if (ent) Inventory::items.Add(ent->item->name); }
        UnregisterEntity(entity); drawInventoryButton = false; if (entityFromInterface) this->entity->SendMessage({"InventoryUI :: Show"});
    }
    

    
    void Apartment::RegisterEntity(RoomEntity* entity, bool sorting) {
        entities.push_back(entity); entity->positionInArray = entities.size() - 1;
        entity->vector = &entities; if (sorting) SortEntities(); }
    void Apartment::UnregisterEntity(RoomEntity* entity) { entity->offline = true; }
    void Apartment::SortEntities() {
        std::sort(entities.begin(), entities.end(), [](const RoomEntity* a, const RoomEntity* b) { return a->y < b->y; });
        for (unsigned long i = 0; i < entities.size(); ++i) entities[i]->positionInArray = i; }
    void Apartment::RegisterFurniture(FurnitureEntity* entity, bool sorting) { furnitures.push_back(entity); if (sorting) SortFurniture(); }
    void Apartment::UnregisterFurniture(FurnitureEntity* entity, bool del) {
        for (auto it = furnitures.begin(); it != furnitures.end(); ++it)
            if ((*it) == entity) { if (del) delete entity; furnitures.erase(it); return; } }
    void Apartment::SortFurniture() {
        std::sort(furnitures.begin(), furnitures.end(), [](const FurnitureEntity* a, const FurnitureEntity* b) { return a->y < b->y; }); }
    
    
    
    void Apartment::SaveApartment()
    {
        if (!base::FileExists(utf16(documentsPath())))
            base::CreateDirectory(base::utf16(documentsPath()));
        
        std::wofstream wof;
#ifdef _WIN32
        wof.open(base::utf16(documentsPath()) + L"Apartment.nekoui");
#else
        wof.open(documentsPath() + "Apartment.nekoui");
#endif
        wof.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        if (wof.is_open())
        {
            wof << L"ent" << endl;
            for (auto& e : entities) if (!e->offline) e->Save(wof);
            wof << L"e" << endl;
            
            wof << L"neko" << endl;
            neko.SaveActivity(wof);
            wof << L"e" << endl;
        }
        savingIsRequired = false;
    }
    void Apartment::LoadApartment()
    {
        std::wifstream wif;
#ifdef _WIN32
        wif.open(utf16(documentsPath()) + L"Apartment.nekoui");
#else
        wif.open(documentsPath() + "Apartment.nekoui");
#endif
        wif.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        
        if (wif.is_open())
        {
            std::wstring line;
            nss::CommandSettings command;
            
            while (!wif.eof())
            {
                std::getline(wif, line); command.Command(line);
                if (nss::Command(command, L"ent"))
                {
                    std::getline(wif, line); command.Command(line);
                    while (line != L"e")
                    {
                        int type = nss::ParseAsInt(command); nss::SkipSpaces(command);
                        float x = nss::ParseAsFloat(command); nss::SkipSpaces(command);
                        float y = nss::ParseAsFloat(command); nss::SkipSpaces(command);
                        if (type == 0)
                        {
                            int count = nss::ParseAsInt(command); nss::SkipSpaces(command);
                            std::wstring wname = nss::ParseUntil(command, L'\0');
                            std::string name = utf8(wname);
                            auto it = Inventory::map.find(name);
                            if (it != Inventory::map.end())
                            {
                                Item* itembase = (*it).second;
                                ItemEntity* item = new ItemEntity();
                                item->x = x; item->y = y;
                                item->vector = &entities;
                                item->Init(itembase); item->Resize();
                                entities.push_back(item);
                            }
                        }
                        std::getline(wif, line); command.Command(line);
                    }
                }
                else if (nss::Command(command, L"neko"))
                {
                    std::getline(wif, line); command.Command(line);
                    while (line != L"e")
                    {
                        if (nss::Command(command, L"name "))
                        {
                            loadedActivity = utf8(nss::ParseUntil(command, L'\0'));
                            if (loadedActivity == "ReturnToFood")
                            {
                                std::getline(wif, line); command.Command(line);
                                std::wstring strx = nss::ParseUntil(command, L' '); nss::SkipSpaces(command);
                                std::wstring stry = nss::ParseUntil(command, L'\0');
                                float x = base::atof(strx), y = base::atof(stry);
                                loaded_ReturnToFood_entity = nullptr;
                                for (auto& e : entities) if (e->x == x && e->y == y) { loaded_ReturnToFood_entity = e; break; }
                            }
                        }
                        else if (nss::Command(command, L"position "))
                        {
                            std::wstring strx = nss::ParseUntil(command, L' '); nss::SkipSpaces(command);
                            std::wstring stry = nss::ParseUntil(command, L'\0');
                            loadedNeko_x = base::atof(strx); loadedNeko_y = base::atof(stry);
                        }
                        std::getline(wif, line); command.Command(line);
                    }
                }
            }
            wif.close();
        }
        
        /*std::sort(entities.begin(), entities.end(), [](const RoomEntity* a, const RoomEntity* b) { return a->y < b->y; });
        for (unsigned long i = 0; i < entities.size(); ++i) entities[i]->positionInArray = i;*/
    }
}
