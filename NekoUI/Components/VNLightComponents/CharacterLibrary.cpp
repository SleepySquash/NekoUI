//
//  CharacterLibrary.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 22/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "CharacterLibrary.hpp"

namespace NekoUI
{
    unordered_map<std::wstring, NovelCharacter*> CharacterLibrary::library;
}
