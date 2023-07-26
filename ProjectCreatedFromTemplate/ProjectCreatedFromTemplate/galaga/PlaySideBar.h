#pragma once

#include <Timer.h>
#include <galaga/Scoreboard.h>
#include <managers/AudioManager.h> 

namespace Galaga
{
    class PlaySideBar : public GameEntity
    {
    private:
        Timer* mTimer;
        AudioManager* mAudio;
        Texture* mBackground;

        Texture* mHighLabel;
        Texture* mScoreLabel;
        Scoreboard* mHighScoreboard;

        Texture* mPlayerOneLabel;
        float mBlinkTimer;
        float mBlinkInterval;
        bool mPlayerOneLabelVisible;
        Scoreboard* mPlayerOneScore;

        static const int MAX_SHIP_TEXTURES = 5;
        GameEntity* mShips;
        Texture* mShipTextures[MAX_SHIP_TEXTURES];
        Scoreboard* mTotalShipsLabel; int mTotalShips;

        GameEntity* mFlags;
        std::vector<Texture*> mFlagTextures;
        int mRemainingLevels;
        float mFlagXOffset;
        float mFlagYOffset;
        float mFlagTimer;
        float mFlagInterval;

        void ClearFlags();
        void AddNextFlag();
        void AddFlag(std::string filename, float width, int value);

    public:
        PlaySideBar();
        ~PlaySideBar();

        void SetHighScore(int score);
        void SetPlayerScore(int score);
        void SetShips(int ships);
        void SetLevel(int level);

        void Update() override;
        void Render() override;
    };

    PlaySideBar::PlaySideBar()
    {
        mTimer = Timer::Instance();
        mAudio = AudioManager::Instance();

        mBackground = new Texture("Black.png");
        mBackground->Parent(this);
        mBackground->Scale(Vector2(3.0f, 10.0f));
        mBackground->Position(45.0f, 0);

        mHighLabel = new Texture("HIGH", "emulogic.ttf", 16, { 150, 0, 0 });
        mHighLabel->Parent(this);
        mHighLabel->Position(-25.0f, 0.0f);

        mScoreLabel = new Texture("SCORE", "emulogic.ttf", 16, { 150, 0, 0 });
        mScoreLabel->Parent(this);
        mScoreLabel->Position(25.0f, 32.0f);

        mHighScoreboard = new Scoreboard();
        mHighScoreboard->Parent(this);

        //**************************************************
        mPlayerOneLabel = new Texture("1UP", "emulogic.ttf", 32, { 150, 0, 0 });
        mPlayerOneLabel->Parent(this);
        mPlayerOneLabel->Position(-45.0f, 160.0f);
        mBlinkTimer = 0.0f;
        mBlinkInterval = 0.5f;
        mPlayerOneLabelVisible = true;

        mPlayerOneScore = new Scoreboard();
        mPlayerOneScore->Parent(this);
        mPlayerOneScore->Position(90.0f, 192.0f);

        mShips = new GameEntity();
        mShips->Parent(this);
        mShips->Position(-40.0f, 420.0f);

        for (int i = 0; i < MAX_SHIP_TEXTURES; ++i)
        {
            mShipTextures[i] = new Texture("PlayerShips.png", 0, 0, 60, 64);
            mShipTextures[i]->Parent(mShips);
            mShipTextures[i]->Position(62.0f * (i % 3), 70.0f * (i / 3));
        }

        mTotalShipsLabel = new Scoreboard();
        mTotalShipsLabel->Parent(mShips);
        mTotalShipsLabel->Position(140.0f, 80.0f);

        mFlags = new GameEntity();
        mFlags->Parent(this);
        mFlags->Position(-50.0f, 0);

        mFlagTimer = 0.0f;
        mFlagInterval = 0.25f;
    }

    PlaySideBar::~PlaySideBar()
    {
        mTimer = nullptr;
        mAudio = nullptr;

        delete mBackground;
        mBackground = nullptr;

        delete mHighLabel;
        mHighLabel = nullptr;

        delete mScoreLabel;
        mScoreLabel = nullptr;

        delete mHighScoreboard;
        mHighScoreboard = nullptr;

        delete mPlayerOneLabel;
        mPlayerOneLabel = nullptr;

        delete mPlayerOneScore;
        mPlayerOneScore = nullptr;

        delete mShips; mShips = nullptr;
        for (int i = 0; i < MAX_SHIP_TEXTURES; ++i)
        {
            delete mShipTextures[i];
            mShipTextures[i] = nullptr;
        }

        delete mTotalShipsLabel;
        mTotalShipsLabel = nullptr;

        delete mFlags;
        mFlags = nullptr;

        ClearFlags();
    }

    void PlaySideBar::Update()
    {
        mBlinkTimer += mTimer->DeltaTime();

        if (mBlinkTimer >= mBlinkInterval)
        {
            mPlayerOneLabelVisible = !mPlayerOneLabelVisible;
            mBlinkTimer = 0.0f;
        }

        if (mRemainingLevels > 0)
        {
            mFlagTimer += mTimer->DeltaTime();
            if (mFlagTimer >= mFlagInterval)
            {
                AddNextFlag();
                mFlagTimer = 0.0f;
            }
        }
    }

    void PlaySideBar::Render()
    {
        mBackground->Render();

        mBackground->Render();
        mHighLabel->Render();
        mScoreLabel->Render();
        mHighScoreboard->Render();

        if (mPlayerOneLabelVisible)
        {
            mPlayerOneLabel->Render();
        }
        mPlayerOneScore->Render();

        for (int i = 0; i < MAX_SHIP_TEXTURES && i < mTotalShips; ++i)
        {
            mShipTextures[i]->Render();
        }

        if (mTotalShips > MAX_SHIP_TEXTURES)
        {
            mTotalShipsLabel->Render();
        }

        for (auto flag : mFlagTextures)
        {
            flag->Render();
        }
    }

    void PlaySideBar::SetHighScore(int score)
    {
        mHighScoreboard->Score(score);
    }

    void PlaySideBar::SetPlayerScore(int score)
    {
        mPlayerOneScore->Score(score);
    }

    void PlaySideBar::SetShips(int ships)
    {
        mTotalShips = ships;
        if (ships > MAX_SHIP_TEXTURES)
        {
            mTotalShipsLabel->Score(ships);
        }
    }

    void PlaySideBar::ClearFlags()
    {
        for (unsigned i = 0; i < mFlagTextures.size(); ++i)
        {
            delete mFlagTextures[i];
            mFlagTextures[i] = nullptr;
        }
        mFlagTextures.clear();
        mFlagYOffset = 0;
    }

    void PlaySideBar::AddNextFlag()
    {
        if (mRemainingLevels >= 50)
        {
            AddFlag("LevelFlags.png", 62, 50);
        }
        else if (mRemainingLevels >= 30)
        {
            AddFlag("LevelFlags.png", 62, 30);
        }
        else if (mRemainingLevels >= 20)
        {
            AddFlag("LevelFlags.png", 62, 20);
        }
        else if (mRemainingLevels >= 10)
        {
            AddFlag("LevelFlags.png", 54, 10);
        }
        else if (mRemainingLevels >= 5)
        {
            AddFlag("LevelFlags.png", 30, 5);
        }
        else
        {
            AddFlag("LevelFlags.png", 30, 1);
        }
    }

    void PlaySideBar::AddFlag(std::string filename, float width, int value)
    {
        int index = (int)mFlagTextures.size();

        if (index > 0)
        {
            mFlagXOffset += width * 0.5f;
        }
        
        if(mFlagXOffset > 140)
        {
            mFlagYOffset += 66;
            mFlagXOffset = 0;
        }

        mRemainingLevels -= value;
        int x = 0;

        switch (value)
        {
        case 50:
            x = 228;
            break;

        case 30:
            x = 168;
            break;

        case 20:
            x = 108;
            break;

        case 10:
            x = 56;
            break;

        case 5:
            x = 28;
            break;
        }

        mFlagTextures.push_back(new Texture(filename, x, 0, (int)width - 2, 64));
        mFlagTextures[index]->Parent(mFlags);
        mFlagTextures[index]->Position(mFlagXOffset, mFlagYOffset);

        mFlagXOffset += width * 0.50f;

        mAudio->PlaySFX("SFX/FlagSound.wav", 0, -1);
    }

    void PlaySideBar::SetLevel(int level)
    {
        ClearFlags();
        mRemainingLevels = level;
        mFlagXOffset = 0.0f;
    }
}