#ifndef __SNAKE_H__
#define __SNAKE_H__
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include "winsys.h"
#include "screen.h"

enum gamestate {
  PLAYING = 0,
  PAUSED = 1,
  HELP = 2,
  ENDED = 3
};

int random_position(int min, int max);

class CSnake:public CFramedWindow {
  private:
    vector<CPoint> snake_list;
    CPoint direction;
    CPoint food;

  public:
    CSnake(CRect r, char _c = ' ');

    void paint();
    bool handleEvent(int key);
    void move_snake(const CPoint &dif);
    void eat_food();
    void spawn_food();
    bool is_snake_here(int y, int x);
    bool has_collided();
    void restart_game();
    void toggle_help();
    void paint_help();
    void paint_level();
    void paint_results();
    void move_content_with_window(int key);

    enum gamestate gamestate; 
};

#endif
