#pragma once

#include <GameEntity.h>
#include <Timer.h>
#include <managers/AudioManager.h>
#include <galaga/BackgroundStars.h>
#include <galaga/PlaySideBar.h>
#include <galaga/Level.h>

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

        Texture* mStartLabel; 
        float mLevelStartTimer; 
        float mLevelStartDelay; 
        bool mGameStarted; 
        Level* mLevel; 
        bool mLevelStarted; 
        int mCurrentStage;
        
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

        mStartLabel = new Texture("START", "emulogic.ttf", 32, { 150, 0, 0 }); 
        mStartLabel->Parent(this); 
        mStartLabel->Position(Graphics::SCREEN_WIDTH * 0.4f, 
            Graphics::SCREEN_HEIGHT * 0.5f); 
        
        mLevel = nullptr; 
        mLevelStartDelay = 1.0f; 
        mLevelStarted = false;

        mSideBar->SetShips(2); 
        mStars->Scroll(false); 
        mGameStarted = false; 
        mLevelStarted = false; 
        mLevelStartTimer = 0.0f; 
        mCurrentStage = 0; 
        mAudio->PlayMusic("MUS/GameStart.wav", 0);
    } 
    
    PlayScreen::~PlayScreen() 
    { 
        mTimer = nullptr; 
        mAudio = nullptr; 
        mStars = nullptr; 

        delete mSideBar; 
        mSideBar = nullptr;

        delete mStartLabel; 
        mStartLabel = nullptr; 
        
        delete mLevel; 
        mLevel = nullptr;
    }

    void PlayScreen::Update() 
    {
        if (mGameStarted) 
        { 
            if (!mLevelStarted) 
            { 
                mLevelStartTimer += mTimer->DeltaTime(); 
                if (mLevelStartTimer >= mLevelStartDelay) 
                { 
                    StartNextLevel(); 
                } 
            } 
            else 
            { 
                mLevel->Update(); 
            } 

            if (mCurrentStage > 0) 
            { 
                mSideBar->Update(); 
            } 
        }
        else 
        { 
            if (!Mix_PlayingMusic()) 
            { 
                mGameStarted = true; 
            } 
        }
    } 
    
    void PlayScreen::Render() 
    {
        mSideBar->Render(); 
        
        if (!mGameStarted) 
        { 
            mStartLabel->Render(); 
        } 

        if (mGameStarted && mLevelStarted) 
        { 
            mLevel->Render(); 
        }
    }

    void PlayScreen::StartNewGame()
    {
        mSideBar->SetHighScore(30000); 
        mSideBar->SetShips(2);
    }
    
    void PlayScreen::StartNextLevel() 
    { 
        mCurrentStage += 1; 
        mLevelStartTimer = 0.0f; 
        mLevelStarted = true; 
        
        delete mLevel; 
        mLevel = new Level(mCurrentStage, mSideBar);
    }
}