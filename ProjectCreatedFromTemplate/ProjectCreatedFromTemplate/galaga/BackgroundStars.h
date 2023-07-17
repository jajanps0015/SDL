#pragma once

#include <galaga/StarLayer.h>

namespace Galaga
{
    class BackgroundStars {

    private:
        static BackgroundStars* sInstance;
        static const int LAYER_COUNT = 3;
        StarLayer* mLayers[LAYER_COUNT];

        BackgroundStars();
        ~BackgroundStars();

    public:
        static BackgroundStars* Instance();
        static void Release();
        void Scroll(bool b);
        void Update();
        void Render();
    };
}