//TETRIS
#include "stdafx.h"
#include "Games.h"
#include <iostream>
#include <windows.h>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <conio.h>

using namespace std;

enum shapeType { L_SHAPE, J_SHAPE, SQUARE_SHAPE, LINE_SHAPE, Z_SHAPE, S_SHAPE, T_SHAPE };
//enum {UP =72, DOWN=80, LEFT =75, RIGHT =77};
enum { UP = 119, DOWN = 115, LEFT = 97, RIGHT = 100 };     //W,A,S,D CONTROLS


														   //Tetris Shape Class
class TetrisShape {
public:
	char shapeArray[4][4];
	int shapeTopLeftX;
	int shapeTopLeftY;
	int shapeOldTopLeftX;
	int shapeOldTopLeftY;
	bool beenRotated;
	TetrisShape(int);
	void populateShape(int);
	int shapeType;
};

//Constructs specific shape and sets initial coordinates
TetrisShape::TetrisShape(int shape) {
	shapeTopLeftX = 4;
	shapeOldTopLeftX = 4;
	shapeTopLeftY = 0;
	shapeOldTopLeftY = 0;
	beenRotated = false;
	shapeType = shape;
	populateShape(shape);
}

//Function to populate shape
void TetrisShape::populateShape(int shape) {
	shapeType = shape;
	switch (shape)
	{
	case L_SHAPE:
		shapeArray[0][0] = ' '; shapeArray[1][0] = 'X'; shapeArray[2][0] = ' '; shapeArray[3][0] = ' ';
		shapeArray[0][1] = ' '; shapeArray[1][1] = 'X'; shapeArray[2][1] = ' '; shapeArray[3][1] = ' ';
		shapeArray[0][2] = ' '; shapeArray[1][2] = 'X'; shapeArray[2][2] = 'X'; shapeArray[3][2] = ' ';
		shapeArray[0][3] = ' '; shapeArray[1][3] = ' '; shapeArray[2][3] = ' '; shapeArray[3][3] = ' ';
		break;
	case J_SHAPE:
		shapeArray[0][0] = ' '; shapeArray[1][0] = ' '; shapeArray[2][0] = 'X'; shapeArray[3][0] = ' ';
		shapeArray[0][1] = ' '; shapeArray[1][1] = ' '; shapeArray[2][1] = 'X'; shapeArray[3][1] = ' ';
		shapeArray[0][2] = ' '; shapeArray[1][2] = 'X'; shapeArray[2][2] = 'X'; shapeArray[3][2] = ' ';
		shapeArray[0][3] = ' '; shapeArray[1][3] = ' '; shapeArray[2][3] = ' '; shapeArray[3][3] = ' ';
		break;
	case SQUARE_SHAPE:
		shapeArray[0][0] = ' '; shapeArray[1][0] = ' '; shapeArray[2][0] = ' '; shapeArray[3][0] = ' ';
		shapeArray[0][1] = ' '; shapeArray[1][1] = ' '; shapeArray[2][1] = ' '; shapeArray[3][1] = ' ';
		shapeArray[0][2] = ' '; shapeArray[1][2] = 'X'; shapeArray[2][2] = 'X'; shapeArray[3][2] = ' ';
		shapeArray[0][3] = ' '; shapeArray[1][3] = 'X'; shapeArray[2][3] = 'X'; shapeArray[3][3] = ' ';
		break;
	case LINE_SHAPE:
		shapeArray[0][0] = ' '; shapeArray[1][0] = 'X'; shapeArray[2][0] = ' '; shapeArray[3][0] = ' ';
		shapeArray[0][1] = ' '; shapeArray[1][1] = 'X'; shapeArray[2][1] = ' '; shapeArray[3][1] = ' ';
		shapeArray[0][2] = ' '; shapeArray[1][2] = 'X'; shapeArray[2][2] = ' '; shapeArray[3][2] = ' ';
		shapeArray[0][3] = ' '; shapeArray[1][3] = 'X'; shapeArray[2][3] = ' '; shapeArray[3][3] = ' ';
		break;
	case Z_SHAPE:
		shapeArray[0][0] = ' '; shapeArray[1][0] = ' '; shapeArray[2][0] = ' '; shapeArray[3][0] = ' ';
		shapeArray[0][1] = 'X'; shapeArray[1][1] = 'X'; shapeArray[2][1] = ' '; shapeArray[3][1] = ' ';
		shapeArray[0][2] = ' '; shapeArray[1][2] = 'X'; shapeArray[2][2] = 'X'; shapeArray[3][2] = ' ';
		shapeArray[0][3] = ' '; shapeArray[1][3] = ' '; shapeArray[2][3] = ' '; shapeArray[3][3] = ' ';
		break;
	case S_SHAPE:
		shapeArray[0][0] = ' '; shapeArray[1][0] = ' '; shapeArray[2][0] = ' '; shapeArray[3][0] = ' ';
		shapeArray[0][1] = ' '; shapeArray[1][1] = 'X'; shapeArray[2][1] = 'X'; shapeArray[3][1] = ' ';
		shapeArray[0][2] = 'X'; shapeArray[1][2] = 'X'; shapeArray[2][2] = ' '; shapeArray[3][2] = ' ';
		shapeArray[0][3] = ' '; shapeArray[1][3] = ' '; shapeArray[2][3] = ' '; shapeArray[3][3] = ' ';
		break;
	case T_SHAPE:
		shapeArray[0][0] = ' '; shapeArray[1][0] = ' '; shapeArray[2][0] = ' '; shapeArray[3][0] = ' ';
		shapeArray[0][1] = ' '; shapeArray[1][1] = ' '; shapeArray[2][1] = ' '; shapeArray[3][1] = ' ';
		shapeArray[0][2] = 'X'; shapeArray[1][2] = 'X'; shapeArray[2][2] = 'X'; shapeArray[3][2] = ' ';
		shapeArray[0][3] = ' '; shapeArray[1][3] = 'X'; shapeArray[2][3] = ' '; shapeArray[3][3] = ' ';
		break;
	default:
		break;
	}
}


//Functions
void setup();
int randomShape();
TetrisShape drop(TetrisShape localTetrisShape);
int getInput();
TetrisShape rotateShape(TetrisShape localShape);
TetrisShape moveShape(TetrisShape localTetrisShape, int key);
bool checkCollisions(TetrisShape localTetrisShape);
void printShape(TetrisShape localTetrisShape);
void eraseOldShape(TetrisShape localTetrisShape);
void placeOnBoard(TetrisShape localTetrisShape);
void updateSpeed();
void updateScore();
bool checkGameOver();
void putCursorTo(int x, int y);
void drawNext(int shapeType);
void checkRows();
bool rowFilled(int row);
void printTetrisBoard();
void removeRow(int row);

//Constants
const int HEIGHT = 25;
const int WIDTH = 12;


//Global Variables
char board[WIDTH][HEIGHT];
int speed = 150;
int score = 0;

void Tetris()
{
	bool gameOver = false;
	bool stopped = true;   //keeps track of if current shape has stopped in order to create new one

	int key = 0;    //player input

	TetrisShape currentShape(1);
	int lastShape = randomShape();


	setup();  //setup game board

			  //main loop
	while (!gameOver) {

		// checks if shape has stopped, if so creates new shape
		if (stopped == true) {
			int nextShape = randomShape();
			currentShape.shapeTopLeftX = 5; currentShape.shapeTopLeftY = 0;
			currentShape.populateShape(lastShape);
			lastShape = nextShape;
			drawNext(nextShape);
			stopped = false;
		}


		currentShape.shapeOldTopLeftX = currentShape.shapeTopLeftX;
		currentShape.shapeOldTopLeftY = currentShape.shapeTopLeftY;
		currentShape.beenRotated = false;

		key = getInput();  //get input

						   //if player hit key
		if (key != 0)
		{
			//move shape according to key
			currentShape = moveShape(currentShape, key);

			//if a direction movement (not a rotation), checks if collision occured - if no proceed, if yes redraw in previous position
			if (!currentShape.beenRotated)
			{
				if (checkCollisions(currentShape)) {
					currentShape.shapeTopLeftX = currentShape.shapeOldTopLeftX;
					currentShape.shapeTopLeftY = currentShape.shapeOldTopLeftY;
				}
				else {

					eraseOldShape(currentShape);
					printShape(currentShape);
				}
			}
			else  //if rotation movement
			{
				if (checkCollisions(currentShape)) {           //if collision occurs due to rotation, then unrotate (three rights = 1 left )

					currentShape = rotateShape(currentShape);
					currentShape = rotateShape(currentShape);
					currentShape = rotateShape(currentShape);
				}
				else {

					eraseOldShape(currentShape);
					printShape(currentShape);

				}
			}
		}

		//sleep, reset old equal to new and get ready for drop
		Sleep(speed);
		currentShape.shapeOldTopLeftX = currentShape.shapeTopLeftX;
		currentShape.shapeOldTopLeftY = currentShape.shapeTopLeftY;

		//moves shape down 1
		currentShape = drop(currentShape);

		//checks for collision, if yes leave shape on board and set 'stopped' flag
		if (checkCollisions(currentShape))
		{
			currentShape.shapeTopLeftX = currentShape.shapeOldTopLeftX;
			currentShape.shapeTopLeftY = currentShape.shapeOldTopLeftY;
			placeOnBoard(currentShape);
			stopped = true;
		}
		else {

			eraseOldShape(currentShape);
			printShape(currentShape);
		}

		Sleep(speed); //controls speed of game 
		checkRows();  //Check for any rows that may have been completed
		gameOver = checkGameOver();
	}

	putCursorTo(19, 22);
	cout << "**** GAME OVER ****";
	system("pause");
}


//Sets up game, creates board with a border & displays it
void setup() {
	//Sets up board array

	for (int i = 0; i<WIDTH; i++)
	{
		for (int j = 0; j<HEIGHT; j++)
		{
			if (i == 0 || i == WIDTH - 1) board[i][j] = '|';
			else if (j == HEIGHT - 1) board[i][j] = '=';
			else board[i][j] = ' ';
		}
	}

	//Prints entire board array
	printTetrisBoard();


	//Prints instructions
	putCursorTo(17, 0);
	cout << "*****************";
	putCursorTo(17, 1);
	cout << "*  T E T R I S  *";
	putCursorTo(17, 2);
	cout << "*****************";
	putCursorTo(17, 4);
	cout << "*****************";
	putCursorTo(17, 5);
	cout << "|   Next Shape  |";
	putCursorTo(17, 6);
	cout << "|               |";
	putCursorTo(17, 7);
	cout << "|               |";
	putCursorTo(17, 8);
	cout << "|               |";
	putCursorTo(17, 9);
	cout << "|               |";
	putCursorTo(17, 10);
	cout << "|               |";
	putCursorTo(17, 11);
	cout << "*****************";
	putCursorTo(19, 13);
	cout << "SCORE : 0";
	putCursorTo(19, 16);
	cout << "CONTROLS:";
	putCursorTo(19, 17);
	cout << "W - Rotate Shape Clockwise";
	putCursorTo(19, 18);
	cout << "A - Move Shape Left";
	putCursorTo(19, 19);
	cout << "D - Move Shape Right";
	putCursorTo(19, 20);
	cout << "S - Move Shape Down";
}

//prints board array
void printTetrisBoard() {
	putCursorTo(0, 0);
	for (int i = 0; i<HEIGHT; i++)
	{

		for (int j = 0; j<WIDTH; j++)
		{
			cout << board[j][i];
		}
		cout << endl;
	}
}

//draw next shape in box on right of baord
void drawNext(int shapeType) {

	TetrisShape tShape(shapeType);
	tShape.shapeTopLeftX = 24;
	tShape.shapeTopLeftY = 6;
	for (int i = 0; i<4; i++)
	{

		for (int j = 0; j<4; j++)
		{
			putCursorTo(tShape.shapeTopLeftX + i, tShape.shapeTopLeftY + j);
			cout << ' ';
			if (tShape.shapeArray[i][j] == 'X') {
				putCursorTo(tShape.shapeTopLeftX + i, tShape.shapeTopLeftY + j); cout << 'X';
			}
		}
	}
}

//checks row by row by calling rowFilled function to see if it is filled with shapes
void checkRows() {

	for (int i = HEIGHT - 2; i>0; i--)
	{
		if (rowFilled(i))
			removeRow(i);
	}
}

//checks through a single row to see if there are any blanks spots, if none row is filled
bool rowFilled(int row) {
	for (int i = 1; i<WIDTH - 2; i++)
	{
		if (board[i][row] == ' ')
		{
			return false;
		}
	}
	return true;
}

//removes specific row by shifting all other rows down upon it.  Updates score and speed when row is removed
void removeRow(int row) {
	for (int i = row; i>0; i--)
	{
		for (int j = 0; j<WIDTH - 2; j++) {
			board[j][i] = board[j][i - 1];
		}
	}
	printTetrisBoard();
	score += 10;
	putCursorTo(19, 13);
	cout << "SCORE : " << score;
	updateSpeed();
}


//returns a random number 0-6 to create corresponding shape
int randomShape() {
	return  (((rand() + time(0)) % 7));

}

//drops shape one spot
TetrisShape drop(TetrisShape localShape) {
	localShape.shapeTopLeftY = localShape.shapeTopLeftY + 1;
	return localShape;
}

//gets user input
int getInput() {
	int key = 0;
	if (_kbhit()) {
		key = _getch();
	}
	return key;
}

//rotates shape by rotating entire array
TetrisShape rotateShape(TetrisShape shape) {
	char tempCellVal = shape.shapeArray[0][0];

	shape.shapeArray[0][0] = shape.shapeArray[0][3];

	shape.shapeArray[0][3] = shape.shapeArray[3][3];

	shape.shapeArray[3][3] = shape.shapeArray[3][0];

	shape.shapeArray[3][0] = tempCellVal;



	tempCellVal = shape.shapeArray[0][1];

	shape.shapeArray[0][1] = shape.shapeArray[1][3];

	shape.shapeArray[1][3] = shape.shapeArray[3][2];

	shape.shapeArray[3][2] = shape.shapeArray[2][0];

	shape.shapeArray[2][0] = tempCellVal;



	tempCellVal = shape.shapeArray[0][2];

	shape.shapeArray[0][2] = shape.shapeArray[2][3];

	shape.shapeArray[2][3] = shape.shapeArray[3][1];

	shape.shapeArray[3][1] = shape.shapeArray[1][0];

	shape.shapeArray[1][0] = tempCellVal;



	tempCellVal = shape.shapeArray[1][1];

	shape.shapeArray[1][1] = shape.shapeArray[1][2];

	shape.shapeArray[1][2] = shape.shapeArray[2][2];

	shape.shapeArray[2][2] = shape.shapeArray[2][1];

	shape.shapeArray[2][1] = tempCellVal;

	shape.beenRotated = true;
	return shape;
}

//moves shape, left, right, up or rotates (if not a square)
TetrisShape moveShape(TetrisShape shape, int key) {
	switch (key)
	{
	case LEFT:
		shape.shapeTopLeftX = shape.shapeTopLeftX - 1;
		break;
	case RIGHT:
		shape.shapeTopLeftX = shape.shapeTopLeftX + 1;
		break;
	case DOWN:
		shape.shapeTopLeftY = shape.shapeTopLeftY + 1;
		break;
	case UP:
		if (shape.shapeType != SQUARE_SHAPE)
			shape = rotateShape(shape);

		break;
	default:
		break;
	}
	return shape;
}

//checks for collisions
bool checkCollisions(TetrisShape shape) {
	for (int i = 0; i<4; i++)
	{

		for (int j = 0; j<4; j++)
		{

			if ((shape.shapeArray[i][j] == 'X') &&
				(board[shape.shapeTopLeftX + i][shape.shapeTopLeftY + j] != ' '))
				return true;
		}
	}

	return false;


}

//places stopped piece into board array
void placeOnBoard(TetrisShape localTetrisShape) {

	for (int i = 0; i<4; i++)
	{

		for (int j = 0; j<4; j++)
		{
			if (localTetrisShape.shapeArray[i][j] == 'X') {                        //only places X's not ' 's
				board[localTetrisShape.shapeTopLeftX + i][localTetrisShape.shapeTopLeftY + j] = localTetrisShape.shapeArray[i][j];
			}
		}
	}

}

//prints shape on screen, does not add to board array
void printShape(TetrisShape localTetrisShape) {


	for (int i = 0; i<4; i++)
	{

		for (int j = 0; j<4; j++)
		{
			if (localTetrisShape.shapeArray[i][j] == 'X') {
				putCursorTo(localTetrisShape.shapeTopLeftX + i, localTetrisShape.shapeTopLeftY + j);
				cout << 'X';
			}

		}
	}

}

//erases the last position of the current shape
void eraseOldShape(TetrisShape localTetrisShape) {


	for (int i = 0; i<4; i++)
	{

		for (int j = 0; j<4; j++)
		{
			if (localTetrisShape.shapeArray[i][j] == 'X') {
				putCursorTo(localTetrisShape.shapeOldTopLeftX + i, localTetrisShape.shapeOldTopLeftY + j);
				cout << ' ';
			}
		}
	}


	if (localTetrisShape.beenRotated == true)                     //rotates back to position if rotated to erase appropriate old piece
	{
		localTetrisShape = rotateShape(localTetrisShape);
		localTetrisShape = rotateShape(localTetrisShape);
		localTetrisShape = rotateShape(localTetrisShape);

		for (int i = 0; i<4; i++)
		{

			for (int j = 0; j<4; j++)
			{
				if (localTetrisShape.shapeArray[i][j] == 'X') {
					putCursorTo(localTetrisShape.shapeOldTopLeftX + i, localTetrisShape.shapeOldTopLeftY + j);
					cout << ' ';
				}
			}
		}
		localTetrisShape = rotateShape(localTetrisShape);
	}
}

//Increases speed of game
void updateSpeed() {
	speed -= 5;
}


//checks if shape has made it to the top
bool checkGameOver() {
	for (int i = 1; i<WIDTH - 2; i++)
	{
		if (board[i][0] == 'X') return true;
	}
	return false;
}

//function to set cursor to specific x,y coordinate on console
void putCursorTo(int x, int y)
{

	HANDLE handle;

	COORD position;

	handle = GetStdHandle(STD_OUTPUT_HANDLE);

	position.X = x;

	position.Y = y;

	SetConsoleCursorPosition(handle, position);

}