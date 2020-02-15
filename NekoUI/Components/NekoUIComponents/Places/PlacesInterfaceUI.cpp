//
//  PlacesInterfaceUI.cpp
//  NekoUI
//
//  Created by Никита Исаенко on 19.01.2020.
//  Copyright © 2020 Melancholy Hill. All rights reserved.
//

#include "PlacesInterfaceUI.hpp"

namespace NekoUI
{
    void PlacesInterfaceUI::ReceiveMessage(MessageHolder& message)
    {
        if (message.info == "PlacesUI :: ApartmentUI" || message.info == "PlacesUI :: GroceryUI" || message.info == "PlacesUI :: ShopkeeperUI" || message.info == "PlacesUI :: JobFNAF" || message.info == "PlacesUI :: TestNovel" || message.info == "PlacesUI :: JobNekoPlace") entity->AddComponent<PlacesTransitionScreen>(message.info);
        else if (message.info == "PlacesUI :: Test2Novel")
        {
            rm::drawScrolldownMenu = rm::drawNeeds = false;
            ns::NovelComponents::CharacterLibrary::nekoPersona = &NekoB::neko;
            ns::NovelComponents::CharacterLibrary::nekoName = Player::display;
            
            auto novel = entity->AddComponent<ns::NovelComponents::Novel>();
            //novel->lines.push_front(L"background \"Data/Images/Backgrounds/park.jpg\"");
            novel->lines.push_front(L"show neko fade:0");
            novel->lines.push_front(L"Неко \"Приветь, хозяин!!! ПОалфдо оплдйуо пуп тмьстмьст ацйлкойцл мовлмолв кцлолкц пыфпды подуйлпоуйд поулпко слмослмо поулоауло поулп оул олпулпупул влыф\"");
            novel->lines.push_front(L"neko \"Дя, йа новелла >3<\"");
            novel->lines.push_front(L"wait 0.5");
        }
        else if (message.info == "Novel :: Destroying")
        {
            rm::drawScrolldownMenu = rm::drawNeeds = true;
            if (rm::location == rm::Location::Park) entity->AddComponent<PlacesTransitionScreen>("PlacesUI :: ApartmentUI");
        }
    }
    
    PlacesTransitionScreen::PlacesTransitionScreen(const std::string& info) : info(info) { }
    void PlacesTransitionScreen::Init()
    {
        gs::PushInterface(this);
        rm::scrolldownMenuOpened = rm::requestCloseButton = rm::allowDTSaving = false;
        rm::drawDatePanel = rm::drawNeeds = rm::drawScrolldownMenu = false;
        rm::canPressScrolldownMenu = rm::canPressDatePanel = rm::canOpenNekoUI = false;
        
        shape.setFillColor(sf::Color::Black);
        text.setFont(*fc::GetFont(L"Impact.ttf"));
        text.setString(L"Идём...");
        gs::ignoreDraw = false;
        
        if (info == "Novel :: Destroying") info = "PlacesUI :: ApartmentUI";
        if (nss::Command(info, "PlacesUI :: Job"))
        {
            entity->system->SendMessage({"RoomUI :: Close"});
            rm::location = rm::Location::Job;
        }
        else if (rm::location == rm::Location::Job) { Player::SaveData(); }
    }
    void PlacesTransitionScreen::Destroy() { gs::RemoveInterface(this); }
    void PlacesTransitionScreen::Update(const sf::Time& elapsedTime)
    {
        gs::requestWindowRefresh = true;
        switch (stage)
        {
            case 0:
                if (elapsedSeconds < elapsedFor)
                {
                    alpha = 255.f * elapsedSeconds/elapsedFor;
                    shape.setFillColor(sf::Color(0,0,0, alpha));
                    text.setFillColor({255, 255, 255, alpha});
                    elapsedSeconds += elapsedTime.asSeconds();
                }
                else { elapsedSeconds = 0.f; alpha = 255;
                    shape.setFillColor(sf::Color(0,0,0, alpha));
                    text.setFillColor({255, 255, 255, alpha});
                    stage = 1; }
                break;
            case 1: stage = 2; break;
            case 2:
                //TODO: waitFor{ 0.1f }
                ic::globalRequestSender->SendMessage({"Apartment :: Destroy"});
                ic::globalRequestSender->SendMessage({"PlacesUI :: ApartmentUI Destroy"});
                it = entity->components.begin();
                std::advance(it, entity->components.size() - 1);
                if (info == "PlacesUI :: GroceryUI") entity->InsertComponent<Places::GroceryUI>(it);
                else if (info == "PlacesUI :: ApartmentUI") { entity->system->SendMessage({"RoomUI :: Show"}); entity->InsertComponent<Apartment>(it); }
                else if (info == "PlacesUI :: ShopkeeperUI") entity->InsertComponent<Places::StrangeShopkeeperUI>(it);
                else if (info == "PlacesUI :: JobFNAF") entity->InsertComponent<Jobs_FNAFGame::FNAF>(it);
                else if (info == "PlacesUI :: JobNekoPlace") entity->InsertComponent<NekoNinja::Controller>(it);
                else if (info == "PlacesUI :: TestNovel")
                {
                    rm::location = rm::Location::Park;
                    
                    ns::NovelComponents::CharacterLibrary::nekoPersona = &NekoB::neko;
                    ns::NovelComponents::CharacterLibrary::nekoName = Player::display;
                    
                    auto novel = entity->AddComponent<ns::NovelComponents::Novel>();
                    novel->lines.push_front(L"background \"Data/Images/Backgrounds/park.jpg\"");
                    novel->lines.push_front(L"show neko");
                    novel->lines.push_front(L"Неко \"Приветь, хозяин!!! ПОалфдо оплдйуо пуп тмьстмьст ацйлкойцл мовлмолв кцлолкц пыфпды подуйлпоуйд поулпко слмослмо поулоауло поулп оул олпулпупул влыф\"");
                    novel->lines.push_front(L"neko \"Дя, йа новелла >3<\"");
                    novel->lines.push_front(L"fadeout");
                }
                stage = 3;
                break;
            case 3: stage = 4; break;
            case 4:
                if (elapsedSeconds < elapsedFor)
                {
                    alpha = 255.f * (elapsedFor - elapsedSeconds)/elapsedFor;
                    shape.setFillColor(sf::Color(0,0,0, alpha));
                    text.setFillColor({255, 255, 255, alpha});
                    elapsedSeconds += elapsedTime.asSeconds();
                }
                else entity->PopComponent(this);
                break;
        }
    }
    void PlacesTransitionScreen::Draw(sf::RenderWindow* window) { window->draw(shape); window->draw(text); }
    void PlacesTransitionScreen::Resize(const unsigned int& width, const unsigned int& height)
    {
        shape.setSize({(float)width, (float)height});
        text.setCharacterSize(94*gs::scale);
        text.setPosition(width - text.getGlobalBounds().width - 30*gs::scale, height - text.getGlobalBounds().height - 30*gs::scale);
    }
}
