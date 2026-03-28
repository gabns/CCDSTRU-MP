#include <stdio.h>
#include "header.h"

int main(void) {
    GameState gs;
    initGame(&gs);

    // TODO: implement input loop
    // Example flow:
    //   1. Read a position (a, b) from the user.
    //   2. Call NextPlayerMove(&gs, pos).
    //   3. Check gs.over; if true, call GameOver(&gs) and print the result.

Pos p;

while (!gs.over){
	
	printf("\nCurrent Turn: %s\n", gs.go ? "R" : "B");
	
	printf("Enter row (1-3):");
	scanf("%d", &p.a);
	
	printf("Enter column (1-3): ");
	scanf("%d", &p.b);
	
	if(inM(p)) {
		NextPlayerMove(&gs, p);
				if(!gs.good && !gs.over){
					printf("Invalid move! Try Again.\n");								
				}
	} 
	else {
		printf("Invalid position! TRY AGAIN.\n");	
	}		
	}
	
	printf("\nGame Over!\n");
	printf("%s\n", GameOver(&gs));
    
    return 0;
}
