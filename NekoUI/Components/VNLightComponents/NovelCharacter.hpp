//
//  NovelCharacter.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 22/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef NovelCharacter_hpp
#define NovelCharacter_hpp

#include <iostream>

namespace NekoUI
{
    struct NovelCharacter
    {
        std::wstring name{ L"" }, display{ L"" }, texturePath{ L"" };
        float novelScale{ 1.f };
    };
}

#endif /* NovelCharacter_hpp */
