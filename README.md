# ABCJam

main3 is the latest

to compile on windows: 
gcc -I include main3.c -o main3 -Wall -L lib -lpdcurses -static

to compile on mac (need to install ncurses using Homebrew first):
gcc main3.c -Wall -lncurses -o main3
