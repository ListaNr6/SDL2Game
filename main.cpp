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

struct Object{
    const char* FileName;
    bool CanYouStandOnIt;
    int number;
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    Object();
    Object(bool CanYouStandOnIt, const char* FileName, SDL_Renderer* renderer);
};
Object::Object(){
};
Object::Object(bool CanYouStandOnIt, const char* FileName, SDL_Renderer* renderer){
    this->FileName = FileName;
    this->CanYouStandOnIt = CanYouStandOnIt;
    if (FileName != "empty")
        this->texture = LoadTexture (renderer, FileName);
    this->renderer = renderer;
    this->number = 0;
}

struct Level{
    Object tree;
    Object open_door;
    Object closed_door;
    Object no_object;
    Object gate;
    Object wall;
    Object lever_left;
    Object lever_right;
    Object key;
    SDL_Texture* grass;
    SDL_Texture* ground;
    SDL_Texture* bird;
    SDL_Texture* bird2;
    SDL_Texture* wooden_floor;
    SDL_Renderer* renderer;
    char RawTerrainMap[40][30];
    Object* ObjectsMap[40][30];
    SDL_Rect Tile[40][30];
    SDL_Rect Bird;
    Object** GetObjectOnTile(int x, int y);

    void LoadNewRawTerrain(const char* TerrainFileName);
    void LoadRawObjects(const char* ObjectsFileName);
    void ChangeObject(Object* object, int x, int y);
    void DrawTerrain();
    void DrawObjects();
//    void DrawTheSky();
    Level(SDL_Renderer* renderer);
};
Level::Level(SDL_Renderer* renderer){
    this->renderer = renderer;
    Adjust_Rectangles(*Tile, 40, 30);
    wooden_floor = LoadTexture (renderer, "tekstury/wooden_floor.png");
    bird = LoadTexture (renderer,"tekstury/bird.png");
    bird2 = LoadTexture (renderer,"tekstury/bird2.png");
    grass = LoadTexture (renderer, "tekstury/trawa.png");
    ground = LoadTexture (renderer, "tekstury/sciezka.png");
    tree = Object(false, "tekstury/drzewo.png", renderer);
    closed_door = Object(false, "tekstury/closed_door.png", renderer);
    open_door = Object(true, "tekstury/open_door.png", renderer);
    gate = Object(false, "tekstury/gate.png", renderer);
    lever_left = Object(false, "tekstury/lever_left.png", renderer);
    lever_right = Object(false, "tekstury/lever_right.png", renderer);
    key = Object(false, "tekstury/key.png", renderer);
    wall = Object(false, "tekstury/wall.png", renderer);
    no_object = Object(true, "empty", renderer);
//    Bird.x = 800; Bird.y = 400; Bird.h = 20; Bird.w = 20;
}
Object** Level::GetObjectOnTile(int x, int y){
    return &this->ObjectsMap[x][y];
}
void Level::LoadNewRawTerrain(const char* TerrainFileName){
    std::ifstream ReadFile;
    ReadFile.open(TerrainFileName);
    char temporary_char;
    int x=0; int y=0;
    while (ReadFile.get(temporary_char)){
        if (temporary_char != '\n'){
            if (x == 40){
                y++; x = 0;
            }
            RawTerrainMap[x][y] = temporary_char;
            x++;
        }
    }
    ReadFile.close();
}
void Level::LoadRawObjects(const char* ObjectsFileName){
    std::ifstream ReadFile;
    ReadFile.open(ObjectsFileName);
    char temporary_char;
    int x=0; int y=0;
    while (ReadFile.get(temporary_char)){
        if (temporary_char != '\n'){
            if (x == 40){ 
                y++; x = 0;
            }
            if (temporary_char == 'T')
                ObjectsMap[x][y] = &tree;
            else if(temporary_char == 'W')
                ObjectsMap[x][y] = &wall;
            else if(temporary_char == 'D')
                ObjectsMap[x][y] = &closed_door;
            else if(temporary_char == 'L')
                ObjectsMap[x][y] = &lever_left;
            else if(temporary_char == 'K')
                ObjectsMap[x][y] = &key;
            else if(temporary_char == 'G')
                ObjectsMap[x][y] = &gate;
            else if(temporary_char == 'X')
                ObjectsMap[x][y] = &no_object;
            else;
            x++;
        }
    }
}
void Level::ChangeObject(Object* object, int x, int y){
    ObjectsMap[x][y] = object;
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
        else if (RawTerrainMap[x][y] == 'F'){
            SDL_RenderCopy(renderer, wooden_floor, NULL, ReturnRectangle(*Tile, x, y));
        }
        else;
        x++;
    }
}
void Level::DrawObjects(){
    int x=0; int y=0;
    while (y < 30){
        if (x == 40) { y++; x = 0;}
        if (ObjectsMap[x][y] != &no_object)
            SDL_RenderCopy(renderer, ObjectsMap[x][y]->texture, NULL, ReturnRectangle(*Tile, x, y));
        else;
        x++;
    }
}
/*void Level::DrawTheSky(){
    if (Bird.x > -20)
        Bird.x--;
    else 
        Bird.x = 800;
    if (Bird.x%80 >= 40)
        SDL_RenderCopy(this->renderer, bird, NULL, &Bird);
    else
        SDL_RenderCopy(this->renderer, bird2, NULL, &Bird);
}*/

struct Hero{
    int x, y;
    int VelocityX, VelocityY;
    SDL_Rect rect;
    std::list<SDL_Scancode> KeysPressed;
    SDL_Texture* body_to_left; SDL_Texture* body_to_right; SDL_Texture* body_to_up; SDL_Texture* body_to_down;
    char LookingDirection;
    Level* level;
    int ActionReady;
    bool use;
    int key;
    
    Object GetObject(int x, int y);
//    Object* GetObjectOnSide(char side);
    Object** InFrontOfYou();
    
    void CheckKeysPressed(SDL_Event* KeyPressed);
    void CheckAction();
    void Move();
    void Use();
    void Action(SDL_Event* KeyPressed);
    void ZeroVelocity();
    void Draw();
    int CanIMove(char side);
    
    Hero(int x, int y, Level* level);
};
Hero::Hero(int x, int y, Level* level){
    this->x = x; this->y = x;
    rect.x = x*20; rect.y = y*20; rect.w = 20; rect.h = 20;
    LookingDirection = 'D';
    this->level = level;
    body_to_left = LoadTexture(level->renderer, "tekstury/character_left.png");
    body_to_right = LoadTexture(level->renderer, "tekstury/character_right.png");
    body_to_up = LoadTexture(level->renderer, "tekstury/character_up.png");
    body_to_down = LoadTexture(level->renderer, "tekstury/character_down.png");
    VelocityX = 0, VelocityY = 0;
    KeysPressed.clear();
    use = false;
    key = 0;
}
void Hero::CheckKeysPressed(SDL_Event* KeyPressed){
    while(SDL_PollEvent (KeyPressed)){
        if (KeyPressed->key.repeat == 0){
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
                        case SDL_SCANCODE_S:
                            KeysPressed.push_back(SDL_SCANCODE_S);
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
                        case SDL_SCANCODE_S:
                            KeysPressed.remove(SDL_SCANCODE_S);
                            break;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }
    }
}
void Hero::CheckAction(){
    if(((rect.x%20) == 0) && ((rect.y % 20) == 0) ){
        if (KeysPressed.empty() == false){
            switch (KeysPressed.back()){
                case (SDL_SCANCODE_LEFT):
                    LookingDirection = 'L';
                    if (CanIMove('L')){
                        VelocityX = -1;
                        VelocityY = 0;
                    }
                    else ZeroVelocity();
                    break;
                case (SDL_SCANCODE_RIGHT):
                    LookingDirection = 'R';
                    if (CanIMove('R')){
                        VelocityX = 1;
                        VelocityY = 0;
                    }
                    else ZeroVelocity();
                    break;
                case (SDL_SCANCODE_UP):
                    LookingDirection = 'U';
                    if (CanIMove('U')){
                        VelocityX = 0;
                        VelocityY = -1;
                    }
                    else ZeroVelocity();
                    break;
                case (SDL_SCANCODE_DOWN):
                    LookingDirection = 'D';
                    if (CanIMove('D')){
                        VelocityX = 0;
                        VelocityY = 1;
                    }
                    else ZeroVelocity();
                    break;
                case (SDL_SCANCODE_S):
                    use = true;
                    ZeroVelocity();
                default:
                    ZeroVelocity();
            }
        }
        else ZeroVelocity();
    }
}
Object** Hero::InFrontOfYou(){
    if (LookingDirection == 'L')
        return &(level->ObjectsMap[((rect.x)/20)-1][rect.y/20]);
    else if (LookingDirection == 'R')
        return &(level->ObjectsMap[((rect.x)/20)+1][rect.y/20]);
    else if (LookingDirection == 'U')
        return &(level->ObjectsMap[((rect.x)/20)][rect.y/20 - 1]);
    else if (LookingDirection == 'D')
        return &(level->ObjectsMap[((rect.x)/20)][rect.y/20 + 1]);
    else
        return NULL;
}
/* Object* Hero::GetObjectOnSide(char side){
    if (side == 'L')
        return level->ObjectsMap[((rect.x)/20)-1][rect.y/20];
    else if (side == 'R')
        return level->ObjectsMap[((rect.x)/20)+1][rect.y/20];
    else if (side == 'U')
        return level->ObjectsMap[((rect.x)/20)][(rect.y)/20 - 1];
    else if (side == 'D')
        return level->ObjectsMap[((rect.x)/20)][(rect.y)/20 + 1];
    else
        return NULL; 
} */
int Hero::CanIMove(char side){
    if (side == 'L'){
        if (level->ObjectsMap[((rect.x)/20)-1][rect.y/20]->CanYouStandOnIt == true)
            return 1;
        else return 0;
    }
    else if (side == 'R'){
        if (level->ObjectsMap[((rect.x)/20)+1][rect.y/20]->CanYouStandOnIt == true)
            return 1;
        else return 0;
    }
        else if (side == 'U'){
        if (level->ObjectsMap[((rect.x)/20)][(rect.y)/20 - 1]->CanYouStandOnIt == true)
            return 1;
        else return 0;
    }
        else if (side == 'D'){
        if (level->ObjectsMap[((rect.x)/20)][(rect.y)/20 + 1]->CanYouStandOnIt == true)
            return 1;
        else return 0;
    }
}
void Hero::ZeroVelocity(){
    VelocityX = 0;
    VelocityY = 0;  
}
void Hero::Use(){
    if (use == true){
        Object** TemporaryObject = InFrontOfYou();
        if (*TemporaryObject == &level->closed_door)
            *TemporaryObject = &level->open_door;
        else if (*TemporaryObject == &level->open_door)
            *TemporaryObject = &level->closed_door;
        else if (*TemporaryObject == &level->lever_left){
            *TemporaryObject = &level->lever_right;
            TemporaryObject = level->GetObjectOnTile(16, 26);
            *TemporaryObject = &level->no_object;
            TemporaryObject = level->GetObjectOnTile(17, 26);
            *TemporaryObject = &level->no_object;
        }
        else if (*TemporaryObject == &level->lever_right){
            *TemporaryObject = &level->lever_left;
            TemporaryObject = level->GetObjectOnTile(16, 26);
            *TemporaryObject = &level->wall;
            TemporaryObject = level->GetObjectOnTile(17, 26);
            *TemporaryObject = &level->wall;
        }
        else if (*TemporaryObject == &level->key){
            *TemporaryObject = &level->no_object;
            key = 1;
        }
        else if (*TemporaryObject == &level->gate && this->key == 1){
            *TemporaryObject = &level->no_object;
            this->key = 0;
        }
        KeysPressed.remove(SDL_SCANCODE_S);
        use = false;
    }
    else;
}
/////// tutaj jest fiu bzdziu. infrontofyou powinno zwracać wskaźnik na wskaźnik, wtedy *wskaźnik = open/closed_door

void Hero::Move(){
    if (VelocityX == -1)
        rect.x--;
    else if (VelocityX == 1)
        rect.x++;
    else if (VelocityY == -1)
        rect.y--;
    else if (VelocityY == 1)
        rect.y++;
}

void Hero::Action(SDL_Event* KeyPressed){
    CheckKeysPressed(KeyPressed);
    CheckAction();
    Move();
    Use();
}

void Hero::Draw(){
    switch (LookingDirection){
        case 'L':
            SDL_RenderCopy(level->renderer, body_to_left, NULL, &this->rect);
            break;
        case 'R':
            SDL_RenderCopy(level->renderer, body_to_right, NULL, &this->rect);
            break;
        case 'U':
            SDL_RenderCopy(level->renderer, body_to_up, NULL, &this->rect);
            break;
        case 'D':
            SDL_RenderCopy(level->renderer, body_to_down, NULL, &this->rect);
            break;
        default:
            break;
    }
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

    SDL_Window* window = SDL_CreateWindow( "Aksiuszke", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN );
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );

    Level NewLevel(renderer);
    Hero MainHero(14, 14, &NewLevel);
    SDL_Event event;

    MakeTheScreenBlack(renderer);
    NewLevel.LoadNewRawTerrain("Mapy/mapa_powierzchni1.txt");
    NewLevel.LoadRawObjects("Mapy/Objects1.txt");

    Timer NewTimer(15);


    while (1) {
        NewLevel.DrawTerrain();
        NewLevel.DrawObjects();
        MainHero.Draw();
        SDL_RenderPresent(renderer);
        NewTimer.SetStartingTime();
        while (1){
            MainHero.Action(&event);
            NewLevel.DrawTerrain();
            NewLevel.DrawObjects();
            MainHero.Draw();
//          NewLevel.DrawTheSky();
            SDL_RenderPresent(renderer);
            if ( event.key.keysym.scancode == SDL_SCANCODE_ESCAPE || event.type == SDL_QUIT){
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 0;
            }
            NewTimer.MakeTheDelay();
        }
    }
}
