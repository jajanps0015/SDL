#pragma once

#include <Graphics.h> 
#include <Timer.h>
#include <GameEntity.h>

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

        GameEntity* mParent; 
        GameEntity* mChild;

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
        mGraphics->Render(); 
    }

    GameManager::GameManager() : mQuit(false)
    { // create Graphics singleton 
        mEvent = {};
        mGraphics = Graphics::Instance(); 
        mTimer = Timer::Instance();
        // verify Graphics module is ready 
        if (!Graphics::Initialized()) 
        { 
            mQuit = true; 
        } 

        mParent = new GameEntity(100.0f, 400.0f); 
        mChild = new GameEntity(100.0f, 500.0f);

        printf("Child local pos: (%f, %f)\n", 
            mChild->Position(GameEntity::Local).x, 
            mChild->Position(GameEntity::Local).y); 
        
        // set parent of mChild to mParent 
        mChild->Parent(mParent); 
        
        // print local position of mChild with parent set 
        printf("Child local pos: (%f, %f)\n", 
            mChild->Position(GameEntity::Local).x, 
            mChild->Position(GameEntity::Local).y);
    }

    GameManager::~GameManager() 
    {
        // release modules 
        Graphics::Release(); 
        mGraphics = nullptr;
        
        Timer::Release();
        mTimer = nullptr;
        
        // Quit SDL subsystems 
        SDL_Quit(); 
    }
}