//
//  NekoStatic.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 14/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef NekoStatic_hpp
#define NekoStatic_hpp

namespace NekoUI
{
    namespace Neko
    {
        struct Static
        {
            enum class MoodEnum{ Happy, Discomfort, Sadness, Horny, Satisfied, Anger, Calm, Crying, Horrified, Shocked, Euphoria, Bored, Lonely, Rejected };
            
            enum class EyebrowsEmotion { Normal, Embarrassed,   DEFAULT };
            enum class EyesEmotion { Normal, Closed, Confused, Embarrassed, ClosedHappy,     DEFAULT };
            enum class MouthEmotion { Smile, Neutral, Dot, Open, Sad,   DEFAULT };
            
            
            static unsigned long money;
            static float affection; /// +Opinion about you
            static unsigned int age;
            static float maxNeed, needHunger, needThirst, needWarmth, needCommunication, needHygiene, needToilet, needEnergy, needEntertaiment;
            static float hungerInSecond, thirstInSecond, communicationInSecond, hygieneInSecond, toiletInSecond, energyInSecond, entertaimentInSecond;
            static float autoCapHunger, autoCapThirst, autoCapWarmth, autoCapCommunication, autoCapHygiene, autoCapToilet, autoCapEnergy, autoCapEntertaiment;
        };
        
        typedef Static s;
    }
    typedef Neko::Static NekoS;
}

#endif /* NekoStatic_hpp */
