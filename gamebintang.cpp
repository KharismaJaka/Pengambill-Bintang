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
    star.y = 1;
    star.active = true;
}

void saveScore(int score) {
    ofstream file("highscore.txt", ios::app);
    file << score << endl;
    file.close();
}

void showHighScore() {
    clear();
    attron(COLOR_PAIR(3));
    mvprintw(2, (COLS - 18) / 2, " HIGH SCORES ");
    mvprintw(3, (COLS - 18) / 2, "-------------");
    attroff(COLOR_PAIR(3));

    ifstream file("highscore.txt");
    int y = 5, score;
    while (file >> score) {
        mvprintw(y++, (COLS - 12) / 2, "Score: %d", score);
    }
    file.close();
    refresh();
    usleep(3000000);
}

void loadingScreen() {
    clear();
    attron(COLOR_PAIR(3));
    mvprintw(LINES / 2 - 1, (COLS - 18) / 2, "Loading Game...");
    attroff(COLOR_PAIR(3));
    for (int i = 1; i < COLS - 1; i++) {
        attron(COLOR_PAIR(2));
        mvprintw(LINES / 2, i, "=");
        attroff(COLOR_PAIR(2));
        refresh();
        usleep(15000);
    }
    usleep(500000);
}

void createDefaultUserFile() {
    ofstream file("users.txt");
    file << "admin admin123\n";
    file << "user1 password1\n";
    file << "user2 password2\n";
    file.close();
}

bool login() {
    char username[20], password[20], file_username[20], file_password[20];
    bool logged_in = false;

    while (!logged_in) {
        clear();
        attron(COLOR_PAIR(1));

        mvprintw(5, 10, "Username: ");
        echo();
        int i = 0;
        while (true) {
            char ch = getch();
            if (ch == '\n' || ch == '\r') break;
            if (ch == KEY_BACKSPACE || ch == 127) {
                if (i > 0) {
                    i--;
                    mvprintw(5, 20 + i, " ");
                }
            } else if (ch >= 32 && ch <= 126) {
                username[i++] = ch;
                mvprintw(5, 20 + i - 1, "%c", ch);
            }
        }
        username[i] = '\0';

        mvprintw(6, 10, "Password: ");
        noecho();
        i = 0;
        while (true) {
            char ch = getch();
            if (ch == '\n' || ch == '\r') break;
            if (ch == KEY_BACKSPACE || ch == 127) {
                if (i > 0) {
                    i--;
                    mvprintw(6, 20 + i, " ");
                }
            } else if (ch >= 32 && ch <= 126) {
                password[i++] = ch;
                mvprintw(6, 20 + i - 1, "*");
            }
        }
        password[i] = '\0';
        noecho();

        attroff(COLOR_PAIR(1));

        if (strlen(username) == 0 || strlen(password) == 0) {
            attron(COLOR_PAIR(4));
            mvprintw(8, 10, "Username/Password tidak boleh kosong.");
            attroff(COLOR_PAIR(4));
            refresh();
            usleep(1500000);
            continue;
        }

        ifstream file("users.txt");
        if (!file) {
            attron(COLOR_PAIR(4));
            mvprintw(8, 10, "Error: users.txt not found!");
            attroff(COLOR_PAIR(4));
            refresh();
            usleep(2000000);
            return false;
        }

        bool found = false;
        while (file >> file_username >> file_password) {
            if (strcmp(username, file_username) == 0 && strcmp(password, file_password) == 0) {
                logged_in = true;
                found = true;
                break;
            }
        }

        file.close();

        if (!found) {
            attron(COLOR_PAIR(4));
            mvprintw(8, 10, "Login failed! Try again.");
            attroff(COLOR_PAIR(4));
            refresh();
            usleep(1500000);
        }
    }
    return logged_in;
}

int main() {
    srand(time(0));
    initscr();
    setupColors();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    createDefaultUserFile();

    animateOpening();
    if (!login()) return 0;
    loadingScreen();
    animateText("SELAMAT DATANG DI GAME PENGAMBIL BINTANG", 50000, 7);
    showHighScore();

    int player_x = COLS / 2;
    int score = 0;
    Star stars[MAX_STARS];
    initStars(stars, MAX_STARS);

    while (true) {
        clear();
        box(stdscr, 0, 0);

        int ch = getch();
        if (ch == 'q') break;

        if (ch == KEY_LEFT) player_x--;
        if (ch == KEY_RIGHT) player_x++;

        mvprintw(LINES - 1, player_x, "A");

        for (int i = 0; i < MAX_STARS; i++) {
            if (stars[i].active) {
                attron(COLOR_PAIR(2));
                mvprintw(stars[i].y, stars[i].x, "*");
                attroff(COLOR_PAIR(2));
                stars[i].y++;

                if (stars[i].y == LINES - 2 && stars[i].x == player_x) {
                    score++;
                    stars[i].active = false;
                }

                if (stars[i].y >= LINES - 1) {
                    respawnStar(stars[i]);
                }
            }
        }

        mvprintw(0, 0, "Score: %d", score);
        refresh();
        usleep(STAR_SPEED);
    }

    endwin();
    saveScore(score);
    animateText("Terimakasih telah bermain, score telah tersimpan!", 50000, 7);

    return 0;
}
