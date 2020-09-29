#include <iostream>
#include <random>
#include <time.h>
#include <vector>
#include <math.h>


using namespace std;

vector<vector<int>> S;

pair<int, int> CheckGuess(vector<int>, vector<int>);
vector<int> GetNextGuess(vector<int>, pair<int, int>);
void InitS();

int main()
{
	// Set random seed to the current time
	srand(time(NULL));

	// Loop Program until exit
	bool running = true;
	while (running)
	{

		vector<int> code;
		vector<int> guess = {0, 0, 1, 1};
		// Generate the random code and ouput to console & initialize the guess variable
		for (int i = 0; i < 4; i++)
			code.push_back(rand() % 5);


		cout << "The code is: ";
		for (int i = 0; i < 4; i++)
			cout << code[i];
		cout << endl;

		pair<int, int> returned = { 0, 0 };
		InitS();

		int x = 1;
		while (returned.second != 4)
		{
			cout << "Guessing ";
			for (int i = 0; i < 4; i++)
				cout << guess[i];
			cout << endl;

			returned = CheckGuess(guess, code);

			cout << "Responded with " << returned.first << " - " << returned.second << endl;

			if (returned.second == 4)
				break;

			S[guess[0] + (guess[1] * pow(5, 1)) + (guess[2] * pow(5, 2)) + (guess[3] * pow(5, 3))] = { -1 };

			guess = GetNextGuess(guess, returned);
			x++;
		}

		cout << "Finished in " << x << " guesses!" << endl;
		cout << "Run again? Y/N    ";
		char runAgain;

		cin >> runAgain;

		if (runAgain != 'Y' && runAgain != 'y')
			running = false;
		
		cout << endl << endl << endl;
	}
}


// Checks a guess against the code and returns the appropriate value (right colour wrong place No., right colour right place No.)
pair<int, int> CheckGuess(vector<int> guess, vector<int> code)
{
	// If they are the same then everything is in the right place
	if (guess == code)
		return make_pair(0, 4);

	// Initialize returned value
	pair<int, int> returned = {0, 0};

	// For each value guessed
	for (int i = 0; i < 4; i++)
	{
		// Is it in the right position, if so add to correct list
		if (guess[i] == code[i])
			returned.second += 1;

		// Else check if this is in the wrong position
		else
		{	
			int n = 0;
			// Loop through the code and see if this integer exists within it that does not have a matched partner within the guess
			for (int j = 0; j < 4; j++)
				if (code[j] == guess[i] && code[j] != guess[j])
					n++;

			// If an unmached int of same value was found within the code check to see if this is the first unmached instance of this value within the guess
			if (n > 0)
				for (int j = 0; j < 4; j++)
					if (guess[j] == guess[i] && code[j] != guess[j])
					{
						if (i == j)
							// If so add to correct wrong place list
							returned.first += 1;

						else if (n > 1)
						{
							n--;
							continue;
						}

						break;
					}
		}
	}

	return returned;
}


// Finds what the next guess should be
vector<int> GetNextGuess(vector<int> lastGuess, pair<int, int> returned)
{
	int x = 0;
	pair<int, int> val;
	vector<int> nextGuess = { -1 };

	for (int i = 0; i < S.size(); i++)
	{
		if (S[i][0] == -1)
			continue;

		val = CheckGuess(lastGuess, S[i]);

		if (val != returned)
		{
			S[i] = { -1 };
			continue;
		}

		if (nextGuess[0] == -1)
			nextGuess = S[i];
		
		for (int j = 0; j < 4; j++)
			cout << S[i][j];
		cout << ' ';
	}
	cout << endl;
	return nextGuess;
}


// Initialize S, the list of all possible guesses
void InitS()
{
	S = { {0, 0, 0, 0} };

	vector<int> nextVal{ 0, 0, 0, 0 };
	static const vector<int> finalVal{ 4, 4, 4, 4 };

	while (nextVal != finalVal)
	{
		int i = 1;

		while (i < 5)
		{
			nextVal[4 - i]++;

			if (nextVal[4 - i] >= 5)
			{
				nextVal[4 - i] = 0;
				i++;
				continue;
			}

			break;
		}

		S.push_back(nextVal);
	}
}