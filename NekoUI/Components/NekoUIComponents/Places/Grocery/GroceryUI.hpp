//
//  ShopUI.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 18.01.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#ifndef GroceryUI_hpp
#define GroceryUI_hpp

#include <iostream>
#include <vector>
#include <unordered_map>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include "../../../../Essentials/ResourcePath.hpp"
#include "../../../../Engine/Settings.hpp"
#include "../../../../Engine/Collectors/Font.hpp"
#include "../../../../Engine/Collectors/Image.hpp"
#include "../../../../Engine/EntitySystem.hpp"
#include "../../../../Engine/GUI/Button/SomeButtons.hpp"
#include "../../../../Engine/GUI/Button/RoundedRectangleButton.hpp"

#include "../../Neko/Static.hpp"
#include "../../Apartment/RoomLibrary.hpp"
#include "../../Engine/Item.hpp"
#include "../../Database/ItemDB.hpp"

#include "ItemDetails.hpp"

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;

namespace NekoUI
{
    namespace Places
    {
        struct ShelfItemStruct
        {
            Item* item;
            sf::Texture* texture{ nullptr };
            int count{ 0 };
            
            ShelfItemStruct(Item* item = nullptr, int qty = 0);
            ~ShelfItemStruct();
            ShelfItemStruct(ShelfItemStruct&& b);
        };
        struct GroceryUI : Component
        {
            bool active{ true };
            float xx, yy; bool done; int xy, selectedxy;
            
            sf::RectangleShape loadingShape;
            GUI::SpriteButton quitB, checkoutB;
            GUI::RoundedRectangleButton discountsB, foodB, drinksB, householdB;
            sf::Uint8 alpha{ 255 };
            
            sf::Sprite background; bool spriteLoaded{ false };
            sf::Sprite shelvesSprite, shelfSprite, shelfItem; sf::RenderTexture shelvesTexture; bool shelvedRendered{ false };
            sf::RectangleShape pricetagShape; sf::Text pricetagText;
            int shelfColumnL{ 0 }, shelfColumnR{ 0 }, shelfColumns{ 0 }, shelfCount{ 0 };
            float shelfItemWidth{ 0 };
            std::vector<ShelfItemStruct> shelfFood;
            
            std::unordered_map<Item*, int> checkList; int checkPrice{ 0 };
            sf::Text checkCaption, checkBalance, checkName, checkQty, checkPriceT;
            GUI::RoundedRectangleButton payB, resetB;
            sf::RoundedRectangleShape checkShape; sf::Sprite nekopaySprite;
            float checkLinesYY{ 0 }, checkLinesXX1{ 0 }, checkLinesXX2{ 0 }, checkLinesXX3{ 0 };
            
            enum class Mode{ Main, Discount, Food, Drinks, Household, Check } mode{ Mode::Main }, prev{ Mode::Main };
            
            void Init() override;
            void Destroy() override;
            void SwitchMode(const Mode& to);
            void PollEvent(sf::Event& event) override;
            void ResizeCheck();
            void Resize(unsigned int width, unsigned int height) override;
            void Draw(sf::RenderWindow* window) override;
            void RecieveMessage(MessageHolder& message) override;
            void UpdateAlpha();
        };
    }
}

#endif /* GroceryUI_hpp */
