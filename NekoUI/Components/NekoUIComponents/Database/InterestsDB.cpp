//
//  InterestsDB.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 10/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "InterestsDB.hpp"

namespace NekoUI
{
    std::unordered_map<std::string, Interest*> Interests::map;
    void InterestDatabaseCollection_LoadInterests()
    {
        Interests::map["Cooking"] = new Interest("Cooking");
        Interests::map["Reading"] = new Interest("Reading");
        Interests::map["Hugs"] = new Interest("Hugs");
        Interests::map["Manga"] = new Interest("Manga");
        Interests::map["Anime"] = new Interest("Anime");
        Interests::map["Music"] = new Interest("Music");
        Interests::map["Kisses"] = new Interest("Kisses");
        Interests::map["Sport"] = new Interest("Sport");
        Interests::map["BeingLazy"] = new Interest("BeingLazy");
        
        // enum class InterestEnum { Love, Beuty, Cuteness, Kindness, IT, Programming, Cosplay, Gothic, Lolita, Games, Bathing, Fashion, Cybersport, Military, Politics, Drawing, Theather, Movies, Art, Gardening, Animals, Nature, Summer, Winter, Autumn, Spring, Science, History, Writing, Philosophy };
    }
}
