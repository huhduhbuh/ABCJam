# ABCJam

to compile on windows: 
gcc -I include FCTR.c -o FCTR -Wall -L lib -lpdcurses -static

to compile on mac (need to install ncurses using Homebrew first):
gcc FCTR.c -Wall -lncurses -o FCTR
