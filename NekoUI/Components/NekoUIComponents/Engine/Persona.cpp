//
//  Persona.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 20/05/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "Persona.hpp"

namespace NekoUI
{
    void Cloth::Set(const std::wstring& pPath) { personPath = chibiPath = pPath; }
    void Cloth::Set(const std::wstring& pPath, const std::wstring& cPath) { personPath = pPath; chibiPath = cPath; }
    void Cloth::Load(const std::wstring& pPath, const std::wstring& cPath)
    {
        if (!personLoaded || pPath != personPath)
        {
            if (personLoaded) ic::DeleteImage(L"Data/Neko/Person/" + personPath);
            if ((personLoaded = (pPath != L"")))
            {
                sf::Texture* texture = ic::LoadTexture(L"Data/Neko/Person/" + pPath);
                if ((personLoaded = texture))
                {
                    personPath = pPath;
                    person.setTexture(*texture, true);
                    person.setOrigin(texture->getSize().x/2, 0);
                }
            }
        }
        
        if (!chibiLoaded || cPath != chibiPath)
        {
            if (chibiLoaded) ic::DeleteImage(L"Data/Neko/Chibi/" + chibiPath);
            if ((chibiLoaded = (cPath != L"")))
            {
                sf::Texture* texture = ic::LoadTexture(L"Data/Neko/Chibi/" + cPath);
                if ((chibiLoaded = texture))
                {
                    chibiPath = cPath;
                    chibi.setTexture(*texture, true);
                    chibi.setOrigin(texture->getSize().x/2, 0);
                }
            }
        }
    }
    void Cloth::Destroy()
    {
        if (personLoaded) ic::DeleteImage(L"Data/Neko/Person/" + personPath);
        if (chibiLoaded) ic::DeleteImage(L"Data/Neko/Chibi/" + chibiPath);
        personLoaded = chibiLoaded = false; offline = true;
    }
    void Cloth::setChibiAlpha(const sf::Uint8& alpha) {
        if (chibiLoaded) chibi.setColor({chibi.getColor().r, chibi.getColor().g, chibi.getColor().b, alpha}); }
    void Cloth::ResizeChibi(const int& relativeBodyHeight)
    {
        if (!chibiLoaded) return;
        float scale = (relativeBodyHeight * relativeChibiScale)/chibi.getLocalBounds().height;
        if (chibiReversed) chibi.setScale(-scale, scale); else chibi.setScale(scale, scale);
    }
    void Cloth::SetOffsetChibi(const pair<float, float>& offset) { offsets.first = offset; }
    void Cloth::UpdatePositionChibi(const float& x, const float& y)
    {
        if (!chibiLoaded) return;
        float scale = (chibi.getGlobalBounds().height/static_cast<float>(gs::relativeHeight)) / relativeChibiScale;
        if (chibiReversed) chibi.setPosition(x - offsets.first.first*scale, y + offsets.first.second*scale);
        else chibi.setPosition(x + offsets.first.first*scale, y + offsets.first.second*scale);
    }
    
    void Cloth::setPersonAlpha(const sf::Uint8& alpha) {
        if (personLoaded) person.setColor({person.getColor().r, person.getColor().g, person.getColor().b, alpha}); }
    void Cloth::ResizePerson(const int& relativeBodyHeight)
    {
        if (!personLoaded) return;
        float scale = (relativeBodyHeight * relativePersonScale)/person.getLocalBounds().height;
        if (personReversed) person.setScale(-scale, scale); else person.setScale(scale, scale);
    }
    void Cloth::SetOffsetPerson(const pair<float, float>& offset) { offsets.second = offset; }
    void Cloth::UpdatePositionPerson(const float& x, const float& y)
    {
        if (!personLoaded) return;
        float scale = (person.getGlobalBounds().height/static_cast<float>(gs::relativeHeight)) / relativePersonScale;
        if (personReversed) person.setPosition(x - offsets.second.first*scale, y + offsets.second.second*scale);
        else person.setPosition(x + offsets.second.first*scale, y + offsets.second.second*scale);
    }
    
    void Cloth::Draw(sf::RenderWindow* window, bool mode) { if (hidden) return; if (mode) window->draw(person); else window->draw(chibi); }
    
    
    
    Persona::~Persona() { for (auto& c : cloth) if (c->owner) delete c; cloth.clear(); }
    void Persona::Init()
    {
        body.Set(L"body.png", L"body.png"); body.depth = 0; body.imOnMyOwn = true; body.owner = false;
        // body.chibi.setOrigin(body.chibi.getOrigin().x, body.chibi.getLocalBounds().height);
        // body.person.setOrigin(body.person.getOrigin().x, body.person.getLocalBounds().height);
        
        arms.Set(L"", L"arms.png"); arms.depth = 70; arms.owner = false;
        arms.relativeChibiScale = 0.2454955; arms.SetOffsetChibi({-24.77477, -416.666});
        
        eyebrows.Set(L"eyebrows_normal.png", L"eyebrows_normal.png"); eyebrows.depth = 121/*40*/; eyebrows.owner = false;
        eyebrows.relativeChibiScale = 0.069701; eyebrows.SetOffsetChibi({14, -654});
        eyebrows.relativePersonScale = 0.023205; eyebrows.SetOffsetPerson({-9, -741});
        
        eyes.Set(L"eyes_normal.png", L"eyes_normal.png"); eyes.depth = 40; eyes.owner = false;
        eyes.relativeChibiScale = 0.131615; eyes.SetOffsetChibi({6, -601});
        eyes.relativePersonScale = 0.042294; eyes.SetOffsetPerson({-11, -729});
        
        nose.Set(L"nose.png", L"nose.png"); nose.depth = 40; nose.owner = false;
        nose.relativePersonScale = 0.002564; nose.SetOffsetPerson({-12, -692});
        
        blush.Set(L"blush.png", L"blush.png"); blush.depth = 30; blush.owner = false;
        blush.relativeChibiScale = 0.104883; blush.SetOffsetChibi({-4, -538});
        blush.relativePersonScale = 0.038189; blush.SetOffsetPerson({-16, -710});
        
        mouth.Set(L"mouth_smile.png", L"mouth_smile.png"); mouth.depth = 40; mouth.owner = false;
        mouth.relativeChibiScale = 0.008141; mouth.SetOffsetChibi({-19, -445});
        mouth.relativePersonScale = 0.004675; mouth.SetOffsetPerson({-17, -680});
        
        fronthair.Set(L"hairfront1.png", L"hairfront1.png"); fronthair.depth = 120; fronthair.owner = false;
        fronthair.relativeChibiScale = 0.583869; fronthair.SetOffsetChibi({31, -827});
        fronthair.relativePersonScale = 0.214175; fronthair.SetOffsetPerson({-9, -798});
        
        backhair.Set(L"hairback1.png", L"hairback1.png"); backhair.depth = -80; backhair.owner = false;
        backhair.relativeChibiScale = 1.012318; backhair.SetOffsetChibi({-44, -840});
        backhair.relativePersonScale = 0.720037; backhair.SetOffsetPerson({-28, -803});
        
        tail.Set(L"tail.png", L"tail.png"); tail.depth = -30; tail.owner = false;
        tail.relativeChibiScale = 0.137941; tail.SetOffsetChibi({-152, -325});
        tail.relativePersonScale = 0.286507; tail.SetOffsetPerson({39, -436});
        
        cloth.push_back(&body);
        cloth.push_back(&arms);
        cloth.push_back(&eyebrows);
        cloth.push_back(&eyes);
        cloth.push_back(&nose);
        cloth.push_back(&blush);
        cloth.push_back(&mouth);
        cloth.push_back(&fronthair);
        cloth.push_back(&backhair);
        cloth.push_back(&tail);
        
        /*Cloth* nipples = new Cloth(); nipples->depth = 1;
        nipples->Set(L"nipples.png", L"nipples.png");
        nipples->relativeChibiScale = 0.181; nipples->SetOffsetChibi({17, -780});
        nipples->relativePersonScale = 0.0209; nipples->SetOffsetPerson({-9, -595});
        cloth.push_back(nipples);*/
        
        Cloth* nekoears = new Cloth(); nekoears->depth = &backhair ? (backhair.depth + 5) : -1;
        nekoears->Set(L"neko ears.png", L"neko ears.png");
        nekoears->relativeChibiScale = 0.365988; nekoears->SetOffsetChibi({20, -921});
        nekoears->relativePersonScale = 0.1434; nekoears->SetOffsetPerson({-2, -844});
        cloth.push_back(nekoears);
        
        SortClothes();
        // RecalculateBounds();
    }

    void Persona::OccupyMemory()
    {
        for (auto c : cloth) c->Load(c->personPath, c->chibiPath);
        body.chibi.setOrigin(body.chibi.getOrigin().x, body.chibi.getLocalBounds().height);
        body.person.setOrigin(body.person.getOrigin().x, body.person.getLocalBounds().height);
        RecalculateBounds();
    }
    void Persona::FreeMemory() { for (auto c : cloth) { c->Destroy(); c->offline = false; } }
    
    void Persona::Dress(Wearable* item, bool sorting)
    {
        if (item->dressed) return; savingIsRequired = true;
        Cloth* clothing = new Cloth(); clothing->item = item;
        clothing->depth = item->depth; item->dressed = true;
        clothing->Load(item->personPath, item->chibiPath);
        clothing->relativeChibiScale = item->relativeChibiScale;   clothing->SetOffsetChibi(item->offsets.first);
        clothing->relativePersonScale = item->relativePersonScale; clothing->SetOffsetPerson(item->offsets.second);
        clothing->chibiReversed = chibiReversed; clothing->personReversed = personReversed;
        clothing->chibi.setColor(item->chibiColor); clothing->person.setColor(item->personColor);
        
        clothing->ResizeChibi(body.chibi.getGlobalBounds().height);
        clothing->UpdatePositionChibi(body.chibi.getPosition().x, body.chibi.getPosition().y);
        clothing->ResizePerson(body.person.getGlobalBounds().height);
        clothing->UpdatePositionPerson(body.person.getPosition().x, body.person.getPosition().y);
        
        cloth.push_back(clothing); if (sorting) SortClothes();
        RecalculateBounds(clothing);
    }
    void Persona::Undress(Wearable* item)
    {
        if (!item->dressed) return; item->dressed = false; savingIsRequired = true;
        vector<Cloth*>::iterator it = cloth.begin();
        while (it != cloth.end()) { if ((*it)->item == item) { (*it)->Destroy(); delete (*it); it = cloth.erase(it); } else ++it; }
    }
    void Persona::Undress(const ClothType& type)
    {
        vector<Cloth*>::iterator it = cloth.begin(); savingIsRequired = true;
        while (it != cloth.end()) if ((*it)->item && (*it)->item->clothing == type) {
            (*it)->item->dressed = false; (*it)->Destroy(); delete (*it); it = cloth.erase(it); } else ++it;
    }
    void Persona::Undress()
    {
        vector<Cloth*>::iterator it = cloth.begin(); savingIsRequired = true;
        while (it != cloth.end()) if ((*it)->item) { (*it)->item->dressed = false; (*it)->Destroy(); delete (*it); it = cloth.erase(it); } else ++it;
    }
    
    void Persona::SortClothes() { std::sort(cloth.begin(), cloth.end(), [](const Cloth* a, const Cloth* b) { return a->depth < b->depth; }); }
    void Persona::RecalculateBounds(Cloth* ffor)
    {
        if (ffor)
        {
            Cloth* c = ffor;
            if (c->chibi.getLocalBounds().width * c->relativeChibiScale > chibiWidth)
                chibiWidth = c->chibi.getLocalBounds().width * c->relativeChibiScale;
            if (c->chibi.getLocalBounds().height * c->relativeChibiScale > chibiHeight)
                chibiHeight = c->chibi.getLocalBounds().height * c->relativeChibiScale;
            if (c->person.getLocalBounds().width * c->relativePersonScale > personWidth)
                personWidth = c->person.getLocalBounds().width * c->relativePersonScale;
            if (c->person.getLocalBounds().height * c->relativePersonScale > personHeight)
                personHeight = c->person.getLocalBounds().height * c->relativePersonScale;
        }
        else
        {
            chibiWidth = personWidth = chibiHeight = personHeight = 0;
            for (auto& c : cloth)
            {
                if (c->chibi.getLocalBounds().width * c->relativeChibiScale > chibiWidth)
                    chibiWidth = c->chibi.getLocalBounds().width * c->relativeChibiScale;
                if (c->chibi.getLocalBounds().height * c->relativeChibiScale > chibiHeight)
                    chibiHeight = c->chibi.getLocalBounds().height * c->relativeChibiScale;
                if (c->person.getLocalBounds().width * c->relativePersonScale > personWidth)
                    personWidth = c->person.getLocalBounds().width * c->relativePersonScale;
                if (c->person.getLocalBounds().height * c->relativePersonScale > personHeight)
                    personHeight = c->person.getLocalBounds().height * c->relativePersonScale;
            }
        }
    }
    
    void Persona::setChibiAlpha(const sf::Uint8& alpha) { for (auto& c : cloth) c->setChibiAlpha(alpha); }
    void Persona::setChibiPosition(float x, float y)
    {
        body.chibi.setPosition(x, y);
        for (auto& c : cloth) if (!c->imOnMyOwn) c->UpdatePositionChibi(x, y);
    }
    void Persona::ResizeChibi()
    {
        body.ResizeChibi(gs::relativeHeight * chibiScale);
        for (auto& c : cloth) if (!c->imOnMyOwn) c->ResizeChibi(body.chibi.getGlobalBounds().height);
    }
    void Persona::setPersonAlpha(const sf::Uint8& alpha) { for (auto& c : cloth) c->setPersonAlpha(alpha); }
    void Persona::setPersonPosition(float x, float y)
    {
        body.person.setPosition(x, y);
        for (auto& c : cloth) if (!c->imOnMyOwn) c->UpdatePositionPerson(x, y);
    }
    void Persona::ResizePerson()
    {
        body.ResizePerson(gs::relativeHeight * personScale);
        for (auto& c : cloth) if (!c->imOnMyOwn) c->ResizePerson(body.person.getGlobalBounds().height);
    }
    void Persona::Draw(sf::RenderWindow* window, bool mode) { for (auto& c : cloth) c->Draw(window, mode); }
}
