//
//  CharacterLibrary.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 22/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef CharacterLibrary_hpp
#define CharacterLibrary_hpp

#include <iostream>
#include <unordered_map>

#include "NovelCharacter.hpp"

using std::unordered_map;

namespace NekoUI
{
    struct CharacterLibrary
    {
        static unordered_map<std::wstring, NovelCharacter*> library;
    };
}

#endif /* CharacterLibrary_hpp */
