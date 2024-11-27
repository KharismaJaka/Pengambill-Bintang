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
