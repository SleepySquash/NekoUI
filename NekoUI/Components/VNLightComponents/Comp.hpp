//
//  Comp.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 01.04.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#ifndef Comp_hpp
#define Comp_hpp

#include <minEH/minEH.h>
using namespace ns;

struct Mesh : Component
{
    sf::Texture* texture;
    sf::Sprite sprite;
    
    bool moveA{ false }, moveD{ false }, moveW{ false }, moveS{ false };
    float movementVelocity{ 0.5f };
    
    bool randomMovement{ true }, onPoint{ false };
    float elapsedStanding = 0.4f;
    sf::Vector2f movementPoint, position;
    
    void Init() override
    {
        texture = ic::LoadTexture(L"Data/Neko/Person/body.png");
        sprite.setTexture(*texture);
        sprite.setScale(0.1f, 0.1f);
    }
    void Update(const sf::Time& elapsedTime) override
    {
        if (randomMovement)
        {
            if (onPoint)
            {
                if (elapsedStanding > 0.f) elapsedStanding -= elapsedTime.asSeconds();
                else
                {
                    movementPoint = { (rand() % 1000)/500.f - 1.f, (rand() % 1000)/500.f - 1.f };
                    onPoint = false; elapsedStanding = (rand() % 1000) / 1000.f;
                }
            }
            else
            {
                if (position.x > movementPoint.x - 0.05f && position.x < movementPoint.x + 0.05f && position.y > movementPoint.y - 0.05f && position.y < movementPoint.y + 0.05f) { onPoint = true; moveA = moveD = moveW = moveS = false; }
                else
                {
                    if (position.x > movementPoint.x + 0.05f) { moveA = true; moveD = false; }
                    else if (position.x < movementPoint.x - 0.05f) { moveA = false; moveD = true; }
                    else { moveA = moveD = false; }
                    if (position.y > movementPoint.y + 0.05f) { moveS = false; moveW = true; }
                    else if (position.y < movementPoint.y - 0.05f) { moveS = true; moveW = false; }
                    else { moveW = moveS = false; }
                }
            }
        }
        
        if (moveA || moveD || moveS || moveW)
        {
            if (moveA) position.x -= movementVelocity * elapsedTime.asSeconds();
            else if (moveD) position.x += movementVelocity * elapsedTime.asSeconds();
            if (moveW) position.y -= movementVelocity * elapsedTime.asSeconds();
            else if (moveS) position.y += movementVelocity * elapsedTime.asSeconds();
            sprite.setPosition((position.x/2.f + 0.5f) * gs::width, (position.y/2.f + 0.5f) * gs::height);
            gs::requestWindowRefresh = true;
            
        }
    }
    void Resize(const unsigned int& width, const unsigned int& height) override
    {
        
    }
    void Draw(sf::RenderWindow* window) override
    {
        for (int i = 0; i < 200; ++i) window->draw(sprite);
    }
};

#endif /* Comp_hpp */
