#include "commands.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <string>

using namespace std;

// принимает название поазде, список городов, и два мапа 
void createTrain(string& nameTrain, vector<string>& townList, map<string, vector<string>>& Trains, map<string, vector<string>>& Towns){
    
    Trains[nameTrain] = townList;

    for (string& town : townList) {
        Towns[town].push_back(nameTrain);
    }
};


void trainsForTown(string& nameTown, map<string, vector<string>>& Trains, map<string, vector<string>>& Towns) {

    // Проверка, существует ли город
    if (Towns.find(nameTown) == Towns.end()) {
        cout << "Город не существует" << endl;
        return;
    }

    const vector<string>& trains = Towns[nameTown];

    if (trains.empty()) {
        cout << "Нет поездов которые проходят город " << nameTown << endl;
        return;
    }

    cout << "Поезда которые проходят город " << nameTown << ": ";
    for (const string& train : trains) {
        cout << train << " ";
    }
    cout << endl;
}



void townsForTrain(string& nameTrain, map<string, vector<string>>& Trains, map<string, vector<string>>& Towns) {

    // Проверим, есть ли такой поезд
    if (Trains.find(nameTrain) == Trains.end()) {
        cout << "Поезд " << nameTrain << " не найден" << endl;
        return;
    }

    cout << "----- Cписок городов -----" << endl;
    for (string& town : Trains[nameTrain]) {
        cout << town << ": ";

        bool hasOtherTrains = false;

        for (const string& train : Towns[town]) {
            if (train != nameTrain) {
                cout << train << " ";
                hasOtherTrains = true;
            }
        }

        if (!hasOtherTrains) {
            cout << "Нет других поездов";
        }

        cout << endl;
    }
}



void listTrains(map<string, vector<string>>& Trains, map<string, vector<string>>& Towns){
    for (const auto& p : Trains) {
        cout << p.first << ": ";
        for (const auto& town : p.second) {
            cout << town << " ";
        }
        cout << endl;
    }
};