// main.cpp
#include <nds.h>
#include <stdio.h>

enum Rotation {
    STAND,
    HORIZONTAL,
    VERTICAL
};

struct Block {
    int x;
    int y;
    Rotation rot;
};

Block block;

void drawMap() {

    consoleClear();

    for(int y = 0; y < 10; y++) {

        for(int x = 0; x < 10; x++) {

            bool blockHere = false;

            if(block.rot == STAND) {

                if(x == block.x && y == block.y)
                    blockHere = true;

            } else if(block.rot == HORIZONTAL) {

                if((x == block.x || x == block.x + 1)
                && y == block.y)
                    blockHere = true;

            } else if(block.rot == VERTICAL) {

                if((y == block.y || y == block.y + 1)
                && x == block.x)
                    blockHere = true;
            }

            if(blockHere)
                iprintf("[]");
            else
                iprintf("..");
        }

        iprintf("\n");
    }
}

void moveRight() {

    if(block.rot == STAND) {

        block.x += 1;
        block.rot = HORIZONTAL;

    } else if(block.rot == HORIZONTAL) {

        block.x += 2;
        block.rot = STAND;

    } else {

        block.x += 1;
    }
}

void moveLeft() {

    if(block.rot == STAND) {

        block.x -= 2;
        block.rot = HORIZONTAL;

    } else if(block.rot == HORIZONTAL) {

        block.x -= 1;
        block.rot = STAND;

    } else {

        block.x -= 1;
    }
}

void moveUp() {

    if(block.rot == STAND) {

        block.y -= 2;
        block.rot = VERTICAL;

    } else if(block.rot == VERTICAL) {

        block.y -= 1;
        block.rot = STAND;

    } else {

        block.y -= 1;
    }
}

void moveDown() {

    if(block.rot == STAND) {

        block.y += 1;
        block.rot = VERTICAL;

    } else if(block.rot == VERTICAL) {

        block.y += 2;
        block.rot = STAND;

    } else {

        block.y += 1;
    }
}

int main(void) {

    consoleDemoInit();

    block.x = 4;
    block.y = 4;
    block.rot = STAND;

    while(1) {

        scanKeys();

        int keys = keysDown();

        if(keys & KEY_RIGHT)
            moveRight();

        if(keys & KEY_LEFT)
            moveLeft();

        if(keys & KEY_UP)
            moveUp();

        if(keys & KEY_DOWN)
            moveDown();

        drawMap();

        swiWaitForVBlank();
    }

    return 0;
}

