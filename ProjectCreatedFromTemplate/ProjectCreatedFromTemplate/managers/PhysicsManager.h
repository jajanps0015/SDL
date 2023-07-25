#pragma once

#include <bitset> 
#include <PhysEntity.h>

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
                if (mLayerMasks[i].test(j) && i <= j) 
                { 
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
}