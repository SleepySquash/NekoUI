//
//  Novel.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 08.02.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#include "Novel.hpp"

namespace NekoUI
{
    void Novel::Init()
    {
        
    }
    void Novel::Update(const sf::Time& elapsedTime)
    {
        if (layersActive) layers.Update(elapsedTime);
    }
    void Novel::PollEvent(sf::Event& event)
    {
        if (layersActive) layers.PollEvent(event);
    }
    void Novel::Resize(const unsigned int& width, const unsigned int& height)
    {
        if (layersActive) layers.Resize(width, height);
    }
    void Novel::Draw(sf::RenderWindow* window)
    {
        if (layersActive) layers.Draw(window);
    }
    void Novel::ReceiveMessage(MessageHolder& message)
    {
        if (layersActive) layers.ReceiveMessage(message);
    }
}
