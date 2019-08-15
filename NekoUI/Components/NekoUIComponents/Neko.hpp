//
//  Neko.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 10/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef Neko_hpp
#define Neko_hpp

#include <iostream>
#include <vector>
#include <list>

#include <SFML/Graphics.hpp>

#include "../../Engine/Settings.hpp"

#include "Static/NekoStatic.hpp"
#include "Engine/Interest.hpp"
#include "Database/InterestsDB.hpp"
#include "Engine/Skill.hpp"
#include "Database/SkillDB.hpp"

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;
using std::list;
using std::vector;

using namespace ns;

namespace NekoUI
{
    struct NekoPersonality
    {
        /// У каждого интереса и способности должны быть float-переменные "сила интереса"/"предпочтение" или вроде того.
        
        enum class InterestEnum { Cooking, Reading, Hugs, Kisses, Love, Beuty, Cuteness, Kindness, Manga, Anime, IT, Programming, Cosplay, Gothic, Lolita, Music, Games, Bathing, Eating, BeingLazy, Fashion, Sport, Cybersport, Military, Politics, Drawing, Theather, Movies, Art, Gardening, Animals, Nature, Summer, Winter, Autumn, Spring, Science, History, Writing, Philosophy };
        enum class ReadingGenres { Horror, Drama, Action, Adventure, Documentary, History, Science, Fiction, ScienceFiction, Poem, Novel };
        enum class MusicGenres{ Rock, Pop, Jazz, KPop, JPop, JRock, Instrumentary, Classic, PostRock, Folk };
        enum class GamesGenres{ TabletopGames, ComputerGames, RTS, FPS, OnlineGames };
        enum class SportActivities{ Running, Exercise, SportGames };
        enum class AnimalsLoving{ Cats, Dogs, Hamsters, Bunnies, GuineaPigs, Parrots, Foxes, Wolves };
        enum class ScienceGenres{ Biology, Physics, Maths, Laws, Philosophy };
        static vector<std::string> foodPreferences, drinkPreferences;
        static vector<std::string> foodAntipathy, drinkAntipathy;
        
        // enum class MoodEnum{ Happy, Discomfort, Sadness, Horny, Satisfied, Anger, Calm, Crying, Horrified, Shocked, Euphoria, Bored, Lonely, Rejected };
        static NekoStatic::MoodEnum mood;
        
        enum class TraitsEnum { Kind, Lovely, Caring, Shy, Cute, Quiet, Playful, Sly, Romantic, Lewd, Scared, Sensetive, Active, Passive, Loyal, Dependent, Independent, Crybabe, Hardworker, Lazy, Calm, Cold, Sweettooth, Slow };
        enum class DiseasesEnum{ Cold, Fever, Albino, Myopia, Hyperopia, Weak, Ulcer, Depression, Paranoid, Deaf };
        enum class ShapeEnum{ Slim, Normal, Thick }; /// Лучше численное представление.
        enum class TemperamentEnum{ Choleric, Melancholic, Phlegmatic, Sanguine };
        
        enum class ActivitiesPossibleEnum{ Sleeping, Toilet, Bath, CleaningTeeth, ChangingClothes, Cooking, Sitting, Walking, Playing, Computer, Exercising, Singing, Dancing, Reading, ListenToMusic, Watching, Eating, Drinking, Cleaning, Gardening };
        
        int Height{ 160 }, Weight{ 40 }, Age{ 10 };
        int BrainPOWEEEEER{ 100 };
        
        
        /* enum class EyebrowsEmotion { Normal };
           enum class EyesEmotion { Normal, Closed, Confused };
           enum class MouthEmotion { Smile, Neutral, Sad }; */
        static NekoStatic::EyebrowsEmotion eyebrowsEmotion;
        static NekoStatic::EyesEmotion eyesEmotion;
        static NekoStatic::MouthEmotion mouthEmotion;
        
        vector<Interest*> interests;
    };
    
    typedef NekoPersonality nekodef;
    typedef NekoPersonality Neko;
}

#endif /* Neko_hpp */