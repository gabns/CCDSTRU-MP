#include "header.h"
#include <stdio.h>


int inSet(PosSet set, Pos pos) {
    int found = 0;
    int i = 0;
    while (i < set.size && !found) {
        if (set.items[i].a == pos.a && set.items[i].b == pos.b) {
            found = 1;
        }
        i++;
    }
    return found; 
}

void addToSet(PosSet *set, Pos pos) {
    if (!inSet(*set, pos) && set->size < MAX_SIZE) {
        set->items[set->size] = pos;
        set->size++;
    }
}

void removeFromSet(PosSet *set, Pos pos) {
    int i = 0;
    int found = 0;
    while (i < set->size) {
        if (!found && set->items[i].a == pos.a && set->items[i].b == pos.b) {
            found = 1;
            int j = i;
            while (j < set->size - 1) {
                set->items[j] = set->items[j + 1];
                j++;
            }
            set->size--;
        } else {
            i++;
        }
    }
}

int inM(Pos pos) {
    
    return (pos.a >= 1 && pos.a <= 3 && pos.b >= 1 && pos.b <= 3);
}


void initGame(GameState *gs) {
    gs->good  = false; 
    gs->go    = true;  
    gs->over  = false; 
    gs->found = false; 
    gs->start = true;  
    gs->val   = 0;     

    gs->R.size = 0; 
    gs->B.size = 0; 
    gs->S.size = 0; 
    gs->T.size = 0; 
}

void displayBoard(GameState *gs) {
    printf("\n    1   2   3\n");
    printf("  +---+---+---+\n");
    int a = 1;
    while (a <= 3) {
        printf("%d |", a);
        int b = 1;
        while (b <= 3) {
            Pos p = {a, b};
            if (inSet(gs->R, p))      printf(" R |");
            else if (inSet(gs->B, p)) printf(" B |");
            else                      printf(" . |");
            b++;
        }
        printf("\n  +---+---+---+\n");
        a++;
    }
}



void updateOver(GameState *gs) {
    int freeCount = 0;
    int a = 1;
    while (a <= 3) {
        int b = 1;
        while (b <= 3) {
            Pos p = {a, b};
            if (!inSet(gs->R, p) && !inSet(gs->B, p)) {
                freeCount++;
            }
            b++;
        }
        a++;
    }

    bool noFree   = (freeCount == 0);
    bool valOver  = (gs->val >= 20);
    bool wipeout  = (!gs->start && ((gs->R.size > 0 && gs->B.size == 0) || 
                                    (gs->B.size > 0 && gs->R.size == 0)));

    gs->over = (noFree || valOver || wipeout);
}

void Remove(GameState *gs, Pos pos) {
    if (gs->go) removeFromSet(&gs->R, pos); 
    else        removeFromSet(&gs->B, pos); 

    removeFromSet(&gs->S, pos); 
    removeFromSet(&gs->T, pos);
}

void Replace(GameState *gs, Pos pos); // Forward decl for recursion

void Expand(GameState *gs, Pos pos) {
    Pos u = {pos.a - 1, pos.b}; 
    Pos d = {pos.a + 1, pos.b}; 
    Pos k = {pos.a, pos.b - 1}; 
    Pos r = {pos.a, pos.b + 1}; 

    Remove(gs, pos); 

    if (gs->go) {
        if (inM(u)) Replace(gs, u); 
        if (inM(d)) Replace(gs, d); 
    }
    if (inM(k)) Replace(gs, k); 
    if (inM(r)) Replace(gs, r); 
}

void Replace(GameState *gs, Pos pos) {
    gs->found = false; 

    if (gs->go) {
        if (inSet(gs->B, pos)) { 
            removeFromSet(&gs->B, pos); 
            gs->found = true; 
        }
        if (inSet(gs->R, pos)) gs->found = true; 
        if (!inSet(gs->R, pos)) addToSet(&gs->R, pos); 
    } else {
        if (inSet(gs->R, pos)) { 
            removeFromSet(&gs->R, pos); 
            gs->found = true; 
        }
        if (inSet(gs->B, pos)) gs->found = true; 
        if (!inSet(gs->B, pos)) addToSet(&gs->B, pos); 
    }

    if (gs->found && !inSet(gs->S, pos)) {
        addToSet(&gs->S, pos); 
        gs->found = false;
    }

    if (gs->found && inSet(gs->S, pos) && !inSet(gs->T, pos)) {
        addToSet(&gs->T, pos); 
        Expand(gs, pos);
    }
}

void Update(GameState *gs, Pos pos) {
    gs->good = false; 

    if (!inSet(gs->S, pos)) {
        addToSet(&gs->S, pos); 
        gs->good = !gs->good;
    }

    if (!gs->good && inSet(gs->S, pos) && !inSet(gs->T, pos)) {
        addToSet(&gs->T, pos); 
        Expand(gs, pos);
    }
}

void NextPlayerMove(GameState *gs, Pos pos) {
    if (!gs->over) {
        if (gs->start) {
            if (gs->go) {
                addToSet(&gs->R, pos); 
                addToSet(&gs->S, pos);
                gs->good = true;
            } else {
                addToSet(&gs->B, pos); 
                addToSet(&gs->S, pos);
                gs->good = true;
            }
        } else {
            bool owned = (gs->go && inSet(gs->R, pos)) || (!gs->go && inSet(gs->B, pos));
            if (owned) {
                Update(gs, pos); 
                gs->good = true;
            }
        }
    }

    if (gs->start && gs->R.size == 1 && gs->B.size == 1) {
        gs->start = false; 
    }

    if (!gs->over && gs->good) {
        gs->good = !gs->good; 
        gs->go   = !gs->go;
        gs->val++;
        updateOver(gs);
    }
}

char *GameOver(GameState *gs) {
    char *result = "draw"; 
    if (gs->over) {
        if (gs->R.size > gs->B.size)      result = "R wins"; 
        else if (gs->R.size < gs->B.size) result = "B wins"; 
        /* Else result stays "draw" */
    }
    return result; 
}