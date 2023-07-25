#pragma once

#include <BoxCollider.h>
#include <CircleCollider.h>
#include <MathHelper.h>

namespace SDLFramework
{
    inline bool CircleVsCircleCollision(CircleCollider* c1, CircleCollider* c2) 
    { 
        return (c1->Position() - c2->Position()).Magnitude() < 
                (c1->GetRadius() + c2->GetRadius()); 
    }

    inline bool BoxVsCircleCollision(BoxCollider* box, CircleCollider* circle) 
    { 
        Vector2 circlePos = circle->Position(); 
        float radius = circle->GetRadius(); 
        
        if (PointToLineDistance(box->GetVertexPos(TopLeft), box->GetVertexPos(TopRight), circlePos) < radius 
            
            || PointToLineDistance(box->GetVertexPos(TopLeft), box->GetVertexPos(BottomLeft), circlePos) < radius
            
            || PointToLineDistance(box->GetVertexPos(BottomLeft), box->GetVertexPos(BottomRight), circlePos) < radius
            
            || PointToLineDistance(box->GetVertexPos(BottomRight), box->GetVertexPos(TopRight), circlePos) < radius) 
        
        { 
            return true; 
        } 
        
        return false; 
    }

    inline bool BoxVsBoxCollision(BoxCollider* b1, BoxCollider* b2)
    { 
        Vector2 projAxes[4]; 

        projAxes[0] = (b1->GetVertexPos((BoxCorner)0) - b1->GetVertexPos((BoxCorner)1)).Normalized();
        projAxes[1] = (b1->GetVertexPos((BoxCorner)0) - b1->GetVertexPos((BoxCorner)2)).Normalized();
        projAxes[2] = (b2->GetVertexPos((BoxCorner)0) - b2->GetVertexPos((BoxCorner)1)).Normalized();
        projAxes[3] = (b2->GetVertexPos((BoxCorner)0) - b2->GetVertexPos((BoxCorner)2)).Normalized();

        float b1Min = 0.0f, b1Max = 0.0f; 
        float b2Min = 0.0f, b2Max = 0.0f; 
        float proj1 = 0.0f, proj2 = 0.0f;

        for (int projectionIndex = 0; projectionIndex < 4; projectionIndex++)
        { 
            for (int vertexIndex = 0; vertexIndex < 4; vertexIndex++)
            { 
                proj1 = Dot(b1->GetVertexPos((BoxCorner)vertexIndex), projAxes[projectionIndex]);
                
                proj2 = Dot(b2->GetVertexPos((BoxCorner)vertexIndex), projAxes[projectionIndex]);

                if (vertexIndex == 0) 
                { 
                    b1Min = b1Max = proj1; 
                    b2Min = b2Max = proj2; 
                }
                else 
                { 
                    if (proj1 < b1Min) 
                        b1Min = proj1; 
                    
                    if (proj1 > b1Max)
                        b1Max = proj1; 
                    
                    if (proj2 < b2Min) 
                        b2Min = proj2; 
                    
                    if (proj2 > b2Max) 
                        b2Max = proj2; 
                }
            }

            float halfDist1 = (b1Max - b1Min) * 0.5f;

            float midPoint1 = b1Min + halfDist1;

            float halfDist2 = (b2Max - b2Min) * 0.5f;

            float midPoint2 = b2Min + halfDist2;

            if (abs(midPoint1 - midPoint2) > (halfDist1 + halfDist2))
            {
                return false;
            }
        }

        return true;
    }

    inline bool ColliderVsColliderCheck(Collider* c1, Collider* c2)
    { 
        if (c1->GetType() == Collider::ColliderType::Circle 
            && c2->GetType() == Collider::ColliderType::Circle) 
            
            return CircleVsCircleCollision(static_cast<CircleCollider*>(c1), static_cast<CircleCollider*>(c2)); 
        
        else if (c1->GetType() == Collider::ColliderType::Box 
            && c2->GetType() == Collider::ColliderType::Circle) 
            
            return BoxVsCircleCollision(static_cast<BoxCollider*>(c1), static_cast<CircleCollider*>(c2)); 
        
        else if (c2->GetType() == Collider::ColliderType::Box 
            && c1->GetType() == Collider::ColliderType::Circle) 
            
            return BoxVsCircleCollision(static_cast<BoxCollider*>(c2), static_cast<CircleCollider*>(c1)); 
        
        else if (c1->GetType() == Collider::ColliderType::Box 
            && c2->GetType() == Collider::ColliderType::Box) 
            
            return BoxVsBoxCollision(static_cast<BoxCollider*>(c1), static_cast<BoxCollider*>(c2)); 
        
        else return false; 
    }
}