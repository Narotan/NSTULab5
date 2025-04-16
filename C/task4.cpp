#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <algorithm>


using namespace std;

// структура для каждой команды 
struct Command {
    string type;
    string i;
    string j;
};


void addFriends (unordered_map<string, vector<string>>& people, string person1, string person2) {
    people[person1].push_back(person2);
    people[person2].push_back(person1);
};


void question(unordered_map<string, vector<string>>& people, string person1, string person2) {
    auto it = find(people[person1].begin(), people[person1].end(), person2);
    if (it != people[person1].end()) {
        cout << "YES" << endl;
    } else {
        cout << "NO" << endl;
    }

};


void countFriends(unordered_map<string, vector<string>>& people, string person) {
    cout << people[person].size() << endl;
};


int main() {
    cout << "Доступные команды: FRIENDS, COUNT, QUESTION "<< endl;
    int count;
    cin >> count;
    cin.ignore();

    vector<Command> commands;

    // считываем команды введенные пользователем 
    for (int k = 0; k < count; ++k) {
        string line;
        getline(cin, line);
        istringstream iss(line);

        string cmd;
        string i = "", j = "";
        iss >> cmd;
        if (cmd == "FRIENDS" || cmd == "QUESTION") {
            iss >> i >> j;
        } else if (cmd == "COUNT") {
            iss >> i;
        } else {
            cout << cmd << " не может быть обработана" << endl;
        }

        commands.push_back({cmd, i, j});
    }

    cout << "------------------------------------" << endl;

    unordered_map<string, vector<string>> people; // мап где у нас будет человек : все его друзья 

    for (Command cmd: commands) {
        if (cmd.type == "FRIENDS") {
            addFriends(people, cmd.i, cmd.j);
        } else if (cmd.type == "QUESTION") {
            question(people, cmd.i, cmd.j);
        } else if (cmd.type == "COUNT") {
            countFriends(people, cmd.i);
        }
    }

     return 0;
}