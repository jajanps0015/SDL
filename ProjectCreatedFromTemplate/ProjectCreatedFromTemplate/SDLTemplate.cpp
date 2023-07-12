#include <iostream>
#include <SDLMain.h>
#include <algorithm>
#include <GameManager.h>

using namespace SDLFramework;

int main(int argc, char* args[])
{
    GameManager* game = GameManager::Instance(); 
    game->Run();
    GameManager::Release(); 
    game = nullptr; 
    return 0;
}