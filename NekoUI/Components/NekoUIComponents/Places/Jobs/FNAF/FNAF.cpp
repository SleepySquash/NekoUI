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
        }
        void FNAF::PollEvent(sf::Event& event)
        {
            if (!clickable) return;
            if (exitButton.PollEvent(event)) { entity->system->SendMessage({"PlacesUI :: ApartmentUI"}); clickable = false; }
        }
        void FNAF::Resize(unsigned int width, unsigned int height)
        {
            exitButton.Resize(width, height);
            exitButton.setPosition(width/2, height/2);
        }
        void FNAF::Draw(sf::RenderWindow* window)
        {
            exitButton.Draw(window);
        }
        void FNAF::RecieveMessage(MessageHolder& message)
        {
            if (message.info == "PlacesUI :: ApartmentUI Destroy") entity->PopComponent(this);
        }
    }
}
