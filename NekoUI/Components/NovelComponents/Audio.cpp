//
//  Audio.cpp
//  NovelSome
//
//  Created by Никита Исаенко on 11/09/2018.
//  Copyright © 2018 Melancholy Hill. All rights reserved.
//

#include "Audio.hpp"

namespace ns
{
    namespace NovelComponents
    {
        void SoundPlayer::Update(const sf::Time& elapsedTime)
        {
            if (audioLoaded)
            {
                switch (mode)
                {
                    case Mode::Appear:
                        if (currentTime < appearTime) currentTime += elapsedTime.asSeconds();
                        if (currentTime >= appearTime) {
                            volume = rmaxVolume; currentTime = 0.f; mode = Mode::Exist;
                            if (messageBack == MessageBack::AtAppearance) novelSystem->SendMessage({"UnHold", this});
                        } else volume = (rmaxVolume * (currentTime / appearTime));
                        sound.setVolume(volume);
                        break;
                        
                    case Mode::Exist:
                        if (!loop && (sound.getPlayingOffset().asSeconds() >= timeToStartDisappearing || sound.getStatus() == sf::Sound::Status::Stopped))
                        {
                            mode = Mode::Disapper;
                            if (messageBack == MessageBack::AtDisappearance) novelSystem->SendMessage({"UnHold", this});
                        }
                        break;
                        
                    case Mode::Disapper:
                        if (currentTime < disappearTime) currentTime += elapsedTime.asSeconds();
                        if (currentTime >= disappearTime) {
                            volume = 0.f; currentTime = 0.f; mode = Mode::Deprecate;
                            if (messageBack == MessageBack::AtDeprecated) novelSystem->SendMessage({"UnHold", this});
                        } else volume = (rmaxVolume - (rmaxVolume * (currentTime / disappearTime)));
                        sound.setVolume(volume);
                        break;
                        
                    case Mode::Deprecate: novelSystem->PopComponent(this); break;
                    default: break;
                }
            }
        }
        void SoundPlayer::Destroy() { sc::DeleteSound(folderPath + audioPath); novelSystem->SendMessage({"Destroy", this}); }
        void SoundPlayer::LoadFromFile(const std::wstring& fileName)
        {
            audioLoaded = false;
            sf::SoundBuffer* buffer = sc::LoadSound(folderPath + fileName);
            if ((audioLoaded = buffer))
            {
                audioPath = fileName;
                sound.setBuffer(*buffer);
                sound.setLoop(loop);
                sound.setVolume(0);
                sound.setPlayingOffset(playingOffset);
                sound.play();
                
                rmaxVolume = maxVolume * gs::maxVolumeGlobal * gs::maxVolumeSound;
                timeToStartDisappearing = (*buffer).getDuration().asSeconds() - disappearTime;
                currentTime = 0.f;
            }
            else
            {
                if (messageBack != MessageBack::No) novelSystem->SendMessage({"UnHold", this});
                novelSystem->PopComponent(this);
            }
        }
        void SoundPlayer::SetStateMode(const Mode& newMode)
        {
            if (mode != newMode)
            {
                currentTime = 0.f; mode = newMode;
                if (newMode == Mode::Disapper && messageBack == MessageBack::AtDisappearance)
                    novelSystem->SendMessage({"UnHold", this});
            }
        }
        void SoundPlayer::ReceiveMessage(MessageHolder& message)
        {
            if (message.info == "GamePause") { if (gs::isPause) sound.pause(); else sound.play(); }
        }
        
        
        
        
        
        
        void MusicPlayer::Update(const sf::Time& elapsedTime)
        {
            if (!audioLoaded) return;
                switch (mode)
                {
                    case Mode::Appear:
                        if (currentTime < appearTime) currentTime += elapsedTime.asSeconds();
                        if (currentTime >= appearTime) {
                            volume = rmaxVolume; currentTime = 0.f; mode = Mode::Exist;
                            if (messageBack == MessageBack::AtAppearance) novelSystem->SendMessage({"UnHold", this});
                        }
                        else volume = (rmaxVolume * (currentTime / appearTime));
                        music.setVolume(volume);
                        break;
                    case Mode::Exist:
                        if (!loop && (music.getPlayingOffset().asSeconds() >= timeToStartDisappearing || music.getStatus() == sf::Sound::Status::Stopped))
                        {
                            mode = Mode::Disapper;
                            if (messageBack == MessageBack::AtDisappearance) novelSystem->SendMessage({"UnHold", this});
                        }
                        break;
                    case Mode::Disapper:
                        if (currentTime < disappearTime) currentTime += elapsedTime.asSeconds();
                        if (currentTime >= disappearTime) {
                            volume = 0.f; currentTime = 0.f; mode = Mode::Deprecate;
                            if (messageBack == MessageBack::AtDeprecated) novelSystem->SendMessage({"UnHold", this});
                        } else volume = (rmaxVolume - (rmaxVolume * (currentTime / disappearTime)));
                        music.setVolume(volume);
                        break;
                    case Mode::Deprecate: novelSystem->PopComponent(this); break;
                    default: break;
                }
        }
        void MusicPlayer::Destroy() { novelSystem->SendMessage({"Destroy", this}); }
        void MusicPlayer::LoadFromFile(const std::wstring& fileName)
        {
            audioLoaded = false;
            if ((audioLoaded = sc::LoadMusic(music, folderPath + fileName, fileInMemory)))
            {
                audioPath = fileName;
                music.setLoop(loop);
                music.setVolume(0);
                music.setPlayingOffset(playingOffset);
                music.play();
                
                rmaxVolume = maxVolume * gs::maxVolumeGlobal * (ambient ? gs::maxVolumeAmbient : gs::maxVolumeMusic);
                if (!loop) timeToStartDisappearing = music.getDuration().asSeconds() - disappearTime;
                else timeToStartDisappearing = 0;
                currentTime = 0.f;
            }
            else
            {
                if (messageBack != MessageBack::No)
                    novelSystem->SendMessage({"UnHold", this});
                novelSystem->PopComponent(this);
            }
        }
        void MusicPlayer::SetStateMode(const Mode& newMode)
        {
            if (mode != newMode)
            {
                currentTime = 0.f; mode = newMode;
                if (newMode == Mode::Disapper && messageBack == MessageBack::AtDisappearance) novelSystem->SendMessage({"UnHold", this});
            }
        }
    }
}
