#pragma once

#include <SDLMain.h> 

namespace SDLFramework
{
    class Timer
    {
    private:
        static Timer* sInstance;
        unsigned int mStartTicks;
        unsigned int mElapsedTicks;
        float mDeltaTime;
        float mTimeScale;

        Timer();
        ~Timer();

    public:
        static Timer* Instance();
        static void Release();
        void Reset();
        float DeltaTime() const;
        void TimeScale(float ts);
        float TimeScale() const;
        void Update();
    };

    Timer* Timer::sInstance = nullptr;

    Timer* Timer::Instance()
    {
        if (sInstance == nullptr)
        {
            sInstance = new Timer();
        }
        return sInstance;
    }

    void Timer::Release()
    {
        delete sInstance;
        sInstance = nullptr;
    }

    void Timer::Reset()
    {
        mStartTicks = SDL_GetTicks();
    }

    float Timer::DeltaTime() const
    { 
        return mDeltaTime; 
    }
    
    void Timer::TimeScale(float ts) 
    { 
        mTimeScale = ts;
    }

    float Timer::TimeScale() const
    {
        return mTimeScale; 
    }

    void Timer::Update()
    {
        mElapsedTicks = SDL_GetTicks() - mStartTicks; 
        mDeltaTime = mElapsedTicks * 0.001f; // convert to seconds 
    }

    Timer::Timer()
    { 
        Reset(); 
        mElapsedTicks = 0; 
        mDeltaTime = 0.0f; 
        mTimeScale = 1.0f; 
    } 
    
    Timer::~Timer() { }
}