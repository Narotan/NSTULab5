#include "commands.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

map<string, vector<string>> Trains;
map<string, vector<string>> Towns;

// Функция для преобразования строки в команду
Type parseCommand(const string& input) {
    static const map<string, Type> commandMap = {
        {"CREATE_TRAIN", Type::CREATE_TRAIN},
        {"TRAINS_FOR_TOWN", Type::TRAINS_FOR_TOWN},
        {"TOWNS_FOR_TRAIN", Type::TOWNS_FOR_TRAIN},
        {"TRAINS", Type::TRAINS}
    };
    
    auto it = commandMap.find(input);
    return (it != commandMap.end()) ? it->second : Type::TRAINS;
}

int main() {
    // Чтобы было куда поезду ехать 
    vector<string> allTowns = {"Москва", "Петербург", "Новосибирск", "Казань", "Екатеринбург", "Челябинск", "Томск", "Барнаул", "Уфа"};

    for (const auto& town : allTowns) {
        cout << town << ' ';
    }
    cout << endl;

    string input;
    while (true) {
        cout << "> ";
        getline(cin, input);

        if (input.empty()) break;

        stringstream ss(input);
        string command;
        ss >> command;

        switch (parseCommand(command)) {
            case Type::CREATE_TRAIN: {
                string trainName, town;
                vector<string> townsList;

                ss >> trainName;
                while (ss >> town) {
                    townsList.push_back(town);
                }

                // Проверка, что города, переданные в команду, существуют
                for (const auto& town : townsList) {
                    if (find(allTowns.begin(), allTowns.end(), town) == allTowns.end()) {
                        cout << "Город " << town << " не существует. Используйте один из следующих: ";
                        for (const auto& validTown : allTowns) {
                            cout << validTown << " ";
                        }
                        cout << endl;
                        return 0;  // Прерываем выполнение, так как город не найден
                    }
                }

                // Создаём поезд с городами
                createTrain(trainName, townsList, Trains, Towns);
                break;
            }
            case Type::TRAINS_FOR_TOWN: {
                string townName;
                ss >> townName;
                trainsForTown(townName, Trains, Towns);
                "\n";
                break;
            }
            case Type::TOWNS_FOR_TRAIN: {
                string trainName;
                ss >> trainName;
                townsForTrain(trainName, Trains, Towns);
                "\n";
                break;
            }
            case Type::TRAINS: {
                listTrains(Trains, Towns);
                "\n";
                break;
            }
            default:
                cout << "Неизвестная команда!" << endl;
        }
    }

    return 0;
}
