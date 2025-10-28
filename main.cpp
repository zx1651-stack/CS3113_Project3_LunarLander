/**
* Author: Zirui Xu
* Assignment: Lunar Lander
* Date due: 2025-10-27, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/


#include "CS3113/cs3113.h"
#include "Entity.h"


constexpr int SCREEN_WIDTH = 1200;
constexpr int SCREEN_HEIGHT = 800;
constexpr int FPS = 60;
constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;
constexpr float GRAVITY = 5.0f;
constexpr float HORIZONTAL_ACCEL = 80.0f;
constexpr float BRAKE_FACTOR = 8.0f;
constexpr float FRICTION = 4.0f;
constexpr float FUEL_MAX = 100.0f;
constexpr float FUEL_CONSUME_RATE = 5.0f;
constexpr float PLATFORM_SPEED = 120.0f;
constexpr char BG_COLOUR[] = "#101010";
AppStatus gAppStatus = RUNNING;
float gPreviousTicks = 0.0f;
float gAccumulator = 0.0f;
Entity* rocket;
Entity* platform;
float gFuel = FUEL_MAX;
bool gUsingHorizontalInput = false;
bool gMissionFailed = false;
bool gMissionAccomplished = false;


bool isCollidingAABB(const Vector2& posA, const Vector2& sizeA, const Vector2& posB, const Vector2& sizeB)
{
    bool overlapX = fabs(posA.x - posB.x) <= (sizeA.x + sizeB.x) / 2.0f;
    bool overlapY = fabs(posA.y - posB.y) <= (sizeA.y + sizeB.y) / 2.0f;
    return overlapX && overlapY;
}


void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lunar Lander - Platform Interaction");
    rocket = new Entity({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT * 0.2f }, "assets/rocket.png");
    rocket->setAcceleration({0.0f, GRAVITY});
    rocket->setDrawScale(0.05f);
    platform = new Entity({ SCREEN_WIDTH / 2.0f, 0.0f }, "assets/wooden_bat.png");
    platform->setDrawScale(0.3f);
    Vector2 psize = platform->getSize();
    float platformY = (float)SCREEN_HEIGHT - (psize.y / 2.0f) - 10.0f;
    platform->setPosition({ SCREEN_WIDTH / 2.0f, platformY });
    platform->setVelocity({ -PLATFORM_SPEED, 0.0f });
    platform->setAcceleration({0.0f, 0.0f});
    SetTargetFPS(FPS);
}


void processInput()
{
    if (WindowShouldClose()) gAppStatus = TERMINATED;
    if (gMissionFailed || gMissionAccomplished) return;
    float inputAccelX = 0.0f;
    bool leftDown = IsKeyDown(KEY_LEFT);
    bool rightDown = IsKeyDown(KEY_RIGHT);
    if (leftDown) inputAccelX = -HORIZONTAL_ACCEL;
    else if (rightDown) inputAccelX = HORIZONTAL_ACCEL;
    else inputAccelX = 0.0f;
    Vector2 vel = rocket->getVelocity();
    float brakeAccelX = 0.0f;
    if (inputAccelX != 0.0f && gFuel > 0.0f)
    {
        if (vel.x * inputAccelX < 0.0f)
        {
            brakeAccelX = -vel.x * BRAKE_FACTOR;
        }
        else
        {
            brakeAccelX = 0.0f;
        }
        gUsingHorizontalInput = true;
    }
    else
    {
        brakeAccelX = -vel.x * FRICTION;
        gUsingHorizontalInput = false;
    }
    Vector2 accel = { inputAccelX + brakeAccelX, GRAVITY };
    if (gFuel <= 0.0f)
    {
        accel.x = -vel.x * FRICTION;
        gUsingHorizontalInput = false;
    }
    rocket->setAcceleration(accel);
}


void update()
{
    if (gMissionFailed || gMissionAccomplished) return;
    float ticks = GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;
    deltaTime += gAccumulator;
    if (deltaTime < FIXED_TIMESTEP)
    {
        gAccumulator = deltaTime;
        return;

    }
    while (deltaTime >= FIXED_TIMESTEP)
    {
        rocket->update(FIXED_TIMESTEP);
        platform->update(FIXED_TIMESTEP);
        if (gUsingHorizontalInput && gFuel > 0.0f)
        {
            gFuel -= FUEL_CONSUME_RATE * FIXED_TIMESTEP;
            if (gFuel < 0.0f) gFuel = 0.0f;
        }
        Vector2 ppos = platform->getPosition();
        Vector2 psize = platform->getSize();
        float phalfW = psize.x / 2.0f;
        if (ppos.x - phalfW <= 0.0f)
        {
            ppos.x = phalfW;
            platform->setPosition(ppos);
            Vector2 pv = platform->getVelocity();
            platform->setVelocity({ fabs(pv.x), pv.y });
        }
        if (ppos.x + phalfW >= (float)SCREEN_WIDTH)
        {
            ppos.x = (float)SCREEN_WIDTH - phalfW;
            platform->setPosition(ppos);
            Vector2 pv = platform->getVelocity();
            platform->setVelocity({ -fabs(pv.x), pv.y });
        }
        Vector2 rpos = rocket->getPosition();
        Vector2 rsize = rocket->getSize();
        float rhalfW = rsize.x / 2.0f;
        float rhalfH = rsize.y / 2.0f;
        if (rpos.x - rhalfW <= 0.0f || rpos.x + rhalfW >= (float)SCREEN_WIDTH || rpos.y + rhalfH >= (float)SCREEN_HEIGHT)
        {
            gMissionFailed = true;
        }
        Vector2 pcenter = platform->getPosition();
        Vector2 pcenterSize = platform->getSize();
        if (isCollidingAABB(rpos, rsize, pcenter, pcenterSize))
        {
            if (rpos.y + rsize.y / 2.0f <= platform->getPosition().y) {
                gMissionAccomplished = true;
            }

        }
        deltaTime -= FIXED_TIMESTEP;
    }
    gAccumulator = deltaTime;
}


void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));
    if (!gMissionFailed && !gMissionAccomplished)
    {
        rocket->render();
        platform->render();
        float fuelRatio = gFuel / FUEL_MAX;
        if (fuelRatio < 0.0f) fuelRatio = 0.0f;
        if (fuelRatio > 1.0f) fuelRatio = 1.0f;
        int fuelPercent = (int)(fuelRatio * 100.0f);
        DrawText(TextFormat("Fuel: %d%%", fuelPercent), 20, 20, 20, WHITE);
    }
    else if (gMissionFailed)
    {
        const char* msg = "Mission Failed";
        int fontSize = 48;
        int textWidth = MeasureText(msg, fontSize);
        int x = SCREEN_WIDTH / 2 - textWidth / 2;
        int y = SCREEN_HEIGHT / 2 - fontSize / 2;
        DrawText(msg, x, y, fontSize, WHITE);
    }
    else if (gMissionAccomplished)
    {
        const char* msg = "Mission Accomplished";
        int fontSize = 48;
        int textWidth = MeasureText(msg, fontSize);
        int x = SCREEN_WIDTH / 2 - textWidth / 2;
        int y = SCREEN_HEIGHT / 2 - fontSize / 2;
        DrawText(msg, x, y, fontSize, WHITE);
    }
    EndDrawing();
}


void shutdown()
{
    delete rocket;
    delete platform;
    CloseWindow();
}


int main()
{
    initialise();
    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }
    shutdown();
    return 0;
}





