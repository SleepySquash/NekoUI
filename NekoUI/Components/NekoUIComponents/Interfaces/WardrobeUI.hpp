//
//  WardrobeUI.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 28/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef WardrobeUI_hpp
#define WardrobeUI_hpp

#include <iostream>
#include <list>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <minEH/Engine/EntitySystem.hpp>
#include <minEH/Engine/Settings.hpp>
#include <minEH/Engine/Collectors.hpp>
#include <minEH/Engine/GUI/Button/SomeButtons.hpp>

#include "../Player.hpp"
#include "../Apartment/NekoEntity.hpp"
#include "../Apartment/RoomLibrary.hpp"
#include "../Database/ItemDB.hpp"

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;

using namespace ns;

namespace NekoUI
{
    struct WardrobeUI : Component
    {
        bool active{ false };
        
        NekoEntity* neko{ nullptr };
        GUI::SpriteButton quitButton;
        sf::Sprite background; bool spriteLoaded{ false };
        sf::RectangleShape nintShape;
        
        enum class Category { Head, Top, Bottom, Onepiece, Underwear, Socks, Shoes, Accessories, Sets } category{ Category::Head };
        float categoriesButtonsScaling{ 1.f }, categoriesStartXX{ 0 };
        GUI::SpriteButtons categoriesButtons; sf::Sprite selectedCategorySprite, selectedClothSprite, categoriesSprite;
        GUI::TextButton filterButtons;
        
        bool isEmpty{ true }, updateWindowRefresh{ false }; sf::Text emptyText;
        sf::Sprite slotSprite; GUI::SpriteButton slotUndressButton; GUI::SpriteButtons itemButtons;
        int slotsXXCount{ 0 }; float startSlotsXX{ 0 };
        InventoryBase<Wearable>* inventory{ nullptr };
        std::list<Item*> filter; bool filterApplied{ false };
        
        enum modeEnum { appearing, existing, disappearing };
        modeEnum mode{ appearing }; sf::Uint8 alpha{ 0 };
        float currentTime{ 0.f }, appearTime{ 0.3f }, disappearTime{ 0.2f };
        
        void Init() override;
        void Destroy() override;
        void Update(const sf::Time& elapsedTime) override;
        void CleanUp();
        void PollEvent(sf::Event& event) override;
        void DressUndressItem(Wearable* item, bool sort);
        void UndressByCategory(const ClothType& category);
        void Resize(const unsigned int& width, const unsigned int& height) override;
        void Draw(sf::RenderWindow* window) override;
        void ReceiveMessage(MessageHolder& message) override;
        void Switch(const bool& on);
        void UpdateAlpha();
        void UpdateCategory();
        void ResizeSlots();
    };
}

#endif /* WardrobeUI_hpp */
