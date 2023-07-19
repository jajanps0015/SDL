#pragma once

#include <GameEntity.h>
#include <Timer.h>
#include <Texture.h>

using namespace SDLFramework;

namespace Galaga
{
    class Bullet : public GameEntity
    {
    private:
        static const int OFFSCREEN_BUFFER = 10;
        Timer* mTimer;
        Texture* mTexture;
        float mSpeed;
    public:
        Bullet();
        ~Bullet();

        void Fire(Vector2 pos);
        void Reload();

        void Update() override;
        void Render() override;
    };

    Bullet::Bullet()
    {
        mTimer = Timer::Instance();

        mTexture = new Texture("Bullet.png");
        mTexture->Parent(this);
        mTexture->Position(Vec2_Zero);
        mSpeed = 200;

        Reload();
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
}