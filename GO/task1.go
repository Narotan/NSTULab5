package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

// Структура для описания ячейки на складе
// Включает координаты и содержимое
type Cell struct {
	Section         string         // Зона (A или B)
	SectionCount    int            // Номер стойки
	VerticalSection int            // Вертикальный уровень
	ShelvesSection  int            // Полка
	Item            map[string]int // Карта товаров в ячейке (название -> количество)
}

// Показывает меню команд
func showMenu() {
	fmt.Println("================= МЕНЮ =================")
	fmt.Println("Команды для работы со складом:")
	fmt.Println("ADD <товар> <кол-во> <ячейка>   — добавить товар")
	fmt.Println("REMOVE <товар> <кол-во> <ячейка> — удалить товар")
	fmt.Println("INFO                            — показать информацию о складе")
	fmt.Println("EXIT                            — выход из программы")
	fmt.Println("=======================================")
}

// Функция добавления товара на склад
func add(storage map[string]*Cell, capacity *int, info *[]*Cell, loaded map[string]int, args []string) {
	if len(args) < 3 {
		fmt.Println("Недостаточно аргументов для команды ADD")
		return
	}
	tname := args[0]                  // название товара
	num, err := strconv.Atoi(args[1]) // кол-во товара
	if err != nil {
		fmt.Println("Ошибка: неверное количество товара")
		return
	}
	cell := args[2] // адрес ячейки

	// Проверяем, существует ли ячейка
	if c, ok := storage[cell]; ok {
		// Считаем текущее кол-во товара в ячейке
		currentCount := 0
		for _, qty := range c.Item {
			currentCount += qty
		}

		// Проверка на переполнение ячейки (максимум 10)
		if currentCount+num > 10 {
			fmt.Printf("Ошибка: в ячейке %s уже %d единиц. Можно добавить максимум %d.\n", cell, currentCount, 10-currentCount)
			return
		}

		// Обновляем содержимое и статистику
		c.Item[tname] += num
		*capacity -= num
		loaded["All"] += num
		loaded[c.Section] += num

		// Обновляем или добавляем инфу об этой ячейке в info
		itemFound := false
		for _, cellInfo := range *info {
			if cellInfo.Section == c.Section &&
				cellInfo.SectionCount == c.SectionCount &&
				cellInfo.VerticalSection == c.VerticalSection &&
				cellInfo.ShelvesSection == c.ShelvesSection {
				cellInfo.Item[tname] += num
				itemFound = true
				break
			}
		}
		if !itemFound {
			*info = append(*info, c)
		}
		fmt.Printf("Товар добавлен в ячейку %s\n", cell)
	} else {
		fmt.Printf("Ошибка: ячейка %s не существует!\n", cell)
	}
}

// Удаление товара с ячейки
func remove(storage map[string]*Cell, capacity *int, info *[]*Cell, loaded map[string]int, args []string) {
	if len(args) < 3 {
		fmt.Println("Недостаточно аргументов для команды REMOVE")
		return
	}
	tname := args[0]
	num, err := strconv.Atoi(args[1])
	if err != nil {
		fmt.Println("Ошибка: неверное количество товара")
		return
	}
	cell := args[2]

	if c, ok := storage[cell]; ok {
		if c.Item[tname] >= num {
			c.Item[tname] -= num
			*capacity += num
			loaded["All"] -= num
			loaded[c.Section] -= num

			// Обновляем или удаляем ячейку из info
			for i, cellInfo := range *info {
				if cellInfo.Section == c.Section &&
					cellInfo.SectionCount == c.SectionCount &&
					cellInfo.VerticalSection == c.VerticalSection &&
					cellInfo.ShelvesSection == c.ShelvesSection {
					if cellInfo.Item[tname] >= num {
						cellInfo.Item[tname] -= num
						if cellInfo.Item[tname] == 0 {
							delete(cellInfo.Item, tname)
						}
					}
					if len(cellInfo.Item) == 0 {
						*info = append((*info)[:i], (*info)[i+1:]...)
					}
					break
				}
			}
			fmt.Printf("Товар удален из ячейки %s\n", cell)
		} else {
			fmt.Println("Ошибка: недостаточно товара в ячейке!")
		}
	} else {
		fmt.Printf("Ошибка: ячейка %s не существует!\n", cell)
	}
}

// Вывод информации о загруженности склада и ячейках
func information(loaded map[string]int, info []*Cell, storage map[string]*Cell) {
	fmt.Println("\n=== Информация о складе ===")
	fmt.Printf("Загруженность всего склада: %.2f%%\n", float64(loaded["All"])/7200.0*100)
	fmt.Printf("Зона A: %.2f%%\n", float64(loaded["A"])/7200.0*100)
	fmt.Printf("Зона B: %.2f%%\n", float64(loaded["B"])/7200.0*100)

	fmt.Println("\nЗанятые ячейки:")
	for _, cell := range info {
		addr := cell.Section + strconv.Itoa(cell.SectionCount) + strconv.Itoa(cell.VerticalSection) + strconv.Itoa(cell.ShelvesSection)
		fmt.Printf("%s: ", addr)
		for name, count := range cell.Item {
			fmt.Printf("%s (%d шт.), ", name, count)
		}
		fmt.Println()
	}

	fmt.Println("\nПустые ячейки:")
	for addr, cell := range storage {
		if len(cell.Item) == 0 {
			fmt.Printf("%s ", addr)
		}
	}
	fmt.Println()
}

// Точка входа, инициализация склада и обработка команд
func main() {
	storage := make(map[string]*Cell)                  // Хранилище ячеек
	info := []*Cell{}                                  // Информация о занятых ячейках
	loaded := map[string]int{"A": 0, "B": 0, "All": 0} // Загруженность по зонам
	capacity := 7200                                   // Общая вместимость

	// Параметры генерации адресов ячеек
	storageName := []string{"A", "B"}
	rackNum := []int{1, 2, 3, 4}
	verticalNum := makeRange(1, 18)
	shelvNum := []int{1, 2, 3, 4, 5}

	// Заполняем карту ячеек
	for _, name := range storageName {
		for _, rack := range rackNum {
			for _, vertical := range verticalNum {
				for _, shelf := range shelvNum {
					addr := name + strconv.Itoa(rack) + strconv.Itoa(vertical) + strconv.Itoa(shelf)
					storage[addr] = &Cell{
						Section:         name,
						SectionCount:    rack,
						VerticalSection: vertical,
						ShelvesSection:  shelf,
						Item:            make(map[string]int),
					}
				}
			}
		}
	}

	// Запускаем цикл чтения команд
	showMenu()
	scanner := bufio.NewScanner(os.Stdin)
	for {
		fmt.Print("\nВведите команду: ")
		if !scanner.Scan() {
			break
		}
		line := scanner.Text()
		parts := strings.Fields(line)
		if len(parts) == 0 {
			continue
		}
		cmd := parts[0]
		args := parts[1:]

		switch cmd {
		case "ADD":
			add(storage, &capacity, &info, loaded, args)
		case "REMOVE":
			remove(storage, &capacity, &info, loaded, args)
		case "INFO":
			information(loaded, info, storage)
		case "EXIT":
			fmt.Println("Выход из программы.")
			return
		default:
			fmt.Println("Неизвестная команда. Введите снова.")
		}
	}
}

// Создание диапазона чисел от start до end включительно
func makeRange(start, end int) []int {
	r := make([]int, end-start+1)
	for i := range r {
		r[i] = start + i
	}
	return r
}
