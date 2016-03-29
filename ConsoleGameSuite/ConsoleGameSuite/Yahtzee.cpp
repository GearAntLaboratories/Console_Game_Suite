#include "stdafx.h"
#include "Games.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

//Functions
int rollDie();
void printBoxes(int[13]);
int scoreRoll(int, int[5]);
int totalScore(int[13]);
void printDice(int[5]);

int Yahtzee()
{
	//declaration of variables
	int dice[5];
	int selection;
	string rollInput;

	int box[13] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 };  //Keeps score of each Yahtzee scoring box

	srand(static_cast<unsigned int>(time(0))); //seed random num generator

	cout << "Welcome to YAHTZEE! \nFor now you are playing by yourself, how good of score can you get?" << endl;

	for (int i = 0; i<13; i++) {    //Outer loop, goes through main game logic 13 times to fill up a yahtzee board/boxes
		rollInput = "rrrrr";
		for (int i = 0; i<5; i++) {             //rolling five dice on first roll everytime
			if (rollInput[i] == 'r') {
				dice[i] = rollDie();
			}
		}

		//Outputs
		cout << "\n\nYour first roll is: ";
		printDice(dice);
		cout << "\n\nWhich dice would you like to hold or reroll (Format: hrrhr)?" << endl;

		cin >> rollInput;   //Get input for next roll

		if (rollInput != "hhhhh") {              //Roll again if not all 'holds'
			for (int i = 0; i<5; i++) {
				if (rollInput[i] == 'r') {
					dice[i] = rollDie();
				}
			}
			cout << "\nYour Second roll is: ";
			printDice(dice);


			cout << "\n\nWhich dice would you like to hold or reroll (Format: hrrhr)?" << endl;

			cin >> rollInput;
			if (rollInput != "hhhhh") {
				for (int i = 0; i<5; i++) {
					if (rollInput[i] == 'r') {
						dice[i] = rollDie();
					}
				}
				cout << "\nYour Third roll is: ";
				printDice(dice);
			}
		}


		cout << "\n\n";

		printBoxes(box);   //Prints boxes and status of boxes - filled/unfilled

		do {                                                      //Loop gets an empty box
			cout << "\nPlease enter an unused box for roll: ";
			printDice(dice);
			cout << "\n";
			cin >> selection;
			selection--;
		} while (box[selection] != -1);

		box[selection] = scoreRoll(selection, dice);         //calls function to score roll for box selected

		cout << "Box #" << selection + 1 << " is now filled with : " << box[selection] << " Points." << endl;
		cout << " --------------------------------" << endl;
	}

	cout << "You filled up all 13 Boxes!" << endl;
	cout << "Your final score is : " << totalScore(box) << endl;

	system("pause");
	return 0;
}

//Simple function to roll random die
int rollDie() {

	return (rand() % 6 + 1);
}

//Outputs all 13 yahtzee boxes, with status
void printBoxes(int box[13]) {
	cout << "UPPER SECTION BOXES:" << endl;
	cout << "\tBOX #1: All 1's";
	if (box[0] != -1) cout << " - Filled - Points: " << box[0];
	cout << "\n";
	cout << "\tBOX #2: All 2's";
	if (box[1] != -1) cout << " - Filled - Points: " << box[1];
	cout << "\n";
	cout << "\tBOX #3: All 3's";
	if (box[2] != -1) cout << " - Filled - Points: " << box[2];
	cout << "\n";
	cout << "\tBOX #4: All 4's";
	if (box[3] != -1) cout << " - Filled - Points: " << box[3];
	cout << "\n";
	cout << "\tBOX #5: All 5's";
	if (box[4] != -1) cout << " - Filled - Points: " << box[4];
	cout << "\n";
	cout << "\tBOX #6: All 6's";
	if (box[5] != -1) cout << " - Filled - Points: " << box[5];
	cout << "\n";
	cout << "LOWER SECTION BOXES:" << endl;
	cout << "\tBOX #7: 3 of a Kind";
	if (box[6] != -1) cout << " - Filled - Points: " << box[6];
	cout << "\n";
	cout << "\tBOX #8: 4 of a Kind";
	if (box[7] != -1) cout << " - Filled - Points: " << box[7];
	cout << "\n";
	cout << "\tBOX #9: Full House";
	if (box[8] != -1) cout << " - Filled - Points: " << box[8];
	cout << "\n";
	cout << "\tBOX #10: Small Straight";
	if (box[9] != -1) cout << " - Filled - Points: " << box[9];
	cout << "\n";
	cout << "\tBOX #11: Large Straight";
	if (box[10] != -1) cout << " - Filled - Points: " << box[10];
	cout << "\n";
	cout << "\tBOX #12: YAHTZEE (5 of a kind)";
	if (box[11] != -1) cout << " - Filled - Points: " << box[11];
	cout << "\n";
	cout << "\tBOX #13: Chance (Tot of 5 Dice)";
	if (box[12] != -1) cout << " - Filled - Points: " << box[12];
	cout << "\n";
}

//Scores the five dice sent to it, depending on box
int scoreRoll(int sel, int dice[5]) {
	int score = 0;
	switch (sel) {
	case 0:                      //First 6 cases are for upper boxes, simply adds appropriate die amount depending on box
		for (int i = 0; i<5; i++)
		{
			if (dice[i] == 1) score++;
		}
		return (score);

	case 1:
		for (int i = 0; i<5; i++)
		{
			if (dice[i] == 2) score += 2;
		}
		return (score);
	case 2:
		for (int i = 0; i<5; i++)
		{
			if (dice[i] == 3) score += 3;
		}
		return (score);
	case 3:
		for (int i = 0; i<5; i++)
		{
			if (dice[i] == 4) score += 4;
		}
		return (score);
	case 4:
		for (int i = 0; i<5; i++)
		{
			if (dice[i] == 5) score += 5;
		}
		return (score);
	case 5:
		for (int i = 0; i<5; i++)
		{
			if (dice[i] == 6) score += 6;
		}
		return (score);
	case 6:                              //scores 3 of a kind. Sorts ascending, then checks for triples of any
		sort(dice, dice + 5);
		if ((dice[0] == dice[1] && dice[1] == dice[2]) ||
			(dice[1] == dice[2] && dice[2] == dice[3]) ||
			(dice[2] == dice[3] && dice[3] == dice[4]) ||
			(dice[3] == dice[4] && dice[4] == dice[5])
			)
			score = (dice[0] + dice[1] + dice[2] + dice[3] + dice[4]);
		else score = 0;
		return (score);
	case 7:                                  //Scores 4 of a kind, Sorts ascending, then checks for quads of any
		sort(dice, dice + 5);
		if ((dice[0] == dice[1] && dice[1] == dice[2] && dice[2] == dice[3]) ||
			(dice[1] == dice[2] && dice[2] == dice[3] && dice[3] == dice[4])
			)
			score = (dice[0] + dice[1] + dice[2] + dice[3] + dice[4]);
		else score = 0;
		return (score);
	case 8:                      //Full house; sorts, then checks for 3 of a kind followed by two of a kind and vice versa
		sort(dice, dice + 5);
		if (((dice[0] == dice[1] && dice[1] == dice[2]) &&
			(dice[3] == dice[4])) ||
			((dice[0] == dice[1]) &&
				(dice[2] == dice[3] && dice[3] == dice[4])))
			score = 25;
		else score = 0;
		return (score);
	case 9:      //Small Straight.  Have to account for two of a kind next to eachother, move duplicate to end, then check for 4 consec.
		sort(dice, dice + 5);
		for (int j = 0; j < 4; j++)
		{
			int temp = 0;
			if (dice[j] == dice[j + 1])
			{
				temp = dice[j];

				for (int k = j; k < 4; k++)
				{
					dice[k] = dice[k + 1];
				}

				dice[4] = temp;
			}
		}

		if (((dice[0] + 1 == dice[1]) &&
			(dice[1] + 1 == dice[2]) &&
			(dice[2] + 1 == dice[3])) ||
			((dice[1] + 1 == dice[2]) &&
				(dice[2] + 1 == dice[3]) &&
				(dice[3] + 1 == dice[4])))

			score = 30;
		else score = 0;

		return (score);
	case 10:              //Large Straight, sort then check for consecutive numbers
		sort(dice, dice + 5);
		if ((dice[0] + 1 == dice[1]) &&
			(dice[1] + 1 == dice[2]) &&
			(dice[2] + 1 == dice[3]) &&
			(dice[3] + 1 == dice[4]))

			score = 40;
		else score = 0;

		return (score);
	case 11:               //YAHTZEE! 5 of a kind
		if (dice[0] == dice[1] && dice[1] == dice[2] && dice[2] == dice[3] && dice[3] == dice[4])
			score = 50; else score = 0;
		return (score);
	case 12:              //Chance, add all dice
		return (dice[0] + dice[1] + dice[2] + dice[3] + dice[4]);
	default:
		return 0;

	}
}

//Totals score of all boxes.  First totals top 6 and gives 35 point bonus if above 62, then totals bottoms and adds together
int totalScore(int filledBox[13]) {
	int totScore = 0;
	for (int i = 0; i<6; i++) {
		totScore += filledBox[i];
	}

	if (totScore>62) totScore += 35;
	for (int i = 6; i<13; i++) {
		totScore += filledBox[i];
	}

	return (totScore);
}

//Function to print dice, simply to enhance readability in main
void printDice(int dice[5]) {
	for (int i = 0; i <5; i++)
	{
		cout << dice[i];
	}
}