//MASTERMIND
//GRANT GOTWALD
#include "stdafx.h"
#include "Games.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

//Functions
void generateNumber(int ans[]);
void getGuess(int guess[]);
bool checkGuess(int guess[], int answer[], int code[]);
void checkForCorrects(int guess[], int answer[], int code[]);
void checkForCorrectNumWrongPos(int guess[], int answer[], int code[]);
void printStatus(int code[], int guess[]);
void printEndGameStats(int numGuess);
void printArray(int arr[]);
void printInstructions();
void resetCode(int code[]);

int Mastermind()
{
	//declaration of variables
	int answer[5] = { -1,-1,-1,-1,-1 };
	int guess[5] = { -1,-1,-1,-1,-1 };
	int code[5];
	int numGuesses = 0;
	bool win = false;

	srand(static_cast<unsigned int>(time(0))); //seed random num generator

	generateNumber(answer);  //Generates answer 

	printInstructions();

	//Main Game Loop
	while (!win) {
		resetCode(code);
		getGuess(guess);
		numGuesses++;
		win = checkGuess(guess, answer, code);
		if (!win) printStatus(code, guess);
	}

	printEndGameStats(numGuesses);


	system("pause");
	return 0;
}

//Function to generate answer
void generateNumber(int ans[]) {

	for (int i = 0; i<5; i++)
	{
		ans[i] = (rand() % 10);
	}
}

//Function to prompt and retrieve users guess
void getGuess(int guess[]) {
	int fullGuess;
	cout << "------------------------------------------------" << endl;
	cout << "Please enter a five digit number for your guess:" << endl;
	cin >> fullGuess;
	for (int i = 4; i >= 0; i--) {
		guess[i] = fullGuess % 10;
		fullGuess /= 10;
	}
}

//Function to print array (of length 5) 
void printArray(int arr[]) {
	for (int i = 0; i<5; i++)
	{
		cout << arr[i];
	}
	cout << "\n";
}

//Checks to see what numbers are right or wrong, returns true if game won
bool checkGuess(int guess[], int answer[], int code[]) {
	checkForCorrects(guess, answer, code);
	checkForCorrectNumWrongPos(guess, answer, code);
	if ((code[0] == 2) && (code[1] == 2) && (code[2] == 2) && (code[3] == 2) && (code[4] == 2)) return true;
	else return false;
}

//Checks for correct number in correct spot (fills in 2's in code)
void checkForCorrects(int guess[], int answer[], int code[]) {
	for (int i = 0; i<5; i++) {
		if (guess[i] == answer[i]) code[i] = 2;
	}
}

//Checks for correct numbers in wrong spot (fills in 1's in code)
void checkForCorrectNumWrongPos(int guess[], int answer[], int code[]) {
	int used[5] = { 0,0,0,0,0 };

	for (int i = 0; i<5; i++)
	{
		if (code[i] == 0)
		{
			for (int j = 0; j<5; j++)
			{
				if ((guess[i] == answer[j]) && (guess[j] != answer[j]) && (used[j] != 1))
				{
					code[i] = 1;
					used[j] = 1;
					break;
				}
			}

		}

	}


}

//Resets code to {0,0,0,0,0}
void resetCode(int code[]) {
	for (int i = 0; i<5; i++)
	{
		code[i] = 0;
	}
}

void printStatus(int code[], int guess[])
{
	cout << "\nYou didn't get it right this time.\n";

	cout << "Guess:";
	printArray(guess);
	cout << "Code :";
	printArray(code);

}

void printInstructions() {
	cout << "Welcome to Mastermind! \n\n";
	cout << "Your goal is to guess my 5 digit number in as little guesses as possible. \n(free hint: zeros can appear anywhere)\n";
	cout << "After every guess I will give you a hint in the form of a 5 digit code.\nEvery digit in the code represents the digit in your guess.\n\n";
	cout << "**************************************************\n";
	cout << "CODE KEY:  \n0 = Number is not in the answer.  \n1 = You've got the correct number, but the wrong placement.  \n2 = Correct number & placement!\n";
	cout << "**************************************************\n\n";
	cout << "How many guesses will it take you to figure out the 5 digit number?\n\n";
}

void printEndGameStats(int numGuesses) {
	cout << "**************************************************\n";
	cout << "Congratulations ! !\n";
	cout << "You truly are a mastermind!  \nYou finished the game in only " << numGuesses << " turns!\n";
	cout << "**************************************************\n";
}