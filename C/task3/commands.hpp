#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <map>
#include <vector>
#include <string>

using namespace std;

enum class Type {
    CREATE_TRAIN,
    TRAINS_FOR_TOWN,
    TOWNS_FOR_TRAIN,
    TRAINS
};

// Функция создания поезда: имя, маршрут, оба словаря
void createTrain(string& nameTrain, vector<string>& townList, map<string, vector<string>>& Trains, map<string, vector<string>>& Towns);

// Вывод поездов, проходящих через город
void trainsForTown(string& nameTown, map<string, vector<string>>& Trains, map<string, vector<string>>& Towns);

// Вывод городов маршрута поезда и других поездов для каждого города
void townsForTrain(string& nameTrain, map<string, vector<string>>& Trains, map<string, vector<string>>& Towns);

// Вывод всех поездов с их маршрутами
void listTrains(map<string, vector<string>>& Trains, map<string, vector<string>>& Towns);

#endif
