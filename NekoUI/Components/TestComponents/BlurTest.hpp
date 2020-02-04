//
//  BlurTest.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 31.01.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#ifndef BlurTest_hpp
#define BlurTest_hpp

#include <iostream>

#include <SFML/Graphics.hpp>

#include "../../Engine/EntitySystem.hpp"
#include "../../Engine/GUI/Blur.hpp"

struct BlurTest : Component
{
    sf::Image image, blur;
    sf::Texture texture;
    sf::RectangleShape shape;
    
    void Init() override;
    void Resize(unsigned int width, unsigned int height) override;
    void Draw(sf::RenderWindow* window) override;
};

#endif /* BlurTest_hpp */
