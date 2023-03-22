#include <curses.h>
#include <time.h>
#include <stdlib.h>

#define PLAT_OFFSET 9   // platform offset
#define WIDTH 3         //  width and height of player
#define HEIGHT 2
#define WIN_WIDTH 110      // width/height of window
#define WIN_HEIGHT 26
#define LOG_AMNT 15     

typedef struct {
    int length;
    int tail;
    double delay;
    clock_t time_start;
    int xpos;
} log;

int main()
{
    srand(time(NULL));                  // time vars
    clock_t start = clock(), current;  // Get the start time
    double elapsed;

    int logLen[] = {7, 8, 9, 10, 11, 12};  // list of possible log lengths
    double delayList[] = {0.2, 0.3, 0.4}; // list of possible delay times (between each log movement, in seconds, basically changes speed)
    int downLogX = 11;  // starting x pos of first down-going log
    int upLogX = 14;    // starting x pos of first up-going log
    log downLogs[LOG_AMNT];
    log upLogs[LOG_AMNT];

    int i, j, x = 4, y = 11; // (x, y) = top left corner of player character
    int bad = 0;
    char ch = 0;

    // logs are printed alternately, going down, going up, going down, going up, etc.
    for (i = 0; i < LOG_AMNT; i++)  // initialize log objects
    {
        downLogs[i].length = logLen[rand() % sizeof(logLen)/sizeof(logLen[0])];         // logs that go down
        downLogs[i].tail = -downLogs[i].length;
        downLogs[i].delay = delayList[rand() % sizeof(delayList)/sizeof(delayList[0])];
        downLogs[i].time_start = start;
        downLogs[i].xpos = downLogX;

        upLogs[i].length = logLen[rand() % sizeof(logLen)/sizeof(logLen[0])];           // logs that go up
        upLogs[i].tail = WIN_HEIGHT + upLogs[i].length - 1;
        upLogs[i].delay = delayList[rand() % sizeof(delayList)/sizeof(delayList[0])];
        upLogs[i].time_start = start;
        upLogs[i].xpos = upLogX;

        downLogX += WIDTH*2;
        upLogX += WIDTH*2;
    }

    // Initialize curses mode
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE); 
    curs_set(0);
    nodelay(stdscr, TRUE);

    // Create a new window with dimensions WIN_HEIGHTxWIN_WIDTH at position (5, 1) (x, y)
    WINDOW *win = newwin(WIN_HEIGHT, WIN_WIDTH, 1, 5); 

    while (ch != 'q') // quit key
    {
        current = clock(); // get current time
        werase(win);    // clear window, similar to system("cls")
        box(win, 0, 0); // draw box on borders of window

        // draw start and end platforms
        for (i = 1; i < getmaxy(win)-1; i++)
            mvwprintw(win, i, PLAT_OFFSET, "|");
        for (i = 1; i < getmaxy(win)-1; i++)
            mvwprintw(win, i, getmaxx(win)-PLAT_OFFSET-1, "|");

        for (i = 0; i < LOG_AMNT; i++)  // down log behaviour
        {
            elapsed = (double) (current - downLogs[i].time_start) / CLOCKS_PER_SEC;
            if (elapsed >= downLogs[i].delay) // right-hand integer = how many seconds between movements
            {
                downLogs[i].time_start = current;
                downLogs[i].tail++;
            }
            for (j = 0; j < downLogs[i].length && downLogs[i].tail+j < WIN_HEIGHT-1; j++) // print tail to head
                mvwprintw(win, downLogs[i].tail+j, downLogs[i].xpos, "|");
            for (;j < downLogs[i].length; j++)      // print extra segments at start again
                mvwprintw(win, downLogs[i].length-j-1, downLogs[i].xpos, "|");

            if (downLogs[i].tail > WIN_HEIGHT-2) downLogs[i].tail = 0;
        }
        for (i = 0; i < LOG_AMNT; i++)  // up log behaviour
        {
            elapsed = (double) (current - upLogs[i].time_start) / CLOCKS_PER_SEC;
            if (elapsed >= upLogs[i].delay) // right-hand integer = how many seconds between movements
            {
                upLogs[i].time_start = current;
                upLogs[i].tail--;
            }
            for (j = 0; j < upLogs[i].length && upLogs[i].tail-j >= 1; j++) // print tail to head
                mvwprintw(win, upLogs[i].tail-j, upLogs[i].xpos, "|");
            for (; j < upLogs[i].length; j++)       // print extra segments at start again
                mvwprintw(win, WIN_HEIGHT-(upLogs[i].length-j), upLogs[i].xpos, "|");
            
            if (upLogs[i].tail < 1) upLogs[i].tail = WIN_HEIGHT-1;

        }

        // get user input for movement
        ch = getch();
        switch (ch) 
        {
            case 'w': y -= y - HEIGHT < 1 ? 0 : HEIGHT; break;
            case 's': y += y + HEIGHT >= WIN_HEIGHT-1 ? 0 : HEIGHT; break;
            case 'a': x -= x - WIDTH < 1 ? 0 : WIDTH; break;
            case 'd': x += x + WIDTH >= WIN_WIDTH-1 ? 0 : WIDTH; break;
        }

        // draw player at (x, y)
        bad = 0;
        for (i = y; i < y + HEIGHT; i++)
            for (j = x; j < x + WIDTH; j++)
            {
                bad += (mvwinch(win, i, j) & A_CHARTEXT) == ' ' ? 1 : 0; // for checking if next step results in game over
                mvwprintw(win, i, j, "O");                               // game over = all empty spaces beneath player
            }

        // Refresh the screen to display the window
        refresh();      
        wrefresh(win);  

        // if player died, game over, break out of loop
        if (bad == WIDTH * HEIGHT && x > PLAT_OFFSET && x < getmaxx(win)-PLAT_OFFSET-1)
        {
            mvwprintw(win, WIN_HEIGHT/2, WIN_WIDTH/2-5, "GAME OVER!");
            mvwprintw(win, WIN_HEIGHT/2+1, WIN_WIDTH/2-11, "Press any key to exit");
            refresh();      
            wrefresh(win);  
            nodelay(stdscr, FALSE);
            getch();
            break;
        }
    }
    // Clean up and exit curses mode
    delwin(win); 
    endwin();
    return 0;
}


