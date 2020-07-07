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
        float Personality::moodValue{ 50 };
    
        Character::Temperament Character::temperament{ Character::Temperament::Realist };
        Character::MBTI Character::mbti{ Character::MBTI::ENFJ };
    
        unsigned char NekoT::Adventurous, NekoT::Helpful, NekoT::Affable, NekoT::Humble, NekoT::Capable, NekoT::Imaginative, NekoT::Charming, NekoT::Impartial, NekoT::Confident, NekoT::Independent, NekoT::Dependent, NekoT::Conscientious, NekoT::Keen, NekoT::Cultured, NekoT::Meticulous, NekoT::Dependable, NekoT::Observant, NekoT::Discreet, NekoT::Optimistic, NekoT::Dutiful, NekoT::Persistent, NekoT::Encouraging, NekoT::Precise, NekoT::Exuberant, NekoT::Reliable, NekoT::Fair, NekoT::Sociable, NekoT::Fearless, NekoT::Trusting, NekoT::Gregarious, NekoT::Valiant, NekoT::Honest, NekoT::Loyal, NekoT::Generous, NekoT::Devoted, NekoT::Sincere, NekoT::Patient, NekoT::Charisma, NekoT::Playful, NekoT::Wild, NekoT::Funny, NekoT::Talkative, NekoT::Shy, NekoT::Lively;
        unsigned char NekoT::Arrogant, NekoT::Quarrelsome, NekoT::Boorish, NekoT::Rude, NekoT::Bossy, NekoT::Sarcastic, NekoT::Conceited, NekoT::Selfcentered, NekoT::Cowardly, NekoT::Slovenly, NekoT::Sneaky, NekoT::Finicky, NekoT::Stingy, NekoT::Impulsive, NekoT::Sullen, NekoT::Lazy, NekoT::Surly, NekoT::Malicious, NekoT::Thoughtless, NekoT::Obnoxious, NekoT::Unfriendly, NekoT::Picky, NekoT::Unruly, NekoT::Pompous, NekoT::Vulgar, NekoT::Dishonest, NekoT::Disloyal, NekoT::Impatient, NekoT::Greedy, NekoT::Cruel, NekoT::Selfish, NekoT::Unforgiving, NekoT::Stubborn, NekoT::Tough;
    
        bool Condition::exhausted{ false }, Condition::starvated{ false }, Condition::dehydrated{ false }, Condition::sleepy{ false }, Condition::lonely{ false }, Condition::melancholy{ false }, Condition::depressed{ false };
    }
}
