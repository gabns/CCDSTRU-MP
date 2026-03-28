#include "header.h"
#include <stdio.h>

// ── Set helpers ───────────────────────────────────────────────────────────────

int inSet(PosSet set, Pos pos) {
    int i = 0;
    while (i < set.size) {
        if (set.items[i].a == pos.a && set.items[i].b == pos.b)
            return 1;
        i++;
    }
    return 0;
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
            for (int j = i; j < set->size - 1; j++) {
                set->items[j] = set->items[j + 1];
            }
            set->size--;
        } else {
            i++;
        }
    }
}

/* Validates coordinates against Set C (positive integers < 4) [cite: 7, 79] */
int inM(Pos pos) {
    return (pos.a >= 1 && pos.a <= 3 && pos.b >= 1 && pos.b <= 3);
}

// ── Initialization [cite: 17, 93] ──────────────────────────────────────────────

void initGame(GameState *gs) {
    gs->good  = false; // [cite: 19, 94]
    gs->go    = true;  // [cite: 20, 95]
    gs->over  = false;
    gs->found = false; // [cite: 22, 102]
    gs->start = true;  // [cite: 21, 96]
    gs->val   = 0;     // [cite: 23, 103]

    gs->R.size = 0;    // [cite: 24, 104]
    gs->B.size = 0;    // [cite: 24, 116]
    gs->S.size = 0;    // [cite: 24, 117]
    gs->T.size = 0;    // [cite: 24, 118]
}

// ── displayBoard() (Custom Helper for Visualization) ─────────────────────────

void displayBoard(GameState *gs) {
    printf("\n    1   2   3\n");
    printf("  +---+---+---+\n");
    for (int a = 1; a <= 3; a++) {
        printf("%d |", a);
        for (int b = 1; b <= 3; b++) {
            Pos p = {a, b};
            if (inSet(gs->R, p))      printf(" R |");
            else if (inSet(gs->B, p)) printf(" B |");
            else                      printf(" . |");
        }
        printf("\n  +---+---+---+\n");
    }
}

// ── updateOver()  ──────────────────────────────────────────────

void updateOver(GameState *gs) {
    int freeCount = 0;
    for (int a = 1; a <= 3; a++) {
        for (int b = 1; b <= 3; b++) {
            Pos p = {a, b};
            if (!inSet(gs->R, p) && !inSet(gs->B, p))
                freeCount++;
        }
    }

    /* Game ends if |F|=0, val >= 20, or a wipeout occurs post-start  */
    bool noFree   = (freeCount == 0);
    bool valLimit = (gs->val >= 20);
    bool wipeout  = (!gs->start && ((gs->R.size > 0 && gs->B.size == 0) || 
                                    (gs->R.size == 0 && gs->B.size > 0)));

    gs->over = noFree || valLimit || wipeout;
}

// ── Remove(pos) [cite: 26, 98] ────────────────────────────────────────────────

void Remove(GameState *gs, Pos pos) {
    if (gs->go) removeFromSet(&gs->R, pos); // [cite: 27, 99]
    else        removeFromSet(&gs->B, pos); // [cite: 28, 100]

    removeFromSet(&gs->S, pos); // [cite: 29, 101]
    removeFromSet(&gs->T, pos); // [cite: 30, 105]
}

// ── Expand(pos) [cite: 41, 119] ───────────────────────────────────────────────

void Expand(GameState *gs, Pos pos) {
    Pos u = {pos.a - 1, pos.b}; // [cite: 44, 122]
    Pos d = {pos.a + 1, pos.b}; // [cite: 45, 123]
    Pos k = {pos.a, pos.b - 1}; // [cite: 46, 124]
    Pos r = {pos.a, pos.b + 1}; // [cite: 47, 125]

    Remove(gs, pos); // [cite: 48, 126]

    if (gs->go) {
        if (inM(u)) Replace(gs, u); // [cite: 49, 127]
        if (inM(d)) Replace(gs, d); // [cite: 50, 128]
    }
    if (inM(k)) Replace(gs, k); // [cite: 51, 129]
    if (inM(r)) Replace(gs, r); // [cite: 52, 130]
}

// ── Replace(pos) [cite: 31, 106] ──────────────────────────────────────────────

void Replace(GameState *gs, Pos pos) {
    gs->found = false; // [cite: 32, 107]

    if (gs->go) {
        if (inSet(gs->B, pos)) {
            removeFromSet(&gs->B, pos); // [cite: 33, 108]
            gs->found = true;
        }
        if (inSet(gs->R, pos)) gs->found = true; // [cite: 34, 109]
        if (!inSet(gs->R, pos)) addToSet(&gs->R, pos); // [cite: 35, 110]
    } else {
        if (inSet(gs->R, pos)) {
            removeFromSet(&gs->R, pos); // [cite: 36, 111]
            gs->found = true;
        }
        if (inSet(gs->B, pos)) gs->found = true; // [cite: 37, 112]
        if (!inSet(gs->B, pos)) addToSet(&gs->B, pos); // [cite: 38, 113]
    }

    if (gs->found && !inSet(gs->S, pos)) {
        addToSet(&gs->S, pos); // [cite: 39, 114]
        gs->found = false;
    }

    if (gs->found && inSet(gs->S, pos) && !inSet(gs->T, pos)) {
        addToSet(&gs->T, pos); // [cite: 40, 115]
        Expand(gs, pos);
    }
}

// ── Update(pos) [cite: 53, 131] ───────────────────────────────────────────────

void Update(GameState *gs, Pos pos) {
    gs->good = false; // [cite: 55, 132]

    if (!inSet(gs->S, pos)) {
        addToSet(&gs->S, pos); // [cite: 56, 133]
        gs->good = !gs->good;
    }

    if (!gs->good && inSet(gs->S, pos) && !inSet(gs->T, pos)) {
        addToSet(&gs->T, pos); // [cite: 57, 134]
        Expand(gs, pos);
    }
}

// ── NextPlayerMove(pos) [cite: 58, 135] ───────────────────────────────────────

void NextPlayerMove(GameState *gs, Pos pos) {
    if (!gs->over) {
        if (gs->start) {
            if (gs->go) {
                addToSet(&gs->R, pos); // [cite: 59, 136]
                addToSet(&gs->S, pos);
                gs->good = true;
            } else {
                addToSet(&gs->B, pos); // [cite: 60, 137]
                addToSet(&gs->S, pos);
                gs->good = true;
            }
        } else {
            bool owned = (gs->go && inSet(gs->R, pos)) || (!gs->go && inSet(gs->B, pos));
            if (owned) {
                Update(gs, pos); // [cite: 62, 138]
                gs->good = true;
            }
        }
    }

    if (gs->start && gs->R.size == 1 && gs->B.size == 1)
        gs->start = false; // [cite: 64, 139]

    if (!gs->over && gs->good) {
        gs->good = !gs->good; // [cite: 65, 140]
        gs->go   = !gs->go;
        gs->val++;
        updateOver(gs);
    }
}

// ── GameOver() [cite: 66, 141] ────────────────────────────────────────────────

char *GameOver(GameState *gs) {
    if (gs->R.size > gs->B.size) return "R wins"; // [cite: 68, 143]
    if (gs->R.size < gs->B.size) return "B wins"; // [cite: 69, 144]
    return "draw"; // [cite: 70, 145]
}