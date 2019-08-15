//
//  Neko.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 10/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "Neko.hpp"

namespace NekoUI
{
    NekoStatic::MoodEnum Neko::mood{ NekoStatic::MoodEnum::Happy };
    
    NekoStatic::EyebrowsEmotion Neko::eyebrowsEmotion{ NekoStatic::EyebrowsEmotion::Normal };
    NekoStatic::EyesEmotion Neko::eyesEmotion{ NekoStatic::EyesEmotion::Normal };
    NekoStatic::MouthEmotion Neko::mouthEmotion{ NekoStatic::MouthEmotion::Smile };
}
