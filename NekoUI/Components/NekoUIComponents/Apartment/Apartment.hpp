//
//  Apartment.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 17/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef Apartment_hpp
#define Apartment_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <list>

#include <thread>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../../../Essentials/ResourcePath.hpp"
#include "../../../Engine/EntitySystem.hpp"
#include "../../../Engine/Settings.hpp"
#include "../../../Engine/Collectors.hpp"
#include "../../../Engine/GUI/Button/SomeButtons.hpp"
#include "../../../Engine/NotificationHolder.hpp"

#include "RoomLibrary.hpp"
#include "NekoEntity.hpp"
#include "ItemEntity.hpp"
#include "FurnitureEntity.hpp"
#include "../Player.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::list;
using ns::base::utf8;
using ns::base::utf16;

using namespace ns;

namespace NekoUI
{
    void renderBackgroundSprite(sf::RenderTexture* backgroundTexture, sf::Sprite* backgroundSprite, sf::Sprite* backgroundRenderSprite);
    struct Apartment : Component
    {
        vector<RoomEntity*> entities;
        vector<FurnitureEntity*> furnitures;
        NekoEntity neko; sf::Sprite sprite;
        bool spriteLoaded, active{ true }, hasFocusOnNeko{ false }, pressedNeko{ false };
        
        sf::Sprite backgroundSprite, backgroundRenderSprite;
        float backgroundXX{ 0 }, backgroundYY{ 0 };
        sf::RenderTexture backgroundTexture;
        bool drawisqueued{ false };
        
        sf::Sprite inventoryMovingSprite;
        bool drawInventoryButton{ false }, lastDrawInventoryButton{ false };
        bool entityIsBeingMoved{ false }, entityFromInterface{ false }, savingIsRequired{ false };
        RoomEntity* movedEntity{ nullptr }, *pressedEntity{ nullptr };
        sf::Vector2f lastValidDot{ 0, 0 };
        bool inAreaToMoveView{ false };
        
        sf::Text hoverText; bool drawHoverText{ false }, hoverIsPossible{ false };
        bool requestHoverUpdate{ false }, lastDrawHoverText{ false };
        
        float timeSinceFirstPress{ 0.f }, timeSinceFirstHold{ 0.f };
        sf::Vector2i dot, pressedNekoPos, middleZoom{ 0, 0 };
        float move_dx{ 0 }, move_dy{ 0 };
        float scrollSensitivity{ 0.02f }, prevZoom{ 1 }, prevDistanceZoom{ 1.f };
        
        bool canMoveAround{ false };
        int scrolldownMenuOffsetY{ 0 };
        sf::IntRect availableToTouchZone{ 0, 0, 0, 0 };
        
        bool drawPointer{ false }, lastMistake{ true }, requestPointerUpdate{ false };
        sf::Sprite nekoPtrSprite, nekoArrowSprite;
        
        FurnitureEntity* nekoHoveringEntity{ nullptr };
        bool checkNekoHovering{ false }; float elapsedNekoHovering{ 0.f };
        
        Apartment();
        void Init() override;
        void Destroy() override;
        void CleanUp();
        void Update(const sf::Time& elapsedTime) override;
        void PollEvent(sf::Event& event) override;
        void Resize(unsigned int width, unsigned int height) override;
        void Draw(sf::RenderWindow* window) override;
        void RecieveMessage(MessageHolder& message) override;
        
        void CalculateCameraPosition();
        void CalculateCameraScale();
        void MoveEntityTo(int x, int y);
        void UpdateNekoPointer();
        sf::Vector2f GetEdgeOfView(float theta);
        float NekoHasBeenSleepingFor(const tm* timeinfo);
        
        void RegisterMovingEntity(RoomEntity* entity, bool fromInterface = false);
        void ReleaseMovingEntity();
        ItemEntity* RegisterItemEntity(Item* itembase, float x, float y, bool removeFromInventory = false);
        void DestroyItemEntity(RoomEntity* entity, bool addToInventory = false);
        
        void RegisterEntity(RoomEntity* entity, bool sorting = false);
        void UnregisterEntity(RoomEntity* entity);
        void SortEntities();
        void RegisterFurniture(FurnitureEntity* entity, bool sorting = false);
        void UnregisterFurniture(FurnitureEntity* entity, bool del = true);
        void SortFurniture();
        
        std::string loadedActivity;
        RoomEntity* loaded_ReturnToFood_entity;
        float loadedNeko_x, loadedNeko_y;
        void SaveApartment();
        void LoadApartment();
    };
}

#endif /* Apartment_hpp */
