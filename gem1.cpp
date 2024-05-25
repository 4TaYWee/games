#include <iostream>
#include <random>

using namespace std;

int main() {
  // Generate a random number between 1 and 100 (inclusive)
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<int> dis(1, 100);
  int secretNumber = dis(gen);

  int guess, numTries = 0;

  cout << "Welcome to the Number Guessing Game!" << endl;
  cout << "I'm thinking of a number between 1 and 100. Try to guess it!" << endl;

  // Loop until the user guesses the number
  do {
    cout << "Enter your guess: ";
    cin >> guess;
    numTries++;

    if (guess > secretNumber) {
      cout << "Your guess is too high. Try again." << endl;
    } else if (guess < secretNumber) {
      cout << "Your guess is too low. Try again." << endl;
    }
  } while (guess != secretNumber);

  cout << "Congratulations! You guessed the number in " << numTries << " tries." << endl;

  return 0;
}
