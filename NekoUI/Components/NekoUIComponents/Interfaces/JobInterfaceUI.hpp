//
//  JobInterfaceUI.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 23/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef JobInterfaceUI_hpp
#define JobInterfaceUI_hpp

#include <iostream>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include "../../../Essentials/ResourcePath.hpp"
#include "../../../Engine/Settings.hpp"
#include "../../../Engine/Collectors/Font.hpp"
#include "../../../Engine/Collectors/Image.hpp"
#include "../../../Engine/EntitySystem.hpp"
#include "../../../Engine/GUI/Button/SomeButtons.hpp"
#include "../../../Engine/GUI/Button/TexturedRectangleButton.hpp"

#include "../Player.hpp"
#include "../Apartment/RoomLibrary.hpp"

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;

namespace NekoUI
{
    struct JobInterfaceUI : Component
    {
        bool active{ false }, clickable{ false };
        
        sf::RectangleShape blackScreenShape;
        sf::Sprite background; bool spriteLoaded{ false };
        GUI::SpriteButton quitButton;
        
        GUI::TexturedRectangleButtons jobs;
        int jobsButtonsCount{ 0 }, jobsBackgroundW, jobsBackgroundH;
        float jobsStartXX{ 0 }, yyScroll{ 0 };
        
        enum modeEnum { appearing, existing, disappearing };
        modeEnum mode{ appearing }; sf::Uint8 alpha{ 0 };
        float currentTime{ 0.f }, appearTime{ 0.3f }, disappearTime{ 0.2f };
        
        void Init() override;
        void Destroy() override;
        void Update(const sf::Time& elapsedTime) override;
        void CleanUp();
        void PollEvent(sf::Event& event) override;
        void Resize(unsigned int width, unsigned int height) override;
        void Draw(sf::RenderWindow* window) override;
        void RecieveMessage(MessageHolder& message) override;
        void Switch(const bool& on);
        void UpdateAlpha();
    };
}

#endif /* JobInterfaceUI_hpp */
