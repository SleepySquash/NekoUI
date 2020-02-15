//
//  NekoLibrary.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 27/02/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "NekoLibrary.hpp"

namespace NekoNinja
{
    NekoLibrary* NekoLibrary::self{ nullptr };
    void NekoLibrary::clear() { for (auto& n : neko) delete n; neko.clear(); }
}
