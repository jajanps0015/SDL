#pragma once

#include <galaga/StartScreen.h>
#include <galaga/PlayScreen.h>
#include <galaga/BackgroundStars.h>

namespace Galaga
{
    class ScreenManager
    {
    private:
        static ScreenManager* sInstance;
        ScreenManager(); ~ScreenManager();

        enum Screens { Start, Play };
        Screens mCurrentScreen;

        InputManager* mInput;
        BackgroundStars* mBackgroundStars;

        StartScreen* mStartScreen;
        PlayScreen* mPlayScreen;

    public:
        static ScreenManager* Instance();
        static void Release();

        void Update();
        void Render();
    };

    ScreenManager* ScreenManager::sInstance = nullptr;

    ScreenManager* ScreenManager::Instance()
    {
        if (sInstance == nullptr)
        {
            sInstance = new ScreenManager();
        }
        return sInstance;
    }

    void ScreenManager::Release()
    {
        delete sInstance;
        sInstance = nullptr;
    }

    ScreenManager::ScreenManager()
    {
        mInput = InputManager::Instance();
        mBackgroundStars = BackgroundStars::Instance();

        mStartScreen = new StartScreen();
        mCurrentScreen = Start;

        mPlayScreen = new PlayScreen();
    }

    ScreenManager::~ScreenManager()
    {
        mInput = nullptr;

        BackgroundStars::Release();
        mBackgroundStars = nullptr;

        delete mStartScreen;
        mStartScreen = nullptr;

        delete mPlayScreen;
        mPlayScreen = nullptr;
    }

    void ScreenManager::Update()
    {
        mBackgroundStars->Update();
        switch (mCurrentScreen)
        {
        case Start:
            mStartScreen->Update();

            if (mInput->KeyPressed(SDL_SCANCODE_RETURN))
            {
                mCurrentScreen = Play;
                mStartScreen->ResetAnimation();
                mPlayScreen->StartNewGame();
            }
            break;

        case Play:
            mPlayScreen->Update();

            if (mPlayScreen->GameOver()) 
            { 
                mCurrentScreen = Start; 
            }
            break;
        }
    }

    void ScreenManager::Render()
    {
        mBackgroundStars->Render();
        switch (mCurrentScreen)
        {
        case Start:
            mStartScreen->Render();
            break;

        case Play:
            mPlayScreen->Render();
            break;
        }
    }
}