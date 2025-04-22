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
const int fps = 240;

double time = 0;
const int gravity = 1000; // acceleration due to gravity (pixels/s^2)
const int jumpVel = 700; // velocity for scarfy's jump

int main(){
    const int windowWidth = 518;
    const int windowHeight = 380;
    InitWindow(windowWidth, windowHeight, "Dapper Dasher");

    // Scarfy variables
    Texture2D scarfy = LoadTexture("textures/scarfy.png");

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
    
    // Loading Nebula
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    // Nebula variables
    float nebVel = -250;

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

    SetTargetFPS(fps);
    while(!WindowShouldClose()){

        BeginDrawing();
        ClearBackground(WHITE);
        float dT = GetFrameTime();
            
        // stop if rect reaches ground
        if(scarfyData.pos.y >= windowHeight - scarfyData.rec.height){ 
            scarfyData.pos.y = windowHeight - scarfyData.rec.height;
            isInAir = false;
            ScarfyVel = 0;
            if (IsKeyPressed(KEY_SPACE) ){
                ScarfyVel -= jumpVel;
            } 
        }else{
            // Update position
            ScarfyVel += gravity * dT;
            isInAir = true;
        }
        // set scarfy velocity
        scarfyData.pos.y += ScarfyVel * dT;
        

        // update character frame
        scarfyData.runningTime += dT;
        if (scarfyData.runningTime >= scarfyData.updateTime){
            scarfyData.runningTime = 0.0; 
            scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
            if (!isInAir) scarfyData.frame++;
            if (scarfyData.frame >= 6) scarfyData.frame = 0;
        }

        for(int i = 0; i < numberOfNebulae; i++){
            // Set nebula velocity
            nebulae[i].pos.x += nebVel * dT;
            nebulae[i].runningTime += dT;

            // Update nebula frames
            if(nebulae[i].runningTime >= nebulae[i].updateTime){
                nebulae[i].runningTime = 0.0;
                nebulae[i].rec.x = nebulae[i].frame * nebulae[i].rec.width;    
                nebulae[i].frame++;                                       
                if (nebulae[i].frame >= 8) nebulae[i].frame = 0;
            }

            
        }

        for (int i = 0; i < numberOfNebulae; i++)
        {
            DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
        }
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
    return 0;
}