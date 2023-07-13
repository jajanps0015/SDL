#pragma once

#include <SDLMain.h>
#include <MathHelper.h>
#include <cstring>

namespace SDLFramework
{
    class InputManager
    {
    private:
        static InputManager* sInstance;
        const Uint8* mKeyboardState;
        Uint8* mPrevKeyboardState;
        int mKeyLength;

        Uint32 mPrevMouseState;
        Uint32 mMouseState;
        int mMouseXPos;
        int mMouseYPos;

        InputManager();
        ~InputManager();

    public:
        enum MouseButton { Left = 0, Right, Middle, Back, Forward };
        static InputManager* Instance();
        static void Release();

        bool KeyDown(SDL_Scancode scancode);
        bool KeyPressed(SDL_Scancode scancode);
        bool KeyReleased(SDL_Scancode scancode);

        bool MouseButtonDown(MouseButton button);
        bool MouseButtonPressed(MouseButton button);
        bool MouseButtonReleased(MouseButton button);

        Vector2 MousePosition();

        void Update();
        void UpdatePrevInput();
    };

    InputManager* InputManager::sInstance = nullptr;

    InputManager* InputManager::Instance()
    {
        if (sInstance == nullptr)
        {
            sInstance = new InputManager();
        }
        return sInstance;
    }

    void InputManager::Release()
    {
        delete sInstance;
        sInstance = nullptr;
    }

    bool InputManager::KeyDown(SDL_Scancode scancode)
    {
        return mKeyboardState[scancode];
    }

    bool InputManager::KeyPressed(SDL_Scancode scancode)
    {
        return !mPrevKeyboardState[scancode] && mKeyboardState[scancode];
    }

    bool InputManager::KeyReleased(SDL_Scancode scancode)
    {
        return mPrevKeyboardState[scancode] && !mKeyboardState[scancode];
    }

    bool InputManager::MouseButtonDown(MouseButton button)
    {
        Uint32 mask = 0;
        switch (button)
        {
        case Left: mask = SDL_BUTTON_LMASK;
            break;

        case Right: mask = SDL_BUTTON_RMASK;
            break;

        case Middle: mask = SDL_BUTTON_MMASK;
            break;

        case Back: mask = SDL_BUTTON_X1MASK;
            break;

        case Forward: mask = SDL_BUTTON_X2MASK;
            break;
        }
        return (mMouseState & mask) != 0;
    }

    bool InputManager::MouseButtonPressed(MouseButton button)
    {
        Uint32 mask = 0;
        switch (button)
        {
        case Left: mask = SDL_BUTTON_LMASK;
            break;

        case Right: mask = SDL_BUTTON_RMASK;
            break;

        case Middle: mask = SDL_BUTTON_MMASK;
            break;

        case Back: mask = SDL_BUTTON_X1MASK;
            break;

        case Forward: mask = SDL_BUTTON_X2MASK;
            break;
        }
        return !(mPrevMouseState & mask) && (mMouseState & mask);
    }

    bool InputManager::MouseButtonReleased(MouseButton button)
    {
        Uint32 mask = 0;
        switch (button)
        {
        case Left: mask = SDL_BUTTON_LMASK;
            break;

        case Right: mask = SDL_BUTTON_RMASK;
            break;

        case Middle: mask = SDL_BUTTON_MMASK;
            break;

        case Back: mask = SDL_BUTTON_X1MASK;
            break;

        case Forward: mask = SDL_BUTTON_X2MASK;
            break;
        }
        return (mPrevMouseState & mask) && !(mMouseState & mask);
    }

    Vector2 InputManager::MousePosition()
    {
        return Vector2((float)mMouseXPos, (float)mMouseYPos);
    }

    void InputManager::Update()
    {
        mMouseState = SDL_GetMouseState(&mMouseXPos, &mMouseYPos);
    }

    void InputManager::UpdatePrevInput()
    {
        memcpy(mPrevKeyboardState, mKeyboardState, mKeyLength);
        mPrevMouseState = mMouseState;
    }

    InputManager::InputManager()
    {
        mKeyboardState = SDL_GetKeyboardState(&mKeyLength);
        mPrevKeyboardState = new Uint8[mKeyLength];
        memcpy(mPrevKeyboardState, mKeyboardState, mKeyLength);
    }

    InputManager::~InputManager()
    {
        delete[] mPrevKeyboardState;
        mPrevKeyboardState = nullptr;
    }
}