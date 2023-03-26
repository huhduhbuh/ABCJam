#include <curses.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PLAT_OFFSET 9   // platform offset
#define WIDTH 3         //  width and height of player
#define HEIGHT 2
#define WIN_WIDTH 74      // width/height of window
#define WIN_HEIGHT 18
#define LOG_AMNT 9     


typedef char string[40];

void delay(int milliseconds) {
    clock_t start_time = clock();
    while (clock() < start_time + milliseconds);
}

void draw_elevator() 
{
    int i, j;
    string a =  "||KANTO-TERRORS--IN-ABC-JAM-CS||";
    
    for (i = 15, j = 16; i > 1; i--, j++)
    {
        system("cls");
        a[i] = '[';
        a[j] = ']';
        printf("\033[1;32m%s\033[0m\n", " +============================+");
        printf("\033[1;32m%s\033[0m\n", a);
        printf("\033[1;32m%s\033[0m\n", a);
        printf("\033[1;32m%s\033[0m\n", a);
        printf("\033[1;32m%s\033[0m\n", a);
        printf("\033[1;32m%s\033[0m\n", a);
        printf("\033[1;32m%s\033[0m\n", a);
        printf("\033[1;32m%s\033[0m\n", a);
        printf("\033[1;32m%s\033[0m\n", a);
        printf("\033[1;32m%s\033[0m\n", a);
        printf("\033[1;32m%s\033[0m\n", a);
        printf("\033[1;32m%s\033[0m\n", a);
        printf("\033[1;32m%s\033[0m\n", a);
        printf("\033[1;32m%s\033[0m\n", a);
        printf("\033[1;32m%s\033[0m\n", a);
        printf("\033[1;32m%s\033[0m\n", a);
        printf("\033[1;32m%s\033[0m\n", a);
        printf("\033[1;32m%s\033[0m\n", " +============================+");
        a[i] = ' ';
        a[j] = ' ';

        delay(100);
    }

    //a = "||WELCOME THIS IS A GAME||";
    system("cls");
    printf("\033[1;32m%s\033[0m\n", " +============================+");
    printf("\033[1;32m%s\033[0m\n", a);
    printf("\033[1;32m%s\033[0m\n", a);
    printf("\033[1;32m%s\n", a);
    printf("||\033[1;33m%s\033[1;32m||\n", " \"Froggy Crossing the Road\" ");
    printf("\033[1;32m%s\033[0m\n", a);
    printf("\033[1;32m%s\033[0m\n", a);
    printf("\033[1;32m%s\033[0m\n", a);
    printf("\033[1;32m%s\033[0m\n", "||   WELCOME THIS IS A GAME   ||");
    printf("\033[1;32m%s\033[0m\n", a);
    printf("\033[1;32m%s\033[0m\n", "||      BY KANTO TERRORS      ||");
    printf("\033[1;32m%s\033[0m\n", a);
    printf("\033[1;32m%s\033[0m\n", "||    Press Enter To Start    ||");
    printf("\033[1;32m%s\033[0m\n", a);
    printf("\033[1;32m%s\033[0m\n", a);
    printf("\033[1;32m%s\033[0m\n", a);
    printf("\033[1;32m%s\033[0m\n", a);
    printf("\033[1;32m%s\033[0m\n", " +============================+");

    fflush (stdout);
    getchar();
    while(getchar() != '\n');
}


typedef struct {
    int length;
    int tail;
    double delay;
    clock_t time_start;
    int xpos;
} log;


void initializeLogs(log downLogs[], log upLogs[], clock_t start, int logLen[], double delayList[], int logLenLen, int delayListLen)
{
    int i;
    int downLogX = 10;  // starting x pos of first down-going log
    int upLogX = 13;    // starting x pos of first up-going log

    // initialize log objects
    // logs are printed alternately, going down, going up, going down, going up, etc.
    for (i = 0; i < LOG_AMNT; i++)  
    {
        downLogs[i].length = logLen[rand() % logLenLen];         // logs that go down
        downLogs[i].tail = -downLogs[i].length;
        downLogs[i].delay = delayList[rand() % delayListLen];
        downLogs[i].time_start = start;
        downLogs[i].xpos = downLogX;

        upLogs[i].length = logLen[rand() % logLenLen];           // logs that go up
        upLogs[i].tail = WIN_HEIGHT + upLogs[i].length - 1;
        upLogs[i].delay = delayList[rand() % delayListLen];
        upLogs[i].time_start = start;
        upLogs[i].xpos = upLogX;

        downLogX += WIDTH*2;
        upLogX += WIDTH*2;
    }
}

int main()
{
    system("cls");
    int choice;

    printf("\nHi player!\n");
    printf("\nType 1 if you are ready to play the game: ");
    scanf("%d", &choice);

    if (choice != 1)
        return 0;

    draw_elevator();

    srand(time(NULL));                  // time vars
    clock_t start = clock(), current;  // Get the start time
    double elapsed;
    
    log downLogs[LOG_AMNT];
    log upLogs[LOG_AMNT];
    int logLen[] = {5, 6, 7, 8, 9, 10};  // list of possible log lengths
    double delayList[] = {0.2, 0.3, 0.4, 0.5}; // list of possible delay times (between each log movement, in seconds, basically changes speed)

    int logLenLen = sizeof(logLen)/sizeof(logLen[0]);
    int delayListLen = sizeof(delayList)/sizeof(delayList[0]);
    int score = 0;

    initializeLogs(downLogs, upLogs, start, logLen, delayList, logLenLen, delayListLen);

    int i, j, x = 4, y = 9; // (x, y) = top left corner of player character
    int bad = 0, good = 0, log = 0, index;
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
        mvprintw(1, 0, "Use WASD to move around, press Q to quit");
        mvprintw(2, 0, "Get to the other side using the elevators, don't fall");

        // draw start and end platforms
        wattron(win, COLOR_PAIR(4));

        for (i = 1; i < getmaxy(win)-1; i++)
            mvwprintw(win, i, 1, "SSSSSSSSS");
        for (i = 1; i < getmaxy(win)-1; i++)
            mvwprintw(win, i, getmaxx(win)-PLAT_OFFSET-1, "EEEEEEEEE");

        wattroff(win, COLOR_PAIR(4));

        // draw elevator lines
        for (i = 11; i < getmaxx(win)-PLAT_OFFSET-1; i+=WIDTH)
            for (j = 1; j < WIN_HEIGHT-1; j++)
                mvwprintw(win, j, i, "|");

        // down log behaviour
        for (i = 0; i < LOG_AMNT; i++)  
        {
            elapsed = (double) (current - downLogs[i].time_start) / CLOCKS_PER_SEC;
            if (elapsed >= downLogs[i].delay) // right-hand number = how many seconds between movements
            {
                downLogs[i].time_start = current;
                downLogs[i].tail++;
                if (x == downLogs[i].xpos && y + 1 < WIN_HEIGHT-2 )y += 1;
            }
            wattron(win, COLOR_PAIR(2));

            for (j = 0; j < downLogs[i].length && downLogs[i].tail+j < WIN_HEIGHT-1; j++) // print tail to head
                mvwprintw(win, downLogs[i].tail+j, downLogs[i].xpos, "LLL");
            for (;j < downLogs[i].length; j++)      // print extra segments at start again
                mvwprintw(win, downLogs[i].length-j-1, downLogs[i].xpos, "LLL");

            wattroff(win, COLOR_PAIR(2));

            if (downLogs[i].tail > WIN_HEIGHT-2) downLogs[i].tail = 0;
        }

        // up log behaviour
        for (i = 0; i < LOG_AMNT; i++)  
        {
            elapsed = (double) (current - upLogs[i].time_start) / CLOCKS_PER_SEC;
            if (elapsed >= upLogs[i].delay) // right-hand number = how many seconds between movements
            {
                upLogs[i].time_start = current;
                upLogs[i].tail--;
                if (x == upLogs[i].xpos && y - 1 >= 1) y -= 1;
            }
            wattron(win, COLOR_PAIR(3));

            for (j = 0; j < upLogs[i].length && upLogs[i].tail-j >= 1; j++) // print tail to head
                mvwprintw(win, upLogs[i].tail-j, upLogs[i].xpos, "LLL");
            for (; j < upLogs[i].length; j++)       // print extra segments at start again
                mvwprintw(win, WIN_HEIGHT-(upLogs[i].length-j), upLogs[i].xpos, "LLL");

            wattron(win, COLOR_PAIR(3));
            
            if (upLogs[i].tail < 1) upLogs[i].tail = WIN_HEIGHT-1;

        }

        // get user input for movement
        ch = tolower(getch());
        switch (ch) 
        {
            case 'w': y -= y - 1 < 1 ? 0 : 1; break;                // ternary operator in c
            case 's': y += y + 1 >= WIN_HEIGHT-2 ? 0 : 1; break;
            case 'a': x -= x - WIDTH < 1 ? 0 : WIDTH; break;
            case 'd': x += x + WIDTH >= WIN_WIDTH-1 ? 0 : WIDTH; break;
        }

        // draw player at (x, y)
        bad = 0;
        good = 0;
        log = 0;
        for (i = y; i < y + HEIGHT; i++)
            for (j = x; j < x + WIDTH; j++)
            {
                wattron(win, COLOR_PAIR(1));
                bad += (mvwinch(win, i, j) & A_CHARTEXT) == '|' ? 1 : 0; // for checking if next step results in game over ; game over = all empty spaces beneath player
                good += (mvwinch(win, i, j) & A_CHARTEXT) == 'E' ? 1 : 0; // for checking if next step results in win ; win == step on E
                log += (mvwinch(win, i, j) & A_CHARTEXT) == 'L' ? 1 : 0; // for checking if next step results in log
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
            mvwprintw(win, WIN_HEIGHT/2+1, WIN_WIDTH/2-8, "Press Q to quit");
            refresh();      
            wrefresh(win);  
            nodelay(stdscr, FALSE);
            while(tolower(getch())!='q');
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

            x = 4;          // reset player
            y = 9;
            index = rand()%logLenLen;
            if (logLen[index] > 5) logLen[index] -= 1;  // update possible log lengths
            for (i = 0; i < delayListLen; i++)              // make logs move faster
                if (delayList[i] > 0.2) delayList[i] -= 0.01;

            initializeLogs(downLogs, upLogs, start, logLen, delayList, logLenLen, delayListLen);
            score++;
            getch();
            nodelay(stdscr, TRUE);
        }
    }
    // Clean up and exit curses mode
    delwin(win); 
    endwin();
    return 0;
}


