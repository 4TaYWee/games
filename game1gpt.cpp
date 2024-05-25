#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>

int main() {
    srand(time(0));
    int secretNumber = rand() % 100 + 1;
    int guess, attempts = 0;

    std::cout << "Welcome to the Number Guessing Game!\n";
    std::cout << "I've chosen a number between 1 and 100. Try to guess it!\n";

    auto start = std::chrono::high_resolution_clock::now(); // Start measuring runtime

    do {
        std::cout << "Enter your guess: ";
        std::cin >> guess;
        attempts++;

        if (guess < secretNumber)
            std::cout << "Too low! Try again.\n";
        else if (guess > secretNumber)
            std::cout << "Too high! Try again.\n";
        else
            std::cout << "Congratulations! You guessed it in " << attempts << " attempts!\n";
    } while (guess != secretNumber);

    auto end = std::chrono::high_resolution_clock::now(); // End measuring runtime
    std::chrono::duration<double> elapsed = end - start; // Calculate runtime
    std::cout << "Runtime: " << elapsed.count() << " seconds.\n";

    return 0;
}
