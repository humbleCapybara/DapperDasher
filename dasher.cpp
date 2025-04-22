#include <stdio.h>
#include <math.h>
#include "raylib.h"

struct AnimData{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

// General variables
const int fps = 60;

double time = 0;
const int gravity = 1000; // acceleration due to gravity (pixels/s^2)
const int jumpVel = 700; // velocity for scarfy's jump


bool isOnGround(AnimData character, int windowHeight){
    return character.pos.y >= windowHeight - character.rec.height;
}

AnimData updateAnimData(AnimData object, float dT, int numberOfFrames){
    object.runningTime += dT;
    if (object.runningTime >= object.updateTime){
        object.runningTime = 0.0; 
        object.rec.x = object.frame * object.rec.width;
        object.frame++;
        if (object.frame >= numberOfFrames) object.frame = 0;
    }
    return object;
}


int main(){
    const int windowWidth = 512;
    const int windowHeight = 380;
    InitWindow(windowWidth, windowHeight, "Dapper Dasher");
    InitAudioDevice();
    Sound jumpSound = LoadSound("sounds/jumpSound.wav");
    Sound looseSound = LoadSound("sounds/loosingSound.wav");
    Sound victorySound = LoadSound("sounds/victorySound.mp3");
    Music bgMusic = LoadMusicStream("sounds/startSound.mp3");
    bgMusic.looping = true;
    


    // Scarfy variables
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    Texture2D background = LoadTexture("textures/far-buildings.png");
    Texture2D middleground = LoadTexture("textures/back-buildings.png");
    Texture2D foreground = LoadTexture("textures/foreground.png");

    DrawText(TextFormat("Loading..."), 120, 120, 20, BROWN);
    // AnimData for scarfy
    AnimData scarfyData{
        {0,0,scarfy.width / (float)6,(float)scarfy.height},                                      // Rectangle rec
        {windowWidth / 2 - scarfyData.rec.width / 2,windowHeight - scarfyData.rec.height},       // Vector2 pos
        0,                                                                                       // int frame
        1.0/12.0,                                                                                // float updateTime
        0.0                                                                                      // float runningTime
    };

    
    // Scarfy variables
    float ScarfyVel = 0;
    bool isInAir = false;
    
    // Nebula variables
    float nebVel;

    // Array of nebulae
    const int numberOfNebulae = 6;
    AnimData nebulae[numberOfNebulae];

    for (int i = 0; i < numberOfNebulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/8;
        nebulae[i].pos.y = windowHeight - nebula.height/8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 1.0 / 12.0;
        nebulae[i].pos.x = windowWidth + i * 300;
    }
    float finishLine = nebulae[numberOfNebulae-1].pos.x;

    // Background Variables
    float bgX{};
    float mgX{};
    float fgX{};

    // Collision
    bool collision = false;

    // start 
    bool start = false;
    SetTargetFPS(fps);

    bool winSoundPlayed = false;
    while(!WindowShouldClose()){
        UpdateMusicStream(bgMusic);
        BeginDrawing();
        ClearBackground(WHITE);
        float dT = GetFrameTime();
        
        
        // update background
        bgX -= 20 * dT;
        mgX += 40 * dT;
        fgX -= 80 * dT;

        // stop if rect reaches ground
        if(isOnGround(scarfyData,windowHeight)){ 
            scarfyData.pos.y = windowHeight - scarfyData.rec.height;
            isInAir = false;
            ScarfyVel = 0;
            if (IsKeyPressed(KEY_SPACE) ){
                ScarfyVel -= jumpVel;
                PlaySound(jumpSound);
            } 
        }else{
            // Update position  
            ScarfyVel += gravity * dT;
            isInAir = true;
        }
        // set scarfy velocity
        scarfyData.pos.y += ScarfyVel * dT;
        

        // update character frame
        if (!isInAir){
            scarfyData = updateAnimData(scarfyData, dT, 5);
        }

        for(int i = 0; i < numberOfNebulae; i++){
            // Set nebula velocity
            nebulae[i].pos.x += nebVel * dT;
            nebulae[i] = updateAnimData(nebulae[i], dT, 8);            
        }

        // update finishLine
        finishLine += nebVel * dT;
        bool win = finishLine <= windowWidth/(float)2;

        for (AnimData nebula : nebulae){
            float pad = 50;
            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2*pad,
                nebula.rec.height - 2*pad
            };

            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };

            if(CheckCollisionRecs(nebRec, scarfyRec) && !collision){
                collision = true;
                PlaySound(looseSound);
            }
        }
        Vector2 bgPos[2] = {{bgX,0},{bgX+background.width*2,0}};
        if (bgX+background.width*2 <= 0){ 
            bgX = 0;
        }

        DrawTextureEx(background,bgPos[0],0.0, 2.0, WHITE);
        DrawTextureEx(background,bgPos[1],0.0, 2.0, WHITE);

        Vector2 mgPos[2] = {{mgX-middleground.width*2,0},{mgX,0}};
        if (mgX >= middleground.width*2){ 
            mgX = 0;
        }

        DrawTextureEx(middleground,mgPos[0],0.0, 2.0, WHITE);
        DrawTextureEx(middleground,mgPos[1],0.0, 2.0, WHITE);

        Vector2 fgPos[2] = {{fgX,0},{fgX+foreground.width*2,0}};
        if (fgX+foreground.width*2 <= 0){ 
            fgX = 0;
        }

        DrawTextureEx(foreground,fgPos[0],0.0, 2.0, WHITE);
        DrawTextureEx(foreground,fgPos[1],0.0, 2.0, WHITE);

        if(start == false){
            nebVel = 0;
            DrawText(TextFormat("Press S to start..."), 150, 120, 20, GREEN);
            if (IsKeyReleased(KEY_S)) start = true;
        }else{
            nebVel = -200;
            PlayMusicStream(bgMusic);
        }

        if (!collision && !win){
            for (int i = 0; i < numberOfNebulae; i++)
            {
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }
            
        }else if(win){
            scarfyData.pos.x += 100 * dT;
            DrawText(TextFormat("You win!"), 150, 120, 50, GREEN);
            DrawText(TextFormat("Press R to play again..."), 150, 200, 15, BLUE);
        }else{
            finishLine = windowWidth;
            scarfyData.pos.x -= 100 * dT;
            StopMusicStream(bgMusic);
            DrawText(TextFormat("You Loose!"), 120, 120, 50, RED);
            DrawText(TextFormat("Press R to play again..."), 150, 200, 15, BLUE);
        }

        if ((collision || win) && IsKeyPressed(KEY_R)){
            collision = false;
            win = false;
            scarfyData.pos.x = windowWidth / 2 - scarfyData.rec.width / 2;
            for (int i = 0; i < numberOfNebulae; i++)
            {
                nebulae[i].rec.x = 0.0;
                nebulae[i].rec.y = 0.0;
                nebulae[i].rec.width = nebula.width/8;
                nebulae[i].rec.height = nebula.height/8;
                nebulae[i].pos.y = windowHeight - nebula.height/8;
                nebulae[i].frame = 0;
                nebulae[i].runningTime = 0.0;
                nebulae[i].updateTime = 1.0 / 12.0;
                nebulae[i].pos.x = windowWidth + i * 300;
            }
            finishLine = nebulae[numberOfNebulae-1].pos.x;
        }
        if(win && !winSoundPlayed){
            // play win sound
            PlaySound(victorySound);
            winSoundPlayed = true;
        }
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(middleground);
    UnloadTexture(foreground);
    UnloadSound(jumpSound);
    UnloadSound(looseSound);
    UnloadSound(victorySound);
    UnloadMusicStream(bgMusic);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}