#pragma once

#include <vector>
#include <GameEntity.h>
#include <Texture.h>
#include <Timer.h>
#include <BezierPath.h>
#include <galaga/Formation.h>
#include <MathHelper.h>

using namespace SDLFramework;

typedef Galaga::Formation Swarm;

namespace Galaga
{
    class Enemy : public GameEntity
    {
    public:
        enum States { FlyIn, Formation, Diving, Dead };
        enum Types { Butterfly, Wasp, Boss };

        static void SetFormation(Swarm* formation);

    protected:
        static std::vector<std::vector<Vector2>> sPaths;
        static Swarm* sFormation;

        Timer* mTimer;
        Texture* mTexture;
        States mCurrentState;
        unsigned mCurrentPath;
        unsigned mCurrentWaypoint;

        int mIndex;
        Types mType;
        bool mChallengeStage;
        Vector2 mDiveStartPosition;

        const float EPSILON = 5.0f;
        float mSpeed;

        virtual void HandleFlyInState();
        virtual void HandleFormationState();

        void HandleStates();

        virtual void RenderFlyInState();
        virtual void RenderFormationState();

        virtual void PathComplete();
        virtual void FlyInComplete();
        void JoinFormation();
        virtual Vector2 WorldFormationPosition();

        virtual Vector2 LocalFormationPosition() = 0;
        virtual void HandleDiveState() = 0;
        virtual void HandleDeadState() = 0;
        virtual void RenderDiveState() = 0;
        virtual void RenderDeadState() = 0;

        void RenderStates();

    public:
        static void CreatePaths();
        Enemy(int path, int index, bool challenge);

        virtual ~Enemy();
        States CurrentState();

        void Update() override;
        void Render() override;

        Types Type();
        int Index();

        virtual void Dive(int type = 0);
    };

    std::vector<std::vector<Vector2>> Enemy::sPaths;
    Swarm* Enemy::sFormation = nullptr;

    void Enemy::SetFormation(Swarm* formation)
    {
        sFormation = formation;
    }

    void Enemy::CreatePaths()
    {
        int screenMidPoint = (int)(Graphics::Instance()->SCREEN_WIDTH * 0.4f);
        int currentPath = 0;

        BezierPath* path = new BezierPath();

        /* path->AddCurve(
             {
                 Vector2(500.0f, 10.0f),
                 Vector2(500.0f, 0.0f),
                 Vector2(500.0f, 310.0f),
                 Vector2(500.0f, 300.0f)
             }, 1);*/

        path->AddCurve(
            {
                Vector2(screenMidPoint + 50.0f, -10.0f),
                Vector2(screenMidPoint + 50.0f, -20.0f),
                Vector2(screenMidPoint + 50.0f, 30.0f),
                Vector2(screenMidPoint + 50.0f, 20.0f) }, 1);

        path->AddCurve(
            {
                Vector2(screenMidPoint + 50.0f, 20.0f),
                Vector2(screenMidPoint + 50.0f, 100.0f),
            Vector2(75.0f, 325.0f),
            Vector2(75.0f, 425.0f)
            }, 25);

        path->AddCurve(
            {
                Vector2(75.0f, 425.0f),
                Vector2(75.0f, 650.0f),
                Vector2(350, 650.0f),
                Vector2(350, 425.0f)
            }, 25);

        sPaths.push_back(std::vector<Vector2>());
        path->Sample(&sPaths[currentPath]);

        delete path;
    }

    void Enemy::HandleStates()
    {
        switch (mCurrentState)

        {
        case FlyIn:
            HandleFlyInState();
            break;

        case Formation:
            HandleFormationState();
            break;

        case Diving:
            HandleDiveState();
            break;

        case Dead:
            HandleDeadState();
            break;
        }
    }

    void Enemy::RenderFlyInState()
    {
        mTexture->Render();

        for (unsigned i = 0; i < sPaths[mCurrentPath].size() - 1; ++i)
        {
            Graphics::Instance()->DrawLine(
                sPaths[mCurrentPath][i].x,
                sPaths[mCurrentPath][i].y,
                sPaths[mCurrentPath][i + 1].x,
                sPaths[mCurrentPath][i + 1].y);
        }
    }

    void Enemy::RenderFormationState()
    {
        mTexture->Render();

        for (unsigned i = 0; i < sPaths[mCurrentPath].size() - 1; ++i)
        {
            Graphics::Instance()->DrawLine(
                sPaths[mCurrentPath][i].x,
                sPaths[mCurrentPath][i].y,
                sPaths[mCurrentPath][i + 1].x,
                sPaths[mCurrentPath][i + 1].y);
        }
    }

    void Enemy::RenderStates()
    {
        switch (mCurrentState)
        {
        case FlyIn:
            RenderFlyInState();
            break;

        case Formation:
            RenderFormationState();
            break;

        case Diving:
            RenderDiveState();
            break;

        case Dead:
            RenderDeadState();
            break;
        }
    }

    Enemy::Enemy(int path, int index, bool challenge) :
        mCurrentPath(path),
        mIndex(index),
        mChallengeStage(challenge)
    {
        mTimer = Timer::Instance();
        mCurrentState = FlyIn;

        mCurrentWaypoint = 1;

        Position(sPaths[mCurrentPath][0]);

        /*mTexture = new Texture("AnimatedEnemies.png", 0, 0, 52, 40);
        mTexture->Parent(this);
        mTexture->Position(Vec2_Zero);*/

        mTexture = nullptr;

        mSpeed = 20;
    }

    Enemy::~Enemy()
    {
        mTimer = nullptr;

        delete mTexture;
        mTexture = nullptr;
    }

    Enemy::States Enemy::CurrentState()
    {
        return mCurrentState;
    }

    void Enemy::Update()
    {
        if (Active())
        {
            HandleStates();
        }
    }

    void Enemy::Render()
    {
        if (Active())
        {
            mTexture->Render();
        }
    }

    void Enemy::HandleFlyInState()
    {
        if ((sPaths[mCurrentPath][mCurrentWaypoint] - Position())
            .MagnitudeSqr() < EPSILON * mSpeed / 25.0f)
        {
            mCurrentWaypoint++;
        }

        std::cout << "mCurrentWaypoint = " << mCurrentWaypoint
            << "and sPaths[mCurrentPath].size() = " << sPaths[mCurrentPath].size() << std::endl;

        if (mCurrentWaypoint >= sPaths[mCurrentPath].size())
        {
            std::cout << "Path complete" << std::endl;
            PathComplete();

            Vector2 dist = WorldFormationPosition() - Position();

            Translate(dist.Normalized() * mSpeed * mTimer->DeltaTime(), World);

            Rotation(atan2(dist.y, dist.x) * RAD_TO_DEG + 90.0f);

            FlyInComplete();
        }
        else
        {
            Vector2 dist = sPaths[mCurrentPath][mCurrentWaypoint] - Position();

            Translate(dist.Normalized() * mSpeed * mTimer->DeltaTime(), World);
        }
    }

    void Enemy::JoinFormation()
    {
        Position(WorldFormationPosition());
        Rotation(0);
        Parent(sFormation);

        mCurrentState = Formation;
    }

    Vector2 Enemy::WorldFormationPosition()
    {
        return sFormation->Position() + LocalFormationPosition();
    }

    void Enemy::FlyInComplete()
    {
        if (mChallengeStage)
        {
            mCurrentState = Dead;
        }
        else
        {
            JoinFormation();
        }
    }

    void Enemy::PathComplete()
    {
        if (mChallengeStage)
        {
            mCurrentState = Dead;
        }
    }

    Enemy::Types Enemy::Type()
    {
        return mType;
    }

    int Enemy::Index()
    {
        return mIndex;
    }

    void Enemy::HandleFormationState()
    {
        Position(LocalFormationPosition());
    }

    void Enemy::Dive(int type)
    {
        Parent(nullptr); // breaks away from formation, see join formation method

        mCurrentState = Diving;
        mDiveStartPosition = Position();
        mCurrentWaypoint = 1;
    }
}