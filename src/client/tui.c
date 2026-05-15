#include "client/tui.h"
#include "client/context.h"
#include "common/common.h"
#include "common/message_queue.h"
#include <ctype.h>
#include <ncurses.h>
#include <pthread.h>
#include <stddef.h>
#include <string.h>

WINDOW *chatscr, *textscr, *chat_container;

char input_buffer[MAX_MESSAGE] = {0};
size_t pos = 0;
pthread_t input_thread;

void *main_loop(void *arg) {
  message_packet_t packet;
  memset(&packet, 0, sizeof(message_packet_t));
  packet.type = PKT_TYPE_CHAT;
  strncpy(packet.username, (const char *)arg, MAX_USERNAME);

  int ch = 0;
  int input_y = 1, input_x = 1;
  wmove(textscr, input_y, input_x);
  wrefresh(textscr);
  while (1) {
    ch = wgetch(textscr);
    if (ch == ERR) {
      // sleep for a while as nodelay is enabled
      napms(10);
      continue;
    }
    pthread_mutex_lock(&tui_mutex);
    if (ch == '\n' || ch == '\r') {
      strncpy(packet.content, input_buffer, MAX_MESSAGE);
      memset(input_buffer, 0, MAX_MESSAGE);
      input_x = 1;
      wclear(textscr);
      box(textscr, 0, 0);
      wmove(textscr, input_y, input_x);
      pos = 0;
      message_queue_push(&packet);
    } else if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b') {
      if (pos > 0) {
        --pos;
        input_buffer[pos] = '\0';
        --input_x;
        mvwaddch(textscr, input_y, input_x, ' ');
        wmove(textscr, input_y, input_x);
      }
    } else if (pos < MAX_MESSAGE - 1 && isprint(ch)) {
      input_buffer[pos++] = (char)ch;
      input_buffer[pos] = '\0';
      waddch(textscr, ch);
      ++input_x;
    }
    wrefresh(textscr);
    pthread_mutex_unlock(&tui_mutex);
  }
}

void init_tui(const char *username) {
  int sx, sy;
  initscr();

  cbreak();
  noecho();

  getmaxyx(stdscr, sy, sx);
  textscr = subwin(stdscr, 3, sx, sy - 3, 0);
  chat_container = subwin(stdscr, sy - 3, sx, 0, 0);
  chatscr = derwin(chat_container, sy - 5, sx - 2, 1, 1);

  nodelay(textscr, TRUE);
  keypad(textscr, TRUE);
  box(textscr, 0, 0);

  scrollok(chatscr, TRUE);
  box(chat_container, 0, 0);
  wrefresh(chat_container);

  pthread_create(&input_thread, NULL, main_loop, (void *)username);
}
