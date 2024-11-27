#include <ncurses.h>
#include <unistd.h>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

#define DELAY 100000
#define MAX_STARS 20

struct Star {
    int x, y;
    bool active;
};

void animateText(const char* text) {
    clear();
    mvprintw(LINES / 2, (COLS - strlen(text)) / 2, text);
    refresh();
    usleep(1000000);
}

void initStars(Star stars[], int max) {
    for (int i = 0; i < max; i++) {
        stars[i].x = rand() % COLS;
        stars[i].y = 0;
        stars[i].active = true;
    }
}

void saveScore(int score) {
    ofstream file("highscore.txt", ios::app);
    file << "Score: " << score << endl;
    file.close();
}

