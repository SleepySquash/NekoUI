//
//  NovelComponents.cpp
//  NovelSome
//
//  Created by Никита Исаенко on 31/08/2018.
//  Copyright © 2018 Melancholy Hill. All rights reserved.
//

#include "Novel.hpp"

namespace ns
{
    namespace NovelComponents
    {
        EventListener::EventListener(Novel* novel) : novel(novel) { }
        void EventListener::ReceiveMessage(MessageHolder& message)
        {
            if (message.info == "UnHold" || message.info == "Destroy" || message.info == "Push" || message.info == "Execute" || message.info == "ExecuteNow" || message.info == "ExecuteInsert" || message.info == "ChooseUnHold" || message.info == "Save" || message.info == "Load") novel->ForwardMessage(message);
            else if (message.info == "FinishedExecute" || message.info == "GamePause") novel->gamePause->ReceiveMessage(message);
            else if (message.info == "GamePause :: Return to menu") novel->entity->ReceiveMessage(message);
        }
        
        void Novel::Init()
        {
            layers.PrioritizeComponent<EventListener>(-31000, this);
            gs::PushInterface(this); entity->SortAbove(this);
            if (CharacterLibrary::nekoPersona) if (!(reinterpret_cast<NekoUI::Persona*>(CharacterLibrary::nekoPersona))->loaded) (reinterpret_cast<NekoUI::Persona*>(CharacterLibrary::nekoPersona))->OccupyMemory();
            
            if (!noGamePause) gamePause = entity->AddComponent<GamePause>();
            entity->SendMessage(MessageHolder("Novel :: Starting"));
        }
        Novel::~Novel()
        {
            if (!noDestroyMessage) entity->SendMessage({"Novel :: Destroying"});
            if (gamePause) entity->PopComponent(gamePause);
            CharacterLibrary::clear();
            
            layers.clear();
            gs::RemoveInterface(this);
            
            FreeGroup<Background>(backgroundGroup);
            FreeGroup<Character>(characterGroup);
            FreeGroup<Persona>(personaGroup);
            FreeGroup<Dialogue>(dialogueGroup);
            FreeGroup<SoundPlayer>(soundGroup);
            FreeGroup<MusicPlayer>(musicGroup);
            
            FreeGroup<NovelObject>(onHold);
        }
        // TODO: Make it possible to ignore some lines of novel
        void Novel::ResourcesPreloading(list<std::wstring>& lines, std::wstring& line)
        {
            /*while (preloadedLines <= preloadLinesAmount && !eof)
            {
                line = lines[preloadedLines]; ++position; // cout << utf8(scenario) << " : " << utf8(line) << endl;
                if (line.length() && line[line.length() - 1] == 13) line.erase(line.begin() + (line.size() - 1));
                
                nss::CommandSettings settings;
                settings.Command(line);
                
                bool shouldPush{ true };
                if (nss::Command(settings, L"choose")) { lines.push_front(line); break; }
                else if (nss::Command(settings, L"//")) shouldPush = false;
                else if (!nss::ContainsUsefulInformation(line)) shouldPush = false;
                else if (nss::Command(settings, L"background hide")) { }
                else if (nss::Command(settings, L"hide ")) { }
                else if (nss::Command(settings, L"sound stop")) { }
                else if (nss::Command(settings, L"background add ") || nss::Command(settings, L"background "))
                {
                    std::wstring filePath = nss::ParseAsQuoteString(settings);
                    ic::PreloadTexture(folder + filePath, 1, true, false, &layers);
                }
                else if (nss::Command(settings, L"show "))
                {
                    std::wstring possibleName = nss::ParseUntil(settings, ' ');
                    if (possibleName.length() != 0)
                    {
                        if (CharacterLibrary::exists(possibleName))
                        {
                            std::wstring state{ L"" };
                            
                            vector<std::wstring> arguments;
                            nss::ParseArguments(settings, arguments);
                            for (auto it = arguments.begin(); it != arguments.end(); ++it)
                            {
                                nss::CommandSettings argument;
                                argument.Command(*it);
                                
                                if (nss::Command(argument, L"fade:")) { }
                                else if (nss::Command(argument, L"fadein:") || nss::Command(argument, L"appear:")) { }
                                else if (nss::Command(argument, L"fadeout:") || nss::Command(argument, L"disappear:")) { }
                                else if (nss::Command(argument, L"alpha:") || nss::Command(argument, L"maxalpha:")) { }
                                else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:")) { }
                                else
                                {
                                    std::wstring possibleStateOrPos;
                                    if (argument.line[0] == L'"')
                                        possibleStateOrPos = nss::ParseAsQuoteString(argument);
                                    else possibleStateOrPos = argument.line;
                                    
                                    if (state.length() == 0) state = possibleStateOrPos;
                                }
                            }
                            
                            CharacterData* characterData = CharacterLibrary::at(possibleName);
                            sf::String fullPath = sf::String(resourcePath()) + folder + characterData->filePath;
                            std::wstring lookForSpritePath = base::GetFolderPath(folder + characterData->filePath);
                            std::wstring spritePath{ L"" };
                            
                            std::wifstream wifc;
#ifdef _WIN32
                            wifc.open(fullPath.toWideString());
#else
                            std::wstring _wpath = fullPath;
                            std::string _path(_wpath.begin(), _wpath.end());
                            wifc.open(_path);
#endif
                            wifc.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
                            
                            if (wifc.is_open())
                            {
                                bool eofc{ false };
                                std::wstring linec;
                                nss::CommandSettings commandc;
                                bool stateReading{ false };
                                
                                while (!eofc)
                                {
                                    if (!wifc.eof())
                                    {
                                        std::getline(wifc, linec); commandc.Command(linec);
                                        if (!stateReading)
                                        {
                                            if (nss::Command(commandc, L"state ") || nss::Command(commandc, L"sprite "))
                                            {
                                                nss::SkipSpaces(commandc); std::wstring stateName;
                                                if (commandc.line[commandc.lastPos] == L'"')
                                                    stateName = nss::ParseAsQuoteString(commandc);
                                                else stateName = nss::ParseUntil(commandc, ' ');
                                                
                                                stateReading = (stateName == state) ? true : false;
                                            }
                                        } else {
                                            if (nss::Command(commandc, L"state ") || nss::Command(commandc, L"end") || nss::Command(commandc, L"}"))
                                                stateReading = false;
                                            else if (nss::Command(commandc, L"image ") || nss::Command(commandc, L"sprite "))
                                                spritePath = nss::ParseAsQuoteString(commandc);
                                            else if (nss::Command(commandc, L"\""))
                                            {
                                                commandc.lastPos--;
                                                spritePath = nss::ParseAsQuoteString(commandc);
                                            }
                                            
                                            eofc = stateReading ? false : true;
                                        }
                                    } else eofc = true;
                                }
                            }
                            wifc.close();
                            if (spritePath.length() != 0) ic::PreloadTexture(lookForSpritePath + spritePath, 2, true, false, &layers);
                        }
                    }
                }
                else if (nss::Command(settings, L"sound "))
                {
                    std::wstring filePath = nss::ParseAsQuoteString(settings);
                    // TODO: sc::PreloadSound(folderPath + filePath);
                    // TODO: Fix crashing due to thread.
                }
                
                if (shouldPush) lines.push_front(line);
            }*/
        }
        void Novel::Draw(sf::RenderWindow* window) { layers.Draw(window); }
        void Novel::Resize(const unsigned int& width, const unsigned int& height) { layers.Resize(width, height); }
        void Novel::PollEvent(sf::Event& event) { if (!gs::isPause && (!gs::ignoreEvent || gs::isActiveInterface(this))) layers.PollEvent(event); }
        void Novel::ForwardMessage(MessageHolder& message)
        {
            if (message.info == "UnHold") UnHold(reinterpret_cast<NovelObject*>(message.address));
            else if (message.info == "Destroy")
            {
                std::list<Background*>::iterator it1 = std::find(backgroundGroup.begin(), backgroundGroup.end(), message.address);
                if (it1 != backgroundGroup.end()) backgroundGroup.erase(it1);
                
                std::list<Character*>::iterator it2 = std::find(characterGroup.begin(), characterGroup.end(), message.address);
                if (it2 != characterGroup.end()) characterGroup.erase(it2);
                
                std::list<Persona*>::iterator it7 = std::find(personaGroup.begin(), personaGroup.end(), message.address);
                if (it7 != personaGroup.end()) personaGroup.erase(it7);
                
                std::list<Dialogue*>::iterator it3 = std::find(dialogueGroup.begin(), dialogueGroup.end(), message.address);
                if (it3 != dialogueGroup.end()) dialogueGroup.erase(it3);
                
                std::list<Choice*>::iterator it4 = std::find(chooseGroup.begin(), chooseGroup.end(), message.address);
                if (it4 != chooseGroup.end()) chooseGroup.erase(it4);
                
                std::list<MusicPlayer*>::iterator it5 = std::find(musicGroup.begin(), musicGroup.end(), message.address);
                if (it5 != musicGroup.end()) musicGroup.erase(it5);
                
                std::list<SoundPlayer*>::iterator it6 = std::find(soundGroup.begin(), soundGroup.end(), message.address);
                if (it6 != soundGroup.end()) soundGroup.erase(it6);
                
                std::list<MusicPlayer*>::iterator it8 = std::find(ambientGroup.begin(), ambientGroup.end(), message.address);
                if (it8 != ambientGroup.end()) ambientGroup.erase(it8);
            }
            else if (message.info == "Push") lines.push_back(message.additional);
            else if (message.info == "ChooseUnHold")
            {
                UnHold(reinterpret_cast<NovelObject*>(message.address));
                if (dialogueGroup.size() != 0)
                    for (auto d : dialogueGroup)
                        if (d->mode == Mode::WaitingForChoose)
                            d->SetStateMode(Mode::Disapper);
            }
            else if (message.info == "Save") entity->SendMessage({"SaveUI"});
            else if (message.info == "Load") entity->SendMessage({"LoadUI"});
        }
        void Novel::ReceiveMessage(MessageHolder &message)
        {
            if (message.info == "GamePause")
            {
                for (auto& s : soundGroup) s->ReceiveMessage(message);
                for (auto& s : musicGroup) s->ReceiveMessage(message);
                for (auto& s : ambientGroup) s->ReceiveMessage(message);
            }
            else if (nss::Command(message.info, "Request")) layers.ReceiveMessage(message);
            else if (message.info == "Push") lines.push_back(message.additional);
            else if (nss::Command(message.info, "DestroyNovel")) { gs::isPause = false; noDestroyMessage = true; entity->PopComponent(this); }
        }
        void Novel::OnHold(NovelObject* component) { onHold.push_front(component); }
        void Novel::UnHold(NovelObject* component)
        {
            std::list<NovelObject*>::iterator it = std::find(onHold.begin(), onHold.end(), component);
            if (it != onHold.end()) onHold.erase(it);
        }
    }
}
