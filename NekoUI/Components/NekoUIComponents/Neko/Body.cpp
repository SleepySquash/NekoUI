//
//  Body.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 15.02.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#include "Body.hpp"

namespace NekoUI
{
    namespace Neko
    {
        Persona Body::neko;
        NekoS::EyebrowsEmotion Body::eyebrowsEmotion{ NekoS::EyebrowsEmotion::Normal };
        NekoS::EyesEmotion Body::eyesEmotion{ NekoS::EyesEmotion::Normal };
        NekoS::MouthEmotion Body::mouthEmotion{ NekoS::MouthEmotion::Smile };
        
        void Body::OccupyPersona() { neko.OccupyMemory(); UpdateNekoEmotion(); neko.chibiReversed = false; }
        void Body::FreePersona() { neko.FreeMemory(); neko.chibiReversed = false; NekoEmotionsAccordingToMood(); }
        
        
        
        void Body::UpdateNekoEmotion()
        {
            if (eyebrowsEmotion != NekoP::eyebrowsEmotion)
            {
                if (NekoP::eyebrowsEmotion == NekoS::EyebrowsEmotion::DEFAULT)
                    NekoEmotionAccordingToMood(RestoringEmotion::Eyebrows);
                eyebrowsEmotion = NekoP::eyebrowsEmotion;
                switch (eyebrowsEmotion)
                {
                    case NekoS::EyebrowsEmotion::Normal: neko.eyebrows.Load(L"eyebrows_normal.png", L"eyebrows_normal.png");
                        neko.eyebrows.relativeChibiScale = 0.069701; neko.eyebrows.SetOffsetChibi({14, -654});
                        neko.eyebrows.relativePersonScale = 0.023205; neko.eyebrows.SetOffsetPerson({-9, -741}); break;
                    case NekoS::EyebrowsEmotion::Embarrassed: neko.eyebrows.Load(L"eyebrows_embarrass.png", L"eyebrows_embarrass.png");
                        neko.eyebrows.relativeChibiScale = 0.113471; neko.eyebrows.SetOffsetChibi({0, -636});
                        neko.eyebrows.relativePersonScale = 0.035975; neko.eyebrows.SetOffsetPerson({-10, -738}); break;
                    default: break;
                }
                neko.eyebrows.ResizeChibi(neko.body.chibi.getGlobalBounds().height);
                neko.eyebrows.UpdatePositionChibi(neko.body.chibi.getPosition().x, neko.body.chibi.getPosition().y);
                neko.eyebrows.ResizePerson(neko.body.person.getGlobalBounds().height);
                neko.eyebrows.UpdatePositionPerson(neko.body.person.getPosition().x, neko.body.person.getPosition().y);
            }
            if (eyesEmotion != NekoP::eyesEmotion)
            {
                if (NekoP::eyesEmotion == NekoS::EyesEmotion::DEFAULT)
                    NekoEmotionAccordingToMood(RestoringEmotion::Eyes);
                eyesEmotion = NekoP::eyesEmotion;
                switch (eyesEmotion)
                {
                    case NekoS::EyesEmotion::Normal: neko.eyes.Load(L"eyes_normal.png", L"eyes_normal.png");
                        neko.eyes.relativeChibiScale = 0.131615; neko.eyes.SetOffsetChibi({6, -601});
                        neko.eyes.relativePersonScale = 0.042294; neko.eyes.SetOffsetPerson({-11, -729}); break;
                    case NekoS::EyesEmotion::Closed: neko.eyes.Load(L"eyes_closed.png", L"eyes_closed.png");
                        neko.eyes.relativeChibiScale = 0.084754; neko.eyes.SetOffsetChibi({-1, -555});
                        neko.eyes.relativePersonScale = 0.021370; neko.eyes.SetOffsetPerson({-17, -712}); break;
                    case NekoS::EyesEmotion::ClosedHappy: neko.eyes.Load(L"eyes_closedhappy.png", L"eyes_closedhappy.png");
                        neko.eyes.relativeChibiScale = 0.083918; neko.eyes.SetOffsetChibi({-3, -557});
                        neko.eyes.relativePersonScale = 0.031657; neko.eyes.SetOffsetPerson({-15, -714}); break;
                    case NekoS::EyesEmotion::Confused: neko.eyes.Load(L"eyes_confused1.png", L"eyes_confused1.png");
                        neko.eyes.relativeChibiScale = 0.154622; neko.eyes.SetOffsetChibi({2, -611});
                        neko.eyes.relativePersonScale = 0.051925; neko.eyes.SetOffsetPerson({-11, -735}); break;
                    case NekoS::EyesEmotion::Embarrassed: neko.eyes.Load(L"eyes_closedembarrass.png", L"eyes_closedembarrass.png");
                        neko.eyes.relativeChibiScale = 0.086685; neko.eyes.SetOffsetChibi({-5, -553});
                        neko.eyes.relativePersonScale = 0.035793; neko.eyes.SetOffsetPerson({-15, -713}); break;
                    default: break;
                }
                neko.eyes.ResizeChibi(neko.body.chibi.getGlobalBounds().height);
                neko.eyes.UpdatePositionChibi(neko.body.chibi.getPosition().x, neko.body.chibi.getPosition().y);
                neko.eyes.ResizePerson(neko.body.person.getGlobalBounds().height);
                neko.eyes.UpdatePositionPerson(neko.body.person.getPosition().x, neko.body.person.getPosition().y);
            }
            if (mouthEmotion != NekoP::mouthEmotion)
            {
                if (NekoP::mouthEmotion == NekoS::MouthEmotion::DEFAULT)
                    NekoEmotionAccordingToMood(RestoringEmotion::Mouth);
                mouthEmotion = NekoP::mouthEmotion;
                switch (mouthEmotion)
                {
                    case NekoS::MouthEmotion::Smile: neko.mouth.Load(L"mouth_smile.png", L"mouth_smile.png");
                        neko.mouth.relativeChibiScale = 0.008141; neko.mouth.SetOffsetChibi({-19, -445});
                        neko.mouth.relativePersonScale = 0.004675; neko.mouth.SetOffsetPerson({-17, -680}); break;
                    case NekoS::MouthEmotion::Neutral: neko.mouth.Load(L"mouth_smile.png", L"mouth_smile.png");
                        neko.mouth.relativeChibiScale = 0.008141; neko.mouth.SetOffsetChibi({-19, -445});
                        neko.mouth.relativePersonScale = 0.004675; neko.mouth.SetOffsetPerson({-17, -680}); break;
                    case NekoS::MouthEmotion::Dot: neko.mouth.Load(L"mouth_dot.png", L"mouth_dot.png");
                        neko.mouth.relativeChibiScale = 0.008141; neko.mouth.SetOffsetChibi({-19, -445});
                        neko.mouth.relativePersonScale = 0.004675; neko.mouth.SetOffsetPerson({-17, -678}); break;
                    case NekoS::MouthEmotion::Open: neko.mouth.Load(L"mouth_open.png", L"mouth_open.png");
                        neko.mouth.relativeChibiScale = 0.055826; neko.mouth.SetOffsetChibi({-20, -473});
                        neko.mouth.relativePersonScale = 0.018589; neko.mouth.SetOffsetPerson({-18, -684}); break;
                    case NekoS::MouthEmotion::Sad: neko.mouth.Load(L"mouth.png", L"mouth.png");
                        neko.mouth.relativeChibiScale = 0.131615; neko.mouth.SetOffsetChibi({6, -601});
                        neko.mouth.relativePersonScale = 0.042294; neko.mouth.SetOffsetPerson({-11, -729}); break;
                    default: break;
                }
                neko.mouth.ResizeChibi(neko.body.chibi.getGlobalBounds().height);
                neko.mouth.UpdatePositionChibi(neko.body.chibi.getPosition().x, neko.body.chibi.getPosition().y);
                neko.mouth.ResizePerson(neko.body.person.getGlobalBounds().height);
                neko.mouth.UpdatePositionPerson(neko.body.person.getPosition().x, neko.body.person.getPosition().y);
            }
        }
        
        void Body::NekoEmotionAccordingToMood(const RestoringEmotion& restoring)
        {
            switch (NekoP::mood)
            {
                case NekoS::MoodEnum::Happy:
                    if (restoring == RestoringEmotion::Eyebrows) NekoP::eyebrowsEmotion = NekoS::EyebrowsEmotion::Normal;
                    else if (restoring == RestoringEmotion::Eyes) NekoP::eyesEmotion = NekoS::EyesEmotion::Normal;
                    else if (restoring == RestoringEmotion::Mouth) NekoP::mouthEmotion = NekoS::MouthEmotion::Smile;
                    break;
                default: break;
            }
        }
        
        void Body::NekoEmotionsAccordingToMood()
        {
            switch (NekoP::mood)
            {
                case NekoS::MoodEnum::Happy:
                    NekoP::eyebrowsEmotion = NekoS::EyebrowsEmotion::Normal;
                    NekoP::eyesEmotion = NekoS::EyesEmotion::Normal;
                    NekoP::mouthEmotion = NekoS::MouthEmotion::Smile;
                    break;
                default: break;
            }
        }
    }
}
