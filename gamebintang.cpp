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

int main() {
    srand(time(0));
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    animateText("Selamat Datang di Game Pengambil Bintang");

    int player_x = COLS / 2;
    int score = 0;
    Star stars[MAX_STARS];
    initStars(stars, MAX_STARS);

    while (true) {
        clear();

        int ch = getch();
        if (ch == KEY_LEFT && player_x > 0) player_x--;
        else if (ch == KEY_RIGHT && player_x < COLS - 1) player_x++;
        else if (ch == 'q') break;

        mvprintw(LINES - 1, player_x, "A");
