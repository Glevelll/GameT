#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <map>
#include <fstream>

class Game {
public:
    void playGame() {
        std::locale::global(std::locale(""));
        std::string userName = getUserName();
        int highScore = 0;
        std::vector<std::string> words = { "leg", "batman", "iron", "horse", "rabbit", "apple" };

        //������ ����
        while (true) {
            std::string randomWord = getRandomWord(words);
            std::string maskedWord = randomWord;

            encryptWord(maskedWord);

            int attempts = 0;
            bool wordGuessed = false;
            std::cout << "�����: " << maskedWord << std::endl;

            //������� ����������� ����
            while (!wordGuessed && attempts < 5) {
                encryptWord(maskedWord);

                std::string userGuess = getUserGuess();

                attempts++;

                std::string matchingLetters = "";
                std::string correctLetters = "";

                //�������� �� ���������� �����
                checkLetters(userGuess, randomWord, maskedWord, matchingLetters, correctLetters);

                std::cout << "�����: " << maskedWord << std::endl;

                //����� �����������
                if (randomWord == userGuess) {
                    wordGuessed = true;
                }
                else {
                    printResult(matchingLetters, correctLetters);
                }
            }

            //���������� �����
            int score = getScore(attempts);

            highScore += score;

            //���������
            if (!wordGuessed && attempts == 5) {
                std::cout << "������� �����������, ���� ��������. ��� ����: " << highScore << std::endl;

                writeToScoresFile(userName, highScore);
                break;
            }

            //������
            std::cout << "�� �������� ����� � ���������� " << score << " ������. ������� ����������? (y/n)" << std::endl;
            std::string answer;
            std::cin >> answer;

            bool validAnswer = false;
            //�����: ���������� ���� ��� ���
            if (answer == "n") {
                std::cout << "�� ���������� " << highScore << " ������" << std::endl;

                writeToScoresFile(userName, highScore);
                break;

            }
            else if (answer == "y") {
                continue;
            }
        }
    }

private:
    std::string getUserName() {
        std::string userName;
        std::cout << "���� ��� ��� ��������? ";
        std::cin >> userName;
        return userName;
    }

    std::string getUserGuess() {
        std::string userGuess;
        std::cout << "���� �������: ";
        std::cin >> userGuess;
        return userGuess;
    }

    std::string getRandomWord(const std::vector<std::string>& words) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, static_cast<int>(words.size()) - 1);
        return words[dis(gen)];
    }

    void checkLetters(const std::string& userGuess, const std::string& randomWord, std::string& maskedWord, std::string& matchingLetters, std::string& correctLetters) {
        for (size_t i = 0; i < userGuess.length() && i < maskedWord.length(); ++i) {
            if (userGuess[i] == randomWord[i]) {
                maskedWord[i] = randomWord[i];
                correctLetters += userGuess[i];
                correctLetters += ' ';
            }
            else {
                if (std::count(randomWord.begin(), randomWord.end(), userGuess[i]) > 0 &&
                    randomWord[i] != userGuess[i] &&
                    matchingLetters.find(userGuess[i]) == std::string::npos &&
                    correctLetters.find(userGuess[i]) == std::string::npos) {
                    if (!matchingLetters.empty()) {
                        matchingLetters += ' ';
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
    }

    void printResult(const std::string& matchingLetters, const std::string& correctLetters) {
        if (!matchingLetters.empty() || !correctLetters.empty()) {
            std::cout << "���������� �����: ";
            std::string uniqueMatchingLetters = "";
            for (char c : matchingLetters) {
                if (uniqueMatchingLetters.find(c) == std::string::npos && correctLetters.find(c) == std::string::npos) {
                    uniqueMatchingLetters += " ";
                    uniqueMatchingLetters += c;
                }
            }
            std::cout << uniqueMatchingLetters;

            std::string uniqueCorrectLetters = "";
            for (char c : correctLetters) {
                if (uniqueCorrectLetters.find(c) == std::string::npos && matchingLetters.find(c) == std::string::npos) {
                    uniqueCorrectLetters += " ";
                    uniqueCorrectLetters += c;
                }
            }
            std::cout << uniqueCorrectLetters;

            std::cout << std::endl;
        }
        else {
            std::cout << "���������� �����: -" << std::endl;
        }
    }

    int getScore(int attempts) {
        std::map<int, int> scoreTable = {
            {1, 5},
            {2, 4},
            {3, 3},
            {4, 2},
            {5, 1}
        };

        return scoreTable[std::max(1, std::min(attempts, 5))];
    }

    void encryptWord(std::string& maskedWord) {
        for (char& c : maskedWord) {
            if (c != ' ') {
                c = '*';
            }
        }
    }

    void writeToScoresFile(const std::string& userName, int highScore) {
        std::ifstream scoreFileRead("score.txt");
        std::vector<std::pair<std::string, int>> scores;

        if (scoreFileRead.is_open()) {
            std::string name;
            int score;
            while (scoreFileRead >> name >> score) {
                scores.push_back(std::make_pair(name, score));
            }
            scoreFileRead.close();
        }
        else {
            std::cout << "�� ���������� ������� ����" << std::endl;
            return;
        }

        scores.push_back(std::make_pair(userName, highScore));

        std::sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
            });

        //����� ���-3
        std::ofstream scoreFileWrite("score.txt");
        if (scoreFileWrite.is_open()) {
            int count = 0;
            for (const auto& entry : scores) {
                if (count < 3) {
                    std::cout << entry.first << " " << entry.second << "\n";
                }
                scoreFileWrite << entry.first << " " << entry.second << "\n";
                count++;
            }
            scoreFileWrite.close();
        }
        else {
            std::cout << "�� ���������� ������� ����" << std::endl;
            return;
        }
    }
};

int main() {
    Game game;
    game.playGame();

    return 0;
}