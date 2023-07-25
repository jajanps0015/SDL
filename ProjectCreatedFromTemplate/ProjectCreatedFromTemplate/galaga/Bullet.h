#pragma once

#include <managers/PhysicsManager.h>
#include <Timer.h>
#include <Texture.h>

using namespace SDLFramework;

namespace Galaga
{
    class Bullet : public PhysEntity
    {
    private:
        static const int OFFSCREEN_BUFFER = 10;
        Timer* mTimer;
        Texture* mTexture;
        float mSpeed;

        bool IgnoreCollisions() override;

    public:
        Bullet(bool friendly);
        ~Bullet();

        void Fire(Vector2 pos);
        void Reload();

        void Update() override;
        void Render() override;
        void Hit(PhysEntity* other) override;
    };

    Bullet::Bullet(bool friendly)
    {
        mTimer = Timer::Instance();

        mTexture = new Texture("Bullet.png");
        mTexture->Parent(this);
        mTexture->Position(Vec2_Zero);
        mSpeed = 200;

        Reload();

        AddCollider(new BoxCollider(mTexture->ScaledDimensions()));
        if (friendly) {
            mId = PhysicsManager::Instance()->RegisterEntity(this,
                PhysicsManager::CollisionLayers::FriendlyProjectiles);
        }
        else {
            mId = PhysicsManager::Instance()->RegisterEntity(this,
                PhysicsManager::CollisionLayers::HostileProjectiles);
        }
    }

    Bullet::~Bullet()
    {
        mTimer = nullptr;

        delete mTexture;
        mTexture = nullptr;
    }

    void Bullet::Fire(Vector2 pos)
    {
        Position(pos);
        Active(true);
    }

    void Bullet::Reload()
    {
        Active(false);
    }

    void Bullet::Render()
    {
        if (Active())
        {
            mTexture->Render();
            PhysEntity::Render();
        }
    }

    void Bullet::Update()
    {
        if (Active())
        {
            Translate(Vec2_Down * mSpeed * mTimer->DeltaTime());
            Vector2 pos = Position(); 
            
            if (pos.y < -OFFSCREEN_BUFFER) 
            { 
                Reload(); 
            }
        }
    }

    bool Bullet::IgnoreCollisions() {
        return !Active();
    }
    
    void Bullet::Hit(PhysEntity* other) {
        Reload();
    }

}