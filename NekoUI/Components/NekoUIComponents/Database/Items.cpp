//
//  ItemDB.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 20/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "Items.hpp"

namespace NekoUI
{
    void ItemDatabaseCollection_LoadItems()
    {
        Inventory::map.emplace("Cupcake", new idc::Cupcake());
        Inventory::map.emplace("Cake", new idc::Cake());
        Inventory::map.emplace("Candy", new idc::Candy());
        Inventory::map.emplace("Star", new idc::Star());
        Inventory::map.emplace("Icecream", new idc::Icecream());
        Inventory::map.emplace("Donut", new idc::Donut());
        Inventory::map.emplace("FractionOfPie", new idc::FractionOfPie());
        Inventory::map.emplace("Lootbox", new idc::Lootbox());
        Inventory::map.emplace("WaterBottle", new idc::WaterBottle());
        
        Inventory::map.emplace("MaidUniform", new idc::MaidUniform());
        Inventory::map.emplace("MaidHeadwear", new idc::MaidHeadwear());
        Inventory::map.emplace("MaidGloves", new idc::MaidGloves());
        Inventory::map.emplace("WhiteStockings", new idc::WhiteStockings());
        Inventory::map.emplace("BlackStockings", new idc::BlackStockings());
        Inventory::map.emplace("MaidShoes", new idc::MaidShoes());
        Inventory::map.emplace("NekoCollar", new idc::NekoCollar());
        Inventory::map.emplace("PleatedSailorSkirt", new idc::PleatedSailorSkirt());
        Inventory::map.emplace("SailorBlouse", new idc::SailorBlouse());
        Inventory::map.emplace("SailorBlouse_top", new idc::SailorBlouse_top());
        Inventory::map.emplace("Trousers", new idc::Trousers());
        Inventory::map.emplace("Bra1", new idc::Bra1());
        Inventory::map.emplace("Panties1", new idc::Panties1());
    }

    namespace idc
    {
        Cupcake::Cupcake() : Item("Cupcake", L"Вкусный пирожочек с вишенкой.", ItemType::Food)
            { display = L"Пирожок"; calories = 150; thirstSatisfuction = -50; price = 50; }
        Cake::Cake() : Item("Cake", L"Шоколадный тортик с вишенкой наверху.", ItemType::Food, 10)
            { display = L"Тортик"; calories = 700; thirstSatisfuction = -350; scale = 1.1f; price = 230; }
        Candy::Candy() : Item("Candy", L"М-м, тот самый ностальгический вкус.", ItemType::Food)
            { display = L"Конфетка"; calories = 50; thirstSatisfuction = -15; scale = 0.5; price = 25; }
        Star::Star() : Item("Star", L"Звёздочка с неба.", ItemType::Other, 90) { display = L"Звезда"; }
        Icecream::Icecream() : Item("Icecream", L"Приятно тает во рту.", ItemType::Food)
            { display = L"Мороженка"; calories = 150; thirstSatisfuction = 50; price = 50; }
        Donut::Donut() : Item("Donut", L"Шоколадный пончик в клубничной глазури.", ItemType::Food)
            { display = L"Пончик"; calories = 100; thirstSatisfuction = -20; scale = 0.75f; price = 60; }
        FractionOfPie::FractionOfPie() : Item("FractionOfPie", L"Кусочек клубничного панкейка.", ItemType::Food)
            { display = L"Кусок тортика"; calories = 200; thirstSatisfuction = -100; price = 100; }
        Lootbox::Lootbox() : Item("Lootbox", L"Интересно, что внутри?", ItemType::Other, 100, true)
            { display = L"Лутбокс"; scale = 3; shadowOffsetYY = -15; }
        WaterBottle::WaterBottle() : Item("WaterBottle", L"Освежающая водичка идеальной температуры из самой-самой чистой скважины.", ItemType::Drink)
            { display = L"Бутылка воды"; calories = 0; thirstSatisfuction = 660; scale = 1.1; price = 30; }
        


        MaidUniform::MaidUniform() : Wearable("MaidUniform", L"maid uniform.png", L"maid uniform.png", L"Костюм мейдочки.")
        {
            display = L"Униформа мейдочки"; scale = 1.5f; depth = 80; clothing = ClothType::Onepiece;
            relativeChibiScale  = 0.329595; offsets.first = {-28, -427};
            relativePersonScale = 0.455800; offsets.second = {27, -659};
        }
        MaidHeadwear::MaidHeadwear() : Wearable("MaidHeadwear", L"headband.png", L"headband.png", L"Ободок мейдочки.")
        {
            display = L"Обруч мейдочки"; scale = 0.8f; depth = 130; clothing = ClothType::Head;
            relativeChibiScale  = 0.24;  offsets.first = {17, -820};
            relativePersonScale = 0.074; offsets.second = {-6, -795};
        }
        MaidGloves::MaidGloves() : Wearable("MaidGloves", L"handcliff.png", L"handcliff.png", L"Аккуратные нежные перчаточки~ <3")
        {
            display = L"Перчатки мейдочки"; depth = 85; clothing = ClothType::Gloves;
            relativeChibiScale  = 0.045; offsets.first = {-1, -264};
            relativePersonScale = 0.032333; offsets.second = {84, -693};
        }
        WhiteStockings::WhiteStockings() : Wearable("WhiteStockings", L"stockings.png", L"stockings.png", L"Белые, как снежок, чулочки.")
        {
            display = L"Белые чулочки"; depth = 10; clothing = ClothType::Socks;
            relativeChibiScale  = 0.180708; offsets.first = {-19, -149};
            relativePersonScale = 0.425710; offsets.second = {-24, -341};
        }
        BlackStockings::BlackStockings() : Wearable("BlackStockings", L"stockings.png", L"stockings.png", L"Чёрные, как ночка, чулочки.")
        {
            display = L"Чёрные чулочки"; depth = 10; clothing = ClothType::Socks;
            relativeChibiScale  = 0.180708; offsets.first = {-19, -149};
            relativePersonScale = 0.425710; offsets.second = {-24, -341};
            chibiColor = personColor = sf::Color(40, 40, 40);
        }
        MaidShoes::MaidShoes() : Wearable("MaidShoes", L"shoes.png", L"shoes.png", L"Словно хрустальные.")
        {
            display = L"Туфельки"; depth = 15; clothing = ClothType::Legwear;
            relativeChibiScale  = 0.024013; offsets.first = {-17, -19};
            relativePersonScale = 0.145778; offsets.second = {-30, -110};
        }
        NekoCollar::NekoCollar() : Wearable("NekoCollar", L"collar.png", L"collar.png", L"Для самой лучшей кошечки.")
        {
            display = L"Неко ошейник"; depth = 77; clothing = ClothType::Accessory;
            relativeChibiScale  = 0.027470; offsets.first = {-20, -418};
            relativePersonScale = 0.049989; offsets.second = {-26, -655};
        }
        PleatedSailorSkirt::PleatedSailorSkirt() : Wearable("PleatedSailorSkirt", L"pleated_sailor_skirt.png", L"pleated_sailor_skirt.png", L"Юбочка в стиле сейлор фуку.")
        {
            display = L"Сейлор юбочка"; depth = 60; clothing = ClothType::Bottom;
            relativeChibiScale  = 0.1295045; offsets.first = {-25.675676, -265.315315};
            relativePersonScale = 0.21396966; offsets.second = {-32.3640961, -490.518331};
        }
        SailorBlouse::SailorBlouse() : Wearable("SailorBlouse", L"sailor_blouse.png", L"sailor_blouse.png", L"Идеально для сейлор фуку.")
        {
            display = L"Сейлор матроска"; depth = 40; clothing = ClothType::Top;
            relativeChibiScale  = 0.244369; offsets.first = {-24.775, -426.126};
            relativePersonScale = 0.24115044; offsets.second = {-23.0088496, -657.648546};
            AddDependency("SailorBlouse_top");
        }
        SailorBlouse_top::SailorBlouse_top() : Wearable("SailorBlouse_top", L"", L"sailor_blouse_top.png")
        {
            depth = 75; clothing = ClothType::Non;
            relativeChibiScale  = 0.061937; offsets.first = {-30.1802, -426.126};
        }

        Trousers::Trousers() : Wearable("Trousers", L"trousers.png", L"trousers.png", L"Очень удобные и обтягивающие спортивные штаны.")
        {
            display = L"Спортивные штаны"; depth = 30; clothing = ClothType::Bottom;
            relativeChibiScale  = 0.29673423; offsets.first = {-24.7747748, -265.765766};
            relativePersonScale = 0.48988622; offsets.second = {-24.0202276, -498.356511};
        }

        Bra1::Bra1() : Wearable("Bra1", L"bra1.png", L"bra1.png", L"Достаточно взрослый бюстгалтер.")
        {
            display = L"Лифчик"; depth = 5; clothing = ClothType::Bra;
            relativeChibiScale  = 0.12894144; offsets.first = {-19.3693694, -419.81982};
            relativePersonScale = 0.11441214; offsets.second = {-17.9519595, -647.029077};
            //personColor = chibiColor = { 229, 90, 97 };
        }
        Panties1::Panties1() : Wearable("Panties1", L"panties1.png", L"panties1.png", L"Достаточно взрослые трусики.")
        {
            display = L"Трусики"; depth = 5; clothing = ClothType::Pantsu;
            relativeChibiScale  = 0.06531532; offsets.first = {-24.7747748, -245.495495};
            relativePersonScale = 0.07206068; offsets.second = {-31.6055626, -459.165613};
            //personColor = chibiColor = { 229, 90, 97 };
        }
    }
}
