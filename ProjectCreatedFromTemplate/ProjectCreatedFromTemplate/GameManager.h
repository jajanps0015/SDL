#pragma once

#include <Graphics.h> 
#include <Timer.h>
#include <GameEntity.h>
#include <Texture.h>
#include <AssetManager.h>
#include <InputManager.h>
#include <AnimatedTexture.h>
#include <AudioManager.h>
#include <galaga/StartScreen.h>

using namespace Galaga;

namespace SDLFramework
{
    Graphics* Graphics::sInstance;
    bool Graphics::sInitialized;

    class GameManager
    {
    private:
        // singleton 
        static GameManager* sInstance;
        // loop control 
        bool mQuit;
        
        // modules 
        Graphics* mGraphics;
        AssetManager* mAssetManager;
        InputManager* mInputManager;
        AudioManager* mAudioManager;

        SDL_Event mEvent;
        const int FRAME_RATE = 60;
        Timer* mTimer;

        //Test
        StartScreen* mStartScreen;
        //End test
    public:
        static GameManager* Instance();
        static void Release();

        void Run();
        void Update();
        void LateUpdate();
        void Render();

        void TestStuff();

        GameManager();
        ~GameManager();
    };

    GameManager* GameManager::sInstance = nullptr;

    GameManager* GameManager::Instance()
    {
        if (sInstance == nullptr)
        {
            sInstance = new GameManager();
        }
        return sInstance;
    }

    void GameManager::Release()
    {
        delete sInstance;
        sInstance = nullptr;
    }

    void GameManager::Run()
    { // main game loop 
        while (!mQuit)
        {
            mTimer->Update();

            if (mTimer->DeltaTime() >= 1.0f / FRAME_RATE)
            {
                mTimer->Reset();
                Update();
                LateUpdate();
                Render();
            }

            // event processing loop 
            while (SDL_PollEvent(&mEvent))
            {
                switch (mEvent.type)
                {
                case SDL_QUIT:
                    mQuit = true;
                    break;
                }
            }
            Update();
            LateUpdate();
            Render();
        }
    }

    void GameManager::Update()
    {
        mInputManager->Update();

        mStartScreen->Update();

        
        if (mInputManager->KeyPressed(SDL_SCANCODE_SPACE))
        {
            std::cout << "Space pressed!" << std::endl;
        }
        if (mInputManager->KeyReleased(SDL_SCANCODE_SPACE))
        {
            std::cout << "Space released!" << std::endl;
        }
        if (mInputManager->MouseButtonPressed(InputManager::Left))
        {
            std::cout << "Left mouse button pressed!" << std::endl;
        }
        if (mInputManager->MouseButtonReleased(InputManager::Left))
        {
            std::cout << "Left mouse button released!" << std::endl;
        }

        if (mInputManager->KeyDown(SDL_SCANCODE_1)) 
        { 
            mAudioManager->PlaySFX("bling.wav", 0, -1); 
        }
    }

    void GameManager::LateUpdate()
    {
        mInputManager->UpdatePrevInput();
    }

    void GameManager::Render()
    {
        mGraphics->ClearBackBuffer();
        
        mStartScreen->Render();
        
        mGraphics->Render();
    }

    void GameManager::TestStuff()
    {
        mStartScreen = new StartScreen();
    }

    GameManager::GameManager() : mQuit(false)
    {
        // create Graphics singleton 
        mEvent = {};

        mGraphics = Graphics::Instance();
        mTimer = Timer::Instance();         
        mInputManager = InputManager::Instance(); 
        mAudioManager = AudioManager::Instance();
        mAssetManager = AssetManager::Instance();

        // verify Graphics module is ready 
        if (!Graphics::Initialized())
        {
            mQuit = true;
        }
        
        TestStuff();
    }

    GameManager::~GameManager()
    {
        // release modules 
        Graphics::Release();
        mGraphics = nullptr;

        Timer::Release();
        mTimer = nullptr;
        
        delete mStartScreen; 
        mStartScreen = nullptr;

        AssetManager::Release();
        mAssetManager = nullptr;

        InputManager::Release(); 
        mInputManager = nullptr; 
        
        AudioManager::Release();
        mAudioManager = nullptr;

        // Quit SDL subsystems 
        SDL_Quit();
    }
}