//
//  NovelUpdate.cpp
//  NovelSome
//
//  Created by Никита Исаенко on 11/09/2018.
//  Copyright © 2018 Melancholy Hill. All rights reserved.
//

#include "Novel.hpp"

namespace ns
{
    namespace NovelComponents
    {
        void Novel::Update(const sf::Time& elapsedTime)
        {
            while (!lines.empty() && onHold.empty() && !gs::isPause)
            {
                // if (!nss::Command(lines.front(), L"choose")) ResourcesPreloading(lines, line);
                line = lines.back(); lines.pop_back();
                if (lines.empty()) { cout << "Warning :: NovelComponent :: lines.empty() caught." << endl; }
                command.Command(line);
                
                bool backgroundAddingMode{ false };
                if (nss::Command(command, L"//")) { /* oh, that's a comment... */ }
                ///---------------------------------------DIALOGUE---------------------------------------
                ///---------------------------------------DIALOGUE---------------------------------------
                ///---------------------------------------DIALOGUE---------------------------------------
                else if (nss::Command(command, L"\""))
                {
                    command.lastPos = command.lastPos - 1;
                    std::wstring dialogueLine = nss::ParseAsQuoteString(command);
                    
                    if (dialogueGroup.size()) for (auto d : dialogueGroup) ++d->priority;
                    auto* component = layers.PrioritizeComponent<Dialogue>(10000);
                    if (command.ExplicitNoMessage()) component->messageBack = MessageBack::No;
                    
                    vector<std::wstring> arguments;
                    nss::ParseArguments(command, arguments);
                    for (auto it = arguments.begin(); it != arguments.end(); ++it)
                    {
                        nss::CommandSettings argument; argument.Command(*it);
                        if (nss::Command(argument, L"fade:"))
                        {
                            float value = nss::ArgumentAsFloat(argument);
                            component->appearTime = value;
                            component->disappearTime = value;
                        }
                        else if (nss::Command(argument, L"fadein:") || nss::Command(argument, L"appear:"))
                            component->appearTime = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"fadeout:") || nss::Command(argument, L"disappear:"))
                            component->disappearTime = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"alpha:") || nss::Command(argument, L"maxalpha:"))
                            component->maxAlpha = nss::ArgumentAsInt(argument);
                        else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                        {
                            std::wstring stringValue = nss::ArgumentAsString(argument);
                            if (stringValue == L"atappearance" || stringValue == L"appearance")
                                component->messageBack = MessageBack::AtAppearance;
                            else if (stringValue == L"atdisappearing" || stringValue == L"disappearing")
                                component->messageBack = MessageBack::AtDisappearance;
                            else if (stringValue == L"atdeprecated" || stringValue == L"deprecated")
                                component->messageBack = MessageBack::AtDeprecated;
                            else if (stringValue == L"nomessage" || stringValue == L"no")
                                component->messageBack = MessageBack::No;
                        }
                        else if (nss::Command(argument, L"afterappearswitchto:") || nss::Command(argument, L"switchby:"))
                        {
                            std::wstring stringValue = nss::ArgumentAsString(argument);
                            if (stringValue == L"waitingforinput" || stringValue == L"input")
                                component->switchTo = Mode::WaitingForInput;
                            else if (stringValue == L"waitingfortime" || stringValue == L"time")
                                component->switchTo = Mode::WaitingForTime;
                        }
                        else if (nss::Command(argument, L"waitingtime:") || nss::Command(argument, L"time:"))
                            component->waitingTime = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"depth:") || nss::Command(argument, L"layer:") || nss::Command(argument, L"d:"))
                            layers.ChangePriorityOf(component, nss::ArgumentAsInt(argument));
                    }
                    
                    if (component->messageBack != MessageBack::No) OnHold(component);
                    dialogueGroup.insert(dialogueGroup.begin(), component);
                    
                    component->SetDialogue(dialogueLine);
                }
                else if (nss::Command(command, L"choose") || nss::Command(command, L"choice"))
                {
                    int openedWithBracket{ 0 }, spacesCount{ 0 };
                    for (int i = 0; i < line.length(); ++i) {
                        if (line[i] == L'{') ++openedWithBracket;
                        else if (line[i] == L'}') if (openedWithBracket > 0) --openedWithBracket; }
                    if (openedWithBracket == 0) spacesCount = command.lastPos - 6;
                    else spacesCount = -1;
                    
                    bool mightBeOnNextLine{ (openedWithBracket == 0) };
                    int lastChooseSpaces = -1;
                    
                    bool chooseEnded{ false }, explicitChoose{ false };
                    auto* component = layers.PrioritizeComponent<Choice>(10000);
                    if (command.ExplicitNoMessage()) component->messageBack = MessageBack::No;
                    
                    std::wstring question = L"";
                    bool dialogueShouldBeStill{ false }; //TODO: Default to Skin
                    
                    vector<std::wstring> arguments;
                    nss::ParseArguments(command, arguments);
                    for (auto it = arguments.begin(); it != arguments.end(); ++it)
                    {
                        nss::CommandSettings argument; argument.Command(*it);
                        if (nss::Command(argument, L"fade:"))
                        {
                            float value = nss::ArgumentAsFloat(argument);
                            component->appearTime = value;
                            component->disappearTime = value;
                        }
                        else if (nss::Command(argument, L"fadein:") || nss::Command(argument, L"appear:"))
                            component->appearTime = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"fadeout:") || nss::Command(argument, L"disappear:"))
                            component->disappearTime = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"alpha:") || nss::Command(argument, L"maxalpha:"))
                            component->maxAlpha = nss::ArgumentAsInt(argument);
                        else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                        {
                            std::wstring stringValue = nss::ArgumentAsString(argument);
                            if (stringValue == L"atappearance" || stringValue == L"appearance")
                                component->messageBack = MessageBack::AtAppearance;
                            else if (stringValue == L"atdisappearing" || stringValue == L"disappearing")
                                component->messageBack = MessageBack::AtDisappearance;
                            else if (stringValue == L"atdeprecated" || stringValue == L"deprecated")
                                component->messageBack = MessageBack::AtDeprecated;
                            else if (stringValue == L"nomessage" || stringValue == L"no")
                                component->messageBack = MessageBack::No;
                        }
                        else if (nss::Command(argument, L"still") || nss::Command(argument, L"dialogue"))
                            dialogueShouldBeStill = true;
                        else if (nss::Command(argument, L"depth:") || nss::Command(argument, L"layer:") || nss::Command(argument, L"d:"))
                            layers.ChangePriorityOf(component, nss::ArgumentAsInt(argument));
                        else question = nss::ParseAsMaybeQuoteString(argument);
                    }
                    
                    //cout << "openedWithBracket: " << openedWithBracket << "  spacesCount: " << spacesCount << "  question: " << ns::base::utf8(question) << endl;
                    while (!chooseEnded && !lines.empty())
                    {
                        if (!lines.empty())
                        {
                            std::wstring parsed{ L"" };
                            if (lines.empty()) {
                                /*std::getline(wif, parsed);*/ ++position;
                                if (line.length() && line[line.length() - 1] == 13) line.erase(line.begin() + (line.size() - 1));
                            } else { parsed = lines.back(); lines.pop_back(); }
                            
                            if (nss::Command(parsed, L"end")) chooseEnded = true;
                            else
                            {
                                if (mightBeOnNextLine || openedWithBracket != 0)
                                {
                                    for (int i = 0; i < parsed.length(); ++i)
                                    {
                                        if (parsed[i] == L'{') ++openedWithBracket;
                                        else if (parsed[i] == L'}') --openedWithBracket;// = false;
                                    }
                                    
                                    if (mightBeOnNextLine && openedWithBracket != 0)
                                    {
                                        std::wstring newParsed; bool found{ false };
                                        for (int i = 0; i < parsed.length() && !found; ++i)
                                        {
                                            if (parsed[i] == L'{')
                                            {
                                                found = true;
                                                for (int j = i + 1; j < parsed.length(); ++j)
                                                    newParsed += parsed[j];
                                            }
                                        }
                                        parsed = newParsed;
                                        
                                        spacesCount = -1;
                                        mightBeOnNextLine = false;
                                    }
                                    
                                    if (!mightBeOnNextLine && openedWithBracket == 0)
                                    {
                                        std::wstring newParsed; bool found{ false };
                                        for (int i = parsed.length() - 1; i >= 0 && !found; --i)
                                        {
                                            if (parsed[i] == L'}')
                                            {
                                                found = true;
                                                for (int j = 0; j < i; ++j)
                                                    newParsed += parsed[j];
                                            }
                                        }
                                        parsed = newParsed;
                                        chooseEnded = true;
                                    }
                                }
                                
                                if (openedWithBracket == 0 && spacesCount != -1)
                                {
                                    int currentSpaces = -1;
                                    for (int i = 0; i < parsed.length() && currentSpaces == -1; ++i)
                                        if (parsed[i] != L' ' && parsed[i] != L'\t') currentSpaces = i;
                                    
                                    if (currentSpaces != -1 && currentSpaces <= spacesCount)
                                        chooseEnded = true;
                                    if (chooseEnded) { lines.push_back(parsed); parsed = L""; }
                                }
                                
                                if (parsed.length() != 0)
                                {
                                    nss::CommandSettings settings;
                                    settings.Command(parsed);
                                    
                                    if (nss::ContainsUsefulInformation(settings))
                                    {
                                        mightBeOnNextLine = false;
                                        
                                        if (nss::Command(settings, L"choice "))
                                        {
                                            int currentSpaces = -1;
                                            for (int i = 0; i < parsed.length() && currentSpaces == -1; ++i)
                                                if (parsed[i] != L' ' && parsed[i] != L'\t') currentSpaces = i;
                                            lastChooseSpaces = currentSpaces;
                                            
                                            explicitChoose = true;
                                            component->AddChoice(nss::ParseAsMaybeQuoteStringFull(settings));
                                        }
                                        else
                                        {
                                            if (lastChooseSpaces == -1)
                                            {
                                                int currentSpaces = -1;
                                                for (int i = 0; i < parsed.length() && currentSpaces == -1; ++i)
                                                    if (parsed[i] != L' ' && parsed[i] != L'\t') currentSpaces = i;
                                                
                                                if (currentSpaces != -1 && currentSpaces > spacesCount)
                                                {
                                                    lastChooseSpaces = currentSpaces;
                                                    component->AddChoice(nss::ParseAsMaybeQuoteStringFull(settings));
                                                }
                                                else component->AddAction(parsed);
                                            }
                                            else
                                            {
                                                int currentSpaces = -1;
                                                for (int i = 0; i < parsed.length() && currentSpaces == -1; ++i)
                                                    if (parsed[i] != L' ' && parsed[i] != L'\t') currentSpaces = i;
                                                
                                                if (currentSpaces != -1 && currentSpaces > lastChooseSpaces)
                                                    { component->AddAction(parsed); explicitChoose = false; }
                                                else if (currentSpaces != -1 && currentSpaces <= lastChooseSpaces)
                                                {
                                                    if ((currentSpaces == lastChooseSpaces || currentSpaces > spacesCount) && !explicitChoose)
                                                        component->AddChoice(nss::ParseAsMaybeQuoteStringFull(settings));
                                                    else component->AddAction(parsed);
                                                }
                                                else component->AddAction(parsed);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    
                    if (dialogueShouldBeStill && dialogueGroup.size() != 0)
                    {
                        for (auto d : dialogueGroup)
                            if (d->mode != Mode::Disapper && d->mode != Mode::Deprecate)
                            {
                                if (d->mode == Mode::Appear)
                                    d->switchTo = Mode::WaitingForChoose;
                                else d->SetStateMode(Mode::WaitingForChoose);
                            }
                    }
                    
                    if (component->messageBack != MessageBack::No) OnHold(component);
                    chooseGroup.insert(chooseGroup.begin(), component);
                    component->InitChoose();
                }
                ///----------------------------------------MISC----------------------------------------
                ///----------------------------------------MISC----------------------------------------
                ///----------------------------------------MISC----------------------------------------
                else if (nss::Command(command, L"event block")) gs::ignoreEvent = true;
                else if (nss::Command(command, L"event unblock")) gs::ignoreEvent = false;
                else if (nss::Command(command, L"draw block")) gs::ignoreDraw = true;
                else if (nss::Command(command, L"draw unblock")) gs::ignoreDraw = false;
                else if (nss::Command(command, L"wait"))
                {
                    nss::SkipSpaces(command);
                    float amount{ 1.f };
                    if (nss::ContainsUsefulInformation(command)) amount = ArgumentAsFloat(command);
                    if (amount > 0.f) {
                        auto* component = layers.PrioritizeComponent<Waiting>(-32000);
                        OnHold(component); component->waitingTime = amount;
                    } else cout << "Warning :: NovelComponent :: Couldn't init \"wait\" command for " << amount << " seconds." << endl;
                }
                else if (nss::Command(command, L"fadeout") || nss::Command(command, L"fade") || nss::Command(command, L"disappear"))
                {
                    nss::SkipSpaces(command);
                    float disappearTime{ 1.f };
                    
                    MessageBack messageBack{ MessageBack::AtDeprecated };
                    if (command.ExplicitNoMessage()) messageBack = MessageBack::No;
                    if (nss::ContainsUsefulInformation(command)) disappearTime = ArgumentAsFloat(command);
                    
                    if (disappearTime >= 0.f)
                    {
                        vector<std::wstring> arguments;
                        nss::ParseArguments(command, arguments);
                        for (auto it = arguments.begin(); it != arguments.end(); ++it)
                        {
                            nss::CommandSettings argument; argument.Command(*it);
                            if (nss::Command(argument, L"for") || nss::Command(argument, L"seconds")) { }
                            else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                            {
                                std::wstring stringValue = nss::ArgumentAsString(argument);
                                if (stringValue == L"atdisappearing" || stringValue == L"disappearing") messageBack = MessageBack::AtDisappearance;
                                else if (stringValue == L"atdeprecated" || stringValue == L"deprecated") messageBack = MessageBack::AtDeprecated;
                                else if (stringValue == L"nomessage" || stringValue == L"no") messageBack = MessageBack::No;
                            }
                        }
                        
                        if (backgroundGroup.size())
                            for (auto b : backgroundGroup)
                            {
                                if (messageBack != MessageBack::No) OnHold(b);
                                switch (messageBack)
                                {
                                    case MessageBack::AtDisappearance: b->messageBack = MessageBack::AtDisappearance; break;
                                    case MessageBack::No: b->messageBack = MessageBack::No; break;
                                    default: b->messageBack = MessageBack::AtDeprecated; break;
                                }
                                if (disappearTime >= 0) b->disappearTime = disappearTime;
                                b->SetStateMode(Mode::Disapper);
                                b->hideAfter = nullptr;
                            }
                        if (characterGroup.size())
                            for (auto c : characterGroup)
                            {
                                if (messageBack != MessageBack::No) OnHold(c);
                                switch (messageBack)
                                {
                                    case MessageBack::AtDisappearance: c->messageBack = MessageBack::AtDisappearance; break;
                                    case MessageBack::No: c->messageBack = MessageBack::No; break;
                                    default: c->messageBack = MessageBack::AtDeprecated; break;
                                }
                                if (disappearTime >= 0) c->disappearTime = disappearTime;
                                c->SetStateMode(Mode::Disapper);
                            }
                        if (personaGroup.size())
                            for (auto c : personaGroup)
                            {
                                if (messageBack != MessageBack::No) OnHold(c);
                                switch (messageBack)
                                {
                                    case MessageBack::AtDisappearance: c->messageBack = MessageBack::AtDisappearance; break;
                                    case MessageBack::No: c->messageBack = MessageBack::No; break;
                                    default: c->messageBack = MessageBack::AtDeprecated; break;
                                }
                                if (disappearTime >= 0) c->disappearTime = disappearTime;
                                c->SetStateMode(Mode::Disapper);
                            }
                        if (musicGroup.size())
                            for (auto m : musicGroup)
                            {
                                if (messageBack != MessageBack::No) OnHold(m);
                                switch (messageBack)
                                {
                                    case MessageBack::AtDisappearance: m->messageBack = MessageBack::AtDisappearance; break;
                                    case MessageBack::No: m->messageBack = MessageBack::No; break;
                                    default: m->messageBack = MessageBack::AtDeprecated; break;
                                }
                                if (disappearTime >= 0) m->disappearTime = disappearTime;
                                m->SetStateMode(Mode::Disapper);
                            }
                        if (ambientGroup.size())
                            for (auto m : ambientGroup)
                            {
                                if (messageBack != MessageBack::No) OnHold(m);
                                switch (messageBack)
                                {
                                    case MessageBack::AtDisappearance: m->messageBack = MessageBack::AtDisappearance; break;
                                    case MessageBack::No: m->messageBack = MessageBack::No; break;
                                    default: m->messageBack = MessageBack::AtDeprecated; break;
                                }
                                if (disappearTime >= 0) m->disappearTime = disappearTime;
                                m->SetStateMode(Mode::Disapper);
                            }
                        if (soundGroup.size())
                            for (auto m : soundGroup)
                            {
                                if (messageBack != MessageBack::No) OnHold(m);
                                switch (messageBack)
                                {
                                    case MessageBack::AtDisappearance: m->messageBack = MessageBack::AtDisappearance; break;
                                    case MessageBack::No: m->messageBack = MessageBack::No; break;
                                    default: m->messageBack = MessageBack::AtDeprecated; break;
                                }
                                if (disappearTime >= 0) m->disappearTime = disappearTime;
                                m->SetStateMode(Mode::Disapper);
                            }
                    }
                    else
                        cout << "Warning :: NovelComponent :: Couldn't init \"fade\" command for " << disappearTime << " seconds." << endl;
                }
                ///--------------------------------------BACKGROUND--------------------------------------
                ///--------------------------------------BACKGROUND--------------------------------------
                ///--------------------------------------BACKGROUND--------------------------------------
                else if (nss::Command(command, L"background hide"))
                {
                    if (backgroundGroup.size() != 0)
                    {
                        float disappearTime{ -1.f };
                        enum sendMessageBackEnum{ atDisappearing, atDeprecated, noMessage };
                        sendMessageBackEnum sendMessageBack{ atDeprecated };
                        if (command.ExplicitNoMessage()) sendMessageBack = noMessage;
                        
                        vector<std::wstring> arguments;
                        nss::ParseArguments(command, arguments);
                        for (auto it = arguments.begin(); it != arguments.end(); ++it)
                        {
                            nss::CommandSettings argument; argument.Command(*it);
                            if (nss::Command(argument, L"fade:")||
                                nss::Command(argument, L"fadeout:") ||
                                nss::Command(argument, L"disappear:"))
                                disappearTime = nss::ArgumentAsFloat(argument);
                            else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                            {
                                std::wstring stringValue = nss::ArgumentAsString(argument);
                                if (stringValue == L"atdisappearing" || stringValue == L"disappearing") sendMessageBack = atDisappearing;
                                else if (stringValue == L"atdeprecated" || stringValue == L"deprecated") sendMessageBack = atDeprecated;
                                else if (stringValue == L"nomessage" || stringValue == L"no") sendMessageBack = noMessage;
                            }
                        }
                        
                        for (auto b : backgroundGroup)
                        {
                            if (sendMessageBack != noMessage) OnHold(b);
                            switch (sendMessageBack)
                            {
                                case atDeprecated: b->messageBack = MessageBack::AtDeprecated; break;
                                case atDisappearing: b->messageBack = MessageBack::AtDisappearance; break;
                                case noMessage: b->messageBack = MessageBack::No; break;
                                default: b->messageBack = MessageBack::AtDeprecated; break;
                            }
                            if (disappearTime >= 0) b->disappearTime = disappearTime;
                            b->SetStateMode(Mode::Disapper);
                            b->hideAfter = nullptr;
                        }
                    }
                }
                else if ((backgroundAddingMode = nss::Command(command, L"background add ")) || nss::Command(command, L"background ") || nss::Command(command, L"задний фон "))
                {
                    std::wstring filePath = nss::ParseAsQuoteString(command);
                    auto* component = layers.PrioritizeComponent<ns::NovelComponents::Background>(0);
                    
                    if (!backgroundAddingMode && backgroundGroup.size() != 0)
                        for (auto b : backgroundGroup)
                        {
                            b->messageBack = MessageBack::No;
                            b->hideAfter = component;
                            // b->SetStateMode(b->disappearing);
                        }
                    
                    component->folder = folder;
                    component->fitMode = component->fillCentre;
                    if (command.ExplicitNoMessage()) component->messageBack = MessageBack::No;
                    
                    vector<std::wstring> arguments;
                    nss::ParseArguments(command, arguments);
                    for (auto it = arguments.begin(); it != arguments.end(); ++it)
                    {
                        nss::CommandSettings argument; argument.Command(*it);
                        if (nss::Command(argument, L"fade:"))
                        {
                            float value = nss::ArgumentAsFloat(argument);
                            component->appearTime = value;
                            component->disappearTime = value;
                        }
                        else if (nss::Command(argument, L"fadein:") || nss::Command(argument, L"appear:"))
                            component->appearTime = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"fadeout:") || nss::Command(argument, L"disappear:"))
                            component->disappearTime = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"alpha:") || nss::Command(argument, L"maxalpha:"))
                            component->maxAlpha = nss::ArgumentAsInt(argument);
                        else if (nss::Command(argument, L"fit:"))
                        {
                            std::wstring stringValue = nss::ArgumentAsString(argument);
                            if (stringValue == L"fill")
                                component->fitMode = component->fillCentre;
                            else if (stringValue == L"center")
                                component->fitMode = component->fillCentre;
                            else if (stringValue == L"fillcenter")
                                component->fitMode = component->fillCentre;
                            else if (stringValue == L"default")
                                component->fitMode = component->defaultFit;
                            else if (stringValue == L"no")
                                component->fitMode = component->noFit;
                            else if (stringValue == L"nofit")
                                component->fitMode = component->noFit;
                            else if (stringValue == L"stretch")
                                component->fitMode = component->stretch;
                        }
                        else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                        {
                            std::wstring stringValue = nss::ArgumentAsString(argument);
                            if (stringValue == L"atappearance" || stringValue == L"appearance")
                                component->messageBack = MessageBack::AtAppearance;
                            else if (stringValue == L"atdisappearing" || stringValue == L"disappearing")
                                component->messageBack = MessageBack::AtDisappearance;
                            else if (stringValue == L"atdeprecated" || stringValue == L"deprecated")
                                component->messageBack = MessageBack::AtDeprecated;
                            else if (stringValue == L"nomessage" || stringValue == L"no")
                                component->messageBack = MessageBack::No;
                        }
                        else if (nss::Command(argument, L"parallax:"))
                        {
                            std::wstring possibleParallax = nss::ArgumentAsString(argument);
                            if (possibleParallax == L"normal" || possibleParallax == L"n")
                                component->parallaxPower = ns::GlobalSettings::defaultParallaxNormal;
                            else if (possibleParallax == L"close" || possibleParallax == L"c")
                                component->parallaxPower = ns::GlobalSettings::defaultParallaxClose;
                            else if (possibleParallax == L"far" || possibleParallax == L"f")
                                component->parallaxPower = ns::GlobalSettings::defaultParallaxFar;
                            else if (possibleParallax == L"background" || possibleParallax == L"back" || possibleParallax == L"b")
                                component->parallaxPower = ns::GlobalSettings::defaultParallaxBackground;
                            else if (possibleParallax == L"frontground" || possibleParallax == L"front" || possibleParallax == L"f")
                                component->parallaxPower = ns::GlobalSettings::defaultParallaxFrontground;
                            else
                                component->parallaxPower = base::atof(possibleParallax);
                        }
                        else if (nss::Command(argument, L"depth:") || nss::Command(argument, L"layer:") || nss::Command(argument, L"d:"))
                            layers.ChangePriorityOf(component, nss::ArgumentAsInt(argument));
                    }
                    
                    if (component->messageBack != MessageBack::No) OnHold(component);
                    
                    backgroundGroup.insert(backgroundGroup.begin(), component);
                    component->LoadImage(filePath);
                }
                ///---------------------------------------CHARACTER--------------------------------------
                ///---------------------------------------CHARACTER--------------------------------------
                ///---------------------------------------CHARACTER--------------------------------------
                else if (nss::Command(command, L"show neko"))
                {
                    bool noMessage{ false }; if (command.ExplicitNoMessage()) noMessage = true;
                    if (CharacterLibrary::nekoPersona)
                    {
                        auto* component = layers.PrioritizeComponent<ns::NovelComponents::Persona>(5000, reinterpret_cast<NekoUI::Persona*>(CharacterLibrary::nekoPersona));
                        component->position = Position::Center;
                        if (noMessage) component->messageBack = MessageBack::No;
                        
                        std::wstring state{ L"" };
                        
                        vector<std::wstring> arguments;
                        nss::ParseArguments(command, arguments);
                        for (auto it = arguments.begin(); it != arguments.end(); ++it)
                        {
                            nss::CommandSettings argument; argument.Command(*it);
                            if (nss::Command(argument, L"fade:"))
                            {
                                float value = nss::ArgumentAsFloat(argument);
                                component->appearTime = value;
                                component->disappearTime = value;
                            }
                            else if (nss::Command(argument, L"fadein:") || nss::Command(argument, L"appear:"))
                                component->appearTime = nss::ArgumentAsFloat(argument);
                            else if (nss::Command(argument, L"fadeout:") || nss::Command(argument, L"disappear:"))
                                component->disappearTime = nss::ArgumentAsFloat(argument);
                            else if (nss::Command(argument, L"alpha:") || nss::Command(argument, L"maxalpha:"))
                                component->maxAlpha = nss::ArgumentAsInt(argument);
                            else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                            {
                                std::wstring stringValue = nss::ArgumentAsString(argument);
                                if (stringValue == L"atappearance" || stringValue == L"appearance")
                                    component->messageBack = MessageBack::AtAppearance;
                                else if (stringValue == L"atdisappearing" || stringValue == L"disappearing")
                                    component->messageBack = MessageBack::AtDisappearance;
                                else if (stringValue == L"atdeprecated" || stringValue == L"deprecated")
                                    component->messageBack = MessageBack::AtDeprecated;
                                else if (stringValue == L"nomessage" || stringValue == L"no")
                                    component->messageBack = MessageBack::No;
                            }
                            else if (nss::Command(argument, L"depth:") || nss::Command(argument, L"layer:") || nss::Command(argument, L"d:"))
                                layers.ChangePriorityOf(component, nss::ArgumentAsInt(argument));
                            else
                            {
                                std::wstring possibleStateOrPos;
                                if (argument.line[0] == L'"')
                                    possibleStateOrPos = nss::ParseAsQuoteString(argument);
                                else possibleStateOrPos = argument.line;
                                
                                if (possibleStateOrPos == L"l" || possibleStateOrPos == L"left")
                                    component->position = Position::Left;
                                else if (possibleStateOrPos == L"cl" || possibleStateOrPos == L"cleft")
                                    component->position = Position::CLeft;
                                else if (possibleStateOrPos == L"c" || possibleStateOrPos == L"center")
                                    component->position = Position::Center;
                                else if (possibleStateOrPos == L"cr" || possibleStateOrPos == L"cright")
                                    component->position = Position::CRight;
                                else if (possibleStateOrPos == L"r" || possibleStateOrPos == L"right")
                                    component->position = Position::Right;
                                else state = possibleStateOrPos;
                            }
                        }
                        //if (arguments.size() && arguments[arguments.size() - 1].length() && arguments[arguments.size() - 1][arguments[arguments.size() - 1].length() - 1] == L'!') component->sendMessageBack = component->noMessage;
                        
                        if (component->messageBack != MessageBack::No) OnHold(component);
                        personaGroup.insert(personaGroup.begin(), component);
                    }
                }
                else if (nss::Command(command, L"show "))
                {
                    bool noMessage{ false }; if (command.ExplicitNoMessage()) noMessage = true;
                    std::wstring possibleName = nss::ParseUntil(command, ' ');
                    if (possibleName.length() != 0)
                    {
                        if (CharacterLibrary::exists(possibleName))
                        {
                            auto* component = layers.PrioritizeComponent<ns::NovelComponents::Character>(5000);
                            component->folder = folder;
                            component->characterData = CharacterLibrary::at(possibleName);
                            component->position = Position::Center;
                            if (noMessage) component->messageBack = MessageBack::No;
                            
                            std::wstring state{ L"" };
                            
                            vector<std::wstring> arguments;
                            nss::ParseArguments(command, arguments);
                            for (auto it = arguments.begin(); it != arguments.end(); ++it)
                            {
                                nss::CommandSettings argument; argument.Command(*it);
                                if (nss::Command(argument, L"fade:"))
                                {
                                    float value = nss::ArgumentAsFloat(argument);
                                    component->appearTime = value;
                                    component->disappearTime = value;
                                }
                                else if (nss::Command(argument, L"fadein:") || nss::Command(argument, L"appear:"))
                                    component->appearTime = nss::ArgumentAsFloat(argument);
                                else if (nss::Command(argument, L"fadeout:") || nss::Command(argument, L"disappear:"))
                                    component->disappearTime = nss::ArgumentAsFloat(argument);
                                else if (nss::Command(argument, L"alpha:") || nss::Command(argument, L"maxalpha:"))
                                    component->maxAlpha = nss::ArgumentAsInt(argument);
                                else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                                {
                                    std::wstring stringValue = nss::ArgumentAsString(argument);
                                    if (stringValue == L"atappearance" || stringValue == L"appearance")
                                        component->messageBack = MessageBack::AtAppearance;
                                    else if (stringValue == L"atdisappearing" || stringValue == L"disappearing")
                                        component->messageBack = MessageBack::AtDisappearance;
                                    else if (stringValue == L"atdeprecated" || stringValue == L"deprecated")
                                        component->messageBack = MessageBack::AtDeprecated;
                                    else if (stringValue == L"nomessage" || stringValue == L"no")
                                        component->messageBack = MessageBack::No;
                                }
                                else if (nss::Command(argument, L"depth:") || nss::Command(argument, L"layer:") || nss::Command(argument, L"d:"))
                                    layers.ChangePriorityOf(component, nss::ArgumentAsInt(argument));
                                else
                                {
                                    std::wstring possibleStateOrPos;
                                    if (argument.line[0] == L'"')
                                        possibleStateOrPos = nss::ParseAsQuoteString(argument);
                                    else possibleStateOrPos = argument.line;
                                    
                                    if (possibleStateOrPos == L"l" || possibleStateOrPos == L"left")
                                        component->position = Position::Left;
                                    else if (possibleStateOrPos == L"cl" || possibleStateOrPos == L"cleft")
                                        component->position = Position::CLeft;
                                    else if (possibleStateOrPos == L"c" || possibleStateOrPos == L"center")
                                        component->position = Position::Center;
                                    else if (possibleStateOrPos == L"cr" || possibleStateOrPos == L"cright")
                                        component->position = Position::CRight;
                                    else if (possibleStateOrPos == L"r" || possibleStateOrPos == L"right")
                                        component->position = Position::Right;
                                    else state = possibleStateOrPos;
                                }
                            }
                            //if (arguments.size() && arguments[arguments.size() - 1].length() && arguments[arguments.size() - 1][arguments[arguments.size() - 1].length() - 1] == L'!') component->sendMessageBack = component->noMessage;
                            
                            if (component->messageBack != MessageBack::No) OnHold(component);
                            characterGroup.insert(characterGroup.begin(), component);
                            component->LoadState(state);
                        }
                    }
                }
                else if (nss::Command(command, L"hide all"))
                {
                    if (characterGroup.size())
                    {
                        std::optional<float> disappearTime;
                        enum sendMessageBackEnum{ atDisappearing, atDeprecated, noMessage };
                        MessageBack messageBack{ MessageBack::AtDeprecated };
                        if (command.ExplicitNoMessage()) messageBack = MessageBack::No;
                        
                        vector<std::wstring> arguments;
                        nss::ParseArguments(command, arguments);
                        for (auto it = arguments.begin(); it != arguments.end(); ++it)
                        {
                            nss::CommandSettings argument; argument.Command(*it);
                            if (nss::Command(argument, L"fade:")||
                                nss::Command(argument, L"fadeout:") ||
                                nss::Command(argument, L"disappear:"))
                                disappearTime = nss::ArgumentAsFloat(argument);
                            else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                            {
                                std::wstring stringValue = nss::ArgumentAsString(argument);
                                if (stringValue == L"atdisappearing" || stringValue == L"disappearing") messageBack = MessageBack::AtDisappearance;
                                else if (stringValue == L"atdeprecated" || stringValue == L"deprecated") messageBack = MessageBack::AtDeprecated;
                                else if (stringValue == L"nomessage" || stringValue == L"no") messageBack = MessageBack::No;
                            }
                        }
                        
                        for (auto c : characterGroup)
                        {
                            if (messageBack != MessageBack::No) OnHold(c);
                            switch (messageBack)
                            {
                                case MessageBack::AtDisappearance: c->messageBack = MessageBack::AtDisappearance; break;
                                case MessageBack::No: c->messageBack = MessageBack::No; break;
                                default: c->messageBack = MessageBack::AtDeprecated; break;
                            }
                            if (disappearTime.has_value()) c->disappearTime = *disappearTime;
                            c->SetStateMode(Mode::Disapper);
                        }
                        for (auto c : personaGroup)
                        {
                            if (messageBack != MessageBack::No) OnHold(c);
                            switch (messageBack)
                            {
                                case MessageBack::AtDisappearance: c->messageBack = MessageBack::AtDisappearance; break;
                                case MessageBack::No: c->messageBack = MessageBack::No; break;
                                default: c->messageBack = MessageBack::AtDeprecated; break;
                            }
                            if (disappearTime.has_value()) c->disappearTime = *disappearTime;
                            c->SetStateMode(Mode::Disapper);
                        }
                    }
                }
                else if (nss::Command(command, L"hide neko"))
                {
                    enum sendMessageBackEnum{ atDisappearing, atDeprecated, noMessage };
                    sendMessageBackEnum sendMessageBack{ atDeprecated };
                    if (command.ExplicitNoMessage()) sendMessageBack = noMessage;
                    
                    if (personaGroup.size())
                    {
                        float disappearTime{ -1.f };
                        vector<std::wstring> arguments;
                        nss::ParseArguments(command, arguments);
                        for (auto it = arguments.begin(); it != arguments.end(); ++it)
                        {
                            nss::CommandSettings argument; argument.Command(*it);
                            if (nss::Command(argument, L"fade:") || nss::Command(argument, L"fadeout:") || nss::Command(argument, L"disappear:")) disappearTime = nss::ArgumentAsFloat(argument);
                            else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                            {
                                std::wstring stringValue = nss::ArgumentAsString(argument);
                                if (stringValue == L"atdisappearing" || stringValue == L"disappearing")
                                    sendMessageBack = atDisappearing;
                                else if (stringValue == L"atdeprecated" || stringValue == L"deprecated")
                                    sendMessageBack = atDeprecated;
                                else if (stringValue == L"nomessage" || stringValue == L"no")
                                    sendMessageBack = noMessage;
                            }
                        }
                        
                        for (auto c : personaGroup)
                        {
                            if (sendMessageBack != noMessage) OnHold(c);
                            switch (sendMessageBack)
                            {
                                case atDisappearing: c->messageBack = MessageBack::AtDisappearance; break;
                                case noMessage: c->messageBack = MessageBack::No; break;
                                default: c->messageBack = MessageBack::AtDeprecated; break;
                            }
                            if (disappearTime >= 0) c->disappearTime = disappearTime;
                            c->SetStateMode(Mode::Disapper);
                        }
                    }
                }
                else if (nss::Command(command, L"hide "))
                {
                    enum sendMessageBackEnum{ atDisappearing, atDeprecated, noMessage };
                    sendMessageBackEnum sendMessageBack{ atDeprecated };
                    if (command.ExplicitNoMessage()) sendMessageBack = noMessage;
                    
                    std::wstring possibleName = nss::ParseUntil(command, ' ');
                    if (possibleName.length() != 0 && CharacterLibrary::exists(possibleName) && characterGroup.size())
                    {
                        float disappearTime{ -1.f };
                        vector<std::wstring> arguments;
                        nss::ParseArguments(command, arguments);
                        for (auto it = arguments.begin(); it != arguments.end(); ++it)
                        {
                            nss::CommandSettings argument; argument.Command(*it);
                            if (nss::Command(argument, L"fade:") || nss::Command(argument, L"fadeout:") || nss::Command(argument, L"disappear:")) disappearTime = nss::ArgumentAsFloat(argument);
                            else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                            {
                                std::wstring stringValue = nss::ArgumentAsString(argument);
                                if (stringValue == L"atdisappearing" || stringValue == L"disappearing")
                                    sendMessageBack = atDisappearing;
                                else if (stringValue == L"atdeprecated" || stringValue == L"deprecated")
                                    sendMessageBack = atDeprecated;
                                else if (stringValue == L"nomessage" || stringValue == L"no")
                                    sendMessageBack = noMessage;
                            }
                        }
                        
                        for (auto c : characterGroup)
                            if (c->characterData && c->characterData->name == possibleName)
                            {
                                if (sendMessageBack != noMessage) OnHold(c);
                                switch (sendMessageBack)
                                {
                                    case atDisappearing: c->messageBack = MessageBack::AtDisappearance; break;
                                    case noMessage: c->messageBack = MessageBack::No; break;
                                    default: c->messageBack = MessageBack::AtDeprecated; break;
                                }
                                if (disappearTime >= 0) c->disappearTime = disappearTime;
                                c->SetStateMode(Mode::Disapper);
                            }
                    }
                }
                ///----------------------------------------AUDIO----------------------------------------
                ///----------------------------------------AUDIO----------------------------------------
                ///----------------------------------------AUDIO----------------------------------------
                else if ((backgroundAddingMode = nss::Command(command, L"ambient stop")) || nss::Command(command, L"music stop") || nss::Command(command, L"stop"))
                {
                    if ((musicGroup.size() != 0 && !backgroundAddingMode) || (ambientGroup.size() != 0 && backgroundAddingMode))
                    {
                        float disappearTime{ -1.f };
                        MessageBack messageBack{ MessageBack::AtDeprecated };
                        if (command.ExplicitNoMessage()) messageBack = MessageBack::No;
                        
                        vector<std::wstring> arguments;
                        nss::ParseArguments(command, arguments);
                        for (auto it = arguments.begin(); it != arguments.end(); ++it)
                        {
                            nss::CommandSettings argument; argument.Command(*it);
                            if (nss::Command(argument, L"fade:") || nss::Command(argument, L"fadeout:") || nss::Command(argument, L"disappear:")) disappearTime = nss::ArgumentAsFloat(argument);
                            else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                            {
                                std::wstring stringValue = nss::ArgumentAsString(argument);
                                if (stringValue == L"atdisappearing" || stringValue == L"disappearing")
                                    messageBack = MessageBack::AtDisappearance;
                                else if (stringValue == L"atdeprecated" || stringValue == L"deprecated")
                                    messageBack = MessageBack::AtDeprecated;
                                else if (stringValue == L"nomessage" || stringValue == L"no")
                                    messageBack = MessageBack::No;
                            }
                        }
                        
                        list<MusicPlayer*>& group = musicGroup;
                        if (backgroundAddingMode) group = ambientGroup;
                        for (auto m : group)
                        {
                            if (messageBack != MessageBack::No) OnHold(m);
                            switch (messageBack)
                            {
                                case MessageBack::AtDisappearance: m->messageBack = MessageBack::AtDisappearance; break;
                                case MessageBack::No: m->messageBack = MessageBack::No; break;
                                default: m->messageBack = MessageBack::AtDeprecated; break;
                            }
                            if (disappearTime >= 0) m->disappearTime = disappearTime;
                            m->SetStateMode(Mode::Disapper);
                        }
                    }
                }
                else if ((backgroundAddingMode = nss::Command(command, L"music add ")) || (backgroundAddingMode = nss::Command(command, L"play add ")) || nss::Command(command, L"music ") || nss::Command(command, L"play "))
                {
                    bool noMessage{ false }; if (command.ExplicitNoMessage()) noMessage = true;
                    std::wstring filePath = nss::ParseAsQuoteString(command);
                    if (!backgroundAddingMode && musicGroup.size() != 0)
                        for (auto b : musicGroup)
                        {
                            b->messageBack = MessageBack::No;
                            b->SetStateMode(Mode::Disapper);
                        }
                    
                    auto* component = layers.PrioritizeComponent<ns::NovelComponents::MusicPlayer>(0);
                    component->folderPath = folder;
                    if (noMessage) component->messageBack = MessageBack::No;
                    
                    vector<std::wstring> arguments;
                    nss::ParseArguments(command, arguments);
                    for (auto it = arguments.begin(); it != arguments.end(); ++it)
                    {
                        nss::CommandSettings argument; argument.Command(*it);
                        if (nss::Command(argument, L"fade:"))
                        {
                            float value = nss::ArgumentAsFloat(argument);
                            component->appearTime = value;
                            component->disappearTime = value;
                        }
                        else if (nss::Command(argument, L"fadein:") || nss::Command(argument, L"appear:"))
                            component->appearTime = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"fadeout:") || nss::Command(argument, L"disappear:"))
                            component->disappearTime = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"volume:") || nss::Command(argument, L"maxvolume:"))
                            component->maxVolume = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                        {
                            std::wstring stringValue = nss::ArgumentAsString(argument);
                            if (stringValue == L"atappearance" || stringValue == L"appearance")
                                component->messageBack = MessageBack::AtAppearance;
                            else if (stringValue == L"atdisappearing" || stringValue == L"disappearing")
                                component->messageBack = MessageBack::AtDisappearance;
                            else if (stringValue == L"atdeprecated" || stringValue == L"deprecated")
                                component->messageBack = MessageBack::AtDeprecated;
                            else if (stringValue == L"nomessage" || stringValue == L"no")
                                component->messageBack = MessageBack::No;
                        }
                        else if (nss::Command(argument, L"loop:") || nss::Command(argument, L"isloop:"))
                            component->loop = nss::ArgumentAsBool(argument);
                        else if (nss::Command(argument, L"from:") || nss::Command(argument, L"start:"))
                            component->playingOffset = sf::seconds(nss::ArgumentAsFloat(argument));
                    }
                    
                    if (component->messageBack != MessageBack::No) OnHold(component);
                    musicGroup.insert(musicGroup.begin(), component);
                    component->LoadFromFile(filePath);
                }
                else if ((backgroundAddingMode = nss::Command(command, L"ambient add ")) || nss::Command(command, L"ambient "))
                {
                    bool noMessage{ false }; if (command.ExplicitNoMessage()) noMessage = true;
                    std::wstring filePath = nss::ParseAsQuoteString(command);
                    if (!backgroundAddingMode && musicGroup.size() != 0)
                        for (auto b : ambientGroup)
                        {
                            b->messageBack = MessageBack::No;
                            b->SetStateMode(Mode::Disapper);
                        }
                    
                    auto* component = layers.PrioritizeComponent<ns::NovelComponents::MusicPlayer>(0);
                    component->folderPath = folder; component->ambient = true;
                    if (noMessage) component->messageBack = MessageBack::No;
                    
                    vector<std::wstring> arguments;
                    nss::ParseArguments(command, arguments);
                    for (auto it = arguments.begin(); it != arguments.end(); ++it)
                    {
                        nss::CommandSettings argument; argument.Command(*it);
                        if (nss::Command(argument, L"fade:"))
                        {
                            float value = nss::ArgumentAsFloat(argument);
                            component->appearTime = value;
                            component->disappearTime = value;
                        }
                        else if (nss::Command(argument, L"fadein:") || nss::Command(argument, L"appear:"))
                            component->appearTime = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"fadeout:") || nss::Command(argument, L"disappear:"))
                            component->disappearTime = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"volume:") || nss::Command(argument, L"maxvolume:"))
                            component->maxVolume = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                        {
                            std::wstring stringValue = nss::ArgumentAsString(argument);
                            if (stringValue == L"atappearance" || stringValue == L"appearance")
                                component->messageBack = MessageBack::AtAppearance;
                            else if (stringValue == L"atdisappearing" || stringValue == L"disappearing")
                                component->messageBack = MessageBack::AtDisappearance;
                            else if (stringValue == L"atdeprecated" || stringValue == L"deprecated")
                                component->messageBack = MessageBack::AtDeprecated;
                            else if (stringValue == L"nomessage" || stringValue == L"no")
                                component->messageBack = MessageBack::No;
                        }
                        else if (nss::Command(argument, L"loop:") || nss::Command(argument, L"isloop:"))
                            component->loop = nss::ArgumentAsBool(argument);
                        else if (nss::Command(argument, L"from:") || nss::Command(argument, L"start:"))
                            component->playingOffset = sf::seconds(nss::ArgumentAsFloat(argument));
                    }
                    
                    if (component->messageBack != MessageBack::No) OnHold(component);
                    ambientGroup.insert(ambientGroup.begin(), component);
                    component->LoadFromFile(filePath);
                }
                else if (nss::Command(command, L"sound stop"))
                {
                    if (soundGroup.size() != 0)
                    {
                        float disappearTime{ -1.f };
                        MessageBack messageBack{ MessageBack::No };
                        if (command.ExplicitNoMessage()) messageBack = MessageBack::No;
                        
                        vector<std::wstring> arguments;
                        nss::ParseArguments(command, arguments);
                        for (auto it = arguments.begin(); it != arguments.end(); ++it)
                        {
                            nss::CommandSettings argument; argument.Command(*it);
                            if (nss::Command(argument, L"fade:") || nss::Command(argument, L"fadeout:") || nss::Command(argument, L"disappear:")) disappearTime = nss::ArgumentAsFloat(argument);
                            else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                            {
                                std::wstring stringValue = nss::ArgumentAsString(argument);
                                if (stringValue == L"atdisappearing" || stringValue == L"disappearing")
                                    messageBack = MessageBack::AtDisappearance;
                                else if (stringValue == L"atdeprecated" || stringValue == L"deprecated")
                                    messageBack = MessageBack::AtDeprecated;
                                else if (stringValue == L"nomessage" || stringValue == L"no")
                                    messageBack = MessageBack::No;
                            }
                        }
                        
                        for (auto m : soundGroup)
                        {
                            if (messageBack != MessageBack::No) OnHold(m);
                            switch (messageBack)
                            {
                                case MessageBack::AtDisappearance: m->messageBack = MessageBack::AtDisappearance; break;
                                case MessageBack::No: m->messageBack = MessageBack::No; break;
                                default: m->messageBack = MessageBack::AtDeprecated; break;
                            }
                            if (disappearTime >= 0) m->disappearTime = disappearTime;
                            m->SetStateMode(Mode::Disapper);
                        }
                    }
                }
                else if (nss::Command(command, L"sound "))
                {
                    std::wstring filePath = nss::ParseAsQuoteString(command);
                    auto* component = layers.PrioritizeComponent<ns::NovelComponents::SoundPlayer>(0);
                    component->folderPath = folder;
                    if (command.ExplicitNoMessage()) component->messageBack = MessageBack::No;
                    
                    vector<std::wstring> arguments;
                    nss::ParseArguments(command, arguments);
                    for (auto it = arguments.begin(); it != arguments.end(); ++it)
                    {
                        nss::CommandSettings argument; argument.Command(*it);
                        if (nss::Command(argument, L"fade:"))
                        {
                            float value = nss::ArgumentAsFloat(argument);
                            component->appearTime = value;
                            component->disappearTime = value;
                        }
                        else if (nss::Command(argument, L"fadein:") || nss::Command(argument, L"appear:"))
                            component->appearTime = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"fadeout:") || nss::Command(argument, L"disappear:"))
                            component->disappearTime = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"volume:") || nss::Command(argument, L"maxvolume:"))
                            component->maxVolume = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                        {
                            std::wstring stringValue = nss::ArgumentAsString(argument);
                            if (stringValue == L"atappearance" || stringValue == L"appearance")
                                component->messageBack = MessageBack::AtAppearance;
                            else if (stringValue == L"atdisappearing" || stringValue == L"disappearing")
                                component->messageBack = MessageBack::AtDisappearance;
                            else if (stringValue == L"atdeprecated" || stringValue == L"deprecated")
                                component->messageBack = MessageBack::AtDeprecated;
                            else if (stringValue == L"nomessage" || stringValue == L"no")
                                component->messageBack = MessageBack::No;
                        }
                        else if (nss::Command(argument, L"loop:") || nss::Command(argument, L"isloop:"))
                            component->loop = nss::ArgumentAsBool(argument);
                        else if (nss::Command(argument, L"from:") || nss::Command(argument, L"start:"))
                            component->playingOffset = sf::seconds(nss::ArgumentAsFloat(argument));
                    }
                    
                    if (component->messageBack != MessageBack::No) OnHold(component);
                    soundGroup.insert(soundGroup.begin(), component);
                    component->LoadFromFile(filePath);
                }
                ///---------------------------------------SPECIFIC---------------------------------------
                ///---------------------------------------SPECIFIC---------------------------------------
                ///---------------------------------------SPECIFIC---------------------------------------
                else
                {
                    std::wstring possibleName = nss::ParseUntil(command, ' ');
                    if (possibleName.length() != 0)
                    {
                        nss::SkipSpaces(command);
                        std::wstring possibleDialogue = nss::ParseAsQuoteString(command);
                        if (possibleDialogue.length() != 0)
                        {
                            CharacterData* characterData = CharacterLibrary::at(possibleName);
                            std::wstring characterName = characterData ? characterData->display : possibleName;
                        
                            if (dialogueGroup.size()) for (auto d : dialogueGroup) ++d->priority;
                            auto* component = layers.PrioritizeComponent<Dialogue>(10000);
                            if (command.ExplicitNoMessage()) component->messageBack = MessageBack::No;
                            
                            vector<std::wstring> arguments;
                            nss::ParseArguments(command, arguments);
                            for (auto arg : arguments)
                            {
                                nss::CommandSettings argument; argument.Command(arg);
                                if (nss::Command(argument, L"fade:"))
                                {
                                    float value = nss::ArgumentAsFloat(argument);
                                    component->appearTime = value;
                                    component->disappearTime = value;
                                }
                                else if (nss::Command(argument, L"fadein:") || nss::Command(argument, L"appear:"))
                                    component->appearTime = nss::ArgumentAsFloat(argument);
                                else if (nss::Command(argument, L"fadeout:") || nss::Command(argument, L"disappear:"))
                                    component->disappearTime = nss::ArgumentAsFloat(argument);
                                else if (nss::Command(argument, L"alpha:") || nss::Command(argument, L"maxalpha:"))
                                    component->maxAlpha = nss::ArgumentAsInt(argument);
                                else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                                {
                                    std::wstring stringValue = nss::ArgumentAsString(argument);
                                    if (stringValue == L"atappearance" || stringValue == L"appearance")
                                        component->messageBack = MessageBack::AtAppearance;
                                    else if (stringValue == L"atdisappearing" || stringValue == L"disappearing")
                                        component->messageBack = MessageBack::AtDisappearance;
                                    else if (stringValue == L"atdeprecated" || stringValue == L"deprecated")
                                        component->messageBack = MessageBack::AtDeprecated;
                                    else if (stringValue == L"nomessage" || stringValue == L"no")
                                        component->messageBack = MessageBack::No;
                                }
                                else if (nss::Command(argument, L"afterappearswitchto:") || nss::Command(argument, L"switchto:"))
                                {
                                    std::wstring stringValue = nss::ArgumentAsString(argument);
                                    if (stringValue == L"waitingforinput" || stringValue == L"input")
                                        component->switchTo = Mode::WaitingForInput;
                                    else if (stringValue == L"waitingfortime" || stringValue == L"time")
                                        component->switchTo = Mode::WaitingForTime;
                                }
                                else if (nss::Command(argument, L"waitingtime:") || nss::Command(argument, L"time:"))
                                    component->waitingTime = nss::ArgumentAsFloat(argument);
                                else if (nss::Command(argument, L"depth:") || nss::Command(argument, L"d:"))
                                    layers.ChangePriorityOf(component, nss::ArgumentAsInt(argument));
                            }
                            
                            if (component->messageBack != MessageBack::No) OnHold(component);
                            
                            dialogueGroup.insert(dialogueGroup.begin(), component);
                            if (possibleName == L"neko")
                            {
                                component->SetCharacterName(CharacterLibrary::nekoName);
                                /*component->charText.setFillColor({100, 100, 100, component->alpha});
                                component->charText.setOutlineColor({100, 100, 100, component->alpha});*/
                            }
                            else component->SetCharacterName(characterName);
                            if (characterData) component->SetCharacter(characterData);
                            component->SetDialogue(possibleDialogue);
                        }
                    }
                }
            }
            if (!gs::isPause) layers.Update(elapsedTime);
            if (onHold.empty() && lines.empty()) entity->PopComponent(this);
        }
    }
}
