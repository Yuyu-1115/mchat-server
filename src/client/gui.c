#include "client/gui.h"
#include <ncurses.h>

void init_gui() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  box(stdscr, 0, 0);
  refresh();
}
