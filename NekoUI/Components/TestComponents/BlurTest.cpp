//
//  BlurTest.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 31.01.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#include "BlurTest.hpp"

void BlurTest::Init()
{
    image.loadFromFile(resourcePath() + "Data/Images/Backgrounds/strangeshopkeeper@3x.jpg");
    
    sf::Clock clock;
    sf::Image blur = GUI::Blur(image, 5);
    cout << "done for " << clock.getElapsedTime().asMilliseconds() << " ms" << endl;
    
    texture.loadFromImage(blur);
    shape.setTexture(&texture, true);
}
void BlurTest::Resize(unsigned int width, unsigned int height) { shape.setSize({(float)width, (float)height}); }
void BlurTest::Draw(sf::RenderWindow* window) { window->draw(shape); }
