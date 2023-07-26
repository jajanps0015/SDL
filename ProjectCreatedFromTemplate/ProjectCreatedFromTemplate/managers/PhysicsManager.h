#pragma once

#include <bitset>
#include <vector>
#include <CircleCollider.h>
#include <PhysicsHelper.h>
#include <GameEntity.h>

namespace SDLFramework
{
    class PhysEntity : public GameEntity
    {
    protected:
        unsigned long mId;
        std::vector<Collider*> mColliders;

        Collider* mBroadPhaseCollider;

        void AddCollider(Collider* collider, Vector2 localPos = Vec2_Zero);

        virtual bool IgnoreCollisions();

    public:
        PhysEntity();

        ~PhysEntity();

        unsigned long GetId();

        bool CheckCollision(PhysEntity* other);

        virtual void Hit(PhysEntity* other);

        virtual void Render() override;
    };

    void PhysEntity::AddCollider(Collider* collider, Vector2 localPos)
    {
        collider->Parent(this);
        collider->Position(localPos);

        mColliders.push_back(collider);

        if (mColliders.size() > 1 || mColliders[0]->GetType() != Collider::ColliderType::Circle)
        {
            float furthestDistance = mColliders[0]->GetFurthestPoint().Magnitude();
            float dist = 0.0f;

            for (int i = 1; i < mColliders.size(); i++)
            {
                dist = mColliders[i]->GetFurthestPoint().Magnitude();

                if (dist > furthestDistance)
                {
                    furthestDistance = dist;
                }
            }

            delete mBroadPhaseCollider; mBroadPhaseCollider = new CircleCollider(furthestDistance, true);
            mBroadPhaseCollider->Parent(this);
            mBroadPhaseCollider->Position(Vec2_Zero);
        };
    }

    bool PhysEntity::IgnoreCollisions()
    {
        return false;
    }

    PhysEntity::PhysEntity()
    {
        mBroadPhaseCollider = nullptr;
        mId = 0;
    }

    unsigned long PhysEntity::GetId()
    {
        return mId;
    }

    bool PhysEntity::CheckCollision(PhysEntity* other)
    {
        if (IgnoreCollisions() || other->IgnoreCollisions())
        {
            return false;
        }

        bool narrowPhaseCheck = false;
        if (mBroadPhaseCollider && other->mBroadPhaseCollider)
        {
            narrowPhaseCheck = ColliderVsColliderCheck(mBroadPhaseCollider, other->mBroadPhaseCollider);
        }
        else
        {
            narrowPhaseCheck = true;
        }

        if (narrowPhaseCheck)
        {
            for (int i = 0; i < mColliders.size(); i++)
            {
                for (int j = 0; j < other->mColliders.size(); j++)
                {
                    if (ColliderVsColliderCheck(mColliders[i], other->mColliders[j]))
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    void PhysEntity::Render()
    {
        for (auto c : mColliders)
        {
            c->Render();
        }

        if (mBroadPhaseCollider)
        {
            mBroadPhaseCollider->Render();
        }
    }

    void PhysEntity::Hit(PhysEntity* other)
    {

    }
}

namespace SDLFramework
{
    class PhysicsManager
    {
    public:
        static PhysicsManager* Instance();

        static void Release();

        enum class CollisionLayers
        {
            Friendly,
            FriendlyProjectiles,
            Hostile,
            HostileProjectiles,
            MaxLayers
        };

        enum class CollisionFlags
        {
            None = 0x00,
            Friendly = 0x01,
            FriendlyProjectiles = 0x02,
            Hostile = 0x04,
            HostileProjectiles = 0x08
        };

        void SetLayerCollisionMask(CollisionLayers layer, CollisionFlags flags);

        unsigned long RegisterEntity(PhysEntity* entity, CollisionLayers layer);

        void UnregisterEntity(unsigned long id);

        void Update();
    private:
        static PhysicsManager* sInstance;

        PhysicsManager();

        ~PhysicsManager();

        std::vector<PhysEntity*> mCollisionLayers[static_cast<unsigned int>(CollisionLayers::MaxLayers)];

        std::bitset<static_cast<unsigned int>(CollisionLayers::MaxLayers)>
            mLayerMasks[static_cast<unsigned int>(CollisionLayers::MaxLayers)];

        unsigned long mLastId;
    };

    inline PhysicsManager::CollisionFlags operator|(
        PhysicsManager::CollisionFlags a,
        PhysicsManager::CollisionFlags b)
    {
        return static_cast<PhysicsManager::CollisionFlags>
            (
                static_cast<unsigned int>(a)
                | static_cast<unsigned int>(b));
    }

    inline PhysicsManager::CollisionFlags operator&(
        PhysicsManager::CollisionFlags a,
        PhysicsManager::CollisionFlags b)
    {
        return static_cast<PhysicsManager::CollisionFlags>
            (
                static_cast<unsigned int>(a)
                & static_cast<unsigned int>(b));
    }

    PhysicsManager* PhysicsManager::sInstance = nullptr;

    PhysicsManager* PhysicsManager::Instance()
    {
        if (sInstance == nullptr)
        {
            sInstance = new PhysicsManager();
        }

        return sInstance;
    }



    PhysEntity::~PhysEntity()
    {
        if (mId != 0)
        {
            PhysicsManager::Instance()->UnregisterEntity(mId);
        }
        for (auto c : mColliders)
        {
            delete c;
        }

        mColliders.clear();
        delete mBroadPhaseCollider;
    }

    void PhysicsManager::Release()
    {
        delete sInstance;
        sInstance = nullptr;
    }

    PhysicsManager::PhysicsManager()
    {
        mLastId = 0;

        for (unsigned int i = 0; i < static_cast<unsigned int>(CollisionLayers::MaxLayers); i++)
        {
            mLayerMasks[i] = std::bitset<static_cast<unsigned int>(CollisionLayers::MaxLayers)>
                (static_cast<unsigned int>(CollisionFlags::None));
        }
    }
    PhysicsManager::~PhysicsManager()
    {
        for (auto l : mCollisionLayers)
        {
            l.clear();
        }
    }

    void PhysicsManager::SetLayerCollisionMask(CollisionLayers layer, CollisionFlags flags)
    {
        mLayerMasks[static_cast<unsigned int>(layer)] =
            std::bitset<static_cast<unsigned int>(CollisionLayers::MaxLayers)>
            (static_cast<unsigned int>(flags));
    }

    unsigned long PhysicsManager::RegisterEntity(PhysEntity* entity, CollisionLayers layer)
    {
        mCollisionLayers[static_cast<unsigned int>(layer)].push_back(entity);
        mLastId++;
        return mLastId;
    }

    void PhysicsManager::UnregisterEntity(unsigned long id)
    {
        bool found = false;

        for (int i = 0; i < static_cast<unsigned int>(CollisionLayers::MaxLayers) && !found; i++)
        {
            for (int j = 0; j < mCollisionLayers[i].size() && !found; j++)
            {
                if (mCollisionLayers[i][j]->GetId() == id)
                {
                    mCollisionLayers[i].erase(mCollisionLayers[i].begin() + j);
                    found = true;
                }
            }
        }
    }

    void PhysicsManager::Update() 
    { 
        for (unsigned int i = 0; i < static_cast<unsigned int>(CollisionLayers::MaxLayers); i++) 
        { 
            for (unsigned int j = 0; j < static_cast<unsigned int>(CollisionLayers::MaxLayers); j++) 
            { 
                if (!mLayerMasks[i].test(j) || i > j) 
                { 
                    continue;
                }

                for (unsigned int k = 0; k < mCollisionLayers[i].size(); k++)
                {
                    for (unsigned int l = 0; l < mCollisionLayers[j].size(); l++)
                    {
                        if (mCollisionLayers[i][k]->CheckCollision(mCollisionLayers[j][l]))
                        {
                            mCollisionLayers[i][k]->Hit(mCollisionLayers[j][l]);
                            mCollisionLayers[j][l]->Hit(mCollisionLayers[i][k]);
                        }
                    }
                }
            } 
        } 
    }
}