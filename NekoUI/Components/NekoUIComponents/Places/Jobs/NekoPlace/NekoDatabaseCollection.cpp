//
//  NekoDatabaseCollection.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 27/02/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "NekoDatabaseCollection.hpp"

namespace NekoNinja
{
    void NekoBase::NovelTalkTo(Entity* entity)
    {
        auto* novel = entity->AddComponent<NovelComponents::Novel>();
        novel->lines.push_front(L"blackend");
        novel->lines.push_front(L"show " + name + L" center message:no");
        novel->lines.push_front(name + L" \"Йей, приветь!!~ :3\" disappear:0.2");
        novel->lines.push_front(L"wait 0.2");
    }
    void NekoBase::NovelIntroduction(Entity* entity)
    {
        auto* novel = entity->AddComponent<NovelComponents::Novel>();
        novel->lines.push_front(L"blackend fade:0.2");
        novel->lines.push_front(L"show " + name + L" center message:no");
        novel->lines.push_front(name + L" \"Приятно познакомиться, я " + display + L", надеюсь, мы подружимся :3\"");
        novel->lines.push_front(L"disappear fade:0.2");
    }
    
    
    
    
    void NekoDatabaseCollection_SortNeko() { std::sort(nl::self->neko.begin(), nl::self->neko.end(), [](const NekoBase* a, const NekoBase* b) { return a->possibility > b->possibility; }); }
    void NekoDatabaseCollection_LoadNeko()
    {
        nl::self->neko.push_back(new ndc::Poop());
        ndc::Poop::ptr = nl::self->neko.back();
        
        nl::self->neko.push_back(new ndc::MezumiNakayano());
        nl::self->neko.push_back(new ndc::AmariMami());
        nl::self->neko.push_back(new ndc::KashikoHola());
        
        nl::self->neko.push_back(new ndc::Azuki());
        nl::self->neko.push_back(new ndc::Shigure());
        nl::self->neko.push_back(new ndc::Cinnamon());
        nl::self->neko.push_back(new ndc::Maple());
        nl::self->neko.push_back(new ndc::Coconut());
        nl::self->neko.push_back(new ndc::Vanilla());
        nl::self->neko.push_back(new ndc::Chocola());
        
        nl::self->neko.push_back(new ndc::Miho());
        nl::self->neko.push_back(new ndc::Kay());
        nl::self->neko.push_back(new ndc::Kanade());
        nl::self->neko.push_back(new ndc::Sims());
        nl::self->neko.push_back(new ndc::Katyusha());
        nl::self->neko.push_back(new ndc::Riria());
        nl::self->neko.push_back(new ndc::Asuna());
        nl::self->neko.push_back(new ndc::Cthulhu());
        nl::self->neko.push_back(new ndc::MiyuEdelfelt());
        nl::self->neko.push_back(new ndc::Anchovy());
        nl::self->neko.push_back(new ndc::Darjeeling());
        nl::self->neko.push_back(new ndc::Altera());
        nl::self->neko.push_back(new ndc::Flandre());
        nl::self->neko.push_back(new ndc::ElizabethBathory());
        
        NekoDatabaseCollection_SortNeko();
    }
    
    
    namespace ndc
    {
        NekoBase* Poop::ptr = nullptr;
        void Azuki::NovelTalkTo(Entity* entity)
        {
            auto* novel = entity->AddComponent<NovelComponents::Novel>();
            novel->lines.push_back(L"blackend");
            novel->lines.push_back(L"show " + name + L" center message:no");
            novel->lines.push_back(name + L" \"Хозяииин~ :3\"");
            novel->lines.push_back(name + L" \"Как тебе мой цап-царап? Надеюсь, я оправдываю твои ожидания! >3<\"");
            novel->lines.push_back(name + L" \"Прости, что после цап-царапа мне приходится чуть-чуть отдохнуть т.т\"");
            novel->lines.push_back(name + L" \"Если буду тренироваться, то смогу помогать тебе чаще, дя!!! :3\"");
            novel->lines.push_back(L"disappear fade:0.2");
        }
        void Cinnamon::NovelTalkTo(Entity* entity)
        {
            auto* novel = entity->AddComponent<NovelComponents::Novel>();
            novel->lines.push_back(L"blackend");
            novel->lines.push_back(L"show Cinnamon cleft message:no");
            novel->lines.push_back(name + L" \"Оппа, а я другая новелла!!!!!!~ :3\"");
            novel->lines.push_back(L"disappear fade:0.2");
        }
    }
}
