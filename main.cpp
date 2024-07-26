#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    // Initialization
    int screenWidth = 800;
    int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Isometric Tilemap");

    // Tilemap Settings
    float TILE_SIZE = 32.0f;
    int MAP_WIDTH = 50;
    int MAP_HEIGHT = 50;        

    // Floor
    Texture2D tileTexture = LoadTexture("Assets/Isometric-Tiles.png");

    // Full tiles
    Rectangle sandSourceRec     = { 0, 0, TILE_SIZE, TILE_SIZE };
    Rectangle stoneSourceRec    = { TILE_SIZE, 0, TILE_SIZE, TILE_SIZE };
    Rectangle iceSourceRec      = { TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE };
    Rectangle iceGrassSourceRec = { TILE_SIZE * 3, 0, TILE_SIZE, TILE_SIZE };
    Rectangle grassSourceRec    = { TILE_SIZE * 4, 0, TILE_SIZE, TILE_SIZE };
    Rectangle waterSourceRec    = { TILE_SIZE * 5, 0, TILE_SIZE, TILE_SIZE };
    
    // Half tiles
    Rectangle sandLayerSourceRec     = { 0, TILE_SIZE, TILE_SIZE, TILE_SIZE };
    Rectangle stoneLayerSourceRec    = { TILE_SIZE, TILE_SIZE, TILE_SIZE, TILE_SIZE };
    Rectangle iceGrassLayerSourceRec = { TILE_SIZE * 2, TILE_SIZE, TILE_SIZE, TILE_SIZE };
    Rectangle grassLayerSourceRec    = { TILE_SIZE * 3, TILE_SIZE, TILE_SIZE, TILE_SIZE };

    // Initialize Map with random values including empty tiles
    int tilemap[MAP_WIDTH][MAP_HEIGHT];

    for(int i = 0;i<MAP_WIDTH;++i){
        for(int j = 0;j<MAP_WIDTH;++j){
            tilemap[i][j] = 1;
        }
    }

    int topTiles[MAP_WIDTH][MAP_HEIGHT];

    for(int i = 0;i<MAP_WIDTH;++i){
        for(int j = 0;j<MAP_WIDTH;++j){
            topTiles[i][j] = GetRandomValue(1, 2);
        }
    }

    // Player Settings
    Texture2D PlayerTexture = LoadTexture("Assets/IsoCharacter.png");
    Rectangle PlayerIdleSourceRec = { 0, 0, TILE_SIZE, TILE_SIZE };
    float PlayerSpeed = 3.5f;
    Vector2 PlayerPos = {(float)screenWidth / 2, (float)screenHeight / 2};

    // Health Bar
    Texture2D healthBar = LoadTexture("Assets/HealthUI.png");
    Rectangle healthBarSrcRec = {0 , 0, 65, 20};

    // Camera Settings
    Camera2D camera = { 0 };
    camera.target = PlayerPos;
    camera.offset = (Vector2){ (float)screenWidth / 2, (float)screenHeight / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 2.0f;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Update
        if (IsKeyDown(KEY_D)) PlayerPos.x += PlayerSpeed;
        if (IsKeyDown(KEY_A)) PlayerPos.x -= PlayerSpeed;
        if (IsKeyDown(KEY_W)) PlayerPos.y -= PlayerSpeed;
        if (IsKeyDown(KEY_S)) PlayerPos.y += PlayerSpeed;

        // Clamp Movement
        if (PlayerPos.x < -900) PlayerPos.x = -900;
        if (PlayerPos.x > 900) PlayerPos.x = 900;
        if (PlayerPos.y > 900) PlayerPos.y = 900;
        if (PlayerPos.y < -900) PlayerPos.y = -900;

        camera.target = PlayerPos;

        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode2D(camera);

            // Draw the isometric tilemap
            for (int y = 0; y < MAP_HEIGHT; y++)
            {
                for (int x = 0; x < MAP_WIDTH; x++)
                {
                    // Isometric projection conversion
                    Vector2 tilePos;
                    tilePos.x = (x - y) * (TILE_SIZE / 2);
                    tilePos.y = (x + y) * (TILE_SIZE / 4);

                    // Draw the full tile
                    switch(tilemap[x][y]) {
                        case 1:
                            DrawTextureRec(tileTexture, sandSourceRec, tilePos, WHITE);
                            break;
                        case 2:
                            DrawTextureRec(tileTexture, stoneSourceRec, tilePos, WHITE);
                            break;
                        case 3:
                            DrawTextureRec(tileTexture, iceSourceRec, tilePos, WHITE);
                            break;
                        case 4:
                            DrawTextureRec(tileTexture, iceGrassSourceRec, tilePos, WHITE);
                            break;
                        case 5:
                            DrawTextureRec(tileTexture, grassSourceRec, tilePos, WHITE);
                            break;
                        case 6:
                            DrawTextureRec(tileTexture, waterSourceRec, tilePos, WHITE);
                            break;
                        default:
                            break;
                    }
                }
            }

            // Draw the half tiles on the topmost layer
            for (int y = 0; y < MAP_HEIGHT; y++)
            {
                for (int x = 0; x < MAP_WIDTH; x++)
                {
                    // Isometric projection conversion
                    Vector2 tilePos;
                    tilePos.x = (x - y) * (TILE_SIZE / 2);
                    tilePos.y = (x + y) * (TILE_SIZE / 4) - 16;

                    // Draw the half tile
                    switch(topTiles[x][y]) {
                        case 1:
                            DrawTextureRec(tileTexture, grassLayerSourceRec, tilePos, WHITE);
                            break;
                        case 2:
                            DrawTextureRec(tileTexture, stoneLayerSourceRec, tilePos, WHITE);
                            break;
                        case -4:
                            DrawTextureRec(tileTexture, iceGrassLayerSourceRec, tilePos, WHITE);
                            break;
                        case -5:
                            DrawTextureRec(tileTexture, grassLayerSourceRec, tilePos, WHITE);
                            break;
                    }
                }
            }

            Vector2 healthBarPos = {PlayerPos.x - 180, PlayerPos.y + 80};

            // Draw the player character
            DrawTextureRec(PlayerTexture, PlayerIdleSourceRec, PlayerPos, WHITE);
            DrawTextureRec(healthBar, healthBarSrcRec, healthBarPos, WHITE);
            EndMode2D();
        EndDrawing();
    }

    // De-Initialization
    UnloadTexture(tileTexture);
    UnloadTexture(PlayerTexture);
    CloseWindow();
    
    return 0;
}
