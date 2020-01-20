//
//  PlacesInterfaceUI.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 19.01.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#ifndef PlacesInterfaceUI_hpp
#define PlacesInterfaceUI_hpp

#include "../../../Engine/EntitySystem.hpp"

#include "Grocery/GroceryUI.hpp"
#include "../Apartment/Apartment.hpp"

namespace NekoUI
{
    struct PlacesInterfaceUI : Component
    {
        void RecieveMessage(MessageHolder& message) override;
    };
}

#endif /* PlacesInterfaceUI_hpp */
