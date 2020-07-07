//
//  ItemDB.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 20/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef ItemDB_hpp
#define ItemDB_hpp

#include <iostream>
#include <fstream>
#include <list>
#include <unordered_map>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <minEH/Essentials/ResourcePath.hpp>
#include <minEH/Essentials/Base.hpp>
#include <minEH/Engine/NovelSomeScript.hpp>
using namespace ns;

#include "../Engine/Inventory.hpp"

namespace NekoUI
{
    namespace idc
    {
        struct Cupcake : Item { Cupcake(); };
        struct Cake : Item { Cake(); };
        struct Candy : Item { Candy(); };
        struct Star : Item { Star(); };
        struct Icecream : Item { Icecream(); };
        struct Donut : Item { Donut(); };
        struct FractionOfPie : Item { FractionOfPie(); };
        struct Lootbox : Item { Lootbox(); };
        struct WaterBottle : Item { WaterBottle(); };
        
    
        struct MaidUniform : Wearable { MaidUniform(); };
        struct MaidHeadwear : Wearable { MaidHeadwear(); };
        struct MaidGloves : Wearable { MaidGloves(); };
        struct WhiteStockings : Wearable { WhiteStockings(); };
        struct BlackStockings : Wearable { BlackStockings(); };
        struct MaidShoes : Wearable { MaidShoes(); };
        struct NekoCollar : Wearable { NekoCollar(); };
    
        struct PleatedSailorSkirt : Wearable { PleatedSailorSkirt(); };
    
        struct SailorBlouse : Wearable { SailorBlouse(); };
        struct SailorBlouse_top : Wearable { SailorBlouse_top(); };
        
        struct Trousers : Wearable { Trousers(); };
            
        struct Bra1 : Wearable { Bra1(); };
        struct Panties1 : Wearable { Panties1(); };
    }
    
    void ItemDatabaseCollection_LoadItems();
}

#endif /* ItemDB_hpp */
