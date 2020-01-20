//
//  Activity.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 10/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "Activity.hpp"

namespace NekoUI
{
    ActivityTask::ActivityTask(Activity* activity) : activity(activity) { }
    ActivityTask::~ActivityTask() { }
    void ActivityTask::Init() { activity->RequestMessage({"Activity :: Update"}); }
    void ActivityTask::Destroy() { }
    void ActivityTask::Update(const sf::Time& elapsedTime) { }
    void ActivityTask::PollEvent(sf::Event& event) { }
    void ActivityTask::Resize(const unsigned int& width, const unsigned int& height) { }
    void ActivityTask::Draw(sf::RenderWindow* window) { }
    void ActivityTask::ReceiveMessage(MessageHolder& message) { }

    Activity::Activity(const std::string name) : name(name) { }
    void Activity::OnStart()
    {
        if (!active) { Init(); active = true; }
        task = tasks.begin();
        doingTheActivity = (*task)->countAsActivityBeingMade;
        ignoreEyebrowsEmotion = (eyebrowsEmotion != NekoS::EyebrowsEmotion::DEFAULT);
        ignoreEyesEmotion = (eyesEmotion != NekoS::EyesEmotion::DEFAULT);
        ignoreMouthEmotion = (mouthEmotion != NekoS::MouthEmotion::DEFAULT);
        if (!ignoreEyebrowsEmotion) eyebrowsEmotion = (*task)->eyebrowsEmotion;
        if (!ignoreEyesEmotion) eyesEmotion = (*task)->eyesEmotion;
        if (!ignoreMouthEmotion) mouthEmotion = (*task)->mouthEmotion;
        blinking = allowBlinking and (*task)->allowBlinking;
        (*task)->Init();
    }
    void Activity::OnEnd(bool success)
    {
        active = doingTheActivity = false;
        vector<ActivityTask*>::iterator it = tasks.begin();
        while (it != tasks.end()) { (*it)->Destroy(); delete (*it); it = tasks.erase(it); }
    }
    void Activity::Init() { }
    void Activity::Destroy() { }
    void Activity::Update(const sf::Time& elapsedTime) { if (active) (*task)->Update(elapsedTime); }
    void Activity::PollEvent(sf::Event& event) { if (active) (*task)->PollEvent(event); }
    void Activity::Resize(const unsigned int& width, const unsigned int& height) { if (active) (*task)->Resize(width, height); }
    void Activity::Draw(sf::RenderWindow* window) { if (active) (*task)->Draw(window); }
    void Activity::ReceiveMessage(MessageHolder& message) {
        if (message.address == (*task) && message.info == "Done") NextTask();
        else if (message.info == "Abort") Abort();
        else if (active) (*task)->ReceiveMessage(message); }
    void Activity::NextTask()
    {
        cout << "Activity :: NextTask" << endl; ++task; if (task == tasks.end()) Abort(false);
        else
        {
            doingTheActivity = (*task)->countAsActivityBeingMade;
            if (!ignoreEyebrowsEmotion) eyebrowsEmotion = (*task)->eyebrowsEmotion;
            if (!ignoreEyesEmotion) eyesEmotion = (*task)->eyesEmotion;
            if (!ignoreMouthEmotion) mouthEmotion = (*task)->mouthEmotion;
            blinking = allowBlinking and (*task)->allowBlinking;
            (*task)->Init();
        }
        
    }
    void Activity::Abort(bool forced) { OnEnd(!forced);
        if (forced) sender->SendMessage({"Activity :: Abort", this}); else sender->SendMessage({"Activity :: Done", this}); }
    void Activity::SendMessage(MessageHolder message) { ReceiveMessage(message); }
    void Activity::RequestMessage(MessageHolder message) { sender->SendMessage(message); }
}
