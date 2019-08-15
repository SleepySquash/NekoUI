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
        sf::Texture* texture = ic::LoadTexture(L"Data/Apartment/Background/room.jpg");
        if ((spriteLoaded = texture))
        {
            Room::width = texture->getSize().x * Room::roomScale; Room::height = texture->getSize().y * Room::roomScale;
            sprite.setTexture(*texture, true);
        }
        Room::mask = ic::LoadImage(L"Data/Apartment/Background/room_mask.png");
        
        texture = ic::LoadTexture(L"Data/Images/UI/PointerToNeko.png");
        if (texture) {
            nekoPtrSprite.setTexture(*texture);
            nekoPtrSprite.setOrigin(texture->getSize().x/2, texture->getSize().y/2); }
        texture = ic::LoadTexture(L"Data/Images/UI/PointerToNeko_arrow.png");
        if (texture) {
            nekoArrowSprite.setTexture(*texture);
            nekoArrowSprite.setOrigin(0, texture->getSize().y/2); }
        texture = ic::LoadTexture(L"Data/Images/UI/scrolldown.png");
        if (texture) {
            inventoryMovingSprite.setTexture(*texture);
            inventoryMovingSprite.setTextureRect({124, 0, 97, 126}); }
    }
    void Apartment::Init()
    {
        neko.Init(); neko.sender = entity;
        neko.positionInArray = 0; neko.vector = &entities; entities.push_back(&neko);
        Player::neko.Init(); hasFocusOnNeko = true; rm::scale = 1.7f;
        entity->SendMessage({"NekoUI :: SelectNeko", &neko});
        
        sf::Texture* texture = ic::LoadTexture(L"Data/Images/UI/scrolldown rev1.png");
        if (texture) scrolldownMenuOffsetY = texture->getSize().y;
        
        sf::Font* font = fc::GetFont(L"Impact.ttf");
        if (font) hoverText.setFont(*font);
        
        FurnitureEntity* bed = new FurnitureEntity(); bed->id = "Bed";
        bed->Init(); bed->furnitureScale = 0.52f; bed->LoadSprite(L"bed1.png");
        bed->sprite.setOrigin(bed->sprite.getOrigin().x, 22);
        bed->x = 60 * Room::roomScale; bed->y = 211 * Room::roomScale;
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
    }
    void Apartment::Destroy() { Player::SaveData(); if (savingIsRequired) SaveApartment(); CleanUp(); }
    void Apartment::CleanUp()
    {
        ic::DeleteImage(L"Data/Apartment/Background/room.jpg");
        ic::DeleteImage(L"Data/Apartment/Background/room_mask.png"); Room::mask = nullptr;
        ic::DeleteImage(L"Data/Images/UI/PointerToNeko.png");
        ic::DeleteImage(L"Data/Images/UI/PointerToNeko_arrow.png");
        ic::DeleteImage(L"Data/Images/UI/scrolldown.png");
        ic::DeleteImage(L"Data/Images/UI/scrolldown rev1.png");
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
                    if (dot.x > gs::width - spdx) rm::x += (-spdx + gs::width - dot.x)/sclx;
                    if (dot.x < spdx) rm::x += (spdx - dot.x)/sclx;
                    if (dot.y > gs::height - spdy) rm::y += (-spdy + gs::height - dot.y)/scly;
                    if (dot.y < spdy) rm::y += (spdy - dot.y)/scly;
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
        else if (event.type == sf::Event::Closed || event.type == sf::Event::LostFocus) { Player::SaveData(); if (savingIsRequired) SaveApartment(); }
    }
    void Apartment::Resize(unsigned int width, unsigned int height)
    {
        if (!active) return;
        Room::xWidth = gs::width/(gs::scale * Room::scale); Room::yHeight = gs::height/(gs::scale * Room::scale);
        if (spriteLoaded) sprite.setScale(Room::roomScale * gs::scale * Room::scale, Room::roomScale * gs::scale * Room::scale);
        CalculateCameraScale();
        
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
        if (spriteLoaded) window->draw(sprite);
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
            savingIsRequired = Inventory::items.savingIsRequired = true;
            nss::CommandSettings command; command.Command(message.additional);
            int x = nss::ParseAsFloat(command); nss::SkipSpaces(command);
            int y = nss::ParseAsFloat(command);
            ItemEntity* item = new ItemEntity();
            item->x = x; item->y = y;
            item->Init(reinterpret_cast<Item*>(message.address));
            item->Resize(); RegisterEntity(item, true);
            if (message.info == "SpawnMB :: Item") neko.SendMessage({"MB :: SpawnItem", item});
        }
        else if ((message.info == "Dropping :: Item" || message.info == "DroppingAt :: Item") && message.address)
        {
            nss::CommandSettings command; command.Command(message.additional);
            int x = nss::ParseAsFloat(command); nss::SkipSpaces(command);
            int y = nss::ParseAsFloat(command);
            Item* itembase = reinterpret_cast<Item*>(message.address);
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
            RegisterEntity(item, true);
            
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
            else if (message.additional == L"Computer") neko.SendMessage({"Apartment :: ReceivePosition", std::to_wstring(72*Room::roomScale) + L" " + std::to_wstring(148*Room::roomScale)});
            else if (message.additional == L"Table") neko.SendMessage({"Apartment :: ReceivePosition", std::to_wstring(227*Room::roomScale) + L" " + std::to_wstring(122*Room::roomScale)});
            else if (message.additional == L"KitchenTable") neko.SendMessage({"Apartment :: ReceivePosition", std::to_wstring(363*Room::roomScale) + L" " + std::to_wstring(130*Room::roomScale)});
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
            RoomEntity* closestEntity{ nullptr }; float shortest{ std::numeric_limits<float>::infinity() };
            for (auto& e : entities)
            {
                Item* reitem; if (e->type == RoomEntity::Type::Item) reitem = reinterpret_cast<ItemEntity*>(e)->item;
                if (((e->type == RoomEntity::Type::Item && (reitem->type == ItemType::Food)) || (e->type == RoomEntity::Type::Furniture && e->id == "Fridge" && !neko.iKnowThereIsNoFoodInTheFridge)) && movedEntity != e && !e->offline)
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
                Item* reitem; if (e->type == RoomEntity::Type::Item) reitem = reinterpret_cast<ItemEntity*>(e)->item;
                if (((e->type == RoomEntity::Type::Item && (reitem->type == ItemType::Drink)) || (e->type == RoomEntity::Type::Furniture && e->id == "Fridge" && !neko.iKnowThereIsNoDrinkInTheFridge)) && movedEntity != e && !e->offline)
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
            neko.SendMessage({"Apartment :: DrinkFridgeSpawned"});
        }
        else if (message.info == "Apartment :: GimmeBedInfo") {
            for (auto& e : entities) if ((e->type == RoomEntity::Type::Furniture && e->id == "Bed" && !e->offline)) {
                neko.SendMessage({"Apartment :: BedInfo", e}); return; } }
        else if (message.info == "Apartment :: GimmeBathInfo") {
            for (auto& e : entities) if ((e->type == RoomEntity::Type::Furniture && e->id == "Bathtub" && !e->offline)) {
                neko.SendMessage({"Apartment :: BathInfo", e}); return; } }
        
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
            Neko::eyesEmotion = NekoStatic::EyesEmotion::Confused; Player::UpdateNekoEmotion();
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
            entityIsBeingMoved = drawInventoryButton = lastDrawInventoryButton = false; rm::drawScrolldownMenu = true;
            if (movedEntity == &neko)
            {
                bool actionTaken{ false }; neko.beingActionedWith = false;
                if (nekoHoveringEntity)
                {
                    if (nekoHoveringEntity->sprite.getGlobalBounds().contains(gs::lastMousePos.first, gs::lastMousePos.second))
                    {
                        actionTaken = true; neko.drawDialogue = neko.unlimitedDrawDialogue = false;
                        if (nekoHoveringEntity->id == "Bed") neko.InsertActivity(adb::activities["Sleeping"]);
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
                                float requiredHunger = 9*NStat::maxNeed/10;
                                if (item->calories < 100) requiredHunger = NStat::maxNeed - item->calories/2;
                                removeFromInventory = (NStat::needHunger <= requiredHunger);
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
                                float requiredThirst = 9*NStat::maxNeed/10;
                                if (item->thirstSatisfuction < 100) requiredThirst = NStat::maxNeed - item->thirstSatisfuction/2;
                                removeFromInventory = (NStat::needThirst <= requiredThirst);
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
            
            if (movedEntity && Room::mask && Room::Collision(movedEntity->x, movedEntity->y))
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
            }
            wif.close();
        }
        
        std::sort(entities.begin(), entities.end(), [](const RoomEntity* a, const RoomEntity* b) { return a->y < b->y; });
        for (unsigned long i = 0; i < entities.size(); ++i) entities[i]->positionInArray = i;
    }
}
