#include "snake.h"

CSnake::CSnake(CRect r, char _c /*=' '*/): CFramedWindow(r, _c) {
    direction = CPoint(1, 0); //right
    snake_list.push_back(CPoint(geom.topleft.x + 5, geom.topleft.y + 5)); //head
    snake_list.push_back(CPoint(geom.topleft.x + 4, geom.topleft.y + 5));
    snake_list.push_back(CPoint(geom.topleft.x + 3, geom.topleft.y + 5));
    spawn_food();

    paint();
}

void CSnake::paint() {
    CPoint head = snake_list[0];
    
    CFramedWindow::paint();

    //printuje łeb
    gotoyx(head.y, head.x);
    printl("+");

    //printuje reszte
    for (unsigned int i = 1; i < snake_list.size(); i++) {
        gotoyx(snake_list[i].y, snake_list[i].x);
        printl("*");
    }

    //printuje jedzenie
    gotoyx(food.y, food.x);
    printl("D");

    //printuje level
    paint_level();

    //printuje pomoc
    if (gamestate == HELP) {
        paint_help();
    }

    //printuje wyniki
    if (gamestate == ENDED) {
        paint_results();
    }
}

bool CSnake::handleEvent(int key) {
    if (gamestate == ENDED) {
        switch (key) {
            case 'r':
            case 'R':
                restart_game();
                return false;
            default: 
                return false;
        }
    }

    else if (gamestate == HELP || gamestate == PAUSED) {
        switch (key) {
            case 'r':
            case 'R':
                restart_game();
                return false;
            case 'h':
            case 'H':
                gamestate == HELP 
                    ? gamestate = PLAYING 
                    : gamestate = HELP;
                paint();
                return false;
            case 'p':
            case 'P':
                gamestate == PAUSED 
                    ? gamestate = PLAYING
                    : gamestate = PAUSED;
                paint();
                return false;
            case KEY_UP:
            case KEY_DOWN:
            case KEY_LEFT:
            case KEY_RIGHT:
                CFramedWindow::handleEvent(key);
                move_content_with_window(key);
                return true;
            default: 
                return false;
        }
    }

    else if (gamestate == PLAYING) {
        switch (key) {
            case 'h':
            case 'H':
                gamestate == HELP 
                    ? gamestate = PLAYING 
                    : gamestate = HELP;
                paint();
                return false;
            case 'p':
            case 'P':
                gamestate == PAUSED 
                    ? gamestate = PLAYING
                    : gamestate = PAUSED;
                paint();
                return false;
            case KEY_UP:
                if (direction.y == 1) return false;
                direction = CPoint(0, -1);
                move_snake(direction);
                return true;
            case KEY_DOWN:
                if (direction.y == -1) return false;
                direction = CPoint(0, 1);
                move_snake(direction);
                return true;
            case KEY_LEFT:
                if (direction.x == 1) return false;
                direction = CPoint(-1, 0);
                move_snake(direction);
                return true;
            case KEY_RIGHT:
                if (direction.x == -1) return false;
                direction = CPoint(1, 0);
                move_snake(direction);
                return true;
            default:
                move_snake(direction);
                return true;
        }
    }

    else return false;
}

void CSnake::move_snake(const CPoint &dif) {
    CPoint &head = snake_list[0];

    for (int i = snake_list.size(); i >= 1; i--) {
        snake_list[i] = snake_list[i-1];
    }

    head += dif;

    if (head.x == geom.topleft.x + geom.size.x - 1) //w prawo
        head.x = geom.topleft.x + 1;
    else if (head.x == geom.topleft.x) //w lewo
        head.x = geom.topleft.x + geom.size.x - 2;
    else if (head.y == geom.topleft.y + geom.size.y - 1) //w dol 
        head.y = geom.topleft.y + 1;
    else if (head.y == geom.topleft.y) //w gore chyba
        head.y = geom.topleft.y + geom.size.y - 2;

    if (head.y == food.y && head.x == food.x) {
        eat_food();
        spawn_food();
    }

    if (has_collided()) {
        gamestate = ENDED;
    }

    napms(120 - 5 * (snake_list.size() / 3));
}

void CSnake::spawn_food() {
    while (1) {
        int yPos = random_position(geom.topleft.y + 1, geom.topleft.y + geom.size.y - 2);
        int xPos = random_position(geom.topleft.x + 1, geom.topleft.x + geom.size.x - 2);

        if (!is_snake_here(yPos, xPos)) {
            food = CPoint(xPos, yPos);
            paint();
            break;
        }
    }
}   

void CSnake::eat_food() {
    CPoint &last = snake_list.back();
    snake_list.push_back(CPoint(last.x, last.y));
}

bool CSnake::is_snake_here(int y, int x) {
    for (unsigned int i = 0; i < snake_list.size(); i++) {
        if (snake_list[i].y == y && snake_list[i].x == x) return true;
    }

    return false;
}

bool CSnake::has_collided() {
    CPoint &head = snake_list[0];

    for (unsigned int i = 1; i < snake_list.size(); i++) {
        if (snake_list[i].x == head.x && snake_list[i].y == head.y) return true;
    }

    return false;
}

void CSnake::restart_game() {
    snake_list.clear();
    
    direction = CPoint(1, 0); //right
    snake_list.push_back(CPoint(geom.topleft.x + 5, geom.topleft.y + 5)); //head
    snake_list.push_back(CPoint(geom.topleft.x + 4, geom.topleft.y + 5));
    snake_list.push_back(CPoint(geom.topleft.x + 3, geom.topleft.y + 5));
    spawn_food();

    gamestate = PLAYING;
    paint();
}

void CSnake::paint_help() {
    gotoyx(geom.topleft.y + 5, geom.topleft.x + 3);
    printl("h - toggle help information");
    gotoyx(geom.topleft.y + 6, geom.topleft.x + 3);
    printl("p - toggle pause/play mode");
    gotoyx(geom.topleft.y + 7, geom.topleft.x + 3);
    printl("r - restart game");
    gotoyx(geom.topleft.y + 8, geom.topleft.x + 3);
    printl("arrows - move snake (in play mode) or");
    gotoyx(geom.topleft.y + 9, geom.topleft.x + 10);
    printl("move window (in pause mode)");
}

void CSnake::paint_level() {
    gotoyx(geom.topleft.y - 1, geom.topleft.x);
    printl("| LEVEL: %d |", snake_list.size() / 3);
}

void CSnake::paint_results() {
    gotoyx(geom.topleft.y + 1, geom.topleft.x + 1);
    printl("GAME OVER, result: %d", (int)snake_list.size());
}

void CSnake::move_content_with_window(int key) {
    CPoint dif(0, 0);

    if (key == KEY_UP) {
        dif.x = 0;
        dif.y = -1;
    }
    else if (key == KEY_DOWN) {
        dif.x = 0;
        dif.y = 1;
    }
    else if (key == KEY_LEFT) {
        dif.x = -1;
        dif.y = 0;
    }
    else if (key == KEY_RIGHT) {
        dif.x = 1;
        dif.y = 0;
    }

    for (unsigned int i = 0; i < snake_list.size(); i++) {
        snake_list[i] += dif;
    }

    food += dif;
}

int random_position(int min, int max) { 
    return min + rand() % ((max + 1) - min);
}