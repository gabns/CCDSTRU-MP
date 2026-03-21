#include "header.h"

// ── Set helpers ───────────────────────────────────────────────────────────────

/* Returns 1 if pos is in set, 0 otherwise */
int inSet(PosSet set, Pos pos) {
    int found = 0;
    int i = 0;
    while (i < set.size && !found) {
        if (set.items[i].a == pos.a && set.items[i].b == pos.b)
            found = 1;
        i++;
    }
    return found;
}

/* Adds pos to set (no-op if already present) */
void addToSet(PosSet *set, Pos pos) {
    if (!inSet(*set, pos) && set->size < MAX_SIZE) {
        set->items[set->size] = pos;
        set->size++;
    }
}

/* Removes pos from set (no-op if not present) */
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

/* Returns 1 if pos is a valid coordinate in M (C×C), 0 otherwise */
int inM(Pos pos) {
    return (pos.a >= 1 && pos.a <= 3 && pos.b >= 1 && pos.b <= 3);
}

// ── Initialization ────────────────────────────────────────────────────────────

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

// ── updateOver() ─────────────────────────────────────────────────────────────
// Recalculates F = M - (R ∪ B), then sets over according to spec.

void updateOver(GameState *gs) {
    int freeCount = 0;
    int a, b;
    for (a = 1; a <= 3; a++) {
        for (b = 1; b <= 3; b++) {
            Pos p = {a, b};
            if (!inSet(gs->R, p) && !inSet(gs->B, p))
                freeCount++;
        }
    }

    bool noFree   = (freeCount == 0);
    bool valOver  = (gs->val >= 20);
    bool rWipeout = (!gs->start && gs->R.size > 0 && gs->B.size == 0);
    bool bWipeout = (!gs->start && gs->B.size > 0 && gs->R.size == 0);

    gs->over = noFree || valOver || rWipeout || bWipeout;
}

// ── Remove(pos) ───────────────────────────────────────────────────────────────

void Remove(GameState *gs, Pos pos) {
    if (gs->go)
        removeFromSet(&gs->R, pos);
    else
        removeFromSet(&gs->B, pos);

    removeFromSet(&gs->S, pos);
    removeFromSet(&gs->T, pos);
}

// ── Expand(pos) ───────────────────────────────────────────────────────────────

void Expand(GameState *gs, Pos pos) {
    int a = pos.a;
    int b = pos.b;

    Pos u = {a - 1, b};   // up
    Pos d = {a + 1, b};   // down
    Pos k = {a, b - 1};   // left
    Pos r = {a, b + 1};   // right

    Remove(gs, pos);

    if (gs->go && inM(u)) Replace(gs, u);
    if (gs->go && inM(d)) Replace(gs, d);
    if (inM(k))           Replace(gs, k);
    if (inM(r))           Replace(gs, r);
}

// ── Replace(pos) ─────────────────────────────────────────────────────────────

void Replace(GameState *gs, Pos pos) {
    gs->found = false;

    if (gs->go) {
        if (inSet(gs->B, pos)) {
            removeFromSet(&gs->B, pos);
            gs->found = true;
        }
        if (inSet(gs->R, pos))
            gs->found = true;
        if (!inSet(gs->R, pos))
            addToSet(&gs->R, pos);
    } else {
        if (inSet(gs->R, pos)) {
            removeFromSet(&gs->R, pos);
            gs->found = true;
        }
        if (inSet(gs->B, pos))
            gs->found = true;
        if (!inSet(gs->B, pos))
            addToSet(&gs->B, pos);
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

// ── Update(pos) ───────────────────────────────────────────────────────────────

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

// ── NextPlayerMove(pos) ───────────────────────────────────────────────────────

void NextPlayerMove(GameState *gs, Pos pos) {
    if (!gs->over && gs->start && gs->go) {
        addToSet(&gs->R, pos);
        addToSet(&gs->S, pos);
        gs->good = true;
    }

    if (!gs->over && gs->start && !gs->go) {
        addToSet(&gs->B, pos);
        addToSet(&gs->S, pos);
        gs->good = true;
    }

    if (!gs->over && !gs->start) {
        bool playerOwnsPos = (gs->go  && inSet(gs->R, pos)) ||
                             (!gs->go && inSet(gs->B, pos));
        if (playerOwnsPos) {
            Update(gs, pos);
            gs->good = true;
        }
    }

    if (gs->start && gs->R.size == 1 && gs->B.size == 1)
        gs->start = false;

    if (!gs->over && gs->good) {
        gs->good = !gs->good;
        gs->go   = !gs->go;
        gs->val++;
        updateOver(gs);
    }
}

// ── GameOver() ────────────────────────────────────────────────────────────────

char *GameOver(GameState *gs) {
    char *result = "draw";

    if (gs->over) {
        if (gs->R.size > gs->B.size)
            result = "R wins";
        else if (gs->R.size < gs->B.size)
            result = "B wins";
        // else result stays "draw"
    }

    return result;
}
