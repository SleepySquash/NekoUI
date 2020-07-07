//
//  Neko.hpp
//  NekoUI
//
//  Created by Никита Исаенко on 10/08/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef NekoPersonality_hpp
#define NekoPersonality_hpp

#include <iostream>
#include <vector>
#include <list>

#include <SFML/Graphics.hpp>

#include <minEH/Engine/Settings.hpp>

#include "Static.hpp"
#include "../Engine/Interest.hpp"
#include "../Engine/Skill.hpp"

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
    namespace Neko
    {
        struct Personality
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
            
            static s::MoodEnum mood;
            static float moodValue;
            
            enum class TraitsEnum { Kind, Lovely, Caring, Shy, Cute, Quiet, Playful, Sly, Romantic, Lewd, Scared, Sensetive, Active, Passive, Loyal, Dependent, Independent, Crybabe, Hardworker, Lazy, Calm, Cold, Sweettooth, Slow };
            enum class DiseasesEnum{ Cold, Fever, Albino, Myopia, Hyperopia, Weak, Ulcer, Depression, Paranoid, Deaf, Aphonia };
            enum class ShapeEnum{ Slim, Normal, Thick }; /// Лучше численное представление.
            enum class TemperamentEnum{ Choleric, Melancholic, Phlegmatic, Sanguine };
            
            enum class ActivitiesPossibleEnum{ Sleeping, Toilet, Bath, CleaningTeeth, ChangingClothes, Cooking, Sitting, Walking, Playing, Computer, Exercising, Singing, Dancing, Reading, ListenToMusic, Watching, Eating, Drinking, Cleaning, Gardening };
            
            int Height{ 160 }, Weight{ 40 }, Age{ 10 };
            int BrainPOWEEEEER{ 100 };
            
            vector<Interest*> interests;
        };
    
        struct Character
        {
            enum class Temperament { Optimist, Pessimist, Realist } static temperament;
            
            /// https://www.myersbriggs.org/my-mbti-personality-type/mbti-basics/the-16-mbti-types.htm?bhcp=1
            enum class MBTI { ISTJ, ISFJ, INFJ, INTJ, ISTP, ISFP, INFP, INTP, ESTP, ESFP, ENFP, ENTP, ESTJ, ESFJ, ENFJ, ENTJ } static mbti;
            /// Openness:          appreciation for a variety of experiences.
            /// Conscientiousness: planning ahead rather than being spontaneous.
            /// Extraversion:      being sociable, energetic and talkative.
            /// Agreeableness:     being kind, sympathetic and happy to help.
            /// Neuroticism:       inclined to worry or be vulnerable or temperamental.
        
            struct Traits
            {
                /// https://examples.yourdictionary.com/examples-of-personality-traits.html
                
                static unsigned char Adventurous, Helpful, Affable, Humble, Capable, Imaginative, Charming, Impartial, Confident, Independent, Dependent, Conscientious, Keen, Cultured, Meticulous, Dependable, Observant, Discreet, Optimistic, Dutiful, Persistent, Encouraging, Precise, Exuberant, Reliable, Fair, Sociable, Fearless, Trusting, Gregarious, Valiant, Honest, Loyal, Generous, Devoted, Sincere, Patient, Charisma, Playful, Wild, Funny, Talkative, Shy, Lively;
                static unsigned char Arrogant, Quarrelsome, Boorish, Rude, Bossy, Sarcastic, Conceited, Selfcentered, Cowardly, Slovenly, Sneaky, Finicky, Stingy, Impulsive, Sullen, Lazy, Surly, Malicious, Thoughtless, Obnoxious, Unfriendly, Picky, Unruly, Pompous, Vulgar, Dishonest, Disloyal, Impatient, Greedy, Cruel, Selfish, Unforgiving, Stubborn, Tough;
            };
        };
        
        struct Condition
        {
            static bool exhausted, starvated, dehydrated, sleepy, lonely, melancholy, depressed;
            
            /*
             
            Состояния должны иметь таймеры или условия, которые должны быть выполнены, чтобы состояние исчезло. Определённые действия могут ускорять таймеры состояний или искоренять их вовсе.
            Состояния должны влиять на поведение кошечки. Состояние "Воодушевление" должно длиться недолго, может быть исключено негативными обстоятельствами, но даёт 20% бонус ко времени выполнения или вероятности успеха творческого дела (например, рисование или написание музыки/программы, приготовление блюда, практикованию танца, физическим упражнениям итд). "Воодушевление" может появиться после приятного разговора, приятной компании, приятного взаимодействия, воодушевляющего разговора при учёте, что настроение было до этого просто нормальным.
            Например, условием может быть неравенство NekoS::Hunger > 300 для голодания. Условия обновляются при изменении числа. Например, каждые 5-10 очков вниз или вверх - аккуратность не гарантируется, а также при потреблении пищи.
            
            Нам нужен мозг - "вычислительный ресурс" - который определит настроение по сумме состояний.
            NekoEntity - это репрезентация кошечки в комнате. Даже вне этой комнаты кошечка продолжает существовать, хотя может быть приостановлена. Потому что когда ты идёшь один, то да, симуляция. А иначе кошечка-то ведь с тобой может гулять - она продолжает иметь состояния и настроение.
            
            Кошечка должна иметь действия-активности, которые:
              1) Определяются настроением и суммой факторов. Кошечка должна "хотеть" этим заняться. Черты характера, т.е. восприятие кошечкой мира и её мировоззрене должны влиять на это.
              2) Имеют уровень мастерства кошечки и её заинтересованность в деле.
            Например, "Чтение" может происходит часто при неотрицательном настроении и состоянии, если кошечка "Книжный червь". Иначе она будет читать только тогда, когда в этом будет необходимость - она скучает (нечего делать), либо ты купил новую книжку, соответствующую её интересам (или её интересы изменились и стали охватывать книгу, которая уже есть). При этом книга может увлечь кошечку, в результате чего она будет читать её как книжный червь (т.е. будет приоритет активности "Чтение" над другими при планировании активности), пока не прочтёт.
            
            Планирование активности должно учитывать настроение, состояния и характер кошечки. Черты характера влияют на выбор, также состояние влияет на выбор, как и настроение в том числе. Однако настроение и состояния коротковременные, а черты характера - долговременные.
            
            Характер
            Черты характера - это не просто флажки, т.к. они должны иметь степень проявления.
            «Я хитрая». «Я наглая». «Я требую много внимания и любви».
            Черты характера переменны. Если ты будешь говорить о том, как плохо быть наглой/хитрой итд, то со временем эта черта будет отходить (т.е. переменная переполняться), пока она не исчезнет. Некоторые черты характера исчезают с увеличением привязанности.

            Интересы
            Интерес - степень знания и любви к объекту. Интерес может быть как положительным, так и отрицательным.
            «Я увлекаюсь … и люблю это настолько-то хорошо», «мне интересен …, но я не разбираюсь», «я разбираюсь в …, но не люблю это».
            Книжный червь - это не черта характера, а то, что ты разрабатываешь, оно определяется твоим интересом к книгам. Всё с чего-то начинается и имеет причину, даже черты характера - потому они и переменны.

            Умения
            «Я умею это делать настолько-то хорошо»
             
             */
        };
        
        typedef Personality p;
    }
    typedef Neko::Personality NekoP;
    typedef Neko::Character::Traits NekoT;
}

#endif /* NekoPersonality_hpp */
