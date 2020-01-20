//
//  PlacesInterfaceUI.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 19.01.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#include "PlacesInterfaceUI.hpp"

namespace NekoUI
{
    void PlacesInterfaceUI::RecieveMessage(MessageHolder& message)
    {
        if (message.info == "PlacesUI :: GroceryUI") entity->AddComponent<Places::GroceryUI>();
        else if (message.info == "PlacesUI :: ApartmentUI") entity->AddComponent<Apartment>();
    }
}
