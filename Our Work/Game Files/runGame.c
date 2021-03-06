// ADD YOUR COMP1917 COMMENT UP HERE.

// Thomas Moffet, thomasmoffet, z5061905
// F09C, Joseph Harris
// 1/05/2015
// This program communicates with game.h to play Knowledge Island.

#include <stdio.h>
#include <stdlib.h>
#include <time.h> /// For the random dice roll function.
#include "Game.h"

#define POINTS_TO_WIN 150

#define LEFT 'L'
#define RIGHT 'R'
#define BACK 'B'

#define DEFAULT_DISCIPLINES {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV, \
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN, \
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS }
#define DEFAULT_DICE {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5}

int playTurn(Game g);
int checkWin(Game g);
void printWinner(Game g, int hasWon);
int makeDiceValue(void);

int main(int argc, char *argv[]) {
	/// This just sets the random dice roll seed.
	srand(time(NULL));
	
	/// Firstly, the game loads the default map data, and then uses that
	/// to initialise the game (g).
	int disciplines[] = DEFAULT_DISCIPLINES;
	int dice[] = DEFAULT_DICE;
	
	// This bit is an alternative that randomly generates the hexs.
	/*
		/// A random number is created to determine what discipline each
		/// hexagon gets. 19 is the number of hexs, 6 is the number of
		/// disciplines.
		// If we can, we should convert these to #defines.
		short pos = 0;
		while (pos < NUM_REGIONS) {
			discipline[pos] = (rand() % 6);
			pos++;
		}
		
		/// Then we do the same for the dice numbers. They are between 2 and
		/// 12, so the formula is as that.
		pos = 0;
		while (pos < NUM_REGIONS) {
			dice[pos] = (rand() % 6) + 2;
			pos++
		}
	*/
	
	Game g = newGame(disciplines, dice);
	
	/// Then, the game begins. This will constantly ask for input, and
	/// loop until the game is won.
	/// This loop is basically a turn.
	int gHasWon = 0;
	int gDiceScore = 0;
	
	/// This system allows one to just create a second newgame, and add
	/// another condition to the while loop to keep it in the game. Then
	/// the loop only ends when both games finish, but won't play a
	/// single game that has finished.
	while (gHasWon == FALSE) {
		if (gHasWon == FALSE) {
			gDiceScore = makeDiceValue();
			throwDice(g, gDiceScore);
			playTurn(g);
		}
		gHasWon = checkWin(g);
		
	}
	
	/// The winner is printed, and the game is unloaded.
	printWinner(g, gHasWon);
	disposeGame(g);
	
	return EXIT_SUCCESS;
}

/// This is the main playing function.
int playTurn(Game g) {
	/// The action is created. The action is given a value so that it
	/// doesn't accidentally skip the user's turn.
	action a;
	short pathpos = 0;
	char bufferChar = 0;
	
	while (a.actionCode != PASS) {
		/// These reset the action for every turn.
		a.actionCode = -1;
		pathpos = 0;
		while (pathpos < PATH_LIMIT) {
			a.destination[pathpos] = 0;
			pathpos++;
		}
		pathpos = 0;
		a.disciplineFrom = -1;
		a.disciplineTo = -1;
		
		pathpos = 0;
		printf("Input an action code: ");
		scanf("%d", &a.actionCode);
		
		if ((a.actionCode == BUILD_CAMPUS) ||
		    (a.actionCode == BUILD_GO8) || 
		    (a.actionCode == OBTAIN_ARC)) {
			printf("Type a path using L, R, and B.\n");
			getchar();
			bufferChar = LEFT;
			
			while ((bufferChar == LEFT) ||
			       (bufferChar == RIGHT) ||
			       (bufferChar == BACK)) {
				bufferChar = getchar();
				a.destination[pathpos] = bufferChar;
				pathpos++;
			}
		}
		
		// What is OBTAIN_PUBICATION and OBTAIN_IP_PATENT?
		
		if (a.actionCode == RETRAIN_STUDENTS) {
			printf("What students are you converting from? ");
			scanf("%d", &a.disciplineFrom);
			printf("And to? ");
			scanf("%d", &a.disciplineTo);
		}
		
		/// Firstly, a blanket check to see if what they're doing is
		/// valid, then perform the action.
		if (isLegalAction(g, a) == TRUE) {
			makeAction(g, a);
		} else {
			printf("That action was not legal.\n");
		}
	}
	return EXIT_SUCCESS;
}

/// This function just checks if any of the players has won the game
/// yet. This runs at the end of a turn to see if a player has won.
int checkWin(Game g) {
	int hasWon = FALSE;
	
	if (getKPIpoints(g, UNI_A) >= POINTS_TO_WIN) {
		hasWon = UNI_A;
	} else if (getKPIpoints(g, UNI_B) >= POINTS_TO_WIN) {
		hasWon = UNI_B;
	} else if (getKPIpoints(g, UNI_C) >= POINTS_TO_WIN) {
		hasWon = UNI_C;
	}
	
	return hasWon;
}

/// This function just prints the winner at the end of the game.
void printWinner(Game g, int hasWon) {
	if (hasWon == UNI_A) {
		printf("Player 1 ");
	} else if (hasWon == UNI_B) {
		printf("Player 2 ");
	} else if (hasWon == UNI_C) {
		printf("Player 3 ");
	}
	printf("is the winner!\n");
}

int makeDiceValue(void) {
	int permutation[36] = {2,3,3,4,4,4,5,5,5,5,6,6,6,6,6,7,7,7,7,7,7,8,8,8,8,8,9,9,9,9,10,10,10,11,11,12};
	int pos = rand() % 36;
	int diceValue = permutation[pos];
	return diceValue;
}