#pragma once

#include <SDLMain.h>
#include <SDL_image.h>
#include <iostream> 

namespace SDLFramework
{
    class Graphics
    {
    public:
        static const short SCREEN_WIDTH = 800;
        static const short SCREEN_HEIGHT = 600;

        static Graphics* Instance();
        static void Release();
        static bool Initialized();
        void ClearBackBuffer();
        void Render();
        SDL_Texture* LoadTexture(std::string path);
        void DrawTexture(SDL_Texture* tex, 
            SDL_Rect* srcRect = nullptr, 
            SDL_Rect* dstRect = nullptr, 
            float angle = 0.0f, 
            SDL_RendererFlip flip = SDL_FLIP_NONE);

    private:
        static Graphics* sInstance;
        static bool sInitialized;

        SDL_Window* mWindow;
        SDL_Renderer* mRenderer;

        Graphics();
        ~Graphics();
        bool Init();
    };

    Graphics* Graphics::Instance()
    {
        if (sInstance == nullptr)
        {
            sInstance = new Graphics();
        }
        return sInstance;
    }


    void Graphics::Release()
    {
        delete sInstance;
        sInstance = nullptr;
        sInitialized = false;
    }

    bool Graphics::Initialized()
    {
        return sInitialized;
    }

    void Graphics::ClearBackBuffer()
    {
        SDL_RenderClear(mRenderer);
    }

    void Graphics::Render()
    {
        SDL_RenderPresent(mRenderer);
    }

    Graphics::Graphics() : mRenderer(nullptr)
    {
        sInitialized = Init();
    }

    Graphics::~Graphics()
    {
        SDL_DestroyRenderer(mRenderer);
        SDL_DestroyWindow(mWindow);
    }

    bool Graphics::Init()
    {
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << "Unable to initialize SDL! SDL Error: "
                << SDL_GetError() << std::endl;
            return false;
        }
        mWindow = SDL_CreateWindow("SDL Tutorial", // window title 
            SDL_WINDOWPOS_UNDEFINED, // window x pos 
            SDL_WINDOWPOS_UNDEFINED, // window y pos 
            SCREEN_WIDTH, // window width 
            SCREEN_HEIGHT, // window height 
            SDL_WINDOW_SHOWN); // window flags 
        if (mWindow == nullptr)
        {
            std::cerr << "Unable to create Window! SDL Error: "
                << SDL_GetError() << std::endl;
            return false;
        }
        mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
        if (mRenderer == nullptr)
        {
            std::cerr << "Unable to create renderer! SDL Error: "
                << SDL_GetError() << std::endl;
            return false;
        } return true;
    }

    SDL_Texture* Graphics::LoadTexture(std::string path)
    {
        SDL_Texture* tex = nullptr;
        SDL_Surface* surface = IMG_Load(path.c_str());

        if (surface == nullptr)
        {
            std::cerr << "Unable to load " << path << ". IMG Error: "
                << IMG_GetError() << std::endl;
            return nullptr;
        }
        tex = SDL_CreateTextureFromSurface(mRenderer, surface);
        if (tex == nullptr)
        {
            std::cerr << "Unable to create texture from surface! IMG Error: "
                << IMG_GetError() << std::endl; SDL_FreeSurface(surface);
            return nullptr;
        }

        SDL_FreeSurface(surface);
        return tex;
    }

    void Graphics::DrawTexture(SDL_Texture* tex, 
        SDL_Rect* srcRect, 
        SDL_Rect* dstRect, 
        float angle, 
        SDL_RendererFlip flip) 
    { 
        SDL_RenderCopyEx(mRenderer, 
            tex, 
            srcRect, 
            dstRect, 
            angle, 
            nullptr, 
            flip); }
}
