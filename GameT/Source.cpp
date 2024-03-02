#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>

int main() {
    std::locale::global(std::locale(""));
    std::string userName;
    std::vector<std::string> words = { "leg", "batman", "iron", "horse", "rabbit", "apple" };

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

    int attempts = 0;
    bool wordGuessed = false;
    std::cout << "Слово: " << maskedWord << " " << randomWord << std::endl;

    while (!wordGuessed) {
        for (char& c : maskedWord) {
            if (c != ' ') {
                c = '*';
            }
        }

        std::string userGuess;
        std::cout << "Ваша попытка: ";
        std::cin >> userGuess;

        attempts++;

        std::string matchingLetters = "";
        std::string correctLetters = "";

        for (size_t i = 0; i < userGuess.length() && i < maskedWord.length(); ++i) {
            if (userGuess[i] == randomWord[i]) {
                maskedWord[i] = randomWord[i];
                correctLetters += userGuess[i];
                correctLetters += " ";

            }
            else {
                if (std::count(randomWord.begin(), randomWord.end(), userGuess[i]) > 0 &&
                    randomWord[i] != userGuess[i] &&
                    std::count(matchingLetters.begin(), matchingLetters.end(), userGuess[i]) == 0 &&
                    std::count(correctLetters.begin(), correctLetters.end(), userGuess[i]) == 0) {
                    if (!matchingLetters.empty()) {
                        matchingLetters += " ";
                    }
                    matchingLetters += userGuess[i];
                }
            }
        }

        for (size_t i = 0; i < maskedWord.length(); ++i) {
            if (maskedWord[i] != randomWord[i]) {
                maskedWord[i] = '*';
            }
        }

        std::cout << "Слово: " << maskedWord << std::endl;

        if (randomWord == userGuess) {
            wordGuessed = true;
        }
        else {
            if (!matchingLetters.empty() || !correctLetters.empty()) {
                std::cout << "Отгаданные буквы: ";

                std::string uniqueMatchingLetters = "";
                for (char c : matchingLetters) {
                    if (uniqueMatchingLetters.find(c) == std::string::npos) {
                        uniqueMatchingLetters += " ";
                        uniqueMatchingLetters += c;                   

                    }
                }
                std::cout << uniqueMatchingLetters;

                std::string uniqueCorrectLetters = "";
                for (char c : correctLetters) {
                    if (uniqueCorrectLetters.find(c) == std::string::npos) {
                        uniqueCorrectLetters += " ";
                        uniqueCorrectLetters += c;                   
                    }
                }
                std::cout << uniqueCorrectLetters;

                std::cout << std::endl;
            }
            else {
                std::cout << "Отгаданные буквы: -" << std::endl;
            }
        }
    }
      

    std::cout << "Вы отгадали слово за " << attempts << " попыток." << std::endl;

    return 0;
}