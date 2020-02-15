//
//  Body.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 15.02.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#ifndef Body_hpp
#define Body_hpp

#include "Static.hpp"
#include "Personality.hpp"
#include "../Engine/Persona.hpp"

namespace NekoUI
{
    namespace Neko
    {
        struct Body
        {
            static Persona neko;
            
            static void OccupyPersona();
            static void FreePersona();
            
            static NekoS::EyebrowsEmotion eyebrowsEmotion;
            static NekoS::EyesEmotion eyesEmotion;
            static NekoS::MouthEmotion mouthEmotion;
            static void UpdateNekoEmotion();
            enum class RestoringEmotion{ Eyebrows, Eyes, Mouth };
            static void NekoEmotionAccordingToMood(const RestoringEmotion& restoring);
            static void NekoEmotionsAccordingToMood();
        };
        
        typedef Body b;
    }
    typedef Neko::Body NekoB;

}

#endif /* Body_hpp */
