#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>

class Muss {
protected:  
    int *data;
    int size;
    int capacity;

public:
    // Конструктор по умолчанию
    Muss(int int_capacity = 10) : capacity(int_capacity), size(0) {
        data = new int[capacity]();
    }

    // Конструктор с массивом
    Muss(int *new_data, int new_size, int int_capacity = 10) 
        : capacity(std::max(int_capacity, new_size)), size(new_size) {
        data = new int[capacity]();
        for (int i = 0; i < size; i++) {
            setvalue(new_data[i], i);
        }
    }

    // Конструктор копирования
    Muss(const Muss& other) : capacity(other.capacity), size(other.size) {
        data = new int[capacity];
        std::copy(other.data, other.data + size, data);
    }

    // Оператор присваивания
    Muss& operator=(const Muss& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            size = other.size;
            data = new int[capacity];
            std::copy(other.data, other.data + size, data);
        }
        return *this;
    }

    // Деструктор
    virtual ~Muss() {
        delete[] data;
    }
    
    // Сеттер 
    bool setvalue(int value, int index) {
        if (abs(value) > 100) {
            std::cout << "Number not in [-100;100]" << std::endl;
            return false;
        }
        if (index >= capacity) {
            resize(index + 1);
        }
        data[index] = value;
        if (index >= size) {
            size = index + 1;
        }
        return true;
    }

    // Геттер
    int get(int index) const {
        if (index < 0 || index >= size) {
            std::cout << "Index out of bounds" << std::endl;
            return 0;
        }
        return data[index];
    }

    void print_data() const {
        std::cout << "[";
        for (int i = 0; i < size; i++) {
            std::cout << data[i];
            if (i < size - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }

    void resize(int newCapacity) {
        int* newData = new int[newCapacity]();
        std::copy(data, data + std::min(size, newCapacity), newData);
        delete[] data;
        data = newData;
        capacity = newCapacity;
        size = std::min(size, newCapacity);
    }

    void add_value(int value) {
        setvalue(value, size);
    }

    // Виртуальный метод для экспорта данных
    virtual void exportData() const {
        std::cout << "Base export method - override in derived classes" << std::endl;
    }

    // Генерация имени файла с текущей датой и временем
    std::string generateFilename(const std::string& extension) const {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d_%H-%M-%S");
        return ss.str() + extension;
    }

    // Геттеры
    int getsize() const { return size; }
    int getcapacity() const { return capacity; }
    const int* getdata() const { return data; }
};

// Класс для экспорта в TXT файл
class ArrTxt : public Muss {
public:
    using Muss::Muss;  // Наследуем конструкторы

    // Переопределяем метод экспорта
    void exportData() const override {
        std::string filename = generateFilename(".txt");
        std::ofstream file(filename);
        
        if (!file.is_open()) {
            std::cout << "Error: Cannot open file " << filename << std::endl;
            return;
        }

        // Записываем метаданные
        file << "Array Data Export - TXT Format\n";
        file << "Generated: " << getCurrentDateTime() << "\n";
        file << "Size: " << size << "\n";
        file << "Capacity: " << capacity << "\n";
        file << "Data: ";
        
        // Записываем элементы массива
        for (int i = 0; i < size; i++) {
            file << data[i];
            if (i < size - 1) file << " ";
        }
        file << "\n";
        
        // Статистика
        file << "Statistics:\n";
        file << "Min: " << findMin() << "\n";
        file << "Max: " << findMax() << "\n";
        file << "Average: " << calculateAverage() << "\n";
        
        file.close();
        std::cout << "Data exported to TXT file: " << filename << std::endl;
    }

private:
    std::string getCurrentDateTime() const {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    int findMin() const {
        if (size == 0) return 0;
        return *std::min_element(data, data + size);
    }

    int findMax() const {
        if (size == 0) return 0;
        return *std::max_element(data, data + size);
    }

    double calculateAverage() const {
        if (size == 0) return 0.0;
        double sum = 0.0;
        for (int i = 0; i < size; i++) {
            sum += data[i];
        }
        return sum / size;
    }
};

// Класс для экспорта в CSV файл
class ArrCSV : public Muss {
public:
    using Muss::Muss;  // Наследуем конструкторы

    // Переопределяем метод экспорта
    void exportData() const override {
        std::string filename = generateFilename(".csv");
        std::ofstream file(filename);
        
        if (!file.is_open()) {
            std::cout << "Error: Cannot open file " << filename << std::endl;
            return;
        }

        // Заголовок CSV
        file << "Index,Value\n";
        
        // Данные в формате CSV
        for (int i = 0; i < size; i++) {
            file << i << "," << data[i] << "\n";
        }
        
        // Статистика в отдельной секции
        file << "\nStatistics\n";
        file << "Min," << findMin() << "\n";
        file << "Max," << findMax() << "\n";
        file << "Average," << calculateAverage() << "\n";
        file << "Size," << size << "\n";
        file << "Capacity," << capacity << "\n";
        
        file.close();
        std::cout << "Data exported to CSV file: " << filename << std::endl;
    }

private:
    int findMin() const {
        if (size == 0) return 0;
        return *std::min_element(data, data + size);
    }

    int findMax() const {
        if (size == 0) return 0;
        return *std::max_element(data, data + size);
    }

    double calculateAverage() const {
        if (size == 0) return 0.0;
        double sum = 0.0;
        for (int i = 0; i < size; i++) {
            sum += data[i];
        }
        return sum / size;
    }
};

// Функция для демонстрации полиморфизма
void exportArrayData(const Muss& array) {
    array.exportData();
}

// Функция для демонстрации полиморфизма через указатель
void exportArrayDataPtr(const Muss* array) {
    array->exportData();
}

int main() {
    std::cout << "=== Array Export Demonstration ===" << std::endl;
    
    // Создаем тестовые данные
    int testData[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int testSize = 10;

    // Создаем объекты разных типов
    ArrTxt txtArray(testData, testSize);
    ArrCSV csvArray(testData, testSize);
    Muss baseArray(testData, testSize);

    std::cout << "\n1. Testing ArrTxt export:" << std::endl;
    txtArray.print_data();
    txtArray.exportData();  // Вызов через объект

    std::cout << "\n2. Testing ArrCSV export:" << std::endl;
    csvArray.print_data();
    csvArray.exportData();  // Вызов через объект

    std::cout << "\n3. Testing polymorphism with references:" << std::endl;
    exportArrayData(txtArray);  // Будет вызван ArrTxt::exportData()
    exportArrayData(csvArray);  // Будет вызван ArrCSV::exportData()
    exportArrayData(baseArray); // Будет вызван Muss::exportData()

    std::cout << "\n4. Testing polymorphism with pointers:" << std::endl;
    Muss* arrays[] = {&txtArray, &csvArray, &baseArray};
    
    for (int i = 0; i < 3; i++) {
        exportArrayDataPtr(arrays[i]);
    }

    std::cout << "\n5. Testing with different data:" << std::endl;
    int randomData[] = {5, -2, 8, 1, -9, 3, 7, -4, 6};
    ArrTxt randomTxtArray(randomData, 9);
    ArrCSV randomCsvArray(randomData, 9);
    
    randomTxtArray.exportData();
    randomCsvArray.exportData();

    return 0;
}