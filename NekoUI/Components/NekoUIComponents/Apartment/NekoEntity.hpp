//
//  NekoEntity.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 17/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef NekoEntity_hpp
#define NekoEntity_hpp

#include <iostream>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include "../../../Essentials/ResourcePath.hpp"
#include "../../../Engine/MessageHolder.hpp"
#include "../../../Engine/Settings.hpp"
#include "../../../Engine/Collectors.hpp"

#include "RoomEntity.hpp"
#include "RoomLibrary.hpp"
#include "../Player.hpp"
#include "../Engine/Persona.hpp"
#include "../Engine/Item.hpp"
#include "../Database/ActivityDB.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::list;
using ns::base::utf8;
using ns::base::utf16;

using namespace ns;

namespace NekoUI
{
    struct NekoEntity : RoomEntity, MessageSender
    {
        bool spriteLoaded{ false };
        MessageSender* sender{ nullptr };
        list<Activity*> tasks;
        Activity* activity{ nullptr };
        
        Activity* abortedActivity{ nullptr };
        int abortedCount{ 0 };
        int eatOrDrinkAttemts{ 0 };
        
        Item* inHands{ nullptr };
        RoomEntity* droppedInHands{ nullptr };
        sf::Sprite inHandsSprite;
        float inHandsScale{ 1.f };
        bool itsTheDroppedFood{ false }, iKnowThereIsNoFoodInTheFridge{ false }, iKnowThereIsNoDrinkInTheFridge{ false };
        
        int previousOccupied{ -1 };
        bool beingOccupied{ false }, drawActionButton{ false };
        sf::Text occupyText; GUI::TextButton occupyActionButton;
        
        bool beingActionedWith{ false }, beingMoved{ false }, sleeping{ false };
        bool moveRight{ false }, moveLeft{ false }, moveDown{ false }, moveUp{ false }, moving{ false };
        float xySpd{ 140 };
        bool chibiFacingIsRight{ true };
        
        RoomEntity* movingToEntity{ nullptr };
        float distanceToMovingEntity{ 0 };
        bool moveTo{ false }, randomMoving{ false }, waitTilEntityStopsBeingMoved{ false };
        float movingTo_x{ 0 }, movingTo_y{ 0 };
        
        bool drawDialogue{ false }, unlimitedDrawDialogue{ false };
        sf::Text dialogue; sf::Sprite dialogueSprite; int previousRandomDialogue{ -1 };
        float elapsedWaiting{ 0 }, waitingTime{ 1.2f }, elapsedDialogue{ (rand() % 12000)/1000.f };
        
        void Init();
        void Destroy() override;
        void Update(const sf::Time& elapsedTime) override;
        void PollEvent(sf::Event& event) override;
        void Resize() override;
        void Draw(sf::RenderWindow* window) override;
        void ReceiveMessage(MessageHolder& message) override;
        void SendMessage(MessageHolder message) override;
        
        void UpdatePosition() override;
        void ResizeDialogue();
        void ResizeOccupied();
        void SetChibiFacingDirection(bool right = true);
        bool ShowOrHidePersonaCloth(const int& type, const bool& hidden = true, const bool& justCheck = false);
        
        void SetDialogue(const std::wstring& dialog, bool unlimited = false);
        std::wstring GenerateRoomDialogue();
        void SetOnScreen();
        
        void ExecuteCurrentActivity();
        void FinishCurrentActivity();
        void InsertActivity(Activity* insertActivity, bool delayCurrent = true);
        void DropCurrentActivity(bool moveToTheNextOneImmediately = true);
        
        void PutItemInHands(Item* item);
        void RemoveItemFromHands(bool drop = true, bool messageBackTheEntity = false);
        
        void Save(std::wofstream& wof) override;
    };
}

#endif /* NekoEntity_hpp */
