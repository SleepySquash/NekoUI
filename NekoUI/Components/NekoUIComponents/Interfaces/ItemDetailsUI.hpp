//
//  ItemDetailsUI.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 03/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef ItemDetailsUI_hpp
#define ItemDetailsUI_hpp

#include <iostream>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include <minEH/Engine/EntitySystem.hpp>
#include <minEH/Engine/Settings.hpp>
#include <minEH/Engine/Collectors.hpp>
#include <minEH/Engine/GUI/Button/SomeButtons.hpp>
#include <minEH/Engine/NovelSomeScript.hpp>

#include "../Engine/Item.hpp"

using std::cin;
using std::cout;
using std::endl;

using namespace ns;

namespace NekoUI
{
    struct ItemDetailsUI : Component
    {
        bool active{ false };
        
        Item* item{ nullptr };
        float x{ 0 }, y{ 0 };
        
        sf::RectangleShape shape;
        sf::Sprite sprite; bool spriteLoaded{ false };
        sf::Text caption, description;
        
        enum modeEnum { appearing, existing, disappearing };
        modeEnum mode{ appearing }; sf::Uint8 alpha{ 0 };
        float currentTime{ 0.f }, appearTime{ 0.05f }, disappearTime{ 0.05f };
        
        void Init() override;
        void Destroy() override;
        void Update(const sf::Time& elapsedTime) override;
        void CleanUp();
        void PollEvent(sf::Event& event) override;
        void Resize(const unsigned int& width, const unsigned int& height) override;
        void Draw(sf::RenderWindow* window) override;
        void ReceiveMessage(MessageHolder& message) override;
        void Switch(const bool& on);
        void UpdateAlpha();
    };
}

#endif /* ItemDetailsUI_hpp */
