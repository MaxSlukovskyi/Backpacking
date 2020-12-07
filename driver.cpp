#include <iostream>
#include <conio.h>
#include <fstream>
#include <Windows.h>

void enter_data(int*, int*, int*&, int*&, int*&);
bool read_file(int*, int*, int*&, int*&, int*&);
int max_weight(int*, int, int, int*);
int max_number(int, int);
int route_recovery(int, int, int**, int[], int*);
void destroyer(int**, int);
void copy_array(int*, int*, int);
void print_result(int*, int, int*, int, int);
bool write_file(int, int, int*, int*, int);
void about();

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int items_number = 0, capacity = 0, * items = NULL, * rezult = NULL, count = 0;
    int* items_copy = NULL;
label1:
    std::cout << "1 - Введення з клавіатури\n" << "2 - Зчитати з файлу\n";
    std::cout << "3 - Обробка\n" << "4 - Вивод на екран\n";
    std::cout << "5 - Запис у файл\n" << "6 - Про програму\n" << "7 - Вихід\n";
    std::cout << "Зробіть свій вибір: ";
    switch (_getch())
    {
    case '1':
    {
        system("cls");
        enter_data(&capacity, &items_number, items, items_copy, rezult);
        system("cls");
        goto label1;
    }
    case '2':
    {
        system("cls");
        if (!read_file(&capacity, &items_number, items, items_copy, rezult))
            goto label1;
        system("pause");
        system("cls");
        goto label1;
    }
    case '3':
    {
        system("cls");
        count = max_weight(items_copy, items_number, capacity, rezult);
        if (count == -1)
        {
            for (int i = 0; i <= items_number; i++)
                rezult[i] = 0;
            count = 1;
        }
        if (count == 0)
        {
            std::cout << "Спочатку введіть інформацію!" << std::endl;
            system("pause");
        }
        system("cls");
        goto label1;
    }
    case '4':
    {
        system("cls");
        if (count == 0)
        {
            std::cout << "Спочатку введіть та обробіть інформацію!" << std::endl;
            system("pause");
            system("cls");
            goto label1;
        }
        print_result(items, items_number, rezult, count, capacity);
        system("pause");
        system("cls");
        goto label1;
    }
    case '5':
    {
        system("cls");
        if (count == 0)
        {
            std::cout << "Спочатку введіть та обробіть інформацію!" << std::endl;
            system("pause");
            system("cls");
            goto label1;
        }
        if (!write_file(items_number, count, items, rezult, capacity))
            goto label1;
        std::cout << "Дані успішно записано!" << std::endl;
        system("pause");
        system("cls");
        goto label1;
    }
    case '6':
    {
        system("cls");
        std::cout << "\tПРО ПРОГРАМУ" << std::endl;
        about();
        system("pause");
        system("cls");
        goto label1;
    }
    case '7':
	{
		std::cout << "Все го хоро шего!" << std::endl;
    	system("pause");
        delete[]rezult;
        delete[]items;
        delete[]items_copy;
        return 0;
	}
    default:
    {
        std::cout << "Ви ввели некоректне значення!" << std::endl;
        system("pause");
        system("cls");
        goto label1;
    }
    }
}

void enter_data(int* capacity, int* items_number, int*& items, int*& items_copy, int*& rezult)
{
    std::cout << "Введіть кількість предметів: ";
    std::cin >> *items_number;
    std::cout << std::endl;
    items = new int[*items_number];
    items_copy = new int[*items_number];
    rezult = new int[*items_number + 1];
    for (int i = 0; i < *items_number; i++)
    {
        std::cout << "Введіть масу " << i + 1 << " предмету: ";
        std::cin >> items[i];
        std::cout << std::endl;
    }
    copy_array(items_copy, items, *items_number);
    std::cout << "Введіть місткість рюкзака: ";
    std::cin >> *capacity;
    std::cin.clear();
    while (std::cin.get() != '\n');
}

bool read_file(int* capacity, int* items_number, int*& items, int*& items_copy, int*& rezult)
{
    char* datafname = new char[80];
    std::cout << "Введіть шлях до файлу з даними: ";
    gets_s(datafname, 80);
    std::ifstream f1(datafname, std::ios::in);
    if (!f1.is_open())
    {
        std::cout << std::endl << "Не вдалося відкрити файл!" << std::endl;
        system("pause");
        system("cls");
        return false;
    }
    f1 >> *capacity;
    f1 >> *items_number;
    items = new int[*items_number];
    items_copy = new int[*items_number];
    rezult = new int[*items_number + 1];
    for (int i = 0; i < *items_number; i++)
        f1 >> items[i];
    copy_array(items_copy, items, *items_number);
    f1.close();
    delete[]datafname;
    return true;
}

int max_weight(int* items, int items_number, int capacity, int* rezult)
{
    if (items == NULL)
        return 0;
    bool check_capacity = false;
    for (int i = 0; i < items_number; i++)
    {
        if (capacity >= items[i])
        {
            check_capacity = true;
            break;
        }
    }
    if (!check_capacity)
        return -1;
    //dynamic 2d int array for "grid"
    int** grid = new int* [capacity + 1];
    for (int i = 0; i <= capacity; i++)
        grid[i] = new int[items_number + 1];
    //base cases:
    for (int i = 0; i <= capacity; i++)
        grid[i][0] = 0;
    for (int j = 0; j <= items_number; j++)
        grid[0][j] = 0;
    //remove items that are larger than the capacity
    for (int i = 0; i < items_number; i++)
        if (items[i] > capacity || items[i] < 0)
            items[i] = 0;
    //recursive cases:
    for (int j = 1; j <= items_number; j++)
    {
        for (int i = 1; i < items[j - 1]; i++)
            grid[i][j] = grid[i][j - 1];
        for (int i = items[j - 1]; i <= capacity; i++)
            grid[i][j] = max_number(grid[i][j - 1], items[j - 1] + grid[i - items[j - 1]][j - 1]);
    }
    //enter max possible weight in array rezult
    rezult[0] = grid[capacity][items_number];
    //route recovery
    int count = route_recovery(capacity, items_number, grid, items, rezult);
    //delete array grid
    destroyer(grid, capacity + 1);
    return count;
}

int max_number(int a, int b)
{
    return a > b ? a : b;
}

int route_recovery(int capacity, int items_number, int** grid, int items[], int* rezult)
{
    int rez_index = 1, count = 0;
    for (int j = items_number; j >= 1; j--)
    {
        if (grid[capacity][j] != grid[capacity][j - 1])
        {
            rezult[rez_index] = j;
            count++;
            rez_index++;
            capacity -= items[j - 1];
        }
    }
    return count;
}

void destroyer(int** mass, int size)
{
    for (int i = 0; i < size; i++)
        delete[] mass[i];
    delete[] mass;
}

void copy_array(int* mass_to, int* mass_from, int size)
{
    for (int i = 0; i < size; i++)
        mass_to[i] = mass_from[i];
}

void print_result(int* items, int items_number, int* rezult, int count, int capacity)
{
    std::cout << "Предмети: " << std::endl;
    for (int i = 0; i < items_number; i++)
        std::cout << "Предмет " << i + 1 << " - " << items[i] << std::endl;
    std::cout << "Ємність рюкзака: " << capacity << std::endl;
    std::cout << "Максимальна можлива вага: " << rezult[0] << std::endl;
    std::cout << "Набір предметів: ";
    for (int i = count; i > 0; i--)
        std::cout << rezult[i] << " ";
    std::cout << std::endl;
}

bool write_file(int items_number, int count, int* items, int* rezult, int capacity)
{
    char* resultfname = new char[80];
    std::cout << "Введіть шлях до файл, в який зберегти результат: ";
    gets_s(resultfname, 80);
    std::ofstream f2(resultfname, std::ios_base::out | std::ios_base::trunc);
    if (!f2.is_open())
    {
        std::cout << "Не вдалося відкрити файл!" << std::endl;
        system("pause");
        system("cls");
        return false;
    }
    f2 << "Предмети: " << std::endl;
    for (int i = 0; i < items_number; i++)
        f2 << "Предмет " << i + 1 << " - " << items[i] << std::endl;
    f2 << "Ємність рюкзака: " << capacity << std::endl;
    f2 << "Максимальна можлива вага: " << rezult[0] << std::endl;
    f2 << "Набір предметів: ";
    for (int i = count; i > 0; i--)
        f2 << rezult[i] << " ";
    f2.close();
    return true;
}

void about()
{
    std::ifstream f("about.txt", std::ios::in);
    char ch = '\0';
	while(!f.eof())
	{
		std::cout << ch;
        ch = f.get();
	}
    std::cout << std::endl;
    f.close();
}