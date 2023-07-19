#pragma once

#include <vector>
#include <GameEntity.h>
#include <Texture.h>
#include <Timer.h>
#include <BezierPath.h>

using namespace SDLFramework;

namespace Galaga
{
    class Enemy : public GameEntity
    {
    public:
        enum States { FlyIn, Formation, Diving, Dead };

    protected:
        static std::vector<std::vector<Vector2>> sPaths;
        Timer* mTimer;
        Texture* mTexture;
        States mCurrentState;
        unsigned mCurrentPath;
        unsigned mCurrentWaypoint;

        const float EPSILON = 5.0f;
        float mSpeed;

        virtual void HandleFlyInState();
        virtual void HandleFormationState();
        virtual void HandleDiveState();
        virtual void HandleDeadState();

        void HandleStates();

        virtual void RenderFlyInState();
        virtual void RenderFormationState();
        virtual void RenderDivingState();
        virtual void RenderDeadState();

        void RenderStates();

    public:
        static void CreatePaths();
        Enemy(int path);

        virtual ~Enemy();
        States CurrentState();

        void Update() override;
        void Render() override;
    };

    std::vector<std::vector<Vector2>> Enemy::sPaths;

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

    void Enemy::HandleFormationState() { }

    void Enemy::HandleDiveState() { }

    void Enemy::HandleDeadState() { }

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

    void Enemy::RenderDivingState()
    { }

    void Enemy::RenderDeadState()
    { }

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
            RenderDivingState();
            break;

        case Dead:
            RenderDeadState();
            break;
        }
    }

    Enemy::Enemy(int path) : mCurrentPath(path)
    {
        mTimer = Timer::Instance();
        mCurrentState = FlyIn;

        mCurrentWaypoint = 1;

        Position(sPaths[mCurrentPath][0]);

        mTexture = new Texture("AnimatedEnemies.png", 0, 0, 52, 40);
        mTexture->Parent(this);
        mTexture->Position(Vec2_Zero);

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

        if (mCurrentWaypoint < sPaths[mCurrentPath].size())
        {
            Vector2 dist = sPaths[mCurrentPath][mCurrentWaypoint] - Position();

            Translate(dist.Normalized() * mSpeed * mTimer->DeltaTime(), World);
        }

        else
        {
            mCurrentState = Formation;
        }
    }
}