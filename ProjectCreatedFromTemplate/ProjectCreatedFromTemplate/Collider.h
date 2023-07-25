#pragma once

#include <Texture.h>

namespace SDLFramework
{
    class Collider : public GameEntity 
    {
    public: 
        enum class ColliderType { Box, Circle }; // type safety 

        Collider(ColliderType type); 
        
        virtual ~Collider(); 
        
        virtual Vector2 GetFurthestPoint() = 0; 
        
        ColliderType GetType(); 
        
        virtual void Render() override;

    protected: 
        ColliderType mType;

        static const bool DEBUG_COLLIDERS = true; 
        
        Texture* mDebugTexture; 
        
        void SetDebugTexture(Texture* tex);
    };

    void Collider::SetDebugTexture(Texture* tex) 
    { 
        delete mDebugTexture; 
        
        mDebugTexture = tex; 
        mDebugTexture->Parent(this); 
    }
    
    Collider::Collider(ColliderType type) : mType(type) 
    { 
        mDebugTexture = nullptr;
    } 
    
    Collider::~Collider() 
    { 
        delete mDebugTexture; 
        mDebugTexture = nullptr; 
    }

    Collider::ColliderType Collider::GetType() 
    { 
        return mType; 
    } 
    
    void Collider::Render() 
    { 
        if (DEBUG_COLLIDERS) 
        { 
            mDebugTexture->Render(); 
        } 
    }
}