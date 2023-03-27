# ABCJam

to compile on windows: 
gcc -I include TLER.c -o TLER -Wall -L lib -lpdcurses -static

to compile on mac (need to install ncurses using Homebrew first):
gcc TLER-mac.c -Wall -lncurses -o TLER-mac
