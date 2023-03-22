#include <curses.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define PLAT_OFFSET 9   // platform offset
#define WIDTH 3         //  width and height of player
#define HEIGHT 2
#define WIN_WIDTH 74      // width/height of window
#define WIN_HEIGHT 18
#define LOG_AMNT 18     

typedef struct {
    int length;
    int tail;
    double delay;
    clock_t time_start;
    int xpos;
    int down;
} log;


void initializeLogs(log logs[], clock_t start, int logLen[], double delayList[], int logLenLen, int delayListLen)
{
    int i;
    int downLogX = 10;  // starting x pos of first log

    // initialize log objects
    // logs are printed alternately, going down, going up, going down, going up, etc.
    for (i = 0; i < LOG_AMNT; i++)  
    {
        logs[i].length = logLen[rand() % logLenLen];         // logs that go down
        logs[i].down = i % 2 == 0 ? 1 : 0;
        logs[i].tail = logs[i].down ? 1 : WIN_HEIGHT - 2;
        logs[i].delay = delayList[rand() % delayListLen];
        logs[i].time_start = start;
        logs[i].xpos = downLogX;

        downLogX += WIDTH;
    }
}

int main()
{
    srand(time(NULL));                  // time vars
    clock_t start = clock(), current;  // Get the start time
    double elapsed;
    
    log logs[LOG_AMNT];
    int logLen[] = {4, 6, 8, 10};  // list of possible log lengths
    int logLenLen = sizeof(logLen)/sizeof(logLen[0]);
    double delayList[] = {0.3, 0.4, 0.5, 0.6}; // list of possible delay times (between each log movement, in seconds, basically changes speed)
    int delayListLen = sizeof(delayList)/sizeof(delayList[0]);
    int score = 0;

    initializeLogs(logs, start, logLen, delayList, logLenLen, delayListLen);

    int i, j, x = 4, y = 9; // (x, y) = top left corner of player character
    int bad = 0, good = 0;
    char ch = 0;

    // Initialize curses mode
    initscr();
    cbreak();
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_YELLOW); // player
    init_pair(2, COLOR_CYAN, COLOR_CYAN); // log 1
    init_pair(3, COLOR_BLUE, COLOR_BLUE); // log 2
    init_pair(4, COLOR_GREEN, COLOR_GREEN); // start end platforms
    noecho();                               // do not print characters that user typed out
    keypad(stdscr, TRUE); 
    curs_set(0);                            // hide cursor
    nodelay(stdscr, TRUE);                // dont wait for user input to execute other code
    //bkgd(COLOR_PAIR(1));

    // Create a new window with dimensions WIN_HEIGHTxWIN_WIDTH at position 
    WINDOW *win = newwin(WIN_HEIGHT, WIN_WIDTH, 3, 1); 
   // wbkgd(win, COLOR_PAIR(4));

    while (ch != 'q') // quit key
    {
        current = clock(); // get current time
        werase(win);    // clear window
        box(win, 0, 0); // draw box on borders of window
        mvprintw(0, 0, "Score: %d", score);
        mvprintw(1, 0, "Use 'wasd' to move around, press 'q' to quit");
        mvprintw(2, 0, "Get to the other side using the elevators, don't fall");

        // draw start and end platforms
        wattron(win, COLOR_PAIR(4));        // for color

        for (i = 1; i < getmaxy(win)-1; i++)
            mvwprintw(win, i, 1, "SSSSSSSSS");
        for (i = 1; i < getmaxy(win)-1; i++)
            mvwprintw(win, i, getmaxx(win)-PLAT_OFFSET-1, "EEEEEEEEE");

        wattroff(win, COLOR_PAIR(4));       // for color

        // draw elevator lines
        for (i = 11; i < getmaxx(win)-PLAT_OFFSET-1; i+=WIDTH)
            for (j = 1; j < WIN_HEIGHT-1; j++)
                mvwprintw(win, j, i, "|");
        
        // log behaviour
        for (i = 0; i < LOG_AMNT; i++)  
        {
            elapsed = (double) (current - logs[i].time_start) / CLOCKS_PER_SEC;
            if (elapsed >= logs[i].delay) // right-hand number = how many seconds between movements
            {
                logs[i].time_start = current;
                if (logs[i].down)
                    logs[i].tail++;
                else 
                    logs[i].tail--;
            }
            if (logs[i].down)
            {
                if (logs[i].tail + logs[i].length == WIN_HEIGHT)
                {
                    logs[i].down = 0;
                    logs[i].tail = WIN_HEIGHT - 3;
                }
            }
            else
            {
                if (logs[i].tail - logs[i].length == 0)
                {
                    logs[i].down = 1;
                    logs[i].tail = 1;
                }
            } 

            // draw logs
            wattron(win, i % 2 == 0 ? COLOR_PAIR(2) : COLOR_PAIR(3));
            if (logs[i].down)
                for (j = 0; j < logs[i].length; j++) // print tail to head
                    mvwprintw(win, logs[i].tail+j, logs[i].xpos, "LLL");
            else
                for (j = 0; j < logs[i].length && logs[i].tail-j >= 1; j++) // print tail to head
                    mvwprintw(win, logs[i].tail-j, logs[i].xpos, "LLL");
            wattroff(win, i % 2 == 0 ? COLOR_PAIR(2) : COLOR_PAIR(3));
        }

        // get user input for movement
        ch = getch();
        switch (ch) 
        {
            case 'w': y -= y - 1 < 1 ? 0 : 1; break;                // ternary operator in c
            case 's': y += y + 1 >= WIN_HEIGHT-2 ? 0 : 1; break;
            case 'a': x -= x - WIDTH < 1 ? 0 : WIDTH; break;
            case 'd': x += x + WIDTH >= WIN_WIDTH-1 ? 0 : WIDTH; break;
        }

        // update player
        bad = 0;
        good = 0;
        int log = 0;
        for (i = y; i < y + HEIGHT; i++)
            for (j = x; j < x + WIDTH; j++)
            {

                bad += (mvwinch(win, i, j) & A_CHARTEXT) == '|' ? 1 : 0; // for checking if next step results in game over ; game over = all empty spaces beneath player
                good += (mvwinch(win, i, j) & A_CHARTEXT) == 'E' ? 1 : 0; // for checking if next step results in win ; win == step on E
                log += (mvwinch(win, i, j) & A_CHARTEXT) == 'L' ? 1 : 0; // for checking if next step results in log
            }
        if (log != 0)   // snapping player to log
        {
            for (i = 0; i < LOG_AMNT; i++)
            {
                if (x == logs[i].xpos)
                {
                    if (logs[i].down) y = (logs[i].tail + logs[i].length / 2) - 1;
                    else y = logs[i].tail - logs[i].length / 2;
                }
            }
        }

        for (i = y; i < y + HEIGHT; i++)    // draw player at x, y
            for (j = x; j < x + WIDTH; j++)
            {
                wattron(win, COLOR_PAIR(1));
                mvwprintw(win, i, j, "O");                               
                wattroff(win, COLOR_PAIR(1));
            }

        // Refresh the screen to display the window
        refresh();      
        wrefresh(win);  

        // if player died, game over, break out of loop
        if (bad != 0 && log == 0)
        {
            mvwprintw(win, WIN_HEIGHT/2, WIN_WIDTH/2-5, "GAME OVER!");
            mvwprintw(win, WIN_HEIGHT/2+1, WIN_WIDTH/2-11, "Press any key to exit");
            refresh();      
            wrefresh(win);  
            nodelay(stdscr, FALSE);
            getch();
            break;
        }

        // if player got to the end
        if (good != 0)
        {
            mvwprintw(win, WIN_HEIGHT/2, WIN_WIDTH/2-12, "WIN! ONTO THE NEXT ROUND");
            mvwprintw(win, WIN_HEIGHT/2+1, WIN_WIDTH/2-12, "Press any key to continue");
            refresh();      
            wrefresh(win);  
            nodelay(stdscr, FALSE);
            getch();
            nodelay(stdscr, TRUE);
            x = 4; 
            y = 9;
            for (i = 0; i < logLenLen; i++)         // shorten possible log lengths
                if (logLen[i] > 4) logLen[i] -= 2;
            for (i = 0; i < delayListLen; i++)              // make logs move faster
                if (delayList[i] > 0.1) delayList[i] -= 0.1;
            score++;
            initializeLogs(logs, start, logLen, delayList, logLenLen, delayListLen);
        }
    }
    // Clean up and exit curses mode
    delwin(win); 
    endwin();
    return 0;
}



