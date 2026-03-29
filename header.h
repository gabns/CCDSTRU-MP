#ifndef GAME_H
#define GAME_H

#include <stdbool.h>


typedef struct {
    int a; // row    (1..3, i.e. C)
    int b; // column (1..3, i.e. C)
} Pos;

#define MAX_SIZE 9

typedef struct {
    Pos items[MAX_SIZE];
    int size;
} PosSet;

typedef struct {
    bool good;
    bool go;
    bool over;
    bool found;
    bool start;
    int  val;      // belongs to N: 0..16 (game ends when >= 20 per spec logic)
    PosSet R;      // red pieces
    PosSet B;      // blue pieces
    PosSet S;      // visited / selected set
    PosSet T;      // expanded set
} GameState;


int  inSet        (PosSet set, Pos pos);
void addToSet     (PosSet *set, Pos pos);
void removeFromSet(PosSet *set, Pos pos);
int  inM          (Pos pos);


void  initGame      (GameState *gs);
void  updateOver    (GameState *gs);
void  Remove        (GameState *gs, Pos pos);
void  Expand        (GameState *gs, Pos pos);
void  Replace       (GameState *gs, Pos pos);
void  Update        (GameState *gs, Pos pos);
void  NextPlayerMove(GameState *gs, Pos pos);
char *GameOver      (GameState *gs);

#endif // GAME_H
