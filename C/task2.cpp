#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Person {
    int time;
    string talon;
};

// Функция для сравнения Person по времени (для сортировки)
bool compareByTime(const Person &a, const Person &b) {
    return a.time > b.time; 
}

int main() {
    int windowTotal;
    cout << "Введите количество окон: ";
    cin >> windowTotal;

    string command;
    vector<Person> allPeople; // Временное хранилище всех посетителей
    
    while (cin >> command) {
        if (command == "ENQUEUE") {
            int time;
            cin >> time;
            
            int randomNum = rand() % 1000;
            string talon = "T" + to_string(randomNum);
            cout << talon << endl;
            
            Person p;
            p.time = time;
            p.talon = talon;
            allPeople.push_back(p);
        }                        
        else if (command == "DISTRIBUTE") {
            sort(allPeople.begin(), allPeople.end(), compareByTime);
            
            vector<pair<int, vector<string>>> windows(windowTotal);
            
            // Распределяем людей по окнам 
            for (const auto &person : allPeople) {
                auto minWindow = min_element(windows.begin(), windows.end(),
                    [](const pair<int, vector<string>> &a, const pair<int, vector<string>> &b) {
                        return a.first < b.first;
                    });
                
                // Добавляем человека в это окно
                minWindow->first += person.time;
                minWindow->second.push_back(person.talon);
            }
            
            for (int i = 0; i < windows.size(); ++i) {
                cout << "Окно " << i + 1 << " (" << windows[i].first << " минут): ";
                for (const auto &talon : windows[i].second) {
                    cout << talon << " ";
                }
                cout << endl;
            }
            break;
        }
    }
    
    return 0;
}