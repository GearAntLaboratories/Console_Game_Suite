//Grant Gotwald
//TRON
//The Classic Game of Awesome.
//USE KEYS W,A,S,D to move character  ** CHANGE THE SPEED CONSTANT IF TOO FAST! **

#include "stdafx.h"
#include "Games.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

enum game_over_t { GAME_NOT_OVER, COMPUTER_LOSES, PLAYER_LOSES };
enum directions { UP = 119, DOWN = 115, LEFT = 97, RIGHT = 100 };

//Functions
void setupBoard();
void tronIntro();
void displayBoard();
int getInput(int pkey);
void updatePlayerPos(int key);
int getComputerKey(int cKey);
void updateComputerPos(int key);
void updateBoard();
game_over_t checkCollisions();
void printEndGameMsg(game_over_t go);
void setCursorAt(int x, int y);
bool askToPlayAgain();

//artificial intelligence related functions
int aiBorderCheck(int i);
bool checkUp(int i);
bool checkDown(int i);
bool checkLeft(int i);
bool checkRight(int i);
bool bestPath(int i);

//Constants
const int HEIGHT = 24;
const int WIDTH = 79;
const int SPEED = 55;

//Global Variables
char board[WIDTH][HEIGHT];

//Positions ux,uy for user.  cx,cy for computer.
int ux, uy, cx, cy;


//Main - This is where the magic happens.
int Tron()
{
	//Variables for the direction of the user and computer.
	int key;
	int cKey;

	bool playAgain = true;
	game_over_t gameOver;

	tronIntro();           //Calls an tronIntroduction function

	while (playAgain) {


		gameOver = GAME_NOT_OVER;

		//Start bikes off towards each other.
		key = RIGHT;
		cKey = LEFT;

		setupBoard();  //setup game board and display it

				  //Main game loop, runs until collision by either biker
		while (gameOver == GAME_NOT_OVER) {
			key = getInput(key);                  //calls function to get user input
			cKey = getComputerKey(cKey);          //calls function to get computer input
			updatePlayerPos(key);                 //updates player position
			updateComputerPos(cKey);              //updates compuer position

			gameOver = checkCollisions();		     //checks for any possible collisions with wall or bike trails
			updateBoard();                       //updates the display

			Sleep(SPEED);                             //controls the speed of the game	
		}

		printEndGameMsg(gameOver);   //prints end game message
		playAgain = askToPlayAgain();  //Checks to see if player wants to try again

	}

	return 0;
}

//Sets up game, creates board with a border and start positions then calls display function
void setupBoard() {
	//Sets up initial conditions for each bike and for new run at game.
	ux = 30;
	uy = 20;
	cx = WIDTH - 30;
	cy = 10;

	for (int i = 0; i<WIDTH; i++)
	{
		for (int j = 0; j<HEIGHT; j++)
		{
			if (i == 0 || i == WIDTH - 1) board[i][j] = '|';
			else if (j == 0 || j == HEIGHT - 1) board[i][j] = '=';
			else board[i][j] = ' ';
		}
	}

	board[ux][uy] = 'x';
	board[cx][cy] = '*';

	displayBoard();
}

//Prints entire board array
void displayBoard() {
	setCursorAt(0, 0);
	for (int i = 0; i<HEIGHT; i++)
	{

		for (int j = 0; j<WIDTH; j++)
		{
			cout << board[j][i];
		}
		cout << endl;
	}
}


//Used to get user input, checks to make sure no backtracking
int getInput(int pkey) {
	int key = pkey;
	if (_kbhit()) {
		key = _getch();
	}

	// Doesn't change direction if user attempts to turn backwards into own path  
	if (
		((pkey == UP) && (key == DOWN)) ||
		((pkey == DOWN) && (key == UP)) ||
		((pkey == LEFT) && (key == RIGHT)) ||
		((pkey == RIGHT) && (key == LEFT))
		) key = pkey;

	return key;
}


//Updates player position based on keys W,A,S,D  
void updatePlayerPos(int key) {
	switch (key)
	{
	case UP:
		uy--;
		break;
	case LEFT:
		ux--;
		break;
	case DOWN:
		uy++;
		break;
	case RIGHT:
		ux++;
		break;
	default:
		break;
	}
}

//Function to determine computer key pressed (direction to go).  Computer is a four part AI.
//First, creates a random number to check whether to try to change direction or not at all.
//Second, checks if there is an obstacle in direction.
//Third, determines to see if there is a safe direction to turn.
//Forth, Determines best route out of multiple options
int getComputerKey(int cKey) {
	int rNum = ((rand() + time(0)) % 4) + 1;        //Random turn or not

	if (rNum != 1) return aiBorderCheck(cKey);      //if no turn, still check if there is an obstacle in direction currently heading
	else {
		rNum = ((rand() + time(0)) % 2) + 1;       //choose random direction
		switch (rNum)
		{

		case 1:
			if (checkUp(1) && checkDown(1))
			{
				if (bestPath(DOWN)) {
					rNum = DOWN;
					break;
				}
				else {
					rNum = UP;
					break;
				}
			}
			else if (checkUp(1))
			{
				rNum = UP;
				break;
			}
			else if (checkDown(1)) {
				rNum = DOWN;
				break;
			}
			else {
				rNum = cKey;
				break;
			}
		case 2:
			if (checkLeft(1) && checkRight(1))
			{
				if (bestPath(LEFT)) {
					rNum = LEFT;
					break;
				}
				else {
					rNum = RIGHT;
					break;
				}
			}
			else if (checkLeft(1))
			{
				rNum = LEFT;
				break;
			}
			else if (checkRight(1)) {
				rNum = RIGHT;
				break;
			}
			else {
				rNum = cKey;
				break;
			}


		default:
			break;
		}

		return aiBorderCheck(rNum);  //once random direction chosen, run further ai checks
	}

}

//Group of functions checks the four directions i spots away from current AI position
bool checkUp(int i) {
	if ((board[cx][cy - i] != 'x') && (board[cx][cy - i] != '*') && (board[cx][cy - i] != '|') && (board[cx][cy - i] != '='))
		return true;
	else
		return false;
}

bool checkDown(int i) {
	if ((board[cx][cy + i] != 'x') && (board[cx][cy + i] != '*') && (board[cx][cy + i] != '|') && (board[cx][cy + i] != '='))
		return true;
	else
		return false;
}

bool checkLeft(int i) {
	if ((board[cx - i][cy] != 'x') && (board[cx - i][cy] != '*') && (board[cx - i][cy] != '|') && (board[cx - i][cy] != '='))
		return true;
	else
		return false;
}

bool checkRight(int i) {
	if ((board[cx + i][cy] != 'x') && (board[cx + i][cy] != '*') && (board[cx + i][cy] != '|') && (board[cx + i][cy] != '='))
		return true;
	else
		return false;
}

//given a possible direction determines whether that is the best or the opposite of that direction is the best
bool bestPath(int direction1) {
	int dirOneSpotsToObstacle = 1;
	int dirTwoSpotsToObstacle = 1;

	if ((direction1 == LEFT) || (direction1 == RIGHT)) {
		while (checkLeft(dirOneSpotsToObstacle)) {
			dirOneSpotsToObstacle++;
		}
		while (checkRight(dirTwoSpotsToObstacle)) {
			dirTwoSpotsToObstacle++;
		}
		if (dirOneSpotsToObstacle>dirTwoSpotsToObstacle) {
			return true;
		}
		else return false;
	}

	if ((direction1 == DOWN) || (direction1 == UP)) {
		while (checkDown(dirOneSpotsToObstacle)) {
			dirOneSpotsToObstacle++;
		}
		while (checkUp(dirTwoSpotsToObstacle)) {
			dirTwoSpotsToObstacle++;
		}
		if (dirOneSpotsToObstacle>dirTwoSpotsToObstacle) {
			return true;
		}
		else return false;
	}
	return true;
}




//Just finished Artificial intelligence.  When AI encounters an obstacle tries to change direction, if multiple
//directions available determines the best route of the two.
int aiBorderCheck(int i) {

	switch (i) {
	case UP:

		if (((board[cx][cy - 1] == 'x') || (board[cx][cy - 1] == '*'))
			|| ((board[cx][cy - 1] == '|') || (board[cx][cy - 1] == '=')))
		{
			if (checkLeft(1) && checkRight(1))
			{
				if (bestPath(LEFT)) {
					i = LEFT;
					break;
				}
				else {
					i = RIGHT;
					break;
				}
			}
			else if (checkLeft(1))
			{
				i = LEFT;
				break;
			}
			else if (checkRight(1)) {
				i = RIGHT;
				break;
			}
		}
		else
			break;

	case DOWN:

		if (((board[cx][cy + 1] == 'x') || (board[cx][cy + 1] == '*'))
			|| ((board[cx][cy + 1] == '|') || (board[cx][cy + 1] == '=')))
		{
			if (checkLeft(1) && checkRight(1))
			{
				if (bestPath(LEFT)) {
					i = LEFT;
					break;
				}
				else {
					i = RIGHT;
					break;
				}
			}
			else if (checkLeft(1))
			{
				i = LEFT;
				break;
			}
			else if (checkRight(1)) {
				i = RIGHT;
				break;
			}
		}
		else
			break;

	case LEFT:

		if (((board[cx - 1][cy] == 'x') || (board[cx - 1][cy] == '*'))
			|| ((board[cx - 1][cy] == '|') || (board[cx - 1][cy] == '=')))
		{
			if (checkUp(1) && checkDown(1))
			{
				if (bestPath(DOWN)) {
					i = DOWN;
					break;
				}
				else {
					i = UP;
					break;
				}
			}
			else if (checkUp(1))
			{
				i = UP;
				break;
			}
			else if (checkDown(1)) {
				i = DOWN;
				break;
			}
		}
		else
			break;

	case RIGHT:

		if (((board[cx + 1][cy] == 'x') || (board[cx + 1][cy] == '*'))
			|| ((board[cx + 1][cy] == '|') || (board[cx + 1][cy] == '=')))
		{
			if (checkUp(1) && checkDown(1))
			{
				if (bestPath(DOWN)) {
					i = DOWN;
					break;
				}
				else {
					i = UP;
					break;
				}
			}
			else if (checkUp(1))
			{
				i = UP;
				break;
			}
			else if (checkDown(1)) {
				i = DOWN;
				break;
			}
		}
		else
			break;

	default:
		break;
	}
	return i;
}



//updates x,y coordinates of computer bike depending on direction key
void updateComputerPos(int key) {
	switch (key)
	{
	case UP:
		cy--;
		break;
	case LEFT:
		cx--;
		break;
	case DOWN:
		cy++;
		break;
	case RIGHT:
		cx++;
		break;
	default:
		break;
	}
}


//Updates game board and displays new bike positions
void updateBoard() {
	setCursorAt(ux, uy);
	board[ux][uy] = 'x';
	cout << "x";

	setCursorAt(cx, cy);
	board[cx][cy] = '*';
	cout << "*";
}

//Checks for various collisions with trails, bikes, and walls
game_over_t checkCollisions() {
	if (((board[ux][uy] == 'x') || (board[ux][uy] == '*'))
		|| ((board[ux][uy] == '|') || (board[ux][uy] == '=')))
	{
		return PLAYER_LOSES;
	}
	else if (((board[cx][cy] == 'x') || (board[cx][cy] == '*'))
		|| ((board[cx][cy] == '|') || (board[cx][cy] == '=')))
	{
		return COMPUTER_LOSES;
	}
	return GAME_NOT_OVER;

}

//Prints end game messages
void printEndGameMsg(game_over_t go) {
	//Flashes signal at spot of the crash
	int crashSpotX;
	int crashSpotY;
	if (go == PLAYER_LOSES) {
		crashSpotX = ux;
		crashSpotY = uy;
	}
	else {
		crashSpotX = cx;
		crashSpotY = cy;
	}
	for (int i = 0; i<10; i++) {
		setCursorAt(crashSpotX, crashSpotY);
		cout << "#" << endl;
		Sleep(120);
		setCursorAt(crashSpotX, crashSpotY);
		cout << " " << endl;
		Sleep(120);
	}


	//Clears screen and flashes winner or loser message
	system("cls");
	for (int i = 0; i<11; i++) {
		setCursorAt(WIDTH / 2 - 20, HEIGHT / 2 - 4);
		cout << "++++++++++++++++++++++++++++++ ";
		setCursorAt(WIDTH / 2 - 20, HEIGHT / 2 - 3);
		if (go == PLAYER_LOSES) {
			cout << "  COMPUTER WINS!  TRY AGAIN!" << endl;
		}
		else
			cout << "      YOU WIN! CONGRATS!";
		setCursorAt(WIDTH / 2 - 20, HEIGHT / 2 - 2);
		cout << "++++++++++++++++++++++++++++++ " << endl;


		Sleep(150);
		setCursorAt(WIDTH / 2 - 20, HEIGHT / 2 - 4);
		cout << "                               ";
		setCursorAt(WIDTH / 2 - 20, HEIGHT / 2 - 3);
		if (go == PLAYER_LOSES) {
			cout << "  COMPUTER WINS!  TRY AGAIN!" << endl;
		}
		else
			cout << "      YOU WIN! CONGRATS!";
		setCursorAt(WIDTH / 2 - 20, HEIGHT / 2 - 2);
		cout << "                               ";

		Sleep(150);
	}
}

//function to set cursor to specific x,y coordinate on console
void setCursorAt(int x, int y)
{

	HANDLE handle;

	COORD position;

	handle = GetStdHandle(STD_OUTPUT_HANDLE);

	position.X = x;

	position.Y = y;

	SetConsoleCursorPosition(handle, position);

}


//Prints an tronIntro to the tron game
void tronIntro() {
	char tron[9][27] = { { '=','=','=','=','=','=','=','=',' ',' ','=','=','=','=',' ',' ','=','=','=','=',' ',' ','=',' ',' ',' ','=' },
	{ '=','=','=','=','=','=','=','=',' ',' ','=',' ',' ','=',' ',' ','=',' ',' ','=',' ',' ','=','=',' ',' ','=' },
	{ ' ',' ',' ','=','=',' ',' ',' ',' ',' ','=',' ','=',' ',' ',' ','=',' ',' ','=',' ',' ','=',' ','=',' ','=' },
	{ ' ',' ',' ','=','=',' ',' ',' ',' ',' ','=','=',' ',' ',' ',' ','=',' ',' ','=',' ',' ','=',' ','=',' ','=' },
	{ ' ',' ',' ','=','=',' ',' ',' ',' ',' ','=','=',' ',' ',' ',' ','=',' ',' ','=',' ',' ','=',' ','=',' ','=' },
	{ ' ',' ',' ','=','=',' ',' ',' ',' ',' ','=',' ','=',' ',' ',' ','=',' ',' ','=',' ',' ','=',' ','=',' ','=' },
	{ ' ',' ',' ','=','=',' ',' ',' ',' ',' ','=',' ',' ','=',' ',' ','=',' ',' ','=',' ',' ','=',' ','=',' ','=' },
	{ ' ',' ',' ','=','=',' ',' ',' ',' ',' ','=',' ',' ','=',' ',' ','=',' ',' ','=',' ',' ','=',' ',' ','=','=' },
	{ ' ',' ',' ','=','=',' ',' ',' ',' ',' ','=',' ',' ','=',' ',' ','=','=','=','=',' ',' ','=',' ',' ',' ','=' }
	};
	setCursorAt(25, 5);
	cout << "         WELCOME TO:      ";
	setCursorAt(25, 6);
	for (int i = 0; i<9; i++) {
		for (int j = 0; j<27; j++) {
			cout << tron[i][j];
			Sleep(15);
		}
		setCursorAt(25, 7 + i);
	}

	setCursorAt(25, 16);
	Sleep(600);
	cout << "You are the bike -->  x  <--";
	Sleep(1100);
	setCursorAt(25, 17);
	cout << "Computer is bike -->  *  <--";
	Sleep(1100);
	setCursorAt(25, 18);
	cout << "Controls:";
	Sleep(900);
	setCursorAt(25, 19);
	cout << "W  --  UP";
	Sleep(900);
	setCursorAt(25, 20);
	cout << "A  --  LEFT";
	Sleep(900);
	setCursorAt(25, 21);
	cout << "S  --  DOWN";
	Sleep(900);
	setCursorAt(25, 22);
	cout << "D  --  RIGHT";
	Sleep(500);

	setCursorAt(25, 24);
	system("pause");
	system("cls");
	setCursorAt(0, 0);

}


//Function to determine if player wants to attempt another run.
bool askToPlayAgain() {
	string againYN;
	setCursorAt(WIDTH / 2 - 20, HEIGHT / 2 - 2);
	cin.clear();  
	cin.ignore();
	cout << "Wanna Play Again? Y/N:   ";
	cin >> againYN;
	if (againYN == "Y" || againYN == "y")
		return true;
	else
		return false;

}