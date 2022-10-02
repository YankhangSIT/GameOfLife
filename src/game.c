#include <stdio.h>
#include "cprocessing.h"

#define GOL_GRID_COLS 30
#define GOL_GRID_ROWS 30
#define GOL_GRID_BUFFERS 2

#define GOL_ALIVE 1
#define GOL_DEAD 0

#define TRUE 1
#define FALSE 0

int gIsPaused;
int gGrids[GOL_GRID_BUFFERS][GOL_GRID_ROWS][GOL_GRID_COLS];

/* Feel free to declare your own variables here */

float wWidth, wHeight;
float cellWidth, cellHeight;

CP_Color black;
CP_Color turqoise;

int i, j;
int gridNo;

void game_init(void)
{
    /* Set every grids' cells as 'dead' */
    for (int row = 0; row < GOL_GRID_ROWS; ++row) {
        for (int col = 0; col < GOL_GRID_COLS; ++col) {
            for (int i = 0; i < GOL_GRID_BUFFERS; ++i) {
                gGrids[i][row][col] = GOL_DEAD;
            }
        }
    }

    /*********************************************************
    *  Let's start with gGrids[0] as the 'reference grid'
    *  and gGrids[1] as the 'displaying grid'.
    *
    *  We initialize gGrids[0] with a simple 'glider'.
    *********************************************************/

    gGrids[0][1][2] = GOL_ALIVE;
    gGrids[0][2][3] = GOL_ALIVE;
    gGrids[0][3][1] = GOL_ALIVE;
    gGrids[0][3][2] = GOL_ALIVE;
    gGrids[0][3][3] = GOL_ALIVE;

    /* We start unpaused */
    gIsPaused = FALSE;

    /* Initialization of your other variables here */

    //Set window width and height to variables
    wWidth = 2 * CP_System_GetWindowWidth();
    wHeight = 2 * CP_System_GetWindowHeight();

    cellWidth = wWidth / GOL_GRID_COLS;
    cellHeight = wHeight / GOL_GRID_ROWS;

    //set window size and center it
    CP_System_SetWindowSize(wWidth, wHeight);
    CP_Settings_RectMode(CP_POSITION_CORNER);

    black = CP_Color_Create(0, 0, 0, 255);
    turqoise = CP_Color_Create(64, 224, 208, 255);
}

void game_update(void)
{
    int gridNo = CP_System_GetFrameCount() % 2;

    CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));

    if (CP_Input_KeyTriggered(KEY_ANY)) {
        gIsPaused = !gIsPaused;
    }

    if (gIsPaused == TRUE) {
        /*
        for each cell in 'displaying grid':
        if mouse is colliding with cell of 'displaying grid' :
            toggle cell state
        */
        if (CP_Input_MouseClicked()) {
            for (int rows = 0; rows < GOL_GRID_ROWS; rows++) {
                for (int cols = 0; cols < GOL_GRID_COLS; cols++) {
                    CP_Vector mouseClickPos = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());

                    float cellX = cellWidth * cols;
                    float cellY = cellHeight * rows;

                    if ((mouseClickPos.x > cellX && mouseClickPos.x < cellX + cellWidth) &&
                        (mouseClickPos.y > cellY && mouseClickPos.y < cellY + cellHeight)) {
                        //gGrids[!gridNo][rows][cols] = !gGrids[gridNo][rows][cols];
                        gGrids[gridNo][rows][cols] = !(gGrids[gridNo][rows][cols]);
                    }
                }
            }
        }
    }

    else if (gIsPaused == FALSE) {

        /*
            simulation case
            for each cell in 'displaying grid':
            if cell is alive in 'reference grid':
            if cell has less than 2 neighbours:
            cell = dead
            else if cell has more than 3 neighbours in 'reference grid':
            cell = dead
            else:
            if cell is dead:
            if cell has exactly 3 neighbours in 'reference grid':
            cell = alive
            render all cells in 'displaying grid'
        */

        for (int rows = 0; rows < GOL_GRID_ROWS; rows++) {
            for (int cols = 0; cols < GOL_GRID_COLS; cols++) {
                int sumOfLive = 0;
                for (i = -1; i < 2; i++) { //counts rows-1 to rows + 1 
                    for (j = -1; j < 2; j++) { // counts columns -1 to columns+1
                        if (gGrids[!gridNo][rows + i][cols + j] == 1) {
                            sumOfLive += 1;
                        };
                    }
                }
                sumOfLive -= gGrids[!gridNo][rows][cols]; // doesnt count itself

                if (gGrids[!gridNo][rows][cols] == GOL_DEAD && sumOfLive == 3) {
                    gGrids[gridNo][rows][cols] = GOL_ALIVE;
                }
                else if (gGrids[!gridNo][rows][cols] == GOL_ALIVE && (sumOfLive < 2 || sumOfLive > 3)) {
                    gGrids[gridNo][rows][cols] = GOL_DEAD;
                }
                else {
                    gGrids[gridNo][rows][cols] = gGrids[!gridNo][rows][cols];
                }
            }
        }

        
    }

    //print grids
    for (int rows = 0; rows < GOL_GRID_ROWS; rows++) {
        for (int cols = 0; cols < GOL_GRID_COLS; cols++) {
            float cellX = cellWidth * cols;
            float cellY = cellHeight * rows;

            if (gGrids[gridNo][rows][cols] == GOL_ALIVE) {
                CP_Settings_Fill(black);
                CP_Graphics_DrawRect(cellX, cellY, cellWidth, cellHeight);
            }
            else {
                CP_Settings_Fill(turqoise);
                CP_Graphics_DrawRect(cellX, cellY, cellWidth, cellHeight);
            }
            gGrids[!gridNo][rows][cols] = gGrids[gridNo][rows][cols];
        }
    }
}

void game_exit(void)
{

}
