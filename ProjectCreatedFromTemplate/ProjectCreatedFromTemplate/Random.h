#pragma once

#include <random>
#include <chrono>

namespace SDLFramework
{
    class Random
    {
    private:
        static Random* sInstance;
        std::mt19937 mGenerator;

        Random();
        ~Random();

    public:
        static Random* Instance();
        static void Release();

        unsigned RandomInt(); // [0,unsigned max] 
        float RandomFloat(); // [0.0,1.0) 

        int RandomRange(int lo, int hi); // [lo,hi] 
        float RandomRange(float lo, float hi); // [lo,hi)
    };

    Random* Random::sInstance = nullptr;

    Random* Random::Instance()
    {
        if (sInstance == nullptr)
        {
            sInstance = new Random();
        }
        return sInstance;
    }

    void Random::Release()
    {
        delete sInstance;
        sInstance = nullptr;
    }

    Random::Random() 
    { 
        unsigned seed = (unsigned)std::chrono::system_clock::now()
            .time_since_epoch()
            .count(); 
        mGenerator.seed(seed); 
    } 
    
    Random::~Random() { }

    unsigned Random::RandomInt() 
    { 
        return mGenerator(); 
    }

    float Random::RandomFloat() 
    { 
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        return dist(mGenerator); 
    } 
    
    int SDLFramework::Random::RandomRange(int lo, int hi) 
    { 
        std::uniform_int_distribution<int> dist(lo, hi); 
        return dist(mGenerator); 
    } 
    
    float SDLFramework::Random::RandomRange(float lo, float hi) 
    { 
        std::uniform_real_distribution<float> dist(lo, hi); 
        return dist(mGenerator); 
    }
}