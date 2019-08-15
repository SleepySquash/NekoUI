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
    struct NekoStatic
    {
        enum class MoodEnum{ Happy, Discomfort, Sadness, Horny, Satisfied, Anger, Calm, Crying, Horrified, Shocked, Euphoria, Bored, Lonely, Rejected };
        
        enum class EyebrowsEmotion { Normal,   DEFAULT };
        enum class EyesEmotion { Normal, Closed, Confused,   DEFAULT };
        enum class MouthEmotion { Smile, Neutral, Sad,   DEFAULT };
        
        
        static unsigned long money;
        static float affection; /// +Opinion about you
        static unsigned int age;
        static float maxNeed, needHunger, needThirst, needWarmth, needCommunication, needHygiene, needToilet, needEnergy, needEntertaiment;
        static float hungerInSecond, thirstInSecond, communicationInSecond, hygieneInSecond, toiletInSecond, energyInSecond, entertaimentInSecond;
    };
    
    typedef NekoStatic NStat;
}

#endif /* NekoStatic_hpp */
