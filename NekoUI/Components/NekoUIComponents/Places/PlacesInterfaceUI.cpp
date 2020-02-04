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
    void PlacesInterfaceUI::RecieveMessage(MessageHolder& message)
    {
        if (message.info == "PlacesUI :: ApartmentUI" || message.info == "PlacesUI :: GroceryUI" || message.info == "PlacesUI :: ShopkeeperUI" || message.info == "PlacesUI :: JobFNAF") entity->AddComponent<PlacesTransitionScreen>(message.info);
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
                ic::globalRequestSender->SendMessage(info + " Destroy");
                it = entity->components.begin();
                std::advance(it, entity->components.size() - 1);
                if (info == "PlacesUI :: GroceryUI") entity->InsertComponent<Places::GroceryUI>(it);
                else if (info == "PlacesUI :: ApartmentUI") { entity->system->SendMessage({"RoomUI :: Show"}); entity->InsertComponent<Apartment>(it); }
                else if (info == "PlacesUI :: ShopkeeperUI") entity->InsertComponent<Places::StrangeShopkeeperUI>(it);
                else if (info == "PlacesUI :: JobFNAF") entity->InsertComponent<Jobs_FNAFGame::FNAF>(it);
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
    void PlacesTransitionScreen::Resize(unsigned int width, unsigned int height)
    {
        shape.setSize({(float)width, (float)height});
        text.setCharacterSize(94*gs::scale);
        text.setPosition(width - text.getGlobalBounds().width - 30*gs::scale, height - text.getGlobalBounds().height - 30*gs::scale);
    }
}
