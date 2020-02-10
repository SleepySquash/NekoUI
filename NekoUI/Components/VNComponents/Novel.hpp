//
//  Novel.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 08.02.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#ifndef Novel_hpp
#define Novel_hpp

#include <iostream>
#include <list>
using std::list;

#include <minEH/Engine/EntitySystem.hpp>
#include <minEH/Engine/NovelSystem.hpp>

#include "Background.hpp"

using namespace ns;

namespace NekoUI
{
    struct Novel : Component
    {
        bool layersActive{ true };
        NovelSystem layers;
        
        std::wstring line;
        std::wifstream wif;
        unsigned long position{ 0 };
        nss::CommandSettings command;
        list<std::wstring> lines;
        
        bool eof{ false }, fileOpened{ false };
        
        list<Background*> backgroundGroup;
        /*list<Character*> characterGroup;
        list<Dialogue*> dialogueGroup;
        list<Choose*> chooseGroup;
        list<SoundPlayer*> soundGroup;
        list<MusicPlayer*> musicGroup;
        list<MusicPlayer*> ambientGroup;
        list<GUISystem*> GUIGroup;*/
        
        void Init() override;
        void Update(const sf::Time& elapsedTime) override;
        void PollEvent(sf::Event& event) override;
        void ResourcesPreloading(list<std::wstring>& lines, std::wstring& line);
        void Resize(const unsigned int& width, const unsigned int& height) override;
        void Draw(sf::RenderWindow* window) override;
        void Save(std::wofstream& wof);
        void ForwardMessage(MessageHolder& message);
        void ReceiveMessage(MessageHolder& message) override;
        void OnHold(NovelObject* component);
        void UnHold(NovelObject* component);
        void VariableChange(const std::wstring& name);
        void LocalVariables_Set(const std::wstring& name, std::wstring value);
        void LocalVariables_Set(const std::wstring& name, bool value);
        void LocalVariables_Set(const std::wstring& name, int value);
        template<typename T> void FreeGroup(list<T*>& elements) { elements.clear(); }
    };
}

#endif /* Novel_hpp */
