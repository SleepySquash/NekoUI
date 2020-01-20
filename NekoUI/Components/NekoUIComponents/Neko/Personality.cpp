//
//  Neko.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 10/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "Personality.hpp"

namespace NekoUI
{
    namespace Neko
    {
        s::MoodEnum Personality::mood{ s::MoodEnum::Happy };
        
        s::EyebrowsEmotion Personality::eyebrowsEmotion{ s::EyebrowsEmotion::Normal };
        s::EyesEmotion Personality::eyesEmotion{ s::EyesEmotion::Normal };
        s::MouthEmotion Personality::mouthEmotion{ s::MouthEmotion::Smile };
    }
}
