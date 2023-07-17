#pragma once
#include <Texture.h>
#include <Timer.h>
#include <Random.h>

using namespace SDLFramework;

namespace Galaga
{
    class Star : public Texture
    {
    private:
        static bool mScroll;
        Timer* mTimer;
        Random* mRand;
        bool mVisible;
        float mFlickerTimer;
        float mFlickerSpeed;
        float mScrollSpeed;

        void ScrollStar();

    public:
        Star(int layer);
        ~Star();

        void Update() override;
        void Render() override;

        static void Scroll(bool b);
    };

    bool Star::mScroll = false;

    void Star::Scroll(bool b)
    {
        mScroll = b;
    }

    Star::Star(int layer) : Texture("Stars.png", 0, 0, 4, 4)
    {
        mTimer = Timer::Instance();
        mRand = Random::Instance();

        int starColor = mRand->RandomInt() % 4;
        mSourceRect.x = starColor * 4;

        Position(Vector2(
            (float)(mRand->RandomInt() % Graphics::Instance()->SCREEN_WIDTH),
            (float)(mRand->RandomInt() % Graphics::Instance()->SCREEN_HEIGHT)
        ));

        mFlickerTimer = 0.0f;
        mFlickerSpeed = mRand->RandomRange(0.15f, 1.0f);

        float inverseLayer = 1.0f / layer;
        Scale(Vec2_One * inverseLayer);
        mScrollSpeed = 4.0f / layer;
    }

    Star::~Star()
    {
        mTimer = nullptr;
        mRand = nullptr;
    }

    void Star::ScrollStar()
    {
        Translate(Vec2_Up * mScrollSpeed, World);
        Vector2 pos = Position(Local);

        if (pos.y > Graphics::Instance()->SCREEN_HEIGHT)
        {
            pos.y = 0.0f;
            pos.x = (float)(mRand->RandomInt() % Graphics::Instance()->SCREEN_WIDTH);
            Position(pos);
        }
    }

    void Star::Update()
    {
        mFlickerTimer += mTimer->DeltaTime();
        if (mFlickerTimer >= mFlickerSpeed)
        {
            mVisible = !mVisible;
            mFlickerTimer = 0.0f;
        }

        if (mScroll)
        {
            ScrollStar();
        }
    }

    void Star::Render() 
    { 
        if (mVisible) 
        { 
            Texture::Render(); 
        } 
    }
}