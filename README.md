System Overview
This project requires implementing a computer program in either C or Java following the provided system specifications.
Applicable Sets
The system uses the following mathematical sets to define the game board and values:
C: Represents the set of positive integers less than 4, defined as C: {x in Z+ | x < 4}.
N: Represents non-negative integers up to 16, defined as N: {x in Z+ U {0} | x <= 16}.
M: Represents a 2D coordinate grid created by pairing the numbers in C, defined as M: C x C.
V: Represents boolean values, defined as V: {true, false}.
System Variables
State Flags: Variables good, go, over, found, and start belong to the boolean set V.
Turn Counter: The variable val belongs to the integer set N.
Grid Subsets: Variables R, B, S, T, and F are subsets of the grid M.
System Facts
Free spaces (F) are calculated by taking the entire grid (M) and subtracting the spaces occupied by R or B, defined as F = M - (R U B).
The game is considered over if there are no free spaces left (|F| = 0), the value of val is 20 or more, or if the game is past the start phase and one player has pieces while the other has none (e.g., |R| > 0 and |B| = 0, or |R| = 0 and |B| > 0).
System Initialization
When the program begins, the variables are set to their default states:
good is set to false.
go is set to true.
start is set to true.
found is set to false.
val is set to 0.
The grid subsets R, B, S, and T all start empty.

System States and Behavior (Functions)
Remove(pos)
If go is true, the position is removed from R.
If go is false (NOT go), the position is removed from B.
The position is removed from S.
The position is removed from T.
Replace(pos)
found is initially set to false.
If it is go's turn and the position is in B, it is removed from B and found becomes true.
If it is go's turn and the position is in R, found becomes true.
If it is go's turn and the position is not in R, it is added to R.
If it is not go's turn (NOT go) and the position is in R, it is removed from R and found becomes true.
If it is not go's turn (NOT go) and the position is in B, found becomes true.
If it is not go's turn (NOT go) and the position is not in B, it is added to B.
If found is true and the position is not in S, it is added to S and found is set to false.
If found is true, the position is in S, and the position is not in T, it is added to T and Expand(pos) is called.
Expand(pos)
The position is defined as coordinates (a, b).
The variables u, d, k, and r are defined as positions in M.
u represents the position above: (a-1, b).
d represents the position below: (a+1, b).
k represents the position to the left: (a, b-1).
r represents the position to the right: (a, b+1).
It calls Remove(pos).
If go is true, it calls Replace(u).
If go is true, it calls Replace(d).
It calls Replace(k).
It calls Replace(r).
Update(pos)
good is set to false.
If the position is not in S, it is added to S and the boolean value of good is toggled (NOT good).
If good is false (NOT good), the position is in S, and the position is not in T, the position is added to T and Expand(pos) is called.
NextPlayerMove(pos)
If the game is not over (NOT over), it is the start phase, and it is go's turn, the position is added to R and S, and good becomes true.
If the game is not over (NOT over), it is the start phase, and it is not go's turn (NOT go), the position is added to B and S, and good becomes true.
If the game is not over (NOT over), it is not the start phase (NOT start), and the player selects a position they own (go selects in R, or NOT go selects in B), Update(pos) is called and good becomes true.
If it is the start phase and both R and B have exactly 1 piece, start becomes false.
If the game is not over (NOT over) and good is true, good and go are toggled, and val increases by 1.
GameOver()
The result variable will be either "R wins", "B wins", or "draw".
If the game is over and R has more pieces than B, the result is "R wins".
If the game is over and R has fewer pieces than B, the result is "B wins".
If the game is over and R and B have an equal number of pieces, the result is "draw".
