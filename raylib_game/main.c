#include "dig.h"

static Tile tiles[GRID_COUNT][GRID_COUNT];

static Player player = { 0 };
static int digPoint = 0;

bool testflag = false;
static bool canDig = false;

//
void InitTile(void)
{
    for (int i = 0; i < GRID_COUNT; i++)
    {
        for (int j = 0; j < GRID_COUNT; j++)
        {
            tiles[i][j].grid.x = i;
            tiles[i][j].grid.y = j;

            tiles[i][j].position.x = i * GRID_SIZE;
            tiles[i][j].position.y = j * GRID_SIZE;

            int blockCount = GRID_COUNT / 3;

            if (j < 2 )
            {
                tiles[i][j].type = Empty;
            }
            else if (j < blockCount)
            {
                tiles[i][j].type = Ice;
                tiles[i][j].texture = PathToTexture("image/tiles/ice_01.png", GRID_SIZE);
            }
            else if ( i < blockCount && j < blockCount*2)
            {
                tiles[i][j].type = Sand;
                tiles[i][j].texture = PathToTexture("image/tiles/sand_1.png", GRID_SIZE);
            }
            else if( i >= blockCount*2 && j < blockCount*2)
            {
                tiles[i][j].type = Igneous;
                tiles[i][j].texture = PathToTexture("image/tiles/igneous_1.png", GRID_SIZE);
            }
            else if( j >= blockCount*2 )
            {
                tiles[i][j].type = Grass;
                tiles[i][j].texture = PathToTexture("image/tiles/grass_1.png", GRID_SIZE);
            }
            else
            {
                tiles[i][j].type = Lava;
                tiles[i][j].texture = PathToTexture("image/tiles/lava_1.png", GRID_SIZE);
            }
        }
    }
}


//----------------------------------------------------------------------------------
// Module functions declaration
//----------------------------------------------------------------------------------
void UpdatePlayer(Player* player, float delta);
void UpdateCameraCenter(Camera2D* camera, Player* player, EnvItem* envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraCenterInsideMap(Camera2D* camera, Player* player, EnvItem* envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraCenterSmoothFollow(Camera2D* camera, Player* player, EnvItem* envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraEvenOutOnLanding(Camera2D* camera, Player* player, EnvItem* envItems, int envItemsLength, float delta, int width, int height);
void UpdateCameraPlayerBoundsPush(Camera2D* camera, Player* player, EnvItem* envItems, int envItemsLength, float delta, int width, int height);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 2d camera");
    InitTile();

    player.position = (Vector2){ 200, 0 };
    player.speed = 0;
    player.canJump = false;
    player.texture = PathToTexture("image/hero/idle_01.png",PLAYER_SIZE);
    EnvItem envItems[] = {
        {{ 0, 0, 1000, 400 }, 0, LIGHTGRAY },
    
    };

    int envItemsLength = sizeof(envItems) / sizeof(envItems[0]);

    Camera2D camera = { 0 };
    camera.target = player.position;
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // Store pointers to the multiple update camera functions
    void (*cameraUpdaters[])(Camera2D*, Player*, EnvItem*, int, float, int, int) = {
        UpdateCameraCenter,
        UpdateCameraCenterInsideMap,
        UpdateCameraCenterSmoothFollow,
        UpdateCameraEvenOutOnLanding,
        UpdateCameraPlayerBoundsPush
    };

    int cameraOption = 0;
    int cameraUpdatersLength = sizeof(cameraUpdaters) / sizeof(cameraUpdaters[0]);

    char* cameraDescriptions[] = {
        "Follow player center",
        "Follow player center, but clamp to map edges",
        "Follow player center; smoothed",
        "Follow player center horizontally; update player center vertically after landing",
        "Player push camera on getting too close to screen edge"
    };

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        // ===* = * = *===  Update  ===* = * = *===
        float deltaTime = GetFrameTime();

        UpdatePlayer(&player,deltaTime);

        camera.zoom += ((float)GetMouseWheelMove() * 0.05f);

        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
        else if (camera.zoom < 0.25f) camera.zoom = 0.25f;

        if (IsKeyPressed(KEY_R))
        {
            camera.zoom = 1.0f;
            player.position = (Vector2){ 400, 280 };
        }

        if (IsKeyPressed(KEY_C)) cameraOption = (cameraOption + 1) % cameraUpdatersLength;

        // Call update camera function by its pointer
        cameraUpdaters[cameraOption](&camera, &player, envItems, envItemsLength, deltaTime, screenWidth, screenHeight);

        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(LIGHTGRAY);

            BeginMode2D(camera);

                for (int i = 0; i < GRID_COUNT; i++)
                {
                    for (int j = 0; j < GRID_COUNT; j++)
                    {
                        Tile tile = tiles[i][j];
                        if (tile.type != Empty)
                        {
                            DrawTexture(tile.texture, tile.position.x - GRID_SIZE/2,tile.position.y - GRID_SIZE/2,WHITE);
                        }
                        else
                        {
                            DrawRectangleRec(ToRectangle(tile.position,GRID_SIZE), LIGHTGRAY);
                        }
                    }
                }
                DrawTexture(player.texture, player.position.x - PLAYER_SIZE / 2, player.position.y - PLAYER_SIZE / 2, WHITE);


            EndMode2D();

        // Debug Info:
        char positionStr[1024];
        sprintf(positionStr, "Player position : x = %f , y = %f", player.position.x, player.position.y);
        DrawText(positionStr,20,20,20,BLUE);

        char gridPositionStr[1024];
        sprintf(gridPositionStr, "Player grid position : x = %d , y = %d", player.grid.x, player.grid.y);
        DrawText(gridPositionStr, 20, 40, 20, BLUE);

        if (testflag) DrawText("Colled", 20, 70, 20, BLUE);
        if(player.canJump) DrawText("Can Jump", 20, 90, 20, BLUE);

        char digPointStr[1024];
        sprintf(digPointStr, "DIG POINT : %d", digPoint);
        DrawText(digPointStr, 400, 50, 30, BLUE);

        EndDrawing();
        

        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

Rectangle ToRectangle(Vector2 position,int size)
{
    return (Rectangle) { position.x - size / 2, position.y - size / 2, size, size };
}

GridPosition WorldToCell(Vector2 worldPosition)
{
    GridPosition gridPostion;
    gridPostion.x =(int)( worldPosition.x + GRID_SIZE/2)/ GRID_SIZE;
    gridPostion.y =(int)( worldPosition.y + GRID_SIZE/2)/ GRID_SIZE;
    return gridPostion;
}

void DigGrid()
{
    Tile targetTile = tiles[player.grid.x][player.grid.y + 1];
    if (targetTile.type != Empty)
    {
        digPoint += 1;
        tiles[player.grid.x][player.grid.y + 1].isHollowed = true;
        tiles[player.grid.x][player.grid.y + 1].texture = HollowTile(targetTile.type);
        canDig = false;
        if (targetTile.type == Sand) canDig = true;
    }
}

bool CheckCollision(void)
{
    GridPosition checkGridPos = WorldToCell(player.position);
    player.grid = checkGridPos;
    checkGridPos.x -= 1;
    checkGridPos.y -= 1;

    for (int i = 0; i <= 2; i++) {
        for (int j = 0; j <= 2; j++)
        {
            if ((checkGridPos.x + i) < 0 || (checkGridPos.x + i) >= GRID_COUNT || (checkGridPos.y +j)< 0 || (checkGridPos.y+j) >= GRID_COUNT) continue;
            Tile tile = tiles[checkGridPos.x + i][checkGridPos.y + j];
            if (tile.type == Empty || tile.isHollowed) 
            {
                continue;
            }
            else if (CheckCollisionRecs(ToRectangle(tile.position, GRID_SIZE),ToRectangle( player.position, PLAYER_SIZE)))
            {
                if (canDig)
                {
                    DigGrid();
                }
                else
                {
                    player.position.y = tile.position.y - GRID_SIZE/2 - PLAYER_SIZE/2;
                    return true;
                }
            }
        }
    }
    return false;
}



void UpdatePlayer(Player* player, float delta)
{
    // === Move ===
    if (IsKeyDown(KEY_LEFT)) player->position.x -= PLAYER_HOR_SPD * delta;
    if (IsKeyDown(KEY_RIGHT)) player->position.x += PLAYER_HOR_SPD * delta;
    
    // === Jump ===
    if (IsKeyDown(KEY_SPACE) && player->canJump)
    {
        player->speed = -PLAYER_JUMP_SPD;
        canDig = true;
        player->canJump = false;
    }

    bool colled = CheckCollision(player);

    int hitObstacle = 0;
    if (colled)
    {
        hitObstacle = 1;
        player->speed = 0.0f;
    }
    testflag = colled;

    // === Gravity ===
    if (!hitObstacle)
    {
        if (player->grid.y < GRID_COUNT/3)
        {
            player->position.y += player->speed * delta;
            player->speed += G * delta;
            player->canJump = false;
        }
        else
        {
            player->position.x += player->speed * delta;
            player->speed += G * delta;
            player->canJump = false;

        }
    }
    else
    {
        player->canJump = true;
    }

    // === Dig ===
    if (IsKeyPressed(KEY_DOWN))
    {
        DigGrid();
    }
}

void UpdateCameraCenter(Camera2D* camera, Player* player, EnvItem* envItems, int envItemsLength, float delta, int width, int height)
{
    camera->offset = (Vector2){ width / 2.0f, height / 2.0f };
    camera->target = player->position;
}

void UpdateCameraCenterInsideMap(Camera2D* camera, Player* player, EnvItem* envItems, int envItemsLength, float delta, int width, int height)
{
    camera->target = player->position;
    camera->offset = (Vector2){ width / 2.0f, height / 2.0f };
    float minX = 1000, minY = 1000, maxX = -1000, maxY = -1000;

    for (int i = 0; i < envItemsLength; i++)
    {
        EnvItem* ei = envItems + i;
        minX = fminf(ei->rect.x, minX);
        maxX = fmaxf(ei->rect.x + ei->rect.width, maxX);
        minY = fminf(ei->rect.y, minY);
        maxY = fmaxf(ei->rect.y + ei->rect.height, maxY);
    }

    Vector2 max = GetWorldToScreen2D((Vector2) { maxX, maxY }, * camera);
    Vector2 min = GetWorldToScreen2D((Vector2) { minX, minY }, * camera);

    if (max.x < width) camera->offset.x = width - (max.x - width / 2);
    if (max.y < height) camera->offset.y = height - (max.y - height / 2);
    if (min.x > 0) camera->offset.x = width / 2 - min.x;
    if (min.y > 0) camera->offset.y = height / 2 - min.y;
}

void UpdateCameraCenterSmoothFollow(Camera2D* camera, Player* player, EnvItem* envItems, int envItemsLength, float delta, int width, int height)
{
    static float minSpeed = 30;
    static float minEffectLength = 10;
    static float fractionSpeed = 0.8f;

    camera->offset = (Vector2){ width / 2.0f, height / 2.0f };
    Vector2 diff = Vector2Subtract(player->position, camera->target);
    float length = Vector2Length(diff);

    if (length > minEffectLength)
    {
        float speed = fmaxf(fractionSpeed * length, minSpeed);
        camera->target = Vector2Add(camera->target, Vector2Scale(diff, speed * delta / length));
    }
}

void UpdateCameraEvenOutOnLanding(Camera2D* camera, Player* player, EnvItem* envItems, int envItemsLength, float delta, int width, int height)
{
    static float evenOutSpeed = 700;
    static int eveningOut = false;
    static float evenOutTarget;

    camera->offset = (Vector2){ width / 2.0f, height / 2.0f };
    camera->target.x = player->position.x;

    if (eveningOut)
    {
        if (evenOutTarget > camera->target.y)
        {
            camera->target.y += evenOutSpeed * delta;

            if (camera->target.y > evenOutTarget)
            {
                camera->target.y = evenOutTarget;
                eveningOut = 0;
            }
        }
        else
        {
            camera->target.y -= evenOutSpeed * delta;

            if (camera->target.y < evenOutTarget)
            {
                camera->target.y = evenOutTarget;
                eveningOut = 0;
            }
        }
    }
    else
    {
        if (player->canJump && (player->speed == 0) && (player->position.y != camera->target.y))
        {
            eveningOut = 1;
            evenOutTarget = player->position.y;
        }
    }
}

void UpdateCameraPlayerBoundsPush(Camera2D* camera, Player* player, EnvItem* envItems, int envItemsLength, float delta, int width, int height)
{
    static Vector2 bbox = { 0.2f, 0.2f };

    Vector2 bboxWorldMin = GetScreenToWorld2D((Vector2) { (1 - bbox.x) * 0.5f * width, (1 - bbox.y) * 0.5f * height }, * camera);
    Vector2 bboxWorldMax = GetScreenToWorld2D((Vector2) { (1 + bbox.x) * 0.5f * width, (1 + bbox.y) * 0.5f * height }, * camera);
    camera->offset = (Vector2){ (1 - bbox.x) * 0.5f * width, (1 - bbox.y) * 0.5f * height };

    if (player->position.x < bboxWorldMin.x) camera->target.x = player->position.x;
    if (player->position.y < bboxWorldMin.y) camera->target.y = player->position.y;
    if (player->position.x > bboxWorldMax.x) camera->target.x = bboxWorldMin.x + (player->position.x - bboxWorldMax.x);
    if (player->position.y > bboxWorldMax.y) camera->target.y = bboxWorldMin.y + (player->position.y - bboxWorldMax.y);
}
