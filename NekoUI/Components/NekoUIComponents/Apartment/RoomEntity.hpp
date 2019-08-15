//
//  RoomEntity.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 05/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef RoomEntity_hpp
#define RoomEntity_hpp

#include <vector>

#include <SFML/Graphics.hpp>

#include "../../../Engine/MessageHolder.hpp"

namespace NekoUI
{
    struct RoomEntity
    {
        float x, y, height, relScale{ 1.f };
        bool movable{ false }, onScreen{ true }, offline{ false };
        unsigned long positionInArray{ 0 };
        std::vector<RoomEntity*>* vector{ nullptr };
        
        enum class Type { Item, Furniture, Neko, Unknown } type{ Type::Unknown };
        std::string id;
        
        sf::Sprite sprite;
        
        ~RoomEntity();
        virtual void Destroy();
        virtual void Update(const sf::Time& elapsedTime);
        void UpdateDepthPosition();
        void UpdateDepthContinuously();
        virtual void PollEvent(sf::Event& event);
        virtual void Resize();
        virtual void UpdatePosition();
        virtual void Draw(sf::RenderWindow* window);
        virtual void ReceiveMessage(ns::MessageHolder& message);
        virtual void Save(std::wofstream& wof);
    };
}

#endif /* RoomEntity_hpp */
