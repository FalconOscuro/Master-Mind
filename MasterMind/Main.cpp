#include <iostream>
#include <random>
#include <time.h>
#include <vector>
#include <math.h>

using namespace std;

// A list of every singlge possible code, eliminated codes are set to { -1 }
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
		// The code to be guessed
		vector<int> code;
		// The guess being made
		vector<int> guess = { 0, 0, 1, 1 };
		// Generate the random code and ouput to console
		for (int i = 0; i < 4; i++)
			code.push_back(rand() % 5);

		// Output the current code to the console
		cout << "The code is: ";
		for (int i = 0; i < 4; i++)
			cout << code[i];
		cout << endl;

		// Initialize S and the returned variable
		pair<int, int> returned = { 0, 0 };
		InitS();

		// Number of guesses made by the program
		int x = 1;
		while (true)
		{
			// Make a guess & inform user
			cout << "Guessing ";
			for (int i = 0; i < 4; i++)
				cout << guess[i];
			cout << endl;

			returned = CheckGuess(guess, code);

			// inform user as to returned variable
			cout << "Responded with " << returned.first << " - " << returned.second << endl;

			// if guessed correctly break here
			if (returned.second == 4)
				break;

			// Otherwise guess was incorrect so can be eliminated
			// since all variable in S are added in order of smallest to largest the position of each code can easily be found similarly to a binary number
			S[guess[0] + (guess[1] * pow(5, 1)) + (guess[2] * pow(5, 2)) + (guess[3] * pow(5, 3))] = { -1 };

			// Get the next guess to be made and increment guesses made
			guess = GetNextGuess(guess, returned);
			x++;
		}

		// Return guesses made & prompt for re-run
		cout << "Finished in " << x << " guesses!" << endl;
		cout << "Run again? Y/N    ";
		char runAgain;

		cin >> runAgain;

		// stop running if not answered with y
		if (runAgain != 'Y' && runAgain != 'y')
			running = false;

		// Console formatting to keep each loop seperate
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
	pair<int, int> returned = { 0, 0 };

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

						// If there is more than 1 instance of this number left in the code keep going until the next one is found
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
	// used for return of CheckGuess
	pair<int, int> val;

	// Initialize next guess as -1
	vector<int> nextGuess = { -1 };

	// For each value in S
	for (int i = 0; i < S.size(); i++)
	{
		// If this value has been eliminated skip
		if (S[i][0] == -1)
			continue;

		// Check as if this were the code when the last guess was made
		val = CheckGuess(lastGuess, S[i]);

		// If the returned value is not the same this cannot be the code, eliminate & skip
		if (val != returned)
		{
			S[i] = { -1 };
			continue;
		}

		// If there is no next guess yet set this to be the next guess
		if (nextGuess[0] == -1)
			nextGuess = S[i];

		// Output as possible solution
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
	// Set S to only have the first value
	S = { {0, 0, 0, 0} };

	// The next value to be added
	vector<int> nextVal{ 0, 0, 0, 0 };

	// The largest value to be added
	static const vector<int> finalVal{ 4, 4, 4, 4 };

	// while there are still values to be added
	while (nextVal != finalVal)
	{
		// indicates how many spaces from the left the current value being worked on is
		int i = 0;

		while (i < 4)
		{
			// Increment by one
			nextVal[3 - i]++;

			// If greater than 4, set this integer to 0 and go onto the next integer to the left
			if (nextVal[3 - i] >= 5)
			{
				nextVal[3 - i] = 0;
				i++;
				continue;
			}

			// Else done and add this value to S
			break;
		}

		S.push_back(nextVal);
	}
}