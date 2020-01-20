//
//  ActivityDB.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 10/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "ActivityDB.hpp"

namespace NekoUI
{
    namespace ActivityTasks
    {
        RandomMovement::RandomMovement(Activity* activity) : ActivityTask(activity) { type = TaskType::randommoving; }
        void RandomMovement::Update(const sf::Time& elapsedTime)
        {
            elapsedDuration -= elapsedTime.asSeconds();
            if (elapsedDuration <= 0) activity->SendMessage({"Done", this});
        }
        
        
        Waiting::Waiting(Activity* activity, const float& howMuch) : ActivityTask(activity), howMuchToWait(howMuch) { }
        void Waiting::Init() { elapsedDuration = howMuchToWait; activity->RequestMessage({"Activity :: Update"}); }
        void Waiting::Update(const sf::Time& elapsedTime)
        {
            elapsedDuration -= elapsedTime.asSeconds();
            if (elapsedDuration <= 0) activity->SendMessage({"Done", this});
        }
        
        
        MovingTo::MovingTo(Activity* activity, const std::wstring& objectName) : ActivityTask(activity)
        {
            movingToObject = objectName;
            occupyString = L"(идёт)";
        }
        void MovingTo::Init()
        {
            type = TaskType::moving;
            activity->RequestMessage({"Apartment :: RequestPosition", movingToObject});
            /*if (movingToObject != L"" && movingToObject.length()) { activity->RequestMessage({"Apartment :: RequestPosition", movingToObject}); movingToObject = L""; }
            else*/ //activity->RequestMessage({"Activity :: Update"});
        }
        void MovingTo::ReceiveMessage(MessageHolder& message)
        {
            if (message.info == "Apartment :: ReceivePosition")
            {
                if (message.additional == L"0") { activity->Abort(); return; }
                nss::CommandSettings command; command.Command(message.additional);
                x = nss::ParseAsFloat(command); nss::SkipSpaces(command);
                y = nss::ParseAsFloat(command);
                activity->RequestMessage({"Activity :: Update"});
            }
        }
        
        
        ReturnToFood::ReturnToFood(Activity* activity, RoomEntity* foodObject) : ActivityTask(activity) { moveToEntity = foodObject; }
        void ReturnToFood::Init()
        {
            type = TaskType::movetoentity; occupyString = L"(идёт к еде)";
            if (moveToEntity) activity->RequestMessage({"Apartment :: RequestPosition", moveToEntity});
            else activity->Abort();
        }
        void ReturnToFood::ReceiveMessage(MessageHolder& message)
        {
            if (message.info == "Apartment :: ReceivePosition")
            {
                if (message.additional == L"0") { moveToEntity = nullptr; activity->RequestMessage({"Activity :: Update"}); return; }
                nss::CommandSettings command; command.Command(message.additional);
                x = nss::ParseAsFloat(command); nss::SkipSpaces(command);
                y = nss::ParseAsFloat(command);
                activity->RequestMessage({"Activity :: Update"});
            }
        }
        
        
        PickUpItem::PickUpItem(Activity* activity, RoomEntity* item) : ActivityTask(activity) { moveToEntity = item; }
        void PickUpItem::Init()
        {
            type = TaskType::pickupitem;
            if (!moveToEntity) moveToEntity = activity->roomEntity;
            if (moveToEntity) activity->RequestMessage({"Activity :: Update", moveToEntity});
            else activity->SendMessage({"Abort"});
        }
        
        ConsumeItemFromHands::ConsumeItemFromHands(Activity* activity) : ActivityTask(activity) { }
        void ConsumeItemFromHands::Init() { type = TaskType::consumefromhands; activity->RequestMessage({"Activity :: Update", moveToEntity}); }
        
        
        PickFoodFromFridge::PickFoodFromFridge(Activity* activity) : ActivityTask(activity) { }
        void PickFoodFromFridge::Init() { activity->RequestMessage({"Apartment :: RequestFoodFromFridge"}); }
        void PickFoodFromFridge::ReceiveMessage(MessageHolder& message)
        {
            if (message.info == "Apartment :: FoodFridgeSpawned")
            {
                if (!message.address) { activity->Abort(); return; }
                activity->roomEntity = reinterpret_cast<RoomEntity*>(message.address);
                activity->SendMessage({"Done", this});
            }
        }
        
        
        PickDrinkFromFridge::PickDrinkFromFridge(Activity* activity) : ActivityTask(activity) { }
        void PickDrinkFromFridge::Init() { activity->RequestMessage({"Apartment :: RequestDrinkFromFridge"}); }
        void PickDrinkFromFridge::ReceiveMessage(MessageHolder& message)
        {
            if (message.info == "Apartment :: DrinkFridgeSpawned")
            {
                if (!message.address) { activity->Abort(); return; }
                activity->roomEntity = reinterpret_cast<RoomEntity*>(message.address);
                activity->SendMessage({"Done", this});
            }
        }
        
        
        InsertActivity::InsertActivity(Activity* activity, const std::string& activityToInsert) : ActivityTask(activity) { this->activityToInsert = activityToInsert; }
        void InsertActivity::Init() { type = TaskType::insertactivity; activity->RequestMessage({"Activity :: Update"}); }
        
        
        Sleeping::Sleeping(Activity* activity, const float& energyInSecond, const float& howMuch) : ActivityTask(activity), howMuchToWait(howMuch), energyInSecond(energyInSecond) { }
        void Sleeping::Init() { elapsedDuration = howMuchToWait; activity->RequestMessage({"Activity :: Update"}); }
        void Sleeping::Update(const sf::Time& elapsedTime)
        {
            NekoS::needEnergy += energyInSecond * elapsedTime.asSeconds();
            if (NekoS::needEnergy >= energyCap) activity->SendMessage({"Done", this});
            /*elapsedDuration -= elapsedTime.asSeconds();
            if (elapsedDuration <= 0) activity->SendMessage({"Done", this});*/
        }
        
        
        DressOrUndress::DressOrUndress(Activity* activity, const bool& dressing, const float& howMuch, const Cloth& clothes) : ActivityTask(activity), howMuchToWait(howMuch), dressing(dressing), clothes(clothes) { }
        void DressOrUndress::Init() { activity->RequestMessage({"Activity :: ClothCheck", std::to_wstring((int)clothes)}); }
        void DressOrUndress::Update(const sf::Time& elapsedTime)
        {
            if (!active) return;
            elapsedDuration -= elapsedTime.asSeconds();
            if (elapsedDuration <= 0)
            {
                active = false;
                if (dressing) type = TaskType::dresshidden; else type = TaskType::hidedressed;
                activity->RequestMessage({"Activity :: Update", std::to_wstring((int)clothes)});
            }
        }
        void DressOrUndress::ReceiveMessage(MessageHolder& message)
        {
            if (message.info == "ClothCheck :: Yes") { active = true; elapsedDuration = howMuchToWait;
                activity->RequestMessage({"Activity :: Update"}); }
            else if (message.info == "ClothCheck :: No") activity->SendMessage({"Done", this});
        }
        
        
        HideDressed::HideDressed(Activity* activity, const Cloth& whatToHide) : ActivityTask(activity), whatToHide(whatToHide) { }
        void HideDressed::Init() { type = TaskType::hidedressed; activity->RequestMessage({"Activity :: Update", std::to_wstring((int)whatToHide)}); }
        
        
        DressHiden::DressHiden(Activity* activity, const Cloth& whatToShow) : ActivityTask(activity), whatToShow(whatToShow) { }
        void DressHiden::Init() { type = TaskType::dresshidden; activity->RequestMessage({"Activity :: Update", std::to_wstring((int)whatToShow)}); }
    }
    
    
    
    namespace Activities
    {
        RandomMovement::RandomMovement() : Activity("RandomMovement") { };
        void RandomMovement::Init() { tasks.push_back(new ActivityTasks::RandomMovement(this)); tasks.back()->countAsActivityBeingMade = true; }
        
        SittingAtComputer::SittingAtComputer() : Activity("SittingAtComputer") { };
        void SittingAtComputer::Init()
        {
            tasks.push_back(new ActivityTasks::MovingTo(this, L"Computer"));
            tasks.push_back(new ActivityTasks::Waiting(this));
            tasks.back()->occupyString = L"(сёрфит Интернет)";
            tasks.back()->actionString = L"Посёрфить вместе";
            tasks.back()->countAsActivityBeingMade = true;
            tasks.back()->facingSet = true; tasks.back()->facing = false;
        }
        
        SittingAtTable::SittingAtTable() : Activity("SittingAtTable") { };
        void SittingAtTable::Init()
        {
            tasks.push_back(new ActivityTasks::MovingTo(this, L"Table"));
            tasks.push_back(new ActivityTasks::Waiting(this));
            tasks.back()->occupyString = L"(сидит)";
            tasks.back()->actionString = L"Присесть рядышком";
            tasks.back()->countAsActivityBeingMade = true;
            tasks.back()->facingSet = tasks.back()->facing = true;
        }
        
        LookingAtKitchen::LookingAtKitchen() : Activity("LookingAtKitchen") { };
        void LookingAtKitchen::Init()
        {
            tasks.push_back(new ActivityTasks::MovingTo(this, L"KitchenTable"));
            tasks.push_back(new ActivityTasks::Waiting(this));
            tasks.back()->occupyString = L"(смотрит на вкусняшки)";
            tasks.back()->actionString = L"Покормить";
            tasks.back()->countAsActivityBeingMade = true;
            tasks.back()->facingSet = tasks.back()->facing = true;
        }
        
        
        
        Eating::Eating() : Activity("Eating") { };
        void Eating::Init()
        {
            canFeed = canNekoUI = false;
            tasks.push_back(new ActivityTasks::Waiting(this, 3.f));
            tasks.back()->occupyString = L"(кушает)";
        }
        
        Drinking::Drinking() : Activity("Drinking") { };
        void Drinking::Init()
        {
            canFeed = canNekoUI = false;
            tasks.push_back(new ActivityTasks::Waiting(this, 3.f));
            tasks.back()->occupyString = L"(пьёт)";
        }
        
        Sleeping::Sleeping() : Activity("Sleeping") { };
        void Sleeping::Init()
        {
            canFeed = false; forceActivityReplica = true;
            tasks.push_back(new ActivityTasks::Sleeping(this));
            tasks.back()->occupyString = L"(спит)";
            tasks.back()->countAsActivityBeingMade = true;
            eyesEmotion = NekoS::EyesEmotion::Closed;
            allowBlinking = false;
        }
        
        Bathing::Bathing() : Activity("Bathing") { };
        void Bathing::Init()
        {
            canFeed = canMove = canNekoUI = false; forceActivityReplica = true;
            tasks.push_back(new ActivityTasks::DressOrUndress(this, false));
            tasks.back()->occupyString = L"(раздевается)";
            tasks.push_back(new ActivityTasks::Waiting(this, 15.f));
            tasks.back()->occupyString = L"(принимает ванну)";
            tasks.back()->type = ActivityTask::TaskType::bathing;
            tasks.back()->countAsActivityBeingMade = true;
            tasks.push_back(new ActivityTasks::DressOrUndress(this));
            tasks.back()->occupyString = L"(одевается)";
        }
        
        
        
        ReturnToFood::ReturnToFood() : Activity("ReturnToFood") { };
        void ReturnToFood::Init()
        {
            if (roomEntity)
            {
                tasks.push_back(new ActivityTasks::ReturnToFood(this, roomEntity));
                tasks.push_back(new ActivityTasks::PickUpItem(this, roomEntity));
                tasks.push_back(new ActivityTasks::ConsumeItemFromHands(this));
            }
        }
        void ReturnToFood::Destroy() { roomEntity = nullptr; }
        
        EatFromFridge::EatFromFridge() : Activity("EatFromFridge") { };
        void EatFromFridge::Init()
        {
            tasks.push_back(new ActivityTasks::MovingTo(this, L"Fridge"));
            tasks.push_back(new ActivityTasks::PickFoodFromFridge(this));
            tasks.push_back(new ActivityTasks::PickUpItem(this, nullptr));
            tasks.push_back(new ActivityTasks::ConsumeItemFromHands(this));
            tasks.front()->occupyString = L"(ищет чего покушать)";
        }
        DrinkFromFridge::DrinkFromFridge() : Activity("DrinkFromFridge") { };
        void DrinkFromFridge::Init()
        {
            tasks.push_back(new ActivityTasks::MovingTo(this, L"Fridge"));
            tasks.push_back(new ActivityTasks::PickDrinkFromFridge(this));
            tasks.push_back(new ActivityTasks::PickUpItem(this, nullptr));
            tasks.push_back(new ActivityTasks::ConsumeItemFromHands(this));
            tasks.front()->occupyString = L"(ищет чего попить)";
        }
        
        GoSleep::GoSleep() : Activity("GoSleep") { };
        void GoSleep::Init()
        {
            tasks.push_back(new ActivityTasks::MovingTo(this, L"Bed"));
            tasks.push_back(new ActivityTasks::InsertActivity(this, "Sleeping"));
            tasks.front()->occupyString = L"(идёт спать)";
        }
        
        GoTakeABath::GoTakeABath() : Activity("GoTakeABath") { };
        void GoTakeABath::Init()
        {
            tasks.push_back(new ActivityTasks::MovingTo(this, L"Bathtub"));
            tasks.push_back(new ActivityTasks::InsertActivity(this, "Bathing"));
            tasks.front()->occupyString = L"(идёт купаться)";
        }
        
        
        ComeToSenses::ComeToSenses() : Activity("ComeToSenses") { };
        void ComeToSenses::Init()
        {
            tasks.push_back(new ActivityTasks::Waiting(this, 0.5f + (rand() % 1000)/1000.f ));
            tasks.back()->occupyString = L"(приходит в себя)";
            eyesEmotion = NekoS::EyesEmotion::Confused;
        }
        
        ComeToSensesAfterSleep::ComeToSensesAfterSleep() : Activity("ComeToSensesAfterSleep") { };
        void ComeToSensesAfterSleep::Init()
        {
            tasks.push_back(new ActivityTasks::Waiting(this, 3.5f + (rand() % 1000)/1000.f ));
            tasks.back()->occupyString = L"(приходит в себя)";
        }
        
        TeleportationCuzStuck::TeleportationCuzStuck() : Activity("TeleportationCuzStuck") { };
        void TeleportationCuzStuck::Init() { tasks.push_back(new ActivityTasks::Waiting(this, 0.5f)); }
    }
    
    
    
    std::unordered_map<std::string, Activity*> adb::activities;
    std::vector<Activity*> adb::availableActivities;
    void ActivityDatabaseCollection_LoadActivities()
    {
        adb::activities["RandomMovement"] = new Activities::RandomMovement();
        adb::activities["SittingAtComputer"] = new Activities::SittingAtComputer();
        adb::activities["SittingAtTable"] = new Activities::SittingAtTable();
        adb::activities["LookingAtKitchen"] = new Activities::LookingAtKitchen();
        
        adb::activities["Eating"] = new Activities::Eating();
        adb::activities["Drinking"] = new Activities::Drinking();
        adb::activities["Sleeping"] = new Activities::Sleeping();
        adb::activities["Bathing"] = new Activities::Bathing();
        
        adb::activities["ReturnToFood"] = new Activities::ReturnToFood();
        adb::activities["EatFromFridge"] = new Activities::EatFromFridge();
        adb::activities["DrinkFromFridge"] = new Activities::DrinkFromFridge();
        adb::activities["GoSleep"] = new Activities::GoSleep();
        adb::activities["GoTakeABath"] = new Activities::GoTakeABath();
        
        adb::activities["ComeToSenses"] = new Activities::ComeToSenses();
        adb::activities["ComeToSensesAfterSleep"] = new Activities::ComeToSensesAfterSleep();
        adb::activities["TeleportationCuzStuck"] = new Activities::TeleportationCuzStuck();
        
        adb::availableActivities.push_back(adb::activities["RandomMovement"]);
        adb::availableActivities.push_back(adb::activities["SittingAtComputer"]);
        adb::availableActivities.push_back(adb::activities["SittingAtTable"]);
        adb::availableActivities.push_back(adb::activities["LookingAtKitchen"]);
    }
}
