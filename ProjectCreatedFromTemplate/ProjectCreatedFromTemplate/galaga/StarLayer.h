#pragma once

#include "Star.h" 
namespace Galaga
{

    class StarLayer
    {
    private:

        static const int STAR_COUNT = 30;
        Star* mStars[STAR_COUNT];

    public:
        StarLayer(int layer);
        ~StarLayer();
        void Update();
        void Render();
    };

    StarLayer::StarLayer(int layer)
    {
        for (int i = 0; i < STAR_COUNT; i++)
        {
            mStars[i] = new Star(layer);
        }
    }

    StarLayer::~StarLayer()
    {
        for (int i = 0; i < STAR_COUNT; i++)
        {
            delete mStars[i];
            mStars[i] = nullptr;
        }
    }

    void StarLayer::Update()
    {
        for (auto star : mStars)
        {
            star->Update();
        }
    }
    
    void StarLayer::Render() 
    { 
        for (auto star : mStars) 
        { 
            star->Render(); 
        } 
    }
}