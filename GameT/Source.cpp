#include <iostream>
#include <vector>
#include <string>
#include <random>

int main() {
    std::string userName;
    std::vector<std::string> words = { "leg", "batman", "iron", "horse", "rabbit", "apple"};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 5);
    int index = dis(gen);

    std::string randomWord = words[index];
    std::string maskedWord = randomWord;
    for (char& c : maskedWord) {
        if (c != ' ') {
            c = '*';
        }
    }

    std::cout << "Word: " << maskedWord << " " << randomWord << std::endl;

    std::string userGuess;
    while (randomWord != userGuess) {
        std::cout << "Your word: ";
        std::cin >> userGuess;
    }

    std::cout << "Congratulations!" << std::endl;

    return 0;
}