//
//  NovelComponents.hpp
//  NovelSome
//
//  Created by Никита Исаенко on 31/08/2018.
//  Copyright © 2018 Melancholy Hill. All rights reserved.
//

#ifndef NovelComponents_hpp
#define NovelComponents_hpp

#include <iostream>
#include <list>
#include <vector>
#include <optional>

//NovelComponent
#include <locale>
#include <fstream>
#include <codecvt>

//Folder path
#include <dirent.h>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <minEH/Essentials/Base.hpp>
#include <minEH/Engine/List.hpp>
#include <minEH/Engine/EntitySystem.hpp>
#include <minEH/Engine/NovelSystem.hpp>
#include <minEH/Engine/Settings.hpp>
#include <minEH/Engine/GUI/Button/SomeButtons.hpp>
#include <minEH/Engine/NovelSomeScript.hpp>

#include "Abstract/CharacterLibrary.hpp"
#include "Abstract/GamePause.hpp"
#include "Abstract/Modes.hpp"

#include "Audio.hpp"
#include "Background.hpp"
#include "Character.hpp"
#include "Persona.hpp"
#include "Dialogue.hpp"
#include "Choice.hpp"
#include "Misc.hpp"

using std::cin;
using std::cout;
using std::endl;
using base::utf8;
using base::utf16;
using std::list;
using std::vector;

#undef interface
namespace ns
{
    namespace NovelComponents
    {
        struct Novel;
        struct EventListener : NovelObject
        {
            Novel* novel;
            EventListener(Novel* novel);
            void ReceiveMessage(MessageHolder& message) override;
        };
        struct Novel : Component
        {
            std::wstring folder, line;
            unsigned long position{ 0 };
            nss::CommandSettings command;
            list<std::wstring> lines;
            int preloadLinesAmount{ 12 }, preloadedLines{ 0 };
            
            bool noDestroyMessage{ false };
            list<NovelObject*> onHold;
            
            GamePause* gamePause{ nullptr };
            bool noGamePause{ false };
            
            NovelSystem layers;
            
            list<Background*> backgroundGroup;
            list<Character*> characterGroup;
            list<Persona*> personaGroup;
            list<Dialogue*> dialogueGroup;
            list<Choice*> chooseGroup;
            list<SoundPlayer*> soundGroup;
            list<MusicPlayer*> musicGroup;
            list<MusicPlayer*> ambientGroup;
            
            ~Novel();
            void Init() override;
            void Update(const sf::Time& elapsedTime) override;
            void PollEvent(sf::Event& event) override;
            void ResourcesPreloading(list<std::wstring>& lines, std::wstring& line);
            void Resize(const unsigned int& width, const unsigned int& height) override;
            void Draw(sf::RenderWindow* window) override;
            void ForwardMessage(MessageHolder& message);
            void ReceiveMessage(MessageHolder& message) override;
            void OnHold(NovelObject* component);
            void UnHold(NovelObject* component);
            template<typename T> void FreeGroup(list<T*>& elements) { elements.clear(); }
        };
    }
}

#endif /* NovelComponents_hpp */
