#pragma once

#include <Graphics.h> 
#include <Timer.h>
#include <GameEntity.h>
#include <Texture.h>
#include <AssetManager.h>

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
        SDL_Event mEvent;
        const int FRAME_RATE = 60; 
        Timer* mTimer;

        //Test
        AssetManager* mAssetManager;
        Texture* mTex;
        //End test
    public:
        static GameManager* Instance();
        static void Release();

        void Run();
        void Update();
        void LateUpdate();
        void Render();

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
        //std::cout << "Delta time: " << mTimer->DeltaTime() << std::endl;
    } 
    
    void GameManager::LateUpdate()
    { } 
    
    void GameManager::Render() 
    { 
        mGraphics->ClearBackBuffer();
        mTex->Render();
        mGraphics->Render(); 
    }

    GameManager::GameManager() : mQuit(false)
    { 
        // create Graphics singleton 
        mEvent = {};
        
        mGraphics = Graphics::Instance(); 
        
        mTimer = Timer::Instance();
        
        // verify Graphics module is ready 
        if (!Graphics::Initialized()) 
        { 
            mQuit = true; 
        } 

        mAssetManager = AssetManager::Instance();
        mTex = new Texture("SpriteSheet.png");
    }

    GameManager::~GameManager() 
    {
        // release modules 
        Graphics::Release(); 
        mGraphics = nullptr;
        
        Timer::Release();
        mTimer = nullptr;

        delete mTex; 
        mTex = nullptr;

        AssetManager::Release();
        
        // Quit SDL subsystems 
        SDL_Quit(); 
    }
}