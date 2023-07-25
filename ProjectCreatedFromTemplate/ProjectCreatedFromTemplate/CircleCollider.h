#pragma once

#include <Collider.h>

namespace SDLFramework
{
    const float DEBUG_IMAGE_SIZE = 50;

    class CircleCollider : public Collider 
    {
    private: 
        
        float mRadius; 
    
    public: 
        CircleCollider(float radius, bool broadPhase = false); 
        
        ~CircleCollider(); 
        
        Vector2 GetFurthestPoint() override; 
        
        float GetRadius();
    };

    CircleCollider::CircleCollider(float radius, bool broadPhase) : 
        Collider(ColliderType::Circle) 
    {
        mRadius = radius; 
        
        if (DEBUG_COLLIDERS) 
        {
            if (broadPhase) 
            { 
                SetDebugTexture(new Texture("BroadPhaseCollider.png")); 
            }
            else 
            { 
                SetDebugTexture(new Texture("CircleCollider.png")); 
            } 
            
            // circle collider file size is 50px 
            
            mDebugTexture->Scale(Vec2_One * (radius * 2 / SDLFramework::DEBUG_IMAGE_SIZE)); }
    } 
    
    CircleCollider::~CircleCollider() 
    {

    }

    Vector2 CircleCollider::GetFurthestPoint() 
    { 
        return Vec2_Right * 
            (mRadius + Position(GameEntity::Local).Magnitude()); 
    }
    
    float CircleCollider::GetRadius() 
    { 
        return mRadius; 
    }
}