#pragma once
#include <managers/PhysicsManager.h>
#include <Timer.h>
#include <managers/AudioManager.h>
#include <managers/InputManager.h>
#include <AnimatedTexture.h>
#include <galaga/Bullet.h>
#include <managers/PhysicsManager.h>

using namespace SDLFramework;

namespace Galaga
{
    class Player : public PhysEntity
    {
    private:

        Timer* mTimer;
        InputManager* mInput;
        AudioManager* mAudio;

        bool mVisible;
        bool mAnimating;
        int mScore;
        int mLives;
        Texture* mShip;
        AnimatedTexture* mDeathAnimation;
        float mMoveSpeed;
        Vector2 mMoveBounds;

        static const int MAX_BULLETS = 2;
        Bullet* mBullets[MAX_BULLETS];

        bool mWasHit;

        void HandleMovement();
        void HandleFiring();

    public:
        Player();
        ~Player();

        void Update() override;
        void Render() override;

        void Visible(bool visible);
        bool IsAnimating();
        int Score();
        int Lives();
        void AddScore(int change);
        bool WasHit(); 
        
        // Inherited from PhysEntity 
        bool IgnoreCollisions() override; 
        void Hit(PhysEntity * other) override;
    };

    Player::Player()
    {
        mTimer = Timer::Instance();
        mInput = InputManager::Instance();
        mAudio = AudioManager::Instance();

        mVisible = false;
        mAnimating = false;
        mScore = 0;
        mLives = 2;

        mShip = new Texture("PlayerShips.png", 0, 0, 60, 64);
        mShip->Parent(this);
        mShip->Position(Vec2_Zero);

        mMoveSpeed = 100.0f;
        mMoveBounds = Vector2(0.0f, 800.0f);

        mDeathAnimation = new AnimatedTexture("PlayerExplosion.png", 0, 0, 128, 128, 4, 3.0f, AnimDir::Horizontal);

        mDeathAnimation->Parent(this);
        mDeathAnimation->Position(Vec2_Zero);
        mDeathAnimation->SetWrapMode(WrapMode::Once);

        for (int i = 0; i < MAX_BULLETS; ++i)
        {
            mBullets[i] = new Bullet(true);
        }

        mWasHit = false; 
        AddCollider(new BoxCollider(Vector2(16.0f, 67.0f))); 
        AddCollider(new BoxCollider(Vector2(20.0f, 37.0f)), Vector2(18.0f, 10.0f)); 
        AddCollider(new BoxCollider(Vector2(20.0f, 37.0f)), Vector2(-18.0f, 10.0f)); 
        
        mId = PhysicsManager::Instance()->RegisterEntity
        (this, PhysicsManager::CollisionLayers::Friendly);
    }

    Player::~Player()
    {
        mTimer = nullptr;
        mInput = nullptr;
        mAudio = nullptr;

        delete mShip;
        mShip = nullptr;

        delete mDeathAnimation;
        mDeathAnimation = nullptr;

        for (int i = 0; i < MAX_BULLETS; ++i)
        {
            delete mBullets[i];

            mBullets[i] = nullptr;
        }
    }

    void Player::Visible(bool visible)
    {
        mVisible = visible;
    }

    bool Player::IsAnimating()
    {
        return mAnimating;
    }

    int Player::Score()
    {
        return mScore;
    }

    int Player::Lives()
    {
        return mLives;
    }

    void Player::AddScore(int change)
    {
        mScore += change;
    }

    void Player::HandleMovement()
    {
        if (mInput->KeyDown(SDL_SCANCODE_RIGHT))
        {
            Translate(Vec2_Right * mMoveSpeed * mTimer->DeltaTime(), World);
        }
        else if (mInput->KeyDown(SDL_SCANCODE_LEFT))
        {
            Translate(Vec2_Left * mMoveSpeed * mTimer->DeltaTime(), World);
        }

        Vector2 pos = Position(Local);

        if (pos.x < mMoveBounds.x + mShip->ScaledDimensions().x * 0.5f)
        {
            pos.x = mMoveBounds.x + mShip->ScaledDimensions().x * 0.5f;
        }
        else if (pos.x > mMoveBounds.y - mShip->ScaledDimensions().x * 0.5f)
        {
            pos.x = mMoveBounds.y - mShip->ScaledDimensions().x * 0.5f;
        }

        Position(pos);
    }

    void Player::HandleFiring()
    {
        if (mInput->KeyPressed(SDL_SCANCODE_SPACE))
        {
            for (int i = 0; i < MAX_BULLETS; ++i)
            {
                if (!mBullets[i]->Active())
                {
                    mBullets[i]->Fire(Position());

                    mAudio->PlaySFX("SFX/Fire.wav", 0, -1);
                    break;
                }
            }
        }
    }

    void Player::Update()
    {
        if (mAnimating)
        {
            mDeathAnimation->Update();
            mAnimating = mDeathAnimation->IsAnimating();
        }
        else
        {
            if (Active())
            {
                HandleMovement();
                HandleFiring();
            }
        }

        for (int i = 0; i < MAX_BULLETS; ++i)
        {
            mBullets[i]->Update();
        }
    }

    void Player::Render()
    {
        if (mVisible)
        {
            if (mAnimating)
            {
                mDeathAnimation->Render();
            }
            else
            {
                mShip->Render();
            }

            for (int i = 0; i < MAX_BULLETS; ++i)
            {
                mBullets[i]->Render();
            }

            PhysEntity::Render();
        }
    }
    
    bool Player::IgnoreCollisions() {
        return !mVisible || mAnimating;
    }

    void Player::Hit(PhysEntity* other) {
        mLives -= 1;
        mAnimating = true;
        mDeathAnimation->ResetAnimation();
        mAudio->PlaySFX("SFX/PlayerExplosion.wav");
        mWasHit = true;
    }

    bool Player::WasHit() 
    {
        return mWasHit;
    }

}