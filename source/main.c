// main.c
// Nintendo DS Bloxorz-Style Puzzle Prototype
// D-Pad only controls
// START = Pause
// A = Confirm
// B = Back

#include <nds.h>
#include <stdio.h>
#include <stdbool.h>

#define MAP_W 10
#define MAP_H 10

typedef enum
{
    STAND,
    LAY_X,
    LAY_Y
} BlockState;

typedef struct
{
    int x;
    int y;
    BlockState state;
} Block;

typedef enum
{
    GAME,
    PAUSE
} GameMode;

GameMode mode = GAME;

int pauseOption = 0;

Block player = {2,2,STAND};

char level[MAP_H][MAP_W + 1] =
{
    "0000000000",
    "0011111100",
    "0011111100",
    "0011000100",
    "001111G100",
    "0001111100",
    "0001111100",
    "0000000000",
    "0000000000",
    "0000000000"
};

bool validTile(int x, int y)
{
    if(x < 0 || y < 0 || x >= MAP_W || y >= MAP_H)
        return false;

    return level[y][x] != '0';
}

bool checkPosition()
{
    switch(player.state)
    {
        case STAND:
            return validTile(player.x, player.y);

        case LAY_X:
            return validTile(player.x, player.y) &&
                   validTile(player.x + 1, player.y);

        case LAY_Y:
            return validTile(player.x, player.y) &&
                   validTile(player.x, player.y + 1);
    }

    return false;
}

void resetPlayer()
{
    player.x = 2;
    player.y = 2;
    player.state = STAND;
}

void moveLeft()
{
    switch(player.state)
    {
        case STAND:
            player.x -= 2;
            player.state = LAY_X;
            break;

        case LAY_X:
            player.x -= 1;
            player.state = STAND;
            break;

        case LAY_Y:
            player.x -= 1;
            break;
    }
}

void moveRight()
{
    switch(player.state)
    {
        case STAND:
            player.x += 1;
            player.state = LAY_X;
            break;

        case LAY_X:
            player.x += 2;
            player.state = STAND;
            break;

        case LAY_Y:
            player.x += 1;
            break;
    }
}

void moveUp()
{
    switch(player.state)
    {
        case STAND:
            player.y -= 2;
            player.state = LAY_Y;
            break;

        case LAY_Y:
            player.y -= 1;
            player.state = STAND;
            break;

        case LAY_X:
            player.y -= 1;
            break;
    }
}

void moveDown()
{
    switch(player.state)
    {
        case STAND:
            player.y += 1;
            player.state = LAY_Y;
            break;

        case LAY_Y:
            player.y += 2;
            player.state = STAND;
            break;

        case LAY_X:
            player.y += 1;
            break;
    }
}

void drawGame()
{
    consoleClear();

    for(int y = 0; y < MAP_H; y++)
    {
        for(int x = 0; x < MAP_W; x++)
        {
            bool drawBlock = false;

            switch(player.state)
            {
                case STAND:
                    if(x == player.x && y == player.y)
                        drawBlock = true;
                    break;

                case LAY_X:
                    if((x == player.x || x == player.x + 1) && y == player.y)
                        drawBlock = true;
                    break;

                case LAY_Y:
                    if(x == player.x && (y == player.y || y == player.y + 1))
                        drawBlock = true;
                    break;
            }

            if(drawBlock)
                iprintf("B");
            else
                iprintf("%c", level[y][x]);
        }

        iprintf("\n");
    }

    iprintf("\nSTART = Pause");
}

void drawPause()
{
    consoleClear();

    iprintf("== PAUSED ==\n\n");

    if(pauseOption == 0)
        iprintf("> Resume\n");
    else
        iprintf("  Resume\n");

    if(pauseOption == 1)
        iprintf("> Quit Level\n");
    else
        iprintf("  Quit Level\n");
}

int main()
{
    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);
    consoleInit(NULL, 3, BgType_Text4bpp, BgSize_Text_256x256, 15, 0, true, true);

    while(1)
    {
        swiWaitForVBlank();
        scanKeys();

        uint32 keysDown = keysDown();

        if(mode == GAME)
        {
            if(keysDown & KEY_LEFT)
            {
                moveLeft();
                if(!checkPosition())
                    resetPlayer();
            }
            if(keysDown & KEY_RIGHT)
            {
                moveRight();
                if(!checkPosition())
                    resetPlayer();
            }
            if(keysDown & KEY_UP)
            {
                moveUp();
                if(!checkPosition())
                    resetPlayer();
            }
            if(keysDown & KEY_DOWN)
            {
                moveDown();
                if(!checkPosition())
                    resetPlayer();
            }
            if(keysDown & KEY_START)
            {
                mode = PAUSE;
                pauseOption = 0;
            }

            drawGame();
        }
        else if(mode == PAUSE)
        {
            if(keysDown & KEY_UP)
                pauseOption = 0;
            if(keysDown & KEY_DOWN)
                pauseOption = 1;
            if(keysDown & KEY_A)
            {
                if(pauseOption == 0)
                    mode = GAME;
                else
                    resetPlayer();
            }

            drawPause();
        }
    }

    return 0;
}
