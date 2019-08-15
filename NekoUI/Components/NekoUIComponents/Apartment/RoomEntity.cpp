//
//  RoomEntity.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 05/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "RoomEntity.hpp"

namespace NekoUI
{
    RoomEntity::~RoomEntity() { }
    void RoomEntity::Destroy() { }
    void RoomEntity::Update(const sf::Time &elapsedTime) { }
    void RoomEntity::UpdateDepthPosition()
    {
        if (!vector) return;
        if (positionInArray > 0)
        {
            RoomEntity* prev = (*vector)[positionInArray - 1];
            if (prev->y > y) { (*vector)[positionInArray - 1] = this; (*vector)[positionInArray] = prev;
                --positionInArray; ++prev->positionInArray; }
        }
        if (positionInArray + 1 < vector->size())
        {
            RoomEntity* next = (*vector)[positionInArray + 1];
            if (next->y < y) { (*vector)[positionInArray + 1] = this; (*vector)[positionInArray] = next;
                ++positionInArray; --next->positionInArray; }
        }
    }
    void RoomEntity::UpdateDepthContinuously()
    {
        if (!vector) return; bool settled{ false };
        while (!settled)
        {
            settled = true;
            if (positionInArray > 0)
            {
                RoomEntity* prev = (*vector)[positionInArray - 1];
                if (prev->y > y) { (*vector)[positionInArray - 1] = this; (*vector)[positionInArray] = prev;
                    --positionInArray; ++prev->positionInArray; settled = false; }
            }
            if (positionInArray + 1 < vector->size())
            {
                RoomEntity* next = (*vector)[positionInArray + 1];
                if (next->y < y) { (*vector)[positionInArray + 1] = this; (*vector)[positionInArray] = next;
                    ++positionInArray; --next->positionInArray; settled = false; }
            }
        }
    }
    void RoomEntity::PollEvent(sf::Event& event) { }
    void RoomEntity::Resize() { }
    void RoomEntity::UpdatePosition() { }
    void RoomEntity::Draw(sf::RenderWindow* window) { }
    void RoomEntity::ReceiveMessage(ns::MessageHolder& message) { }
    void RoomEntity::Save(std::wofstream &wof) { }
}
