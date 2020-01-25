//
//  Activity.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 10/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef Activity_hpp
#define Activity_hpp

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#include "../../../Engine/Settings.hpp"
#include "../../../Engine/MessageHolder.hpp"

#include "../Apartment/RoomEntity.hpp"
#include "../Neko/Static.hpp"

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;
using std::vector;

using namespace ns;

namespace NekoUI
{
    struct Activity;
    struct ActivityTask
    {
        bool isSuccess{ false };
        Activity* activity;
        
        bool countAsActivityBeingMade{ false };
        std::wstring occupyString, actionString;
        enum class TaskType { moving, randommoving, movetoentity, pickupitem, consumefromhands, insertactivity, hidedressed, dresshidden, bathing, other } type{ TaskType::other };
        std::wstring movingToObject; float x{ 0 }, y{ 0 };
        RoomEntity* moveToEntity{ nullptr };
        std::string activityToInsert;
        
        bool facingSet{ false }, facing;
        bool allowBlinking{ true };
        
        Neko::s::EyebrowsEmotion eyebrowsEmotion{ Neko::s::EyebrowsEmotion::DEFAULT };
        Neko::s::EyesEmotion eyesEmotion{ Neko::s::EyesEmotion::DEFAULT };
        Neko::s::MouthEmotion mouthEmotion{ Neko::s::MouthEmotion::DEFAULT };
        
        ActivityTask(Activity* activity);
        ~ActivityTask();
        virtual void Init();
        virtual void Destroy();
        virtual void Update(const sf::Time& elapsedTime);
        virtual void PollEvent(sf::Event& event);
        virtual void Resize(const unsigned int& width, const unsigned int& height);
        virtual void Draw(sf::RenderWindow* window);
        virtual void ReceiveMessage(MessageHolder& message);
    };
    
    struct Activity : MessageSender
    {
        std::string name;
        bool active{ false }, doingTheActivity{ false }, forceActivityReplica{ false };
        bool canFeed{ true }, canMove{ true }, canNekoUI{ true };
        MessageSender* sender;
        
        RoomEntity* roomEntity{ nullptr };
        int parameter1{ 0 };
        
        Neko::s::EyebrowsEmotion eyebrowsEmotion{ Neko::s::EyebrowsEmotion::DEFAULT };
        Neko::s::EyesEmotion eyesEmotion{ Neko::s::EyesEmotion::DEFAULT };
        Neko::s::MouthEmotion mouthEmotion{ Neko::s::MouthEmotion::DEFAULT };
        bool ignoreEyebrowsEmotion, ignoreEyesEmotion, ignoreMouthEmotion, allowBlinking{ true }, blinking;
        bool drawShadow{ true }, ignoreBeingActionedWith{ false };
        
        vector<ActivityTask*> tasks;
        vector<ActivityTask*>::iterator task;
        
        Activity(const std::string name);
        void OnStart();
        void OnEnd(bool success = true);
        virtual void Init();
        virtual void Destroy();
        void Update(const sf::Time& elapsedTime);
        void PollEvent(sf::Event& event);
        void Resize(const unsigned int& width, const unsigned int& height);
        void Draw(sf::RenderWindow* window);
        virtual void ReceiveMessage(MessageHolder& message);
        void NextTask();
        void Abort(bool forced = true);
        void SendMessage(MessageHolder message) override;
        void RequestMessage(MessageHolder message);
    };
}

#endif /* Activity_hpp */
