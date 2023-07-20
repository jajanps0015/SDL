#pragma once

#include <galaga/Enemy.h>

namespace Galaga
{
    class Boss : public Enemy
    {
    private:
        // Inherited via Enemy 
        Vector2 LocalFormationPosition() override;

        void HandleDiveState() override;
        void HandleDeadState() override;

        void RenderDiveState() override;
        void RenderDeadState() override;

        static std::vector<std::vector<Vector2>> sDivePaths;
        bool mCaptureDive;

    public:
        Boss(int path, int index, bool challenge);
        ~Boss();
        static void CreateDivePaths();
        void Dive(int type = 0) override;
    };

    Vector2 Boss::LocalFormationPosition()
    {
        Vector2 retVal;

        int dir = mIndex % 2 == 0 ? -1 : 1;

        retVal.x = (sFormation->GridSize().x +
            sFormation->GridSize().x * 2 * (mIndex / 2)
            ) * (float)dir;

        retVal.y = -sFormation->GridSize().y;

        return retVal;
    }

    void Boss::HandleDeadState() { }

    void Boss::RenderDeadState() { }

    Boss::Boss(int path, int index, bool challenge) : Enemy(path, index, challenge)
    {
        mTexture = new Texture("Bosses.png", 0, 0, 64, 64);

        mTexture->Parent(this);
        mTexture->Position(Vec2_Zero);
        mType = Enemy::Boss;

        mTextures[0] = new Texture("Bosses.png", 0, 0, 64, 64);
        mTextures[1] = new Texture("Bosses.png", 64, 0, 64, 64);

        for (int i = 0; i < 2; i++)
        {
            mTextures[i]->Parent(this);
            mTextures[i]->Position(Vec2_Zero);
        }
    }

    Boss::~Boss()
    { }

    std::vector<std::vector<Vector2>> Boss::sDivePaths;

    void Boss::CreateDivePaths()
    {
        int currentPath = 0;

        BezierPath* path = new BezierPath();

        path->AddCurve({ Vector2(0.0f, 0.0f), Vector2(0.0f, -60.0f), Vector2(-90.0f, -60.0f), Vector2(-90.0f, 0.0f) }, 15);
        path->AddCurve({ Vector2(-90.0f, 0.0f), Vector2(-90.0f, 60.0f), Vector2(-100.0f, 272.0f), Vector2(-15.0f, 275.0f) }, 15);
        path->AddCurve({ Vector2(-15.0f, 275.0f), Vector2(85.0f, 275.0f), Vector2(85.0f, 125.0f), Vector2(-15.0f, 125.0f) }, 15);
        path->AddCurve({ Vector2(-15.0f, 125.0f), Vector2(-175.0f, 125.0f), Vector2(0.0f, 450.0f), Vector2(125.0f, 450.0f) }, 25);
        path->AddCurve({ Vector2(120.0f, 450.0f), Vector2(160.0f, 450.0f), Vector2(200.0f, 500.0f), Vector2(200.0f, 550.0f) }, 15);
        path->AddCurve({ Vector2(200.0f, 550.0f), Vector2(200.0f, 540.0f), Vector2(200.0f, 810.0f), Vector2(200.0f, 800.0f) }, 15);

        sDivePaths.push_back(std::vector<Vector2>());
        path->Sample(&sDivePaths[currentPath]);

        //****************2********************

        delete path;

        currentPath = 1;

        path = new BezierPath();
        path->AddCurve({ Vector2(0.0f, 0.0f), Vector2(0.0f, -60.0f), Vector2(90.0f, -60.0f), Vector2(90.0f, 0.0f) }, 15);
        path->AddCurve({ Vector2(90.0f, 0.0f), Vector2(90.0f, 60.0f), Vector2(100.0f, 272.0f), Vector2(15.0f, 275.0f) }, 15);
        path->AddCurve({ Vector2(15.0f, 275.0f), Vector2(-85.0f, 275.0f), Vector2(-85.0f, 125.0f), Vector2(15.0f, 125.0f) }, 15);
        path->AddCurve({ Vector2(15.0f, 125.0f), Vector2(175.0f, 125.0f), Vector2(0.0f, 450.0f), Vector2(-125.0f, 450.0f) }, 25);
        path->AddCurve({ Vector2(-120.0f, 450.0f), Vector2(-160.0f, 450.0f), Vector2(-200.0f, 500.0f), Vector2(-200.0f, 550.0f) }, 15);
        path->AddCurve({ Vector2(-200.0f, 550.0f), Vector2(-200.0f, 540.0f), Vector2(-200.0f, 810.0f), Vector2(-200.0f, 800.0f) }, 15);

        sDivePaths.push_back(std::vector<Vector2>());
        path->Sample(&sDivePaths[currentPath]);

        delete path;
    }

    void Boss::Dive(int type)
    {
        mCaptureDive = type != 0;
        Enemy::Dive();
    }

    void Boss::HandleDiveState()
    {
        int currentPath = mIndex % 2;

        if (mCaptureDive)
        {
            currentPath += 2;
        }

        if (mCurrentWaypoint < sDivePaths[currentPath].size())
        {
            // follow dive path 
            Vector2 waypointPos = mDiveStartPosition + sDivePaths[currentPath][mCurrentWaypoint];
            Vector2 dist = waypointPos - Position();

            Translate(dist.Normalized() * mSpeed * mTimer->DeltaTime(), World);
            Rotation(atan2(dist.y, dist.x) * RAD_TO_DEG + 90.0f);

            if ((waypointPos - Position()).MagnitudeSqr() < EPSILON * mSpeed / 25.0f)
            {
                mCurrentWaypoint += 1;
            }
        }
        else
        {
            // return to formation 
            Vector2 dist = WorldFormationPosition() - Position();
            Translate(dist.Normalized() * mSpeed * mTimer->DeltaTime(), World);

            Rotation(atan2(dist.y, dist.x) * RAD_TO_DEG + 90.0f);

            if (dist.MagnitudeSqr() < EPSILON * mSpeed / 25.0f)
            {
                JoinFormation();
            }
        }
    }

    void Boss::RenderDiveState()
    {
        mTexture->Render();
    }

}