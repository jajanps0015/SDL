#pragma once

#include <vector>
#include <managers/PhysicsManager.h>
#include <Texture.h>
#include <Timer.h>
#include <BezierPath.h>
#include <galaga/Formation.h>
#include <MathHelper.h>

using namespace SDLFramework;

typedef Galaga::Formation Swarm;

namespace Galaga
{
    class Enemy : public PhysEntity
    {
    public:
        enum States { FlyIn, Formation, Diving, Dead };
        enum Types { Butterfly, Wasp, Boss };

        static void SetFormation(Swarm* formation);
        bool InDeathAnimation();

    protected:

        static Player* sPlayer;

        static std::vector<std::vector<Vector2>> sPaths;
        static Swarm* sFormation;

        Timer* mTimer;
        
        Texture* mTexture;
        Texture* mTextures[2];

        States mCurrentState;
        unsigned mCurrentPath;
        unsigned mCurrentWaypoint;

        int mIndex;
        Types mType;
        bool mChallengeStage;
        Vector2 mDiveStartPosition; 
        
        AnimatedTexture* mDeathAnimation;

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

        bool IgnoreCollisions() override;

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

        static void CurrentPlayer(Player* player);

        virtual void Hit(PhysEntity* other) override;
    };

    Player* Enemy::sPlayer = nullptr;

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

        //**********************************************************

        currentPath = 1; 
        
        path = new BezierPath(); 
        
        int fullScreen = screenMidPoint * 2; 
        
        path->AddCurve({ Vector2(screenMidPoint - 50.0f, -10.0f), Vector2(screenMidPoint - 50.0f, -20.0f), Vector2(screenMidPoint - 50.0f, 30.0f), Vector2(screenMidPoint - 50.0f, 20.0f) }, 1); 
        path->AddCurve({ Vector2(screenMidPoint - 50.0f, 20.0f), Vector2(screenMidPoint - 50.0f, 100.0f), Vector2(fullScreen - 75.0f, 325.0f), Vector2(fullScreen - 75.0f, 425.0f) }, 25); 
        path->AddCurve({ Vector2(fullScreen - 75.0f, 425.0f), Vector2(fullScreen - 75.0f, 650.0f), Vector2(fullScreen - 350.0f, 650.0f), Vector2(fullScreen - 350.0f, 425.0f) }, 25); 
        
        sPaths.push_back(std::vector<Vector2>()); 
        path->Sample(&sPaths[currentPath]); 
        delete path;

        //**********************************************************

        currentPath = 2; 
        float temp = screenMidPoint - 100.0f; 
        
        path = new BezierPath(); 
        
        path->AddCurve({ Vector2(-40.0f, 720.0f), Vector2(-50.0f, 720.0f), Vector2(10.0f, 720.0f), Vector2(0.0f, 720.0f) }, 1); 
        path->AddCurve({ Vector2(0.0f, 720.0f), Vector2(200.0f, 720.0f), Vector2(temp, 500.0f), Vector2(temp, 400.0f) }, 15); 
        path->AddCurve({ Vector2(temp, 400.0f), Vector2(temp, 200.0f), Vector2(40.0f, 200.0f), Vector2(40.0f, 400.0f) }, 15); 
        path->AddCurve({ Vector2(40.0f, 400.0f), Vector2(40.0f, 500.0f), Vector2(temp - 120.0f, 600.0f), Vector2(temp - 40.0f, 440.0f) }, 15); 
        
        sPaths.push_back(std::vector<Vector2>()); 
        path->Sample(&sPaths[currentPath]); 
        
        delete path; 

        //**********************************************************

        currentPath = 3; 
        temp = screenMidPoint + 60.0f; 
        float temp2 = fullScreen - 40.0f; 
        
        path = new BezierPath(); path->AddCurve({ Vector2(temp2 + 40.0f, 720.0f), Vector2(temp2 + 50.0f, 720.0f), Vector2(temp2 + 10.0f, 720.0f), Vector2(temp2, 720.0f) }, 1); 
        path->AddCurve({ Vector2(temp2, 720.0f), Vector2(temp2 - 200.0f, 720.0f), Vector2(temp, 500.0f), Vector2(temp, 400.0f) }, 15);
        path->AddCurve({ Vector2(temp, 400.0f), Vector2(temp, 200.0f), Vector2(temp2 - 40.0f, 200.0f), Vector2(temp2 - 40.0f, 400.0f) }, 15); 
        path->AddCurve({ Vector2(temp2 - 40.0f, 400.0f), Vector2(temp2 - 40.0f, 500.0f), Vector2(temp + 120.0f, 600.0f), Vector2(temp + 40.0f, 440.0f) }, 15); 
        
        sPaths.push_back(std::vector<Vector2>()); 
        path->Sample(&sPaths[currentPath]); delete path;

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
        mTextures[sFormation->GetTick() % 2]->Render();

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

        PhysEntity::Render();
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

        mTexture = nullptr; 
        mTextures[0] = nullptr; 
        mTextures[1] = nullptr;

        mSpeed = 20;

        mId = PhysicsManager::Instance()->RegisterEntity(this,
            PhysicsManager::CollisionLayers::Hostile);
        mDeathAnimation =
            new AnimatedTexture("EnemyExplosion.png", 0, 0, 128, 128, 5, 1.0f,
                Horizontal);
        mDeathAnimation->Parent(this);
        mDeathAnimation->Position(Vec2_Zero);
        mDeathAnimation->SetWrapMode(Once);

    }

    Enemy::~Enemy()
    {
        mTimer = nullptr;

        delete mTexture;
        mTexture = nullptr;

        for (int i = 0; i < 2; i++) 
        { 
            delete mTextures[i]; 
            mTextures[i] = nullptr; 
        }
        delete mDeathAnimation;
        mDeathAnimation = nullptr;
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
            RenderStates();
        }
    }

    void Enemy::HandleFlyInState()
    {
        if (mCurrentWaypoint < sPaths[mCurrentPath].size()) {
            Vector2 dist = sPaths[mCurrentPath][mCurrentWaypoint] - Position();

            Translate(dist.Normalized() * mSpeed * mTimer->DeltaTime(), World);
            Rotation(atan2(dist.y, dist.x) * RAD_TO_DEG + 90.0f);

            if ((sPaths[mCurrentPath][mCurrentWaypoint] - Position()).MagnitudeSqr() < EPSILON * mSpeed / 25.0f) {
                mCurrentWaypoint++;
            }

            if (mCurrentWaypoint >= sPaths[mCurrentPath].size()) {
                PathComplete();
            }
        }
        else {
            Vector2 dist = WorldFormationPosition() - Position();

            Translate(dist.Normalized() * mSpeed * mTimer->DeltaTime(), World);
            Rotation(atan2(dist.y, dist.x) * RAD_TO_DEG + 90.0f);

            if (dist.MagnitudeSqr() < EPSILON * mSpeed / 25.0f) {
                FlyInComplete();
            }
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

        float rotation = Rotation(); 
        
        if (rotation != 0.0f) 
        { 
            // epsilon for rotation 
            if (rotation > 5.0f) 
            {
                float rotationSpeed = 200.0f; 
                float rotationDir = (rotation >= 180.0f) ? 1.0f : -1.0f; 
                Rotate(rotationDir * mTimer->DeltaTime() * rotationSpeed);
            } 
            else 
            { 
                Rotation(0.0f); 
            } 
        }
    }

    void Enemy::Dive(int type)
    {
        Parent(nullptr); // breaks away from formation, see join formation method

        mCurrentState = Diving;
        mDiveStartPosition = Position();
        mCurrentWaypoint = 1;
    }

    void Enemy::CurrentPlayer(Player* player) {
        sPlayer = player;
    }

    bool Enemy::InDeathAnimation() {
        return mDeathAnimation->IsAnimating();
    }

    bool Enemy::IgnoreCollisions() {
        return mCurrentState == Dead;
    }

    void Enemy::Hit(PhysEntity* other) {
        if (mCurrentState == Formation) {
            Parent(nullptr);
        }
        mCurrentState = Dead;
    }

}