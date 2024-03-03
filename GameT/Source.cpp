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
        std::string userName;
        int highScore = 0;
        std::vector<std::string> words = { "leg", "batman", "iron", "horse", "rabbit", "apple" };

        std::cout << "Ваше имя для рейтинга? ";
        std::cin >> userName;

        std::map<int, int> scoreTable;
        scoreTable[1] = 5;
        scoreTable[2] = 4;
        scoreTable[3] = 3;
        scoreTable[4] = 2;
        scoreTable[5] = 1;

        //Начало игры
        while (true) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, 5);
            int index = dis(gen);

            std::string randomWord = words[index];
            std::string maskedWord = randomWord;

            encryptWord(maskedWord);

            int attempts = 0;
            bool wordGuessed = false;
            std::cout << "Слово: " << maskedWord << std::endl;
            //Для проверки, что за слово (Специльно оставил, чтобы можно было проверить)
    //        std::cout << "Скрытое слово: " << randomWord << std::endl;

            //Условия продолжения игры
            while (!wordGuessed && attempts < 5) {
                encryptWord(maskedWord);

                std::string userGuess;
                std::cout << "Ваша попытка: ";
                std::cin >> userGuess;

                attempts++;

                std::string matchingLetters = "";
                std::string correctLetters = "";

                //Проверка на правильные буквы
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

                //Вывод результатов
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

            //Начисление очков
            int score = scoreTable[std::max(1, std::min(attempts, 5))];

            highScore += score;

            //Поражение
            if (attempts == 5) {
                std::cout << "Попытки закончились, игра окончена. Ваш счёт: " << highScore << std::endl;

                writeToScoresFile(userName, highScore);
                break;
            }

            //Победа
            std::cout << "Вы отгадали слово и заработали " << score << " баллов. Желаете продолжить? (y/n)" << std::endl;
            std::string answer;
            std::cin >> answer;


            bool validAnswer = false;
            //Выбор: продолжать игру или нет
            if (answer == "n") {
                std::cout << "Вы заработали " << highScore << " баллов" << std::endl;

                writeToScoresFile(userName, highScore);
                break;

            }
            else if (answer == "y") {
                continue;
            }
        }
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
            std::cout << "Не получилось открыть файл" << std::endl;
            return;
        }

        scores.push_back(std::make_pair(userName, highScore));

        std::sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
            });

        //Вывод топ-3
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
            std::cout << "Не получилось открыть файл" << std::endl;
            return;
        }
    }
};

int main() {
    Game game;
    game.playGame();

    return 0;
}