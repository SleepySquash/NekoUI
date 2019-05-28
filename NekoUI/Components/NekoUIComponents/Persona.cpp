//
//  Persona.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 20/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "Persona.hpp"

namespace NekoUI
{
    void Cloth::Load(const std::wstring& pPath, const std::wstring& cPath)
    {
        if (personLoaded) ic::DeleteImage(L"Data/Neko/Person/" + personPath);
        if (chibiLoaded) ic::DeleteImage(L"Data/Neko/Chibi/" + chibiPath);
        
        sf::Texture* texture = ic::LoadTexture(L"Data/Neko/Person/" + pPath);
        if ((personLoaded = texture))
        {
            this->personPath = pPath;
            person.setTexture(*texture, true);
            person.setOrigin(texture->getSize().x/2, 0);
        }
        
        texture = ic::LoadTexture(L"Data/Neko/Chibi/" + cPath);
        if ((chibiLoaded = texture))
        {
            this->chibiPath = cPath;
            chibi.setTexture(*texture, true);
            chibi.setOrigin(texture->getSize().x/2, 0);
        }
    }
    
    void Cloth::setChibiAlpha(const sf::Uint8& alpha) { chibi.setColor({chibi.getColor().r, chibi.getColor().g, chibi.getColor().b, alpha}); }
    void Cloth::ResizeChibi(const int& relativeBodyHeight)
    {
        float scale = (relativeBodyHeight * relativeChibiScale)/chibi.getLocalBounds().height;
        chibi.setScale(scale, scale);
    }
    void Cloth::SetOffsetChibi(pair<int, int> offset) { offsets.first = offset; }
    void Cloth::UpdatePositionChibi(const float& x, const float& y)
    {
        float scale = (chibi.getGlobalBounds().height/static_cast<float>(gs::relativeHeight)) / relativeChibiScale;
        chibi.setPosition(x + offsets.first.first*scale, y + offsets.first.second*scale);
    }
    
    void Cloth::setPersonAlpha(const sf::Uint8& alpha) { person.setColor({person.getColor().r, person.getColor().g, person.getColor().b, alpha}); }
    void Cloth::ResizePerson(const int& relativeBodyHeight)
    {
        float scale = (relativeBodyHeight * relativePersonScale)/person.getLocalBounds().height;
        person.setScale(scale, scale);
    }
    void Cloth::SetOffsetPerson(pair<int, int> offset) { offsets.second = offset; }
    void Cloth::UpdatePositionPerson(const float& x, const float& y)
    {
        float scale = (person.getGlobalBounds().height/static_cast<float>(gs::relativeHeight)) / relativePersonScale;
        person.setPosition(x + offsets.second.first*scale, y + offsets.second.second*scale);
    }
    
    void Cloth::Draw(sf::RenderWindow* window, bool mode) { if (mode) window->draw(person); else window->draw(chibi); }
    
    
    
    Persona::~Persona() { for (auto& c : cloth) if (c->owner) delete c; cloth.clear(); }
    void Persona::Init()
    {
        body.Load(L"body.png", L"body.png"); body.depth = 0; body.imOnMyOwn = true; body.owner = false;
        body.chibi.setOrigin(body.chibi.getOrigin().x, body.chibi.getLocalBounds().height);
        body.person.setOrigin(body.person.getOrigin().x, body.person.getLocalBounds().height);
        
        face.Load(L"face.png", L"face.png"); face.depth = 40; face.owner = false;
        face.relativeChibiScale = 0.289; face.SetOffsetChibi({4, -673});
        face.relativePersonScale = 0.088; face.SetOffsetPerson({-11, -747});
        
        hair.Load(L"hair1.png", L"hair1.png"); hair.depth = 80; hair.owner = false;
        hair.relativeChibiScale = 1.033; hair.SetOffsetChibi({-54, -844});
        hair.relativePersonScale = 0.7226; hair.SetOffsetPerson({-28, -804});
        
        cloth.push_back(&body);
        cloth.push_back(&face);
        cloth.push_back(&hair);
        
        
        /*Cloth* nipples = new Cloth(); nipples->depth = 1;
        nipples->Load(L"nipples.png", L"nipples.png");
        nipples->relativeChibiScale = 0.181; nipples->SetOffsetChibi({17, -780});
        nipples->relativePersonScale = 0.0209; nipples->SetOffsetPerson({-9, -595});
        cloth.push_back(nipples);*/
        
        Cloth* maidoutfit = new Cloth(); maidoutfit->depth = 60;
        maidoutfit->Load(L"maid uniform.png", L"maid uniform.png");
        maidoutfit->relativeChibiScale = 0.330814; maidoutfit->SetOffsetChibi({-26, -428});
        maidoutfit->relativePersonScale = 0.417774; maidoutfit->SetOffsetPerson({27, -659});
        cloth.push_back(maidoutfit);
        
        Cloth* stockings = new Cloth(); stockings->depth = 10;
        stockings->Load(L"white stockings.png", L"white stockings.png");
        stockings->relativeChibiScale = 0.188139; stockings->SetOffsetChibi({-18, -150});
        stockings->relativePersonScale = 0.428326; stockings->SetOffsetPerson({-24, -343});
        cloth.push_back(stockings);
        
        Cloth* headband = new Cloth(); headband->depth = 90;
        headband->Load(L"headband.png", L"headband.png");
        headband->relativeChibiScale = 0.24; headband->SetOffsetChibi({17, -820});
        headband->relativePersonScale = 0.074; headband->SetOffsetPerson({-6, -795});
        cloth.push_back(headband);
        
        Cloth* nekoears = new Cloth(); nekoears->depth = &hair ? (hair.depth + 5) : -1;
        nekoears->Load(L"neko ears.png", L"neko ears.png");
        nekoears->relativeChibiScale = 0.365988; nekoears->SetOffsetChibi({20, -921});
        nekoears->relativePersonScale = 0.1434; nekoears->SetOffsetPerson({-2, -844});
        cloth.push_back(nekoears);
        
        std::sort(cloth.begin(), cloth.end(), [](const Cloth* a, const Cloth* b) { return a->depth < b->depth; });
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
