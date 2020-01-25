//
//  Vasya.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 25.01.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#include "Vasya.hpp"

void Vasya::ASDAS()
{
    sprite.setTexture(*ic::LoadTexture(L"Data/Items/Cake.png"));
}
void Vasya::Draw()
{
    gs::window->draw(sprite);
}
