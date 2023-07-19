#pragma once

#include <vector>
#include <BezierCurve.h>

namespace SDLFramework
{
    class BezierPath
    {
    private:
        std::vector<BezierCurve> mCurves;
        std::vector<int> mSamples;

    public:
        BezierPath();
        ~BezierPath();

        void AddCurve(BezierCurve curve, int samples);
        void Sample(std::vector<Vector2>* sampledPath);
    };

    BezierPath::BezierPath()
    { }

    BezierPath::~BezierPath()
    { }

    void BezierPath::AddCurve(BezierCurve curve, int samples)
    {
        mCurves.push_back(curve);
        mSamples.push_back(samples);
    }

    void BezierPath::Sample(std::vector<Vector2>* sampledPath)
    {
        for (unsigned i = 0; i < mCurves.size(); ++i)
        {
            for (float t = 0.0f; t <= 1.0f; t += (1.0f / mSamples[i]))
            {
                sampledPath->push_back(mCurves[i].CalculatePointAlongCurve(t));
            }
        }
    }
}