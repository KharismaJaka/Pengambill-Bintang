#include <ncurses.h>
#include <unistd.h>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>
using namespace std;

#define DELAY 100000
#define MAX_STARS 20
#define OPENING_DELAY 30000
#define STAR_SPEED 50000

struct Star {
    int x, y;
    bool active;
};

void setupColors() {
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_BLACK, COLOR_WHITE);
    init_pair(7, COLOR_WHITE, COLOR_BLUE);
    init_pair(8, COLOR_BLACK, COLOR_YELLOW);
}

void animateText(const char* text, int delay, int color) {
    clear();
    attron(COLOR_PAIR(color));
    int len = strlen(text);
    for (int i = 0; i < len; i++) {
        mvprintw(LINES / 2, (COLS - len) / 2 + i, "%c", text[i]);
        refresh();
        usleep(delay);
}
attroff(COLOR_PAIR(color));
refresh();
usleep(1000000);
}

void animateOpening() {
    clear();
    attron(COLOR_PAIR(5));
    for (int i = 0; i < COLS; i++) {
        mvprintw(LINES / 2, i, "=");
        refresh();
        usleep(OPENING_DELAY);
    }
    attroff(COLOR_PAIR(5));
    refresh();
    usleep(500000);

    animateText("SELAMAT DATANG DI GAME PENGAMBIL BINTANG", 50000, 7);
    animateText("MEMUAT...", 100000, 3);
}

void initStars(Star stars[], int max) {
    for (int i = 0; i < max; i++) {
        stars[i].x = rand() % (COLS - 2) + 1;
        stars[i].y = 1;
        stars[i].active = true;
    }
}

void respawnStar(Star& star) {
    star.x = rand() % (COLS - 2) + 1;
    star.y = 1
    star.active = true;
}

void saveScore(int score) {
    ofstream file("highscore.txt", ios::app);
    file << score << score << endl;
    file.close();
}

void showHighScore() {
    clear();
    attron(COLOR_PAIR(3));
    mvprintw(2, (COLS - 18) / 2, " HIGH SCORE ");
    mvprintw(3, (COLS - 18) / 2, "------------");
    attroff(COLOR-PAIR(3));

    ifstream file("highscore.txt");
    int y = 5, score;
    while (file >> score) {
        mvprintw(y++, (COLS - 12) / 2, "Score: %d", score);
    }
    file.close();
    refresh();
    usleep(30000000);
}

void loadingScreen() {
    clear();
    attron(COLOR_PAIR(3));
    mvprintw(LINES / 2 - 1, (COLS - 18) / 2, "Loading Game...");
    attroff(COLOR_PAIR(3));
    


        
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

for (int i = 0; < MAX_STARS; i++) {
    if (stars[i].active) {
        mvprintw(stars[i].y, stars[i].x, "*");
        stars[i].y,++;

        if (stars[i].y = LINES - 1 && stars[i].x = player_x) {
            score++;
            stars[i].active = false;
        }
    }
}

for (int i = 0; i < MAX_STARS; I++) {
    if (!stars[i].active || stars[i].y >= LINES) {
        stars[i].x = rand() % COLS;
        stars[i].y = 0;
        stars[i].active = true;
    }
}

mvprintw(0, 0, "Score: %d, score);
refresh();
usleep(DELAY); 
}



endwin();
saveScore(score);
animaText("Terimakasih telah bermain, score telah tersimpan!");
return 0;
} 
