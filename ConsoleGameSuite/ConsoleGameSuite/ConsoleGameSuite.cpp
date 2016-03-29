// ConsoleGameSuite.cpp Entry point for the console application.
// Simple Menu to choose which console game to play.
#include "stdafx.h"
#include "Games.h"
#include <iostream>


using namespace std;


int main()
{

	int choice;              
	bool done = false;

	do {
		system("CLS");
		// Displays menu to user
		cout << "Console Game Suite\n\n";
		cout << "1 - Tron\n";
		cout << "2 - Tetris\n";
		cout << "3 - Risk - Combat Simulator\n";
		cout << "4 - Yahtzee\n";
		cout << "5 - Mastermind\n";
		cout << "6 - Tic Tac Toe\n";
		cout << "0 - EXIT\n";

		// Gets menu choice from user
		cout << "Choice: \n";
		cin >> choice;

		switch (choice)
		{
		case 0:
			cout << "Thanks for playing \n\n\n";
			done = true;
			break;

		case 1:		
			cout << "Tron \n";
			system("CLS");
			Tron();
			break;
	
		case 2:
			cout << "Tetris \n";
			system("CLS");
			Tetris();
			break;

		case 3:
			cout << "Risk Combat Simulator \n";
			system("CLS");
			Risk();
			break;

		case 4:
			cout << "Yahtzee \n";
			system("CLS");
			Yahtzee();
			break;

		case 5:
			cout << "Mastermind \n";
			system("CLS");
			Mastermind();
			break;

		case 6:
			cout << "Tic Tac Toe \n";
			system("CLS");
			TicTacToe();
			break;
		default:
			cout << "You made an illegal choice.  Please enter again. \n\n";

			cin.clear();  //If user enters a non-int these lines prevent cin from failing at beginning of loop
			cin.ignore();
			break;
		}

	} while (!done);
	

	return 0;
}