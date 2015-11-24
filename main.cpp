// funkcje intowe niezwracające inta: sdl_setcolorkey, sdl_event

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <list>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>

SDL_Texture* LoadTexture(SDL_Renderer* Renderer, const char* FileName){
    SDL_Surface* NewSurface = IMG_Load (FileName);
    Uint32 ColorKey = SDL_MapRGB(NewSurface->format, 255, 0, 255);
    SDL_SetColorKey(NewSurface, SDL_TRUE, ColorKey);
    SDL_Texture* NewTexture = SDL_CreateTextureFromSurface(Renderer, NewSurface);
    SDL_FreeSurface(NewSurface);
    return NewTexture;
}

void Adjust_Rectangles(SDL_Rect* FirstElementOfArray, int x, int y){
    SDL_Rect* Element = FirstElementOfArray;
    for (int i = 0; i<x; i++){
        for (int j = 0; j<y; j++){
            (*Element).x = i*20; (*Element).y = j*20; (*Element).w = 20; (*Element).h = 20;
            Element = Element+1;
        }
    }
}

SDL_Rect* ReturnRectangle(SDL_Rect* FirstElementOfArray, int x, int y){
    return (FirstElementOfArray + (x*30) + y);
}

struct Level{
    SDL_Texture* grass;
    SDL_Texture* ground;
    SDL_Texture* tree;
    SDL_Renderer* renderer;
    char RawTerrainMap[40][30];
    char RawObjectsMap[40][30];
    SDL_Rect Tile[40][30];

    void LoadNewRawTerrain(const char* TerrainFileName);
    void LoadNewRawObjects(const char* ObjectsFileName);
    void DrawTerrain();
    void DrawObjects();
    Level(SDL_Renderer* renderer);
};
Level::Level(SDL_Renderer* renderer){
    this->renderer = renderer;
    Adjust_Rectangles(*Tile, 40, 30);
    grass = LoadTexture (renderer, "tekstury/trawa.png");
    ground = LoadTexture (renderer, "tekstury/sciezka.png");
    tree = LoadTexture (renderer, "tekstury/drzewo.png");
}
void Level::LoadNewRawTerrain(const char* TerrainFileName){
    std::ifstream ReadFile;
    ReadFile.open(TerrainFileName);
    char temporary_char;
    int x=0; int y=0;
    while (ReadFile.get(temporary_char)){
        if (temporary_char != '\n'){
            if (x == 40){ y++; x = 0;}
            RawTerrainMap[x][y] = temporary_char;
            x++;
        }
    }
    ReadFile.close();
}
void Level::LoadNewRawObjects(const char* ObjectsFileName){
    std::ifstream ReadFile;
    ReadFile.open(ObjectsFileName);
    char temporary_char;
    int x=0; int y=0;
    while (ReadFile.get(temporary_char)){
        if (temporary_char != '\n'){
            if (x == 40){ y++; x = 0;}
            RawObjectsMap[x][y] = temporary_char;
            x++;
        }
    }
    ReadFile.close();
}
void Level::DrawTerrain(){
    int x=0; int y=0;
    while (y < 30){
        if (x == 40) { y++; x = 0;}
        if (RawTerrainMap[x][y] == 'T'){
            SDL_RenderCopy(renderer, grass, NULL, ReturnRectangle(*Tile, x, y));
        }
        else if (RawTerrainMap[x][y] == 'K'){
            SDL_RenderCopy(renderer, ground, NULL, ReturnRectangle(*Tile, x, y));
        }
        else;
        x++;
    }
}
void Level::DrawObjects(){
    int x=0; int y=0;
    while (y < 30){
        if (x == 40) { y++; x = 0;}
        if (RawObjectsMap[x][y] == 'T'){
            SDL_RenderCopy(renderer, tree, NULL, ReturnRectangle(*Tile, x, y));
        }
        else;
        x++;
    }
}

struct Hero{
    SDL_Rect rect;
    SDL_Texture* body;
    std::list<SDL_Scancode> KeysPressed;
    char MovementDirection;
    Level* level;
    int VelocityX, VelocityY;
    void GetVelocity();
    void CheckKeysPressed(SDL_Event* KeyPressed);
    void Move();
    void Draw();
    Hero(int x, int y, Level* level);
};
Hero::Hero(int x, int y, Level* level){
    rect.x = x; rect.y = y; rect.w = 20; rect.h = 20;
    MovementDirection = '0';
    this->level = level;
    body = LoadTexture(level->renderer, "tekstury/character.png");
    VelocityX = 0, VelocityY = 0;
    KeysPressed.clear();
}

void Hero::CheckKeysPressed(SDL_Event* KeyPressed){
    while(SDL_PollEvent (KeyPressed)){
        switch (KeyPressed->key.type){
            case SDL_KEYDOWN:
                switch (KeyPressed->key.keysym.scancode) {
                    case SDL_SCANCODE_LEFT:
                        KeysPressed.push_back(SDL_SCANCODE_LEFT);
                        break;
                    case SDL_SCANCODE_RIGHT:
                        KeysPressed.push_back(SDL_SCANCODE_RIGHT);
                        break;
                    case SDL_SCANCODE_UP:
                        KeysPressed.push_back(SDL_SCANCODE_UP);
                        break;
                    case SDL_SCANCODE_DOWN:
                        KeysPressed.push_back(SDL_SCANCODE_DOWN);
                        break;
                    default:
                        break;
                }

                break;
            case SDL_KEYUP:
                switch (KeyPressed->key.keysym.scancode) {
                    case SDL_SCANCODE_LEFT:
                        KeysPressed.remove(SDL_SCANCODE_LEFT);
                        break;
                    case SDL_SCANCODE_RIGHT:
                        KeysPressed.remove(SDL_SCANCODE_RIGHT);
                        break;
                    case SDL_SCANCODE_UP:
                        KeysPressed.remove(SDL_SCANCODE_UP);
                        break;
                    case SDL_SCANCODE_DOWN:
                        KeysPressed.remove(SDL_SCANCODE_DOWN);
                        break;
                }
                break;
            default:
                break;
        }
    }
}

void Hero::GetVelocity(){
    if(((rect.x%20) == 0) && ((rect.y % 20) == 0) ){
        if (KeysPressed.empty() == false){
            switch (KeysPressed.back()){
                case (SDL_SCANCODE_LEFT):
                    if (level->RawObjectsMap[((rect.x)/20)-1][rect.y/20] == 'X'){
                        VelocityX = -1;
                        VelocityY = 0;
                    }
                    else{
                        VelocityX = 0;
                        VelocityY = 0;                        
                    }
                    break;

                case (SDL_SCANCODE_RIGHT):
                    if (level->RawObjectsMap[((rect.x)/20)+1][rect.y/20] == 'X'){
                        VelocityX = 1;
                        VelocityY = 0;
                    }
                    else{
                        VelocityX = 0;
                        VelocityY = 0;                        
                    }
                    break;

                case (SDL_SCANCODE_UP):
                    if (level->RawObjectsMap[((rect.x)/20)][(rect.y)/20 - 1] == 'X'){
                        VelocityX = 0;
                        VelocityY = -1;
                    }
                    else{
                        VelocityX = 0;
                        VelocityY = 0;                        
                    }
                    break;

                case (SDL_SCANCODE_DOWN):
                    if (level->RawObjectsMap[((rect.x)/20)][(rect.y)/20 + 1] == 'X'){
                        VelocityX = 0;
                        VelocityY = 1;
                    }
                    else{
                        VelocityX = 0;
                        VelocityY = 0;                        
                    }
                    break;
                default:
                    break;
            }
        }
        else{
            VelocityX = 0;
            VelocityY = 0;
        }
    }
    
}

void Hero::Move(){
    GetVelocity();
    if (VelocityX == -1){
        rect.x--;
    }
    else if (VelocityX == 1){
        rect.x++;
    }
    else if (VelocityY == -1){
        rect.y--;
    }
    else if (VelocityY == 1){
        rect.y++;
    }
}
void Hero::Draw(){
    SDL_RenderCopy(level->renderer, body, NULL, &this->rect);
}

struct Timer{
    Uint32 MilisecondsPerFrame;
    Uint32 StartingLoopTime;
    Uint32 EndingLoopTime;
    Uint32 DifferenceBetweenTimes;
    void SetStartingTime();
    void SetEndingTime();
    void SetDifferenceBetweenTimes();
    void MakeTheDelay();
    Timer(Uint32 MilisecondsForFrame);
};
Timer::Timer(Uint32 MilisecondsPerFrame){
    this->MilisecondsPerFrame = MilisecondsPerFrame;
}
void Timer::SetStartingTime(){
    this->StartingLoopTime = SDL_GetTicks();
}
void Timer::SetEndingTime(){
    this->EndingLoopTime = SDL_GetTicks();
}
void Timer::SetDifferenceBetweenTimes(){
    DifferenceBetweenTimes = (this->EndingLoopTime - this->StartingLoopTime);
}
void Timer::MakeTheDelay(){
    SetEndingTime();
    SetDifferenceBetweenTimes();
    if (DifferenceBetweenTimes < MilisecondsPerFrame)
        SDL_Delay(MilisecondsPerFrame - DifferenceBetweenTimes);
    StartingLoopTime = EndingLoopTime;
}

void MakeTheScreenBlack(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

int main( int argc, char* argv[] ) {

    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetMainReady();

    SDL_Window* window = SDL_CreateWindow( "Szalwiuszke", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN );
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );

    Level NewLevel(renderer);
    Hero MainHero(100, 100, &NewLevel);
    SDL_Event event;

    MakeTheScreenBlack(renderer);
    NewLevel.LoadNewRawTerrain("Mapy/mapa_powierzchni1.txt");
    NewLevel.LoadNewRawObjects("Mapy/Objects1.txt");

    Timer NewTimer(10);


    while (1) {
        if (SDL_PollEvent(&event) && event.key.keysym.scancode == SDL_SCANCODE_A) {
            NewLevel.DrawTerrain();
            NewLevel.DrawObjects();
            MainHero.Draw();
            SDL_RenderPresent(renderer);
            NewTimer.SetStartingTime();
            while (1){
                MainHero.CheckKeysPressed(&event);
                MainHero.Move();
                NewLevel.DrawTerrain();
                NewLevel.DrawObjects();
                MainHero.Draw();
                SDL_RenderPresent(renderer);
                if ( event.key.keysym.scancode == SDL_SCANCODE_D){
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    return 0;
                }
                NewTimer.MakeTheDelay();
            }
        }
    }
}