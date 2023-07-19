#pragma once

#include <MathHelper.h>

namespace SDLFramework
{
    struct BezierCurve
    {
        Vector2 p0;
        Vector2 p1;
        Vector2 p2;
        Vector2 p3;
        
        Vector2 CalculatePointAlongCurve(float t)
        {
            float tt = t * t;
            float ttt = tt * t;
            float u = 1.0f - t;
            float uu = u * u;
            float uuu = uu * u;
            Vector2 point = (uuu * p0) +
                (3 * uu * t * p1) +
                (3 * u * tt * p2) +
                (ttt * p3);
            point.x = (float)round(point.x);
            point.y = (float)round(point.y);
            return point;
        }
    };
}