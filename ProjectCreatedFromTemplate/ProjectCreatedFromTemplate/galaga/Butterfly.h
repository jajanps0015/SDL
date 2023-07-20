#pragma once

#include <galaga/Enemy.h>

namespace Galaga
{
    class Butterfly : public Enemy
    {
    private:
        // Inherited via Enemy 
        Vector2 LocalFormationPosition() override;
        void HandleDiveState() override;
        void HandleDeadState() override;
        void RenderDiveState() override;
        void RenderDeadState() override;

        static std::vector<std::vector<Vector2>> sDivePaths;
        bool mEscort;

    public:
        Butterfly(int path, int index, bool challenge);

        ~Butterfly();

        static void CreateDivePaths();

        void Dive(int type = 0) override;
    };

    Vector2 Butterfly::LocalFormationPosition()
    {
        Vector2 retVal;

        int dir = mIndex % 2 == 0 ? -1 : 1;
        retVal = sFormation->GridSize().x * (float)dir;

        retVal.x = sFormation->GridSize().x * (float)dir;
        retVal.y = sFormation->GridSize().y * ((mIndex % 4) / 2);

        retVal.x = (sFormation->GridSize().x +
            sFormation->GridSize().x * 2 * (mIndex / 4)
            ) * (float)dir;

        return retVal;
    }

    void Butterfly::HandleDiveState()
    {
        int currentPath = mIndex % 2;
        if (mEscort)
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
    
    void Butterfly::HandleDeadState() { }

    void Butterfly::RenderDiveState()
    {
        mTexture->Render();
    }
    
    void Butterfly::RenderDeadState() { }

    Butterfly::Butterfly(int path, int index, bool challenge) : Enemy(path, index, challenge)
    {
        mTexture = new Texture("AnimatedEnemies.png", 0, 0, 52, 40);
        mTexture->Parent(this);
        mTexture->Position(Vec2_Zero);
        mType = Enemy::Butterfly;

        mTextures[0] = new Texture("AnimatedEnemies.png", 0, 0, 52, 40);
        mTextures[1] = new Texture("AnimatedEnemies.png", 52, 0, 52, 40);

        for (int i = 0; i < 2; i++)
        {
            mTextures[i]->Parent(this);
            mTextures[i]->Position(Vec2_Zero);
        }
    }

    Butterfly::~Butterfly()
    {

    }

    std::vector<std::vector<Vector2>> Butterfly::sDivePaths;

    void Butterfly::CreateDivePaths()
    {
        // *************** Curve 1 ***************************
        int currentPath = 0;

        BezierPath* path = new BezierPath();

        path->AddCurve({ Vector2(0.0f, 0.0f), Vector2(0.0f, -45.0f), Vector2(-60.0f, -45.0f), Vector2(-60.f, 0.0f) }, 15);
        path->AddCurve({ Vector2(-60.0f, 0.0f), Vector2(-60.0f, 80.0f), Vector2(200.0f, 125.0f), Vector2(200.0f, 200.0f) }, 15);
        path->AddCurve({ Vector2(200.0f, 200.0f), Vector2(200.0f, 275.0f), Vector2(175.0f, 250.0f), Vector2(175.0f, 325.0f) }, 15);
        path->AddCurve({ Vector2(175.0f, 325.0f), Vector2(175.0f, 425.0f), Vector2(375.0f, 425.0f), Vector2(375.0f, 525.0f) }, 15);
        path->AddCurve({ Vector2(375.0f, 525.0f), Vector2(375.0f, 575.0f), Vector2(300.0f, 625.0f), Vector2(300.0f, 775.0f) }, 15);

        sDivePaths.push_back(std::vector<Vector2>());
        path->Sample(&sDivePaths[currentPath]);

        // *************** Curve 1 ***************************

        delete path;
        currentPath = 1;

        path = new BezierPath();

        path->AddCurve({ Vector2(0.0f, 0.0f), Vector2(0.0f, -45.0f), Vector2(60.0f, -45.0f), Vector2(60.f, 0.0f) }, 15);
        path->AddCurve({ Vector2(60.0f, 0.0f), Vector2(60.0f, 80.0f), Vector2(-200.0f, 125.0f), Vector2(-200.0f, 200.0f) }, 15);
        path->AddCurve({ Vector2(-200.0f, 200.0f), Vector2(-200.0f, 275.0f), Vector2(-175.0f, 250.0f), Vector2(-175.0f, 325.0f) }, 15);
        path->AddCurve({ Vector2(-175.0f, 325.0f), Vector2(-175.0f, 425.0f), Vector2(-375.0f, 425.0f), Vector2(-375.0f, 525.0f) }, 15);
        path->AddCurve({ Vector2(-375.0f, 525.0f), Vector2(-375.0f, 575.0f), Vector2(-300.0f, 625.0f), Vector2(-300.0f, 775.0f) }, 15);

        sDivePaths.push_back(std::vector<Vector2>());
        path->Sample(&sDivePaths[currentPath]);

        // *************** Curve 2 ***************************
        delete path;
        currentPath = 2;

        path = new BezierPath();

        path->AddCurve({ Vector2(0.0f, 0.0f), Vector2(0.0f, -60.0f), Vector2(-90.0f, -60.0f), Vector2(-90.0f, 0.0f) }, 15);
        path->AddCurve({ Vector2(-90.0f, 0.0f), Vector2(-90.0f, 60.0f), Vector2(-100.0f, 272.0f), Vector2(-15.0f, 275.0f) }, 15);
        path->AddCurve({ Vector2(-15.0f, 275.0f), Vector2(85.0f, 275.0f), Vector2(85.0f, 125.0f), Vector2(-15.0f, 125.0f) }, 15);
        path->AddCurve({ Vector2(-15.0f, 125.0f), Vector2(-175.0f, 125.0f), Vector2(0.0f, 450.0f), Vector2(125.0f, 450.0f) }, 25);
        path->AddCurve({ Vector2(120.0f, 450.0f), Vector2(160.0f, 450.0f), Vector2(200.0f, 500.0f), Vector2(200.0f, 550.0f) }, 15);
        path->AddCurve({ Vector2(200.0f, 550.0f), Vector2(200.0f, 540.0f), Vector2(200.0f, 810.0f), Vector2(200.0f, 800.0f) }, 15);

        sDivePaths.push_back(std::vector<Vector2>());
        path->Sample(&sDivePaths[currentPath]);

        //****************3********************

        delete path;

        currentPath = 3;

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

    void Butterfly::Dive(int type)
    {
        mEscort = type != 0;

        Enemy::Dive();
    }
}