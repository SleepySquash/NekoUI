//
//  Choice.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 13.02.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#include "Choice.hpp"

namespace ns
{
    namespace NovelComponents
    {
        void Choice::Init()
        {
            appearTime = .4f;
            disappearTime = .4f;
            maxAlpha = 255;
            characterSize = 32;
            fontName = L"Noteworthy-Bold.ttf";
        }
        void Choice::Update(const sf::Time& elapsedTime)
        {
            // if (guiSystem) guiSystem->Update(elapsedTime);
            switch (mode)
            {
                case Mode::Appear: gs::requestWindowRefresh = true;
                    if (currentTime < appearTime) currentTime += elapsedTime.asSeconds();
                    if (currentTime >= appearTime) {
                        alpha = maxAlpha; currentTime = 0.f; mode = Mode::WaitingForInput;
                        if (messageBack == MessageBack::AtAppearance) novelSystem->SendMessage({"UnHold", this});
                    } else alpha = (sf::Uint8)(maxAlpha * (currentTime / appearTime));
                    button.setAlpha(alpha);
                    break;
                    
                case Mode::Disapper: gs::requestWindowRefresh = true;
                    if (currentTime < disappearTime) currentTime += elapsedTime.asSeconds();
                    if (currentTime >= disappearTime) {
                        alpha = 0; currentTime = 0.f; mode = Mode::Deprecate;
                        if (messageBack == MessageBack::AtDeprecated) novelSystem->SendMessage({"UnHold", this});
                    } else alpha = (sf::Uint8)(maxAlpha - (maxAlpha * (currentTime / disappearTime)));
                    button.setAlpha(alpha);
                    break;

                case Mode::Deprecate: gs::requestWindowRefresh = true; novelSystem->PopComponent(this); break;
                default: break;
            }
        }
        void Choice::PollEvent(sf::Event& event)
        {
            if (mode == Mode::WaitingForInput)
            {
                // if (guiSystem && visible) guiSystem->PollEvent(event);
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) visible = !visible;
                if (visible)
                {
                    if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::TouchBegan)
                        button.PollEvent(event);
                    else if (event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::TouchEnded)
                    {
                        int yy = startingYY; bool found{ false };
                        for (int i = 0; i < choices.size() && !found; ++i)
                        {
                            button.setString(choices[i]);
                            button.setPosition(gs::width/2, yy);
                            
                            if ((found = button.PollEvent(event)))
                            {
                                int until = i+1 < choices.size() ? choiceStart[i+1] : actions.size();
                                for (int j = until - 1; j >= choiceStart[i]; --j)
                                    novelSystem->SendMessage({"Push", actions[j]});
                                mode = Mode::Disapper;
                                if (messageBack == MessageBack::AtDisappearance) novelSystem->SendMessage({"ChooseUnHold", this});
                            }
                            
                            yy += button.text.getGlobalBounds().height + 10*gs::scaley;
                        }
                    }
                }
            }
        }
        void Choice::Draw(sf::RenderWindow* window)
        {
            if (!visible) return;
            
            // if (guiSystem) { guiSystem->SetAlpha(alpha); guiSystem->Draw(window); }
            int yy = startingYY;
            for (int i = 0; i < choices.size(); ++i)
            {
                button.setString(choices[i]);
                button.setPosition(gs::width/2, yy);
                button.draw(window);
                
                yy += button.text.getGlobalBounds().height + 10*gs::scaley;
            }
        }
        void Choice::Destroy() { novelSystem->SendMessage({"Destroy", this}); }
        void Choice::Resize(const unsigned int& width, const unsigned int& height)
        {
            // if (guiSystem) guiSystem->Resize(width, height);
            button.Resize(width, height);
            if (choices.size()) button.setString(choices[0]);
            startingYY = (gs::height/2 - (float)((button.text.getGlobalBounds().height + 10) * choices.size())/2) + button.text.getGlobalBounds().height/2;
        }
        void Choice::SetStateMode(const Mode& newMode) { if (mode != newMode) { currentTime = 0.f; mode = newMode; } }
        void Choice::AddChoice(const std::wstring& line) { choices.push_back(line); choiceStart.push_back(actions.size()); }
        void Choice::AddAction(const std::wstring& line) { actions.push_back(line); }
        void Choice::InitChoose()
        {
            /*cout << "   choices: " << endl;
            for (int i = 0; i < choices.size; ++i)
                cout << utf8(choices[i]) << endl;
            
            cout << "   choicesStart: " << endl;
            for (int i = 0; i < choiceStart.size; ++i)
                cout << choiceStart[i] << endl;
            
            cout << "   actions: " << endl;
            for (int i = 0; i < actions.size; ++i)
                cout << utf8(actions[i]) << endl;*/
            
            if (choices.size() == 0)
            {
                if (messageBack != MessageBack::No) novelSystem->SendMessage({"UnHold", this});
                novelSystem->PopComponent(this);
            }
            else
            {
                button.setFont(fontName);
                button.characterSize = characterSize;
                button.ignoreWasPressed = true;
                button.halign = Halign::Center;
                button.valign = Valign::Center;
                
                Resize(gs::width, gs::height);
            }
        }
        void Choice::ReceiveMessage(MessageHolder &message) { if (message.info == "Show/Hide Interface") visible = !visible; }
    }
}
