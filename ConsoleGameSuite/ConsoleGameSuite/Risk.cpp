//Combat Simulator
//Battle system modeled in a way that it can be built upon to create Risk
//In game instructions

#include "stdafx.h"
#include "Games.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <time.h>
#include <stdlib.h>

using namespace std;

enum game_over_t { GAME_NOT_OVER, ENEMY_LOSES, PLAYER_LOSES };

struct unitMovements_t {
	int start;
	int num;
	int end;
};

//Functions
/*=========================================================================*/
//printing/output functions
void intro();
void printBoard();
void printEndGame(game_over_t go);
void printUnits(int cell);
void updateBoard();
void printEnemyMove(int start, int num, int end);
void announceAttack(int attackers, int start, int end);

//board units
const int startingUnits = 9;
//Contains the number of units each board spot contains.
//Positive Numbers represent the Player units
//Negative Numbers represent the Enemey Units
//The 0th array element is the player base
//The 10th array element is the enemy base
int boardUnits[11] = { startingUnits,0,0,0,0,0,0,0,0,0,-startingUnits };



//battle and movement functions
unitMovements_t getEnemyMove();
bool isBattle(int numUnits, int destination);
void doBattle(int start, int end);
void updateUnits();
void moveUnits(int start, int num, int end);
void diceBattle(int start, int end);

//user movement functions
int getUserUnitStart();
int getUserUnitNumbers(int cell);
int	getUserUnitDestination(int cellStart, int numUnits);


// AI movement functions
int getEnemyStartPos();
int getEnemyNumUnits(int cell);
int getEnemyDestination(int cell);

//DEPLOY
void deployUnits();
void deploy();
void aiDeploy();
void getDeploymentUnits();

//random helpers
bool areUnitsInCell(int x);
bool inRange(int start, int destination);
char askYesNo(string question);
void setCursorTo(int x, int y);
void clearOutputLines();
game_over_t checkGameOver();
int roll();
int returnLowestDieNum(int a, int b, int c);
int returnLowestDieNum(int a, int b);

/*=========================================================================*/

int Risk()
{
	srand(time(NULL));

	intro();
	printBoard();

	game_over_t gameOver = GAME_NOT_OVER;

	//Main game loop, runs until one sides units are all dead
	while (gameOver == GAME_NOT_OVER) {

		deployUnits();       //deployment stage
		printBoard();


		//Get User Movements
		int startPos = getUserUnitStart();   //get user start
		int numUnits = getUserUnitNumbers(startPos);  //get num units to move
		int destination = getUserUnitDestination(startPos, numUnits);  //get destination

																	   //checks if movement results in a battle
		if (isBattle(numUnits, destination)) {
			doBattle(startPos, destination);
			gameOver = checkGameOver();
		}
		else moveUnits(startPos, numUnits, destination);

		//Get AI Enemy Movements
		if (gameOver == GAME_NOT_OVER) {
			unitMovements_t enemyMove = getEnemyMove();  //get AI MOVE

			if (isBattle(enemyMove.num, enemyMove.end)) {   //If move against player, start battle
				doBattle(enemyMove.start, enemyMove.end);
				gameOver = checkGameOver();
			}
			else {                                     //else just move, announce, and update board
				moveUnits(enemyMove.start, enemyMove.num, enemyMove.end);
				printEnemyMove(enemyMove.start, enemyMove.num, enemyMove.end);
			}
		}

	}
	printEndGame(gameOver);   //prints end game message
	return 0;
}



//checks if all units of either player are completed removed from board, returns status
game_over_t checkGameOver() {
	game_over_t status = GAME_NOT_OVER;
	bool playerUnitsExist = false;
	bool enemyUnitsExist = false;

	for (int i = 0; i<11; i++) {
		if (boardUnits[i]>0) playerUnitsExist = true;
		if (boardUnits[i]<0) enemyUnitsExist = true;
	}

	if (playerUnitsExist && enemyUnitsExist) status = GAME_NOT_OVER;
	else if (playerUnitsExist && !enemyUnitsExist) status = ENEMY_LOSES;
	else if (!playerUnitsExist && enemyUnitsExist) status = PLAYER_LOSES;

	return status;
}

//Follows five rules to determine AI Movement and actions
//Cascading rules in this order.  1) ATTACK 2)FORTIFY 3)RUN 4)PULL FROM BASE 5)RANDOM
unitMovements_t getEnemyMove() {
	unitMovements_t enemyMove;

	//RULE 1 CHECK for attack, attack first found advantage
	//If AI can find a player cell that is weak and in range of an attack then attack
	for (int i = 1; i<11; i++) {
		if (boardUnits[i]<0) {                     //cell contains enemy units
			for (int j = 1; j<10; j++) {
				if (((boardUnits[j]>0) && (abs(boardUnits[i])>boardUnits[j])) && (inRange(i, j))) { //surrounding cell contains user units and is weak
					enemyMove.start = i;
					enemyMove.num = boardUnits[i];
					enemyMove.end = j;
					return enemyMove;
					//cout<<"attack";
				}
			}
		}
	}

	//RULE2 CAN FORTIFY VERSUS STRONGER ENEMY (ENEMY UNITS > PLAYER UNITS +1, after move)
	//If AI is in danger, but can move units from a surrounding cell to make it no longer in danger
	for (int i = 1; i<11; i++) {
		if (boardUnits[i]<0) {                     //cell contains enemy units
			for (int j = 0; j<10; j++) {
				if (((boardUnits[j]>0) && (abs(boardUnits[i])<boardUnits[j])) && (inRange(i, j))) { //surrounding cell contains user units and is strong
					for (int k = 1; k<11; k++) {
						if (boardUnits[k]<0 && inRange(k, i)) {           //surrounding cell contains enemy and can fortify by moving
							if ((abs(boardUnits[k]) + abs(boardUnits[i]))>(boardUnits[j])) {
								enemyMove.start = k;
								enemyMove.num = boardUnits[k];
								enemyMove.end = i;
								//cout<<"FORTIFY START="<<k<<"NUM="<<boardUnits[k]<<"end="<<i;

								return enemyMove;
							}
						}
					}
				}
			}
		}
	}

	//RULE 3 If AI is in danger and can find a safe unit to move to; Run to Safety
	bool safe = true;
	for (int i = 1; i<11; i++) {
		if (boardUnits[i]<0) {                     //cell contains enemy units
			for (int j = 0; j<10; j++) {
				if (((boardUnits[j]>0) && (abs(boardUnits[i])<boardUnits[j])) && (inRange(i, j))) { //surrounding cell contains user units and is strong
					for (int k = 1; k<10; k++) {
						if (boardUnits[k]<1 && inRange(i, k)) {           //surrounding cell contains enemy or open cell
							for (int l = 1; l<10; l++) {                       //check surrounding cells of possible escape cell
								if (inRange(l, k) && (boardUnits[l]>0) && ((abs(boardUnits[k]) + abs(boardUnits[i]))<boardUnits[l])) { safe = false; }
							}
							if (safe) {
								enemyMove.start = i;
								enemyMove.num = boardUnits[i];
								enemyMove.end = k;
								//cout<<"RUN START="<<k<<"NUM="<<boardUnits[i]<<"end="<<i;
								return enemyMove;
							}
						}
					}
				}
			}
		}
	}

	//RULE 4 attack when equal strength
	//If AI can find a player cell that is equal strength and in range of an attack then attack
	for (int i = 1; i<11; i++) {
		if (boardUnits[i]<0) {                     //cell contains enemy units
			for (int j = 1; j<10; j++) {
				if (((boardUnits[j]>0) && (abs(boardUnits[i]) == boardUnits[j])) && (inRange(i, j))) { //surrounding cell contains user units and is weak
					enemyMove.start = i;
					enemyMove.num = boardUnits[i];
					enemyMove.end = j;
					return enemyMove;
				}
			}
		}
	}


	//RULE 5 If found no strategic moves, move RANDOM
	enemyMove.start = getEnemyStartPos();
	enemyMove.num = getEnemyNumUnits(enemyMove.start);
	enemyMove.end = getEnemyDestination(enemyMove.start);
	return enemyMove;
}

//checks if destination contains opposite units, if so the battle is on
bool isBattle(int numUnits, int destination) {
	bool battle = false;
	if ((boardUnits[destination]<0) && (numUnits>0)) battle = true;
	else if ((boardUnits[destination]>0) && (numUnits<0)) battle = true;
	else battle = false;
	return battle;
}

//uses all units from start (attacking) cell to end(defending) cell.
//announces attack and starts a diceBattle repeatedly until one of the cells involved in the battle run out of units
//if the attacker is victorious, move the units to the conquered territory (leaving one unit behind, unless only 1 unit survived)
void doBattle(int start, int end) {
	int attackers = boardUnits[start];
	int defenders = boardUnits[end] * -1;
	int enemyNegativeChanger = 1;

	if (attackers<0)enemyNegativeChanger = -1;

	announceAttack(attackers, start, end);

	//keep calling diceBattle until one cell runs out of units
	while ((abs(boardUnits[start])>0) && (abs(boardUnits[end])>0)) {
		diceBattle(start, end);
	}

	//moves remaining units to new cell, leaving one troop behind
	if ((boardUnits[end] == 0) && (abs(boardUnits[start] != 1))) {
		boardUnits[end] = (abs(boardUnits[start]) - 1)*enemyNegativeChanger;
		boardUnits[start] = 1 * enemyNegativeChanger;
	}

	//if Victor has only 1 unit left, move him to conquered territory
	if ((boardUnits[end] == 0) && (abs(boardUnits[start] == 1))) {
		boardUnits[end] = 1 * enemyNegativeChanger;
		boardUnits[start] = 0;
	}

	printBoard();
	updateUnits();
}

//moves units from start to destination and updates board
void moveUnits(int start, int num, int end) {
	boardUnits[start] = boardUnits[start] - num;
	boardUnits[end] = boardUnits[end] + num;
	updateUnits();

}

//Deployment stage
//first get the amount of units to add to the base (based on number of territories owned and random chance)
//then deploy one by one taking turns while there are units left in base
void deployUnits() {
	getDeploymentUnits();
	bool doneDeploying = false;
	while (!doneDeploying) {
		if (boardUnits[0] != 0) deploy();
		if (boardUnits[10] != 0) aiDeploy();
		if (boardUnits[0] == 0 && boardUnits[10] == 0) doneDeploying = true;
	}
}

//get the amount of units to add to base
//tried multiple methods to make game competitive
//as is, the human player gets a 50% chance to add a player per every unit owned and the computer gets a 70% chance
void getDeploymentUnits() {

	int hChance = 5;
	int cChance = 7;
	int randomChance;

	int hTerritory = 0;
	int cTerritory = 0;

	for (int i = 1; i<10; i++) {
		if (boardUnits[i]>0) {
			randomChance = (rand() % 10);
			if (randomChance<hChance) boardUnits[0]++;

			//hTerritory ++;
			//if (hTerritory == 2){
			//	boardUnits[0]++;
			//	hTerritory =0;
			//}
		}
		if (boardUnits[i]<0) {
			randomChance = (rand() % 10);
			if (randomChance<cChance) boardUnits[10]--;

			//boardUnits[10]--;
			/*cTerritory++;
			if (cTerritory == 2){
			boardUnits[10]--;
			cTerritory=0;
			}*/
		}
	}
	//Update Base Units
	setCursorTo(24, 3);
	cout << boardUnits[0];

	//Update Enemy Base Units
	setCursorTo(24, 16);
	cout << -boardUnits[10];
}

//deploy human unit from base
void deploy() {

	int x;
	bool valid = false;

	while (!valid)
	{

		setCursorTo(0, 22);
		cout << "You have a unit to deploy.  Where do you want to deploy it?                   " << endl;
		cout << "     ";
		setCursorTo(0, 23);
		cin >> x;
		if ((x>0 && x<10) && (boardUnits[x]>-1)) valid = true; else cout << "              INVALID CHOOSE AGAIN.";
	}
	setCursorTo(0, 24);
	cout << "                                         ";
	moveUnits(0, 1, x);

}

//ai randomly deploys is units from base
void aiDeploy() {

	bool goodMove = false;
	int cell = 1;

	while (!goodMove) {
		cell = (rand() % 9) + 1;
		if (boardUnits[cell]<1) goodMove = true;
	}

	moveUnits(10, -1, cell);

	setCursorTo(0, 22);
	cout << "The Enemy is deploying a unit to cell " << cell << "                                                       " << endl;
	Sleep(2000);
}

//dice battle based on the real rules of risk
//defender wins tie dice (tie die)
void diceBattle(int start, int end) {
	int attackers = boardUnits[start];
	int defenders = boardUnits[end] * -1;

	int aNumDie, dNumDie;
	int attackerLostUnits = 0, defenderLostUnits = 0;


	//rolls all dice at beginning regardless of numbers being used
	int aDie1 = roll(), aDie2 = roll(), aDie3 = roll(), dDie1 = roll(), dDie2 = roll();
	int temp;


	char c = 'X';
	int makePositive = 1;

	if (boardUnits[start]<0) {
		c = '#';
		makePositive = -1;
	}


	system("CLS");
	cout << "\n+-----------------DICE BATTLE-----------------+\n\n" << endl;
	cout << "ATTACKERS-------------DEFENDERS-------------------+\n" << endl;

	for (int i = 0; i<boardUnits[start] * makePositive; i++) cout << c;  //prints units
	cout << "    VS    ";

	if (c == 'X') c = '#'; else c = 'X';
	if (c == 'X') makePositive = 1; else makePositive = -1;
	for (int i = 0; i<boardUnits[end] * makePositive; i++) cout << c;  //prints units
	Sleep(1000);


	//finds number of attacking dice to use based on # units attacking
	if (abs(attackers)>2) aNumDie = 3;
	else if (abs(attackers) == 2) aNumDie = 2;
	else aNumDie = 1;

	//number of defending die
	if (abs(defenders)>1) dNumDie = 2; else dNumDie = 1;

	//This section puts the dice in decending order
	if (aNumDie == 3) {
		if (returnLowestDieNum(aDie1, aDie2, aDie3) == 1) {
			temp = aDie1;
			aDie1 = aDie3;
			aDie3 = temp;
		}
		else if (returnLowestDieNum(aDie1, aDie2, aDie3) == 2) {
			temp = aDie2;
			aDie2 = aDie3;
			aDie3 = aDie2;
		}
	}

	if (aNumDie>1 && returnLowestDieNum(aDie1, aDie2) == 1) {
		temp = aDie1;
		aDie1 = aDie2;
		aDie2 = temp;
		if (aNumDie>2 && returnLowestDieNum(aDie2, aDie3) == 1) {
			temp = aDie2;
			aDie2 = aDie3;
			aDie3 = temp;
		}

	}

	if (dNumDie == 2 && returnLowestDieNum(dDie1, dDie2) == 1) {
		temp = dDie1;
		dDie1 = dDie2;
		dDie2 = temp;
	}


	// compares two highest dice, then next highest if necessary (2+ defenders), adds to variable holding amount of units lost
	if (aDie1 > dDie1) defenderLostUnits++; else attackerLostUnits++;
	if (dNumDie == 2 && aNumDie != 1) {
		if (aDie2 > dDie2) defenderLostUnits++; else attackerLostUnits++;
	}

	// update units with lost units
	if (attackers>0) {
		boardUnits[start] = boardUnits[start] - attackerLostUnits;
		boardUnits[end] = boardUnits[end] + defenderLostUnits;
	}
	else {
		boardUnits[start] = boardUnits[start] + attackerLostUnits;
		boardUnits[end] = boardUnits[end] - defenderLostUnits;
	}

	//output the info to the player
	cout << "\n\nATTACKERS ROLL " << aNumDie << " DICE" << endl;
	Sleep(1000);
	cout << "RESULTS :";
	if (aNumDie == 1) cout << aDie1 << endl;
	else if (aNumDie == 2) cout << aDie1 << " , " << aDie2 << endl;
	else if (aNumDie == 3) cout << aDie1 << " , " << aDie2 << " , " << aDie3 << endl;
	Sleep(1000);

	cout << "\nDEFENDERS ROLL " << dNumDie << " DICE" << endl;
	Sleep(1000);
	cout << "RESULTS :";
	if (dNumDie == 1) cout << dDie1 << endl;
	else if (dNumDie == 2) cout << dDie1 << " , " << dDie2 << endl;
	Sleep(1000);


	cout << "\nAttackers lost units:" << attackerLostUnits << endl;
	cout << "Defenders lost units:" << defenderLostUnits << endl;
	Sleep(2000);

	//check for battle complete, when one of the two cells has no more units left
	if (boardUnits[end] == 0) {
		cout << "\nBATTLE COMPLETE" << endl;
		cout << "\nATTACK SUCCESSFUL: TERRITORY CONQUERED" << endl;
	}
	else if (boardUnits[start] == 0) {
		cout << "\nBATTLE COMPLETE" << endl;
		cout << "ATTACK FAILED: TERRITORY DEFENDED" << endl;
	}
	Sleep(3000);
	system("CLS");

}

//returns lowest die position
int returnLowestDieNum(int a, int b, int c) {
	if (a <= b && a <= c) return 1;
	if (b <= a && b <= c) return 2;
	return 3;
}

//returns lowest die position
int returnLowestDieNum(int a, int b) {
	if (a<b)return 1; else return 2;
}

//simple rolling of the die
int roll() {

	return (rand() % 6) + 1;
}




//Gets starting unit spot
int getUserUnitStart() {
	int x;
	bool valid = false;
	while (!valid)
	{

		setCursorTo(0, 22);
		cout << "From which cell would you like to move your units?  (1-9)                   " << endl;
		cout << "     ";
		setCursorTo(0, 23);
		cin >> x;
		if ((x>-1 && x<10) && (areUnitsInCell(x))) valid = true; else cout << "              INVALID CHOOSE AGAIN.";
	}
	setCursorTo(0, 24);
	cout << "                                         ";
	return x;
}

//gets the user input for number of units to move
int getUserUnitNumbers(int cell) {
	int x;
	int unitsAvailable = boardUnits[cell];
	bool valid = false;
	while (!valid)
	{
		setCursorTo(0, 22);
		cout << "How many units would you like to move from cell #" << cell << "? " << " Available:(1-" << unitsAvailable << ")                  " << endl;
		cout << "     ";
		setCursorTo(0, 23);
		cin >> x;
		if (x>0 && x<unitsAvailable + 1) valid = true; else cout << "             INVALID CHOOSE AGAIN.";
	}
	setCursorTo(0, 24);
	cout << "                                            ";
	return x;
}

//gets the user input for destination
int	getUserUnitDestination(int cellStart, int numUnits) {
	int x;
	bool valid = false;
	while (!valid)
	{

		setCursorTo(0, 22);
		cout << "From cell #" << cellStart << " where do you want to move your units to?  (Must be touching cell)             " << endl;
		cout << "         ";
		setCursorTo(0, 23);
		cin >> x;
		if ((x>0 && x<10) && (inRange(cellStart, x))) valid = true; else cout << "          INVALID CHOOSE AGAIN.";
	}
	setCursorTo(0, 24);
	cout << "                                     ";
	return x;
}

//Checks if there are any units in the cell in which the user is trying to move from
bool areUnitsInCell(int cell) {
	if (boardUnits[cell]>0) return true; else return false;
}

//Checks if destination cell is in range
bool inRange(int start, int destination) {

	if ((start == 0) && ((destination == 1) || (destination == 2) || (destination == 3))) return true;
	if ((start == 1) && ((destination == 2) || (destination == 4))) return true;
	if ((start == 2) && ((destination == 1) || (destination == 3) || (destination == 5)))return true;
	if ((start == 3) && ((destination == 2) || (destination == 6))) return true;
	if ((start == 4) && ((destination == 1) || (destination == 5) || (destination == 7)))return true;
	if ((start == 5) && ((destination == 2) || (destination == 4) || (destination == 6) || (destination == 8)))return true;
	if ((start == 6) && ((destination == 9) || (destination == 5) || (destination == 3))) return true;
	if ((start == 7) && ((destination == 8) || (destination == 4)))return true;
	if ((start == 8) && ((destination == 5) || (destination == 7) || (destination == 9)))return true;
	if ((start == 9) && ((destination == 6) || (destination == 8)))return true;
	if ((start == 10) && ((destination == 7) || (destination == 8) || (destination == 9))) return true;
	return false;
}


//sets cursor to specific spot
void setCursorTo(int x, int y)
{

	HANDLE handle;

	COORD position;

	handle = GetStdHandle(STD_OUTPUT_HANDLE);

	position.X = x;

	position.Y = y;

	SetConsoleCursorPosition(handle, position);

}

//prints game board area
void printBoard() {
	char gameBoard[21][49] =
	{
		{ ' ',' ',' ',' ',' ',' ',' ',' ','+','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','+',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
		{ ' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ','P','L','A','Y','E','R',' ','B','A','S','E',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
		{ ' ',' ',' ',' ',' ',' ',' ',' ','+','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','+',' ',' ',' ','P','L','A','Y','E','R',' ','U','N','I','T','S',':',' ','X',' ',' ',' ' },
		{ ' ',' ',' ',' ',' ',' ',' ',' ','|',' ','U','N','I','T','S',' ','I','N',' ','B','A','S','E',':',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
		{ ' ',' ',' ',' ',' ',' ',' ',' ','+','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','+',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
		{ '_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_','_',' ',' ',' ',' ',' ',' ',' ',' ' },
		{ '|',' ',' ','|','C','E','L','L',':','1','|',' ',' ','|',' ',' ',' ','|','C','E','L','L',':','2','|',' ',' ','|',' ',' ','|','C','E','L','L',':','3','|',' ',' ','|',' ',' ',' ',' ',' ',' ',' ' },
		{ '|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ' },
		{ '|','_','_','_','_','_','_','_','_','_','_','_','_','|','_','_','_','_','_','_','_','_','_','_','_','_','_','|','_','_','_','_','_','_','_','_','_','_','_','_','|',' ',' ',' ',' ',' ',' ',' ' },
		{ '|',' ',' ','|','C','E','L','L',':','4','|',' ',' ','|',' ',' ',' ','|','C','E','L','L',':','5','|',' ',' ','|',' ',' ','|','C','E','L','L',':','6','|',' ',' ','|',' ',' ',' ',' ',' ',' ',' ' },
		{ '|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ' },
		{ '|','_','_','_','_','_','_','_','_','_','_','_','_','|','_','_','_','_','_','_','_','_','_','_','_','_','_','|','_','_','_','_','_','_','_','_','_','_','_','_','|',' ',' ',' ',' ',' ',' ',' ' },
		{ '|',' ',' ','|','C','E','L','L',':','7','|',' ',' ','|',' ',' ',' ','|','C','E','L','L',':','8','|',' ',' ','|',' ',' ','|','C','E','L','L',':','9','|',' ',' ','|',' ',' ',' ',' ',' ',' ',' ' },
		{ '|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ' },
		{ '|','_','_','_','_','_','_','_','_','_','_','_','_','|','_','_','_','_','_','_','_','_','_','_','_','_','_','|','_','_','_','_','_','_','_','_','_','_','_','_','|',' ',' ',' ',' ',' ',' ',' ' },
		{ ' ',' ',' ',' ',' ',' ',' ',' ','+','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','+',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
		{ ' ',' ',' ',' ',' ',' ',' ',' ','|',' ','U','N','I','T','S',' ','I','N',' ','B','A','S','E',':',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
		{ ' ',' ',' ',' ',' ',' ',' ',' ','+','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','+',' ',' ',' ','E','N','E','M','Y',' ',' ','U','N','I','T','S',':',' ','#',' ',' ',' ' },
		{ ' ',' ',' ',' ',' ',' ',' ',' ','|',' ',' ',' ','E','N','E','M','Y',' ',' ','B','A','S','E',' ',' ',' ','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' },
		{ ' ',' ',' ',' ',' ',' ',' ',' ','+','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','+',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' }
	};

	setCursorTo(0, 0);
	for (int i = 0; i<21; i++) {
		for (int j = 0; j<49; j++) {
			cout << gameBoard[i][j];

		}
		setCursorTo(0, 1 + i);
	}

	updateUnits();



}

//updates drawing of units in each cell, including bases
void updateUnits() {
	//Update Base Units
	setCursorTo(24, 3);
	cout << boardUnits[0];

	//Update Enemy Base Units
	setCursorTo(24, 16);
	cout << -boardUnits[10];

	//update cell 1
	setCursorTo(2, 7);
	printUnits(1);

	//update cell 2
	setCursorTo(16, 7);
	printUnits(2);

	//update cell 3
	setCursorTo(30, 7);
	printUnits(3);

	//update cell 4
	setCursorTo(2, 10);
	printUnits(4);

	//update cell 5
	setCursorTo(16, 10);
	printUnits(5);

	//update cell 6
	setCursorTo(30, 10);
	printUnits(6);

	//update cell 7
	setCursorTo(2, 13);
	printUnits(7);

	//update cell 8
	setCursorTo(16, 13);
	printUnits(8);

	//update cell 9
	setCursorTo(30, 13);
	printUnits(9);

}

//prints units in a cell, does not position cursor
void printUnits(int cell) {
	char c = 'X';
	int makePositive = 1;

	if (boardUnits[cell]<0) {
		c = '#';
		makePositive = -1;
	}
	for (int i = 0; i<boardUnits[cell] * makePositive; i++) cout << c;  //prints units
	for (int i = 0; i<9 - boardUnits[cell] * makePositive; i++) cout << " ";//covers up any leftover units

};

//prints intro and brief instruction
void intro() {
	char tron[9][27] = { { '=','=','=','=',' ',' ','=','=','=','=','=','=','=','=',' ',' ','=','=','=','=',' ',' ','=',' ',' ',' ','=' },
	{ '=',' ',' ','=',' ',' ',' ',' ',' ','=','=',' ',' ',' ',' ',' ','=',' ',' ','=',' ',' ','=',' ',' ','=',' ' },
	{ '=',' ','=',' ',' ',' ',' ',' ',' ','=','=',' ',' ',' ',' ',' ',' ','=',' ',' ',' ',' ','=',' ','=',' ',' ' },
	{ '=','=',' ',' ',' ',' ',' ',' ',' ','=','=',' ',' ',' ',' ',' ',' ',' ','=',' ',' ',' ','=','=',' ',' ',' ' },
	{ '=','=',' ',' ',' ',' ',' ',' ',' ','=','=',' ',' ',' ',' ',' ',' ',' ',' ','=',' ',' ','=','=',' ',' ',' ' },
	{ '=',' ','=',' ',' ',' ',' ',' ',' ','=','=',' ',' ',' ',' ',' ','=',' ',' ','=',' ',' ','=','=',' ',' ',' ' },
	{ '=',' ',' ','=',' ',' ',' ',' ',' ','=','=',' ',' ',' ',' ',' ','=',' ','=',' ',' ',' ','=',' ','=',' ',' ' },
	{ '=',' ',' ','=',' ',' ',' ',' ',' ','=','=',' ',' ',' ',' ',' ','=','=',' ',' ',' ',' ','=',' ',' ','=',' ' },
	{ '=',' ',' ','=',' ',' ','=','=','=','=','=','=','=','=',' ',' ','=',' ',' ',' ',' ',' ','=',' ',' ',' ','=' }
	};
	setCursorTo(5, 0);
	cout << "         WELCOME TO:      ";
	setCursorTo(5, 1);
	for (int i = 0; i<9; i++) {
		for (int j = 0; j<27; j++) {
			cout << tron[i][j];
			Sleep(15);
		}
		setCursorTo(5, 2 + i);
	}

	setCursorTo(5, 11);
	Sleep(400);
	cout << "COMBAT SIMULATOR." << endl;
	Sleep(800);

	cout << "Follow on-screen prompts to play.  " << endl;
	cout << "Each player starts with 9 units in base." << endl;
	cout << "Player unit denoted by: X " << endl;
	cout << "Enemy units denoted by: # " << endl;
	cout << "GAMEPLAY NOTES:  " << endl;
	cout << "* Can Deploy units from base to ANY cell that is not occupied by the enemy." << endl;
	cout << "* Additional units are added to the base based on number of cells occupied and random chance. " << endl;
	cout << "(50% per cell for human, 70% for AI)" << endl;
	cout << "*After a succesful attack, one unit is left behind and remaining units advance to conquered territory" << endl;
	system("pause");
	system("CLS");
	setCursorTo(0, 0);

}

//asks a yes no question
char askYesNo(string question)
{
	char response;
	setCursorTo(0, 22);
	cout << "                                                                               " << endl << "         ";
	setCursorTo(0, 22);
	do
	{
		cout << question << " (y/n): ";
		cin >> response;
	} while (response != 'y' && response != 'n');

	return response;
}

//find a random cell that contains enemy units
int getEnemyStartPos() {

	int rCell = (rand() % 10) + 1;
	while (boardUnits[rCell]>-1) {
		rCell = (rand() % 10) + 1;
	}
	return rCell;
}

//find random num of units, returns negative number for enemy
int getEnemyNumUnits(int cell) {

	int rUnits = (rand() % (boardUnits[cell] * -1)) + 1;
	return -rUnits;
}

//gets a random boardering destination based on a start cell
int getEnemyDestination(int cell) {

	int rCell = (rand() % 9) + 1;
	while (!(inRange(cell, rCell))) {
		rCell = (rand() % 9) + 1;
	}
	return rCell;
}

//prints the enemy move
void printEnemyMove(int start, int num, int end) {
	clearOutputLines();
	for (int i = 0; i< 7; i++) {
		setCursorTo(0, 21);
		Sleep(100);
		cout << "**   ENEMY MOVES ( " << -num << " ) units to CELL ( " << end << " )   **";
		Sleep(500);
		setCursorTo(0, 21);
		cout << "                                                               ";

	}
}

//clears output so new output can be sent to screen
void clearOutputLines() {
	setCursorTo(0, 21);
	cout << "                                                                                         ";
	setCursorTo(0, 22);
	cout << "                                                                                         ";
	setCursorTo(0, 23);
	cout << "                                                                                         ";
}

//Announces attack to user when enemy attack
void announceAttack(int attackers, int start, int end) {
	clearOutputLines();
	for (int i = 0; i< 7; i++) {
		setCursorTo(0, 21);
		Sleep(100);
		if (attackers<0) {
			cout << "**   ENEMY ATTACKS CELL ( " << end << " ) WITH ( " << -attackers << " ) UNITS!   **";
		}
		else {
			cout << "**   YOU ATTACK ENEMY CELL ( " << end << " ) WITH ( " << attackers << " ) UNITS!   **";
		}
		Sleep(500);
		setCursorTo(0, 21);
		cout << "                                                               ";
	}
}


//prints and blinks end game message
void printEndGame(game_over_t go) {
	string words = "";
	if (go == PLAYER_LOSES) {
		words = "** YOU HAVE BEEN DESTROYED BY THE ENEMY AND HAVE LOST THE GAME **";
	}
	else {
		words = "** YOU HAVE DESTROYED THE ENEMY AND WON THE GAME **";
	}

	clearOutputLines();
	for (int i = 0; i< 40; i++) {
		setCursorTo(0, 21);
		Sleep(100);
		cout << words;
		Sleep(300);
		setCursorTo(0, 21);
		cout << "                                                                          ";
	}
}

