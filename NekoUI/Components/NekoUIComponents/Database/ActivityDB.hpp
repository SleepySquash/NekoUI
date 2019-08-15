//
//  ActivityDB.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 10/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef ActivityDB_hpp
#define ActivityDB_hpp

#include <iostream>
#include <vector>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "../../../Engine/Settings.hpp"
#include "../../../Engine/NovelSomeScript.hpp"

#include "../Engine/Activity.hpp"

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;

using namespace ns;

namespace NekoUI
{
    namespace ActivityTasks
    {
        struct RandomMovement : ActivityTask
        {
            float elapsedDuration{ 20.f };
            RandomMovement(Activity* activity);
            void Update(const sf::Time& elapsedTime) override;
        };
        struct Waiting : ActivityTask
        {
            float elapsedDuration, howMuchToWait;
            Waiting(Activity* activity, const float& howMuch = 20.f);
            void Init() override;
            void Update(const sf::Time& elapsedTime) override;
        };
        struct MovingTo : ActivityTask
        {
            MovingTo(Activity* activity, const std::wstring& objectName);
            MovingTo(Activity* activity, const float& x, const float& y);
            void Init() override;
            void ReceiveMessage(MessageHolder& message) override;
        };
        struct ReturnToFood : ActivityTask
        {
            ReturnToFood(Activity* activity, RoomEntity* foodObject);
            void Init() override;
            void ReceiveMessage(MessageHolder& message) override;
        };
        struct PickUpItem : ActivityTask
        {
            PickUpItem(Activity* activity, RoomEntity* item);
            void Init() override;
        };
        struct ConsumeItemFromHands : ActivityTask
        {
            ConsumeItemFromHands(Activity* activity);
            void Init() override;
        };
        struct PickFoodFromFridge : ActivityTask
        {
            PickFoodFromFridge(Activity* activity);
            void Init() override;
            void ReceiveMessage(MessageHolder& message) override;
        };
        struct PickDrinkFromFridge : ActivityTask
        {
            PickDrinkFromFridge(Activity* activity);
            void Init() override;
            void ReceiveMessage(MessageHolder& message) override;
        };
        struct InsertActivity : ActivityTask
        {
            InsertActivity(Activity* activity, const std::string& activityToInsert);
            void Init() override;
        };
        struct Sleeping : ActivityTask
        {
            float elapsedDuration, howMuchToWait, energyInSecond;
            Sleeping(Activity* activity, const float& energyInSecond = 5.f, const float& howMuch = 60.f);
            void Init() override;
            void Update(const sf::Time& elapsedTime) override;
        };
        struct DressOrUndress : ActivityTask
        {
            float elapsedDuration, howMuchToWait; bool dressing, active{ false };
            enum class Cloth{ All, Accessory, Head, Top, Bottom, Onepiece, Gloves, Bra, Pantsu, Socks, Legwear, Outerwear, Underwear } clothes;
            
            DressOrUndress(Activity* activity, const bool& dressing = true, const float& howMuch = 5.f, const Cloth& clothes = Cloth::All);
            void Init() override;
            void Update(const sf::Time& elapsedTime) override;
            void ReceiveMessage(MessageHolder& message) override;
        };
        struct HideDressed : ActivityTask
        {
            enum class Cloth{ All, Accessory, Head, Top, Bottom, Onepiece, Gloves, Bra, Pantsu, Socks, Legwear, Outerwear, Underwear } whatToHide;
            HideDressed(Activity* activity, const Cloth& whatToHide = Cloth::All);
            void Init() override;
        };
        struct DressHiden : ActivityTask
        {
            enum class Cloth{ All, Accessory, Head, Top, Bottom, Onepiece, Gloves, Bra, Pantsu, Socks, Legwear, Outerwear, Underwear } whatToShow;
            DressHiden(Activity* activity, const Cloth& whatToShow = Cloth::All);
            void Init() override;
        };
    }
    
    namespace Activities
    {
        struct RandomMovement : Activity { RandomMovement(); void Init() override; };
        struct SittingAtComputer : Activity { SittingAtComputer(); void Init() override; };
        struct SittingAtTable : Activity { SittingAtTable(); void Init() override; };
        struct LookingAtKitchen : Activity { LookingAtKitchen(); void Init() override; };
        
        struct Eating : Activity { Eating(); void Init() override; };
        struct Drinking : Activity { Drinking(); void Init() override; };
        struct Sleeping : Activity { Sleeping(); void Init() override; };
        struct Bathing : Activity { Bathing(); void Init() override; };
        
        struct ReturnToFood : Activity { ReturnToFood(); void Init() override; void Destroy() override; };
        struct EatFromFridge : Activity { EatFromFridge(); void Init() override; };
        struct DrinkFromFridge : Activity { DrinkFromFridge(); void Init() override; };
        struct GoSleep : Activity { GoSleep(); void Init() override; };
        struct GoTakeABath : Activity { GoTakeABath(); void Init() override; };
        
        struct ComeToSenses : Activity { ComeToSenses(); void Init() override; };
        struct ComeToSensesAfterSleep : Activity { ComeToSensesAfterSleep(); void Init() override; };
        struct TeleportationCuzStuck : Activity { TeleportationCuzStuck(); void Init() override; };
    }
    
    void ActivityDatabaseCollection_LoadActivities();
    struct ActivityDB
    {
        static std::unordered_map<std::string, Activity*> activities;
        static std::vector<Activity*> availableActivities;
    };
    
    typedef ActivityDB adb;
}

#endif /* ActivityDB_hpp */
