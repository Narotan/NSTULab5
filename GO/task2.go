package main

import (
	"bufio"
	"fmt"
	"math/rand"
	"os"
	"sort"
	"strconv"
	"strings"
	"time"
)

// Структура посетителя с талоном и временем ожидания
type Person struct {
	Time  int
	Talon string
}

// Сравнение по убыванию времени (как в C++)
func compareByTimeDesc(a, b Person) bool {
	return a.Time > b.Time
}

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	fmt.Print("Введите количество окон: ")
	scanner.Scan()
	windowTotal, _ := strconv.Atoi(scanner.Text())

	var allPeople []Person
	rand.Seed(time.Now().UnixNano())

	for {
		fmt.Print("Введите команду: ")
		scanner.Scan()
		line := scanner.Text()
		parts := strings.Fields(line)
		if len(parts) == 0 {
			continue
		}
		command := parts[0]

		if command == "ENQUEUE" {
			if len(parts) < 2 {
				fmt.Println("Ошибка: введите время ожидания после ENQUEUE")
				continue
			}
			timeValue, err := strconv.Atoi(parts[1])
			if err != nil {
				fmt.Println("Ошибка чтения времени")
				continue
			}

			talon := fmt.Sprintf("T%d", rand.Intn(1000))
			fmt.Println(talon)
			allPeople = append(allPeople, Person{Time: timeValue, Talon: talon})
		} else if command == "DISTRIBUTE" {
			sort.Slice(allPeople, func(i, j int) bool {
				return compareByTimeDesc(allPeople[i], allPeople[j])
			})

			windows := make([]struct {
				TimeSum int
				Talons  []string
			}, windowTotal)

			for _, person := range allPeople {
				minIdx := 0
				for i := 1; i < windowTotal; i++ {
					if windows[i].TimeSum < windows[minIdx].TimeSum {
						minIdx = i
					}
				}
				windows[minIdx].TimeSum += person.Time
				windows[minIdx].Talons = append(windows[minIdx].Talons, person.Talon)
			}

			for i, w := range windows {
				fmt.Printf("Окно %d (%d минут): %s\n", i+1, w.TimeSum, strings.Join(w.Talons, " "))
			}
			break
		}
	}
}
