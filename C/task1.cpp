#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

// структура для ячейки 
struct Cell {
    string section; // Зона (A/B)
    int sectionCount; // Стеллаж
    int verticalSection; // Уровень
    int shelvesSection; // Полка
    map<string, int> item;
};

// менюшка
void showMenu() {
    cout << "================= МЕНЮ =================" << endl;
    cout << "Команды для работы со складом:" << endl;
    cout << "ADD <товар> <кол-во> <ячейка>   — добавить товар" << endl;
    cout << "REMOVE <товар> <кол-во> <ячейка> — удалить товар" << endl;
    cout << "INFO                            — показать информацию о складе" << endl;
    cout << "EXIT                            — выход из программы" << endl;
    cout << "=======================================" << endl;
}

void add(map<string, Cell>& storage, int& capacity, vector<Cell>& info, map<string, int>& loaded) {
    string name, cell;
    int num;

    cin >> name >> num >> cell;

    if (storage.find(cell) != storage.end()) {
        int currentCount = 0;
        for (auto& [_, qty] : storage[cell].item) {
            currentCount += qty;
        }

        if (currentCount + num > 10) {
            cout << "Ошибка: в ячейке " << cell << " уже " << currentCount
                 << " единиц. Можно добавить максимум " << 10 - currentCount << "." << endl;
            return;
        }

        storage[cell].item[name] += num;
        capacity -= num;
        loaded["All"] += num;
        loaded[storage[cell].section] += num;

        bool itemFound = false;
        for (auto& cellInfo : info) {
            if (cellInfo.section == storage[cell].section &&
                cellInfo.sectionCount == storage[cell].sectionCount &&
                cellInfo.verticalSection == storage[cell].verticalSection &&
                cellInfo.shelvesSection == storage[cell].shelvesSection) {
                cellInfo.item[name] += num;
                itemFound = true;
                break;
            }
        }
        if (!itemFound) {
            info.push_back(storage[cell]);
        }

        cout << "Товар добавлен в ячейку " << cell << endl;
    } else {
        cout << "Ошибка: ячейка " << cell << " не существует!" << endl;
    }
}


void remove(map<string, Cell>& storage, int& capacity, vector<Cell>& info, map<string, int>& loaded) {
    string name, cell;
    int num;

    cin >> name >> num >> cell;

    if (storage.find(cell) != storage.end()) {
        if (storage[cell].item[name] >= num) {
            storage[cell].item[name] -= num;
            capacity += num;
            loaded["All"] -= num;
            loaded[storage[cell].section] -= num;

            for (auto it = info.begin(); it != info.end(); ++it) {
                auto& cellInfo = *it;
                if (cellInfo.section == storage[cell].section &&
                    cellInfo.sectionCount == storage[cell].sectionCount &&
                    cellInfo.verticalSection == storage[cell].verticalSection &&
                    cellInfo.shelvesSection == storage[cell].shelvesSection) {

                    if (cellInfo.item[name] >= num) {
                        cellInfo.item[name] -= num;
                        if (cellInfo.item[name] == 0) {
                            cellInfo.item.erase(name);
                        }
                    }

                    if (cellInfo.item.empty()) {
                        info.erase(it);
                        break;
                    }
                    break;
                }
            }

            cout << "Товар удален из ячейки " << cell << endl;
        } else {
            cout << "Ошибка: недостаточно товара в ячейке!" << endl;
        }
    } else {
        cout << "Ошибка: ячейка " << cell << " не существует!" << endl;
    }
}

void information(map<string, int>& loaded, vector<Cell>& info, const map<string, Cell>& storage) {
    cout << "\n=== Информация о складе ===" << endl;
    cout << "Загруженность всего склада: " << (loaded["All"] / 7200.0) * 100 << "%" << endl;
    cout << "Зона A: " << (loaded["A"] / 7200.0) * 100 << "%" << endl;
    cout << "Зона B: " << (loaded["B"] / 7200.0) * 100 << "%" << endl;

    cout << "\nЗанятые ячейки:" << endl;
    for (const auto& cell : info) {
        string addr = cell.section + to_string(cell.sectionCount)
                      + to_string(cell.verticalSection) + to_string(cell.shelvesSection);

        cout << addr << ": ";
        for (const auto& item : cell.item) {
            cout << item.first << " (" << item.second << " шт.), ";
        }
        cout << endl;
    }

    cout << "\nПустые ячейки:" << endl;
    for (const auto& [addr, cell] : storage) {
        if (cell.item.empty()) {
            cout << addr << " ";
        }
    }
    cout << endl;
}

int main() {
    map<string, Cell> storage;
    vector<Cell> info;
    map<string, int> loaded = {{"A", 0}, {"B", 0}, {"All", 0}};
    int capacity = 7200;

    vector<string> storageName = {"A", "B"};
    vector<int> rackNum = {1, 2, 3, 4};
    vector<int> verticalNum = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};
    vector<int> shelvNum = {1, 2, 3, 4, 5};

    for (const auto& name : storageName) {
        for (const auto& rack : rackNum) {
            for (const auto& vertical : verticalNum) {
                for (const auto& shelf : shelvNum) {
                    string address = name + to_string(rack) + to_string(vertical) + to_string(shelf);
                    Cell cell = {name, rack, vertical, shelf};
                    storage[address] = cell;
                }
            }
        }
    }

    showMenu();

    string command;
    while (true) {
        cout << "\nВведите команду: ";
        cin >> command;

        if (command == "ADD") {
            add(storage, capacity, info, loaded);
        } else if (command == "REMOVE") {
            remove(storage, capacity, info, loaded);
        } else if (command == "INFO") {
            information(loaded, info, storage);
        } else if (command == "EXIT") {
            cout << "Выход из программы." << endl;
            break;
        } else {
            cout << "Неизвестная команда. Введите снова." << endl;
        }
    }

    return 0;
}
