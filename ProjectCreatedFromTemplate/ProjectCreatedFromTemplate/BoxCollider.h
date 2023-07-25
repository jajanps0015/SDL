#pragma once

#include <Collider.h>

namespace SDLFramework
{
    enum BoxCorner
    {
        TopLeft = 0,
        TopRight,
        BottomLeft,
        BottomRight
    };

    class BoxCollider : public Collider
    {
    private:
        static const int MAX_VERTS = 4;

        GameEntity* mVerts[MAX_VERTS];

        void AddVert(int index, Vector2 pos);

    public:

        BoxCollider(Vector2 size);

        ~BoxCollider();

        Vector2 GetFurthestPoint() override;

        Vector2 GetVertexPos(BoxCorner index);
    };

    void BoxCollider::AddVert(int index, Vector2 pos)
    {
        mVerts[index] = new GameEntity(pos);

        mVerts[index]->Parent(this);
    }

    BoxCollider::BoxCollider(Vector2 size) : Collider(ColliderType::Box)
    {
        AddVert(TopLeft, Vector2(-0.5f * size.x, -0.5f * size.y));
        AddVert(TopRight, Vector2(0.5f * size.x, -0.5f * size.y)); 
        AddVert(BottomLeft, Vector2(-0.5f * size.x, 0.5f * size.y)); 
        AddVert(BottomRight, Vector2(0.5f * size.x, 0.5f * size.y)); 
        
        if (DEBUG_COLLIDERS) 
        { 
            SetDebugTexture(new Texture("BoxCollider.png")); 
            mDebugTexture->Scale(size); 
        }
    } 
    
    BoxCollider::~BoxCollider() 
    { 
        for (auto v : mVerts) 
        { 
            delete v; 
        } 
    }

    Vector2 BoxCollider::GetFurthestPoint() 
    { 
        Vector2 localPos = Position(GameEntity::Local); 
        int furthestIndex = 0; 
        
        float assumedFurthest = (
            localPos + mVerts[0]->Position(GameEntity::Local)).Magnitude(); 
        
        
        for (int i = 0; i < MAX_VERTS; i++) 
        {
            float newVertexDistance = 0.0f;
            newVertexDistance = (localPos + mVerts[i]->Position(GameEntity::Local)).MagnitudeSqr();
            
            if (newVertexDistance > assumedFurthest)
            { 
                furthestIndex = i; 
                assumedFurthest = newVertexDistance; 
            } 
        } 
        return localPos + mVerts[furthestIndex]->Position(GameEntity::Local); 
    }

    Vector2 BoxCollider::GetVertexPos(BoxCorner index) 
    { 
        return mVerts[index]->Position(); 
    }
}