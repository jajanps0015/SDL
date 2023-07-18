#pragma once

#include <GameEntity.h>
#include <Timer.h>
#include <managers/AudioManager.h>
#include <galaga/BackgroundStars.h>
#include <galaga/PlaySideBar.h>

using namespace SDLFramework;

namespace Galaga
{
    class PlayScreen : public GameEntity
    {
    private:
        Timer* mTimer;
        AudioManager* mAudio;
        BackgroundStars* mStars; 
        PlaySideBar* mSideBar; 
        
        void StartNextLevel();

    public:
        PlayScreen();
        ~PlayScreen();

        void StartNewGame();

        void Update() override;
        void Render() override;
    };

    PlayScreen::PlayScreen() 
    { 
        mTimer = Timer::Instance(); 
        
        mAudio = AudioManager::Instance(); 
        mStars = BackgroundStars::Instance(); 

        mSideBar = new PlaySideBar(); 
        mSideBar->Parent(this); 
        mSideBar->Position(Graphics::SCREEN_WIDTH * 0.87f, Graphics::SCREEN_HEIGHT * 0.05f);

        StartNextLevel();
    } 
    
    PlayScreen::~PlayScreen() 
    { 
        mTimer = nullptr; 
        mAudio = nullptr; 
        mStars = nullptr; 

        delete mSideBar; 
        mSideBar = nullptr;
    }

    void PlayScreen::Update() 
    {
        mSideBar->Update();
    } 
    
    void PlayScreen::Render() 
    {
        mSideBar->Render();
    }

    void PlayScreen::StartNewGame()
    {
        mSideBar->SetHighScore(30000); 
        mSideBar->SetShips(2);
    }
    
    void PlayScreen::StartNextLevel() 
    { 
        mSideBar->SetLevel(45); 
    }
}