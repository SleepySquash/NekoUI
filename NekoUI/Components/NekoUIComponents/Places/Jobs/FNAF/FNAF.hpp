//
//  FNAF.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 28.01.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#ifndef JobsFNAF_hpp
#define JobsFNAF_hpp

#include <iostream>
#include "../../../../../Engine/EntitySystem.hpp"
#include "../../../../../Engine/GUI/Button/SomeButtons.hpp"

#include "../../../Neko/Static.hpp"

using namespace ns;

namespace NekoUI
{
    namespace Jobs_FNAFGame
    {
        struct FNAF : Component
        {
            bool clickable{ true };
            GUI::TextButton exitButton, moneyButton;
            
            void Init() override;
            void PollEvent(sf::Event& event) override;
            void Resize(unsigned int width, unsigned int height) override;
            void Draw(sf::RenderWindow* window) override;
            void RecieveMessage(MessageHolder& message) override;
        };
    }
}

#endif /* JobsFNAF_hpp */
