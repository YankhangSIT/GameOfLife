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

float wWidth, wHeight; // window width and height
float cellWidth, cellHeight; // each cell's width and height

CP_Color black; // alive cells
CP_Color turqoise; // dead cells

int i, j; // for checking neighbours
int gridNo; // reference or display grid

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

    // setting of window sizes
    wWidth = 2 * CP_System_GetWindowWidth();
    wHeight = 2 * CP_System_GetWindowHeight();

    // defining cell width and height
    cellWidth = wWidth / GOL_GRID_COLS;
    cellHeight = wHeight / GOL_GRID_ROWS;

    // set window size rectmode corner so my grids start printing from top left corner
    CP_System_SetWindowSize(wWidth, wHeight);
    CP_Settings_RectMode(CP_POSITION_CORNER);

    // define colours for alive and dead cells respectively
    black = CP_Color_Create(0, 0, 0, 255);
    turqoise = CP_Color_Create(64, 224, 208, 255);
}

void game_update(void)
{
    // interswaps every frame
    int gridNo = CP_System_GetFrameCount() % 2;

    CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));

    // to pause the game
    if (CP_Input_KeyTriggered(KEY_ANY)) {
        gIsPaused = !gIsPaused;
    }

    if (gIsPaused == TRUE) { // if paused
        if (CP_Input_MouseClicked()) {
            for (int rows = 0; rows < GOL_GRID_ROWS; rows++) {
                for (int cols = 0; cols < GOL_GRID_COLS; cols++) { // for every cell
                    CP_Vector mouseClickPos = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());

                    float cellX = cellWidth * cols; // find the start of the cell on the x axis
                    float cellY = cellHeight * rows; // find the start of the cell on the y axis

                    if ((mouseClickPos.x > cellX && mouseClickPos.x < cellX + cellWidth) &&
                        (mouseClickPos.y > cellY && mouseClickPos.y < cellY + cellHeight)) {
                        gGrids[gridNo][rows][cols] = !(gGrids[gridNo][rows][cols]); // change cell state
                    }
                }
            }
        }
    }

    else if (gIsPaused == FALSE) { // if unpaused
        for (int rows = 0; rows < GOL_GRID_ROWS; rows++) {
            for (int cols = 0; cols < GOL_GRID_COLS; cols++) { // for every cell
                int sumOfLive = 0; // counter for number of live neighbours
                for (i = -1; i < 2; i++) { // counts rows-1 to rows + 1 
                    for (j = -1; j < 2; j++) { // counts columns -1 to columns+1
                        if (rows + i == -1 || rows + i == 30 || cols + j == -1 || cols + j == 30) { // dont check invalid grids
                            continue;
                        }
                        else if (gGrids[!gridNo][rows + i][cols + j] == 1) { // check alive neighbours
                            sumOfLive += 1; // add to counter
                        };
                    }
                }
                sumOfLive -= gGrids[!gridNo][rows][cols]; // doesnt count itself

                if (gGrids[!gridNo][rows][cols] == GOL_DEAD && sumOfLive == 3) { // if cell is dead and it has 3 neighbours
                    gGrids[gridNo][rows][cols] = GOL_ALIVE; // it comes alive :)
                }
                else if (gGrids[!gridNo][rows][cols] == GOL_ALIVE && (sumOfLive < 2 || sumOfLive > 3)) { // if cell is alive
                    gGrids[gridNo][rows][cols] = GOL_DEAD; // but has less than 2 neighbours or more than 3, it dies :(
                }
                else {
                    gGrids[gridNo][rows][cols] = gGrids[!gridNo][rows][cols]; // else the cell state stays the same
                }
            }
        }        
    }

    //print grids
    for (int rows = 0; rows < GOL_GRID_ROWS; rows++) {
        for (int cols = 0; cols < GOL_GRID_COLS; cols++) { // for every cell
            float cellX = cellWidth * cols;
            float cellY = cellHeight * rows;

            if (gGrids[gridNo][rows][cols] == GOL_ALIVE) {
                CP_Settings_Fill(black);
                CP_Graphics_DrawRect(cellX, cellY, cellWidth, cellHeight); // draw alive cells
            }
            else {
                CP_Settings_Fill(turqoise);
                CP_Graphics_DrawRect(cellX, cellY, cellWidth, cellHeight); // draw dead cells
            }
            gGrids[!gridNo][rows][cols] = gGrids[gridNo][rows][cols]; // update the grids
        }
    }
}

void game_exit(void)
{

}
