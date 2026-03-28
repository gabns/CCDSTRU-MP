#include <stdio.h>
#include "header.h"
#include "functions.c"

int main(void) {
    GameState gs;
    initGame(&gs);

    Pos p;

    while (!gs.over) {
        displayBoard(&gs); // <-- Show the grid at the start of every turn

        printf("\nCurrent Turn: %s\n", gs.go ? "R" : "B");
        
        printf("Enter row (1-3): ");
        scanf("%d", &p.a);
        
        printf("Enter column (1-3): ");
        scanf("%d", &p.b);
        
        if (inM(p)) {
            NextPlayerMove(&gs, p);
            if (!gs.good && !gs.over) {
                printf("Invalid move! Try Again.\n");
            }
        } else {
            printf("Invalid position! TRY AGAIN.\n");
        }	
    }
    
    displayBoard(&gs); // Show final state
    printf("\nGame Over!\n");
    printf("%s\n", GameOver(&gs));

    return 0;
}