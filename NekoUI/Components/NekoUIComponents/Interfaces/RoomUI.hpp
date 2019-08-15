//
//  RoomUI.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 23/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef RoomUI_hpp
#define RoomUI_hpp

#include <iostream>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../../../Engine/EntitySystem.hpp"
#include "../../../Engine/Settings.hpp"
#include "../../../Engine/Collectors.hpp"
#include "../../../Engine/GUIInterface.hpp"

#include "../Player.hpp"
#include "../Static/NekoStatic.hpp"
#include "../Apartment/NekoEntity.hpp"
#include "../Apartment/RoomLibrary.hpp"

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;

using namespace ns;

namespace NekoUI
{
    struct RoomUI : Component
    {
        NekoEntity* neko{ nullptr };
        bool spriteLoaded, active{ true };
        sf::IntRect availableToTouchZone{ 0, 0, 0, 0 };
        bool ignoreEventMove{ false };
        
        float secondsUntilNextDay{ 0.f };
        bool newDayIsComing{ false };
        int partsOf30UntilNextDay{ 0 };
        
        bool shouldDoBackupSaving{ false };
        float dt{ 0.f }, secondsPassed{ 0.f };
        sf::Sprite dateSprite;
        int dateDay{ 0 }, dateMonth{ 0 };
        sf::Text dayText, monthText;
        sf::Text nekoMoodText;
        
        bool needbaseSpriteTransparent{ false };
        sf::Sprite needbaseSprite, needSprite;
        
        sf::Texture* scrolldownTexture_normal{ nullptr }, *scrolldownTexture_close{ nullptr };
        bool pressWillCloseInterfaces{ false };
        GUI::SpriteButton scrolldownMenu;
        GUI::SpriteButtons scrolldownButtons;
        float scrolldownButtonsScaling{ 1.f };
        
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

#endif /* RoomUI_hpp */
