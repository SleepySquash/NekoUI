//
//  FNAF.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 28.01.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#include "FNAF.hpp"

namespace NekoUI
{
    namespace Jobs_FNAFGame
    {
        void FNAF::Init()
        {
            exitButton.setFont(L"Pacifica.ttf");
            exitButton.setCharacterSize(50);
            exitButton.setString(L"Выйти");
            
            moneyButton.setFont(L"Pacifica.ttf");
            moneyButton.setCharacterSize(50);
            moneyButton.setString(L"Получить зарплату");
        }
        void FNAF::PollEvent(sf::Event& event)
        {
            if (!clickable) return;
            if (exitButton.PollEvent(event)) { entity->system->SendMessage({"PlacesUI :: ApartmentUI"}); clickable = false; }
            else if (moneyButton.PollEvent(event)) NekoS::money += 500;
        }
        void FNAF::Resize(unsigned int width, unsigned int height)
        {
            moneyButton.Resize(width, height);
            moneyButton.setPosition(width/2, height/2 - moneyButton.getGlobalBounds().height/2 - 10*gs::scale);
            
            exitButton.Resize(width, height);
            exitButton.setPosition(width/2, height/2 + moneyButton.getGlobalBounds().height/2 + 10*gs::scale);
        }
        void FNAF::Draw(sf::RenderWindow* window)
        {
            moneyButton.draw(window);
            exitButton.draw(window);
        }
        void FNAF::RecieveMessage(MessageHolder& message)
        {
            if (message.info == "PlacesUI :: ApartmentUI Destroy") entity->PopComponent(this);
        }
    }
}
