#pragma once

#include <galaga/StarLayer.h>

namespace Galaga
{
    class BackgroundStars {

    private:
        static BackgroundStars* sInstance;
        static const int LAYER_COUNT = 3;
        StarLayer* mLayers[LAYER_COUNT];

        BackgroundStars();
        ~BackgroundStars();

    public:
        static BackgroundStars* Instance();
        static void Release();
        void Scroll(bool b);
        void Update();
        void Render();
    };

    BackgroundStars* BackgroundStars::sInstance = nullptr; 
    
    BackgroundStars* BackgroundStars::Instance() 
    { 
        if (sInstance == nullptr) 
        { sInstance = new BackgroundStars(); 
        } 
        return sInstance; 
    } 
    
    void BackgroundStars::Release() 
    { 
        delete sInstance; 
        sInstance = nullptr; 
    } 
    
    void BackgroundStars::Scroll(bool b) 
    { 
        Star::Scroll(b); 
    } 
    
    void BackgroundStars::Update() 
    { 
        for (auto layer : mLayers) 
        { 
            layer->Update(); 
        } 
    } 
    
    void BackgroundStars::Render() 
    { 
        for (auto layer : mLayers) 
        { 
            layer->Render(); 
        } 
    } 
    
    BackgroundStars::BackgroundStars() 
    { 
        for (int i = 0; i < LAYER_COUNT; i++) 
        { 
            mLayers[i] = new StarLayer(i + 1); 
        } 
    } 
    
    BackgroundStars::~BackgroundStars() 
    { 
        for (int i = 0; i < LAYER_COUNT; i++) 
        { 
            delete mLayers[i]; 
            mLayers[i] = nullptr; 
        } 
    }
}