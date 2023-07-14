#pragma once

#include <AnimatedTexture.h>
#include <GameEntity.h>
#include <InputManager.h> 

using namespace SDLFramework;
namespace Galaga
{
    class StartScreen : public GameEntity
    {
    private:
        Timer* mTimer;
        InputManager* mInput;

        GameEntity* mTopBar; 
        Texture* mPlayerOne; 
        Texture* mPlayerTwo; 
        Texture* mHiScore;

    public: 
        StartScreen(); 
        ~StartScreen(); 

        void Update() override; 
        void Render() override;
    };

    StartScreen::StartScreen() 
    { 
        mTimer = Timer::Instance(); 
        mInput = InputManager::Instance(); 

        //empty holder
        mTopBar = new GameEntity(Graphics::SCREEN_WIDTH * 0.5f, 80.0f); 
        
        mPlayerOne = new Texture("1UP", "waltdisney.ttf", 32, { 200, 0, 0 }); 
        mPlayerTwo = new Texture("2UP", "waltdisney.ttf", 32, { 200, 0, 0 }); 
        mHiScore = new Texture("HI SCORE", "waltdisney.ttf", 32, { 200, 0, 0 });

        mTopBar->Parent(this); 
        
        mPlayerOne->Parent(mTopBar); 
        mPlayerTwo->Parent(mTopBar); 
        mHiScore->Parent(mTopBar);

        mPlayerOne->Position(-Graphics::SCREEN_WIDTH * 0.35f, 0.0f); 
        mPlayerTwo->Position(Graphics::SCREEN_WIDTH * 0.2f, 0.0f); 
        mHiScore->Position(-30.0f, 0.0f);

        //mTopBar->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f);
    }

    StartScreen::~StartScreen() 
    { 
        // top bar entities 
        delete mTopBar; 
        mTopBar = nullptr; 
        
        delete mPlayerOne; 
        mPlayerOne = nullptr; 
        
        delete mPlayerTwo; 
        mPlayerTwo = nullptr; 

        delete mHiScore; 

        mTimer = nullptr;         
        mInput = nullptr; 
    }

    void StartScreen::Update() { } 
    
    void StartScreen::Render() 
    { 
        mPlayerOne->Render(); 
        mPlayerTwo->Render(); 
        mHiScore->Render();
    }
}