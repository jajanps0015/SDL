#pragma once

#include <GameEntity.h>
#include <Timer.h>

using namespace SDLFramework;

namespace Galaga
{
    class Formation : public GameEntity 
    { 
    private: 
        Timer* mTimer; 
        // side-to-side movement while still spawning 
        float mOffsetAmount; 
        float mOffsetTimer; 
        float mOffsetDelay; 
        int mOffsetCounter; 
        int mOffsetDirection; 
        
        // breathing animation when formation locked 
        float mSpreadTimer; 
        float mSpreadDelay; 
        int mSpreadCounter; 
        int mSpreadDirection;

        Vector2 mGridSize; //in pixels 
        bool mLocked;

    public:
        Formation(); 
        ~Formation(); 
        
        Vector2 GridSize(); 
        int GetTick(); 
        void Lock(); 
        bool Locked(); 
        
        void Update() override;
    };

    Formation::Formation() 
    { 
        mTimer = Timer::Instance(); 
        mOffsetAmount = 10.0f;
        mOffsetDelay = 0.4f; 
        mOffsetTimer = 0.0f; 
        mOffsetCounter = 4; 
        mOffsetDirection = 1; 
        
        mSpreadTimer = 0.0f; 
        mSpreadDelay = 0.6f; 
        mSpreadCounter = 0; 
        mSpreadDirection = 1; 
        
        mLocked = false; 
        mGridSize = Vector2(32.0f, 64.0f); 
    } 
    
    Formation::~Formation() 
    { 
        mTimer = nullptr; 
    }

    Vector2 Formation::GridSize() 
    { 
        return mGridSize; 
    } 
    
    int Formation::GetTick() 
    { 
        if (!mLocked || mOffsetCounter != 4) 
        { 
            return mOffsetCounter; 
        } 
        else 
        { 
            return mSpreadCounter; 
        } 
    } 
    
    void Formation::Lock() 
    { 
        mLocked = true; 
    } 
    
    bool Formation::Locked() 
    { 
        return mLocked && mOffsetCounter == 4; 
    }

    void Formation::Update() 
    { 
        if (!mLocked || mOffsetCounter != 4) 
        { 
            mOffsetTimer += mTimer->DeltaTime(); 
            
            if (mOffsetTimer >= mOffsetDelay) 
            { 
                mOffsetCounter += 1; 
                Translate(Vec2_Right * (float)mOffsetDirection * mOffsetAmount, World); 
                
                if (mOffsetCounter == 8) 
                { 
                    mOffsetCounter = 0; 
                    mOffsetDirection *= -1; 
                } 
                mOffsetTimer = 0.0f; 
            } 
        }
        else 
        { 
            mSpreadTimer += mTimer->DeltaTime(); 
            
            if (mSpreadTimer >= mSpreadDelay) 
            { 
                mSpreadCounter += mSpreadDirection; 
                mGridSize.x += (mSpreadDirection * ((mSpreadCounter % 2) ? 1 : 2)); 
                
                if (mSpreadCounter == 4 || mSpreadCounter == 0) 
                { 
                    mSpreadDirection *= -1; 
                } 
                mSpreadTimer = 0.0f; 
            } 
        }
    }
}