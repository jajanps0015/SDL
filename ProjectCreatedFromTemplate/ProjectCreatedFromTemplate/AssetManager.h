#pragma once

#include <Graphics.h>
#include <iostream>
#include <sstream>
#include <string>
#include <map> 

namespace SDLFramework
{
    class AssetManager
    {
    private:
        static AssetManager* sInstance;

        std::map<std::string, SDL_Texture*> mTextures;
        std::map<SDL_Texture*, unsigned> mTextureRefCount;
        
        std::map<std::string, TTF_Font*> mFonts; 
        std::map<std::string, SDL_Texture*> mText;
        
        AssetManager();
        ~AssetManager();

    public:
        static AssetManager* Instance();
        static void Release();

        SDL_Texture* GetTexture(std::string filename, bool managed = false);
        void DestroyTexture(SDL_Texture* texture);

        TTF_Font* GetFont(std::string filename, int size);
        SDL_Texture* GetText(std::string text, 
            std::string filename, 
            int size, SDL_Color color, 
            bool managed = false);
    };

    AssetManager* AssetManager::sInstance = nullptr;

    AssetManager* AssetManager::Instance()
    {
        if (sInstance == nullptr)
        {
            sInstance = new AssetManager();
        }
        return sInstance;
    }

    void AssetManager::Release()
    {
        delete sInstance;
        sInstance = nullptr;
    }

    AssetManager::AssetManager()
    { }

    AssetManager::~AssetManager()
    {
        for (auto tex : mTextures)
        {
            if (tex.second != nullptr)
            {
                SDL_DestroyTexture(tex.second);
            }
        }
        mTextures.clear();
    }

    SDL_Texture* AssetManager::GetTexture(std::string filename, bool managed)
    {
        std::string fullPath = SDL_GetBasePath();
        fullPath.append("Assets\\" + filename);

        if (mTextures[fullPath] == nullptr)
        {
            mTextures[fullPath] = Graphics::Instance()->LoadTexture(fullPath);
        }

        if (mTextures[fullPath] != nullptr && managed)
        {
            mTextureRefCount[mTextures[fullPath]] += 1;
        }
        return mTextures[fullPath];
    }

    void AssetManager::DestroyTexture(SDL_Texture* texture) 
    {
        if (mTextureRefCount.find(texture) != mTextureRefCount.end()) 
        {
            mTextureRefCount[texture] -= 1; 
            if (mTextureRefCount[texture] == 0) 
            {
                for (auto elem : mTextures) 
                {
                    if (elem.second == texture) 
                    {
                        SDL_DestroyTexture(elem.second); 
                        mTextures.erase(elem.first); 
                        return; // work finished, leave function 
                    } 
                }

                for (auto elem : mText) 
                {
                    if (elem.second == texture) 
                    {
                        SDL_DestroyTexture(elem.second); 
                        mText.erase(elem.first); 
                        
                        return; // work finished, leave function 
                    } 
                }
            } 
        } 
        else 
        { 
            SDL_DestroyTexture(texture); 
        } 
    }

    TTF_Font* AssetManager::GetFont(std::string filename, int size) 
    { 
        std::string fullPath = SDL_GetBasePath(); 
        fullPath.append("Assets/" + filename);

        std::stringstream ss; 
        ss << size;
        std::string key = fullPath + ss.str();

        if (mFonts[key] == nullptr) 
        { 
            mFonts[key] = TTF_OpenFont(fullPath.c_str(), size); 
            
            if (mFonts[key] == nullptr) 
            { 
                std::cerr << "Unable to load font " << filename 
                    << "! TTF Error: " << TTF_GetError() << std::endl; 
            } 
        } 
        return mFonts[key];
    }

    SDL_Texture* AssetManager::GetText(std::string text, 
        std::string filename, 
        int size, 
        SDL_Color color, bool managed) 
    { 
        std::stringstream ss; 
        ss << size 
            << (int)color.r 
            << (int)color.g 
            << (int)color.b; 
        
        std::string key = text + filename + ss.str(); 
        
        if (mText[key] == nullptr) 
        { 
            TTF_Font* font = GetFont(filename, size); 
            mText[key] = Graphics::Instance()->CreateTextTexture(font, text, color);
        } 

        if (mText[key] != nullptr && managed) 
        { 
            mTextureRefCount[mText[key]] += 1; 
        } 
        return mText[key]; 
    }
}
