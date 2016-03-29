#include "stdafx.h"
#include "Games.h"
#include <iostream>
#include <string>
#include <cmath>



using namespace std;


//Class for flipping a coin.  
class Coin {
	bool isHeads;
public:
	string toss();
};

string Coin::toss() {
	if (rand() % 2 == 1) return "Heads";
	else return "Tails";
}

int CoinFlip() {
	Coin coin;
	int numTosses;
	int expectedHeads;
	int expectedTails;
	int numSimulations = 100;
	int numHeads = 0;
	int numTails = 0;
	int count = 0;

	cout << "Enter the number of tosses for each simulation:" << endl;
	cin >> numTosses;
	cout << "Enter Expected Return of Heads.  Number must be less than " << numTosses << endl;
	cin >> expectedHeads;

	expectedTails = numTosses - expectedHeads;

	cout << "Monte Carlo Simulation" << endl;
	cout << "----------------------" << endl;

	cout << "Number of coin tosses: " << numTosses << endl;
	cout << "Expected Number of Heads: " << expectedHeads << endl;
	cout << "Expected Number of Tails: " << expectedTails << endl;
	cout << "Number of simulations to be run: " << numSimulations << endl;
	cout << "Press any key to start runnig simulations:" << endl;
	system("pause");

	for (int i = 0; i<numSimulations; i++)
	{
		numHeads = 0;
		numTails = 0;
		for (int j = 0; j<numTosses; j++) {
			if (coin.toss().compare("Heads")) numHeads++; else numTails++;
		}
		cout << "Heads: " << numHeads << " Tails: " << numTails << endl;
		if (numHeads>expectedHeads) count++;

	}
	cout << "-------------------" << endl;
	cout << "Simulation Complete" << endl;
	cout << "-------------------" << endl;
	cout << "Total number of simulations that resulted in Head values greater than the expected result of " << expectedHeads << " is " << count << endl;
	cout << "Now compare the simulated probability to the binomial distribution calculated probability" << endl;
	cout << "Simulated probability: " << ((double)count / numSimulations) << endl;

	system("pause");
	return 0;
}
