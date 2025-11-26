#include <iostream>
#include <cstdlib>
#include <algorithm>  // для sort, min_element, max_element


class Muss {
protected:  
    int *data;
    int size;
    int capacity;

public:
    // Конструктор по умолчанию
    Muss(int int_capacity = 10) {
        capacity = int_capacity;
        size = 0;
        data = new int[capacity];
        for (int i = 0; i < capacity; i++) {
            data[i] = 0;
        }
    }

    // Конструктор с массивом
    Muss(int *new_data, int new_size, int int_capacity = 10) {
        capacity = std::max(int_capacity, new_size);
        size = new_size;
        data = new int[capacity];
        for (int i = 0; i < capacity; i++) {
            data[i] = 0;
        }
        for (int i = 0; i < size; i++){
            setvalue(new_data[i], i);
        }
    }

    // Конструктор копирования
    Muss(const Muss& other) : capacity(other.capacity), size(other.size){
        data = new int[capacity];
        for(int i = 0; i < size; i++){
            data[i] = other.data[i];
        }
    }

    // Деструктор
    ~Muss() {
        delete [] data;
    }
    
    // Сеттер 
    bool setvalue(int value, int index){
        if (!(abs(value) <= 100)){
            std::cout << "Number not in [-100;100]"<< std::endl;
            return false;
        }
        if (index >= capacity){
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
        if ((index >= capacity) || (index < 0)) {
            std::cout << "Going outside the array" << std::endl;
            return 0;
        }
        return data[index];
    }

    void print_data() const {
        print_alldata(data, size);
    }
    
    // Функция вывода элементов массива
    void print_alldata(int *data_1, int data_size) const {
        std::cout << "[";
        for (int i = 0; i < data_size; i++) {
            std::cout << data_1[i];
            if (i < data_size - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }

    void resize(int newCapacity) {
        int* newData = new int[newCapacity]();
        
        // Копируем существующие данные
        for (int i = 0; i < size && i < newCapacity; i++) {
            newData[i] = data[i];
        }
        
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

    void add_value(int value){
        setvalue(value, size);
    }

    void sum(const Muss& other){
        int new_size = std::max(size, other.size);
        int *data_sum = new int[new_size]();
        for (int i = 0; i < new_size; i++) {
            int val1 = 0; 
            int val2 = 0;
            if (i < size){val1 = data[i];}
            if (i < other.size) { val2 = other.data[i];}
            data_sum[i] = val1 + val2;
        }
        print_alldata(data_sum, new_size);
        delete[] data_sum;
    }

    void subtract(const Muss& other){
        int new_size = std::max(size, other.size);
        int *data_sum = new int[new_size]();
        for (int i = 0; i < new_size; i++) {
            int val1 = 0; 
            int val2 = 0;
            if (i < size){val1 = data[i];}
            if (i < other.size) { val2 = other.data[i];}
            data_sum[i] = val1 - val2;
        }
        print_alldata(data_sum, new_size);
        delete[] data_sum;
    }

    int getsize() const { return size; }
    int getcapacity() const { return capacity; }
    const int *getdata() const { return data; }
};

// Наследник класса Muss с дополнительными методами
class AdvancedMuss : public Muss {
public:
    // Конструкторы
    AdvancedMuss(int int_capacity = 10) : Muss(int_capacity) {}
    
    AdvancedMuss(int *new_data, int new_size, int int_capacity = 10) 
        : Muss(new_data, new_size, int_capacity) {}
    
    AdvancedMuss(const AdvancedMuss& other) : Muss(other) {}
    
    AdvancedMuss(const Muss& other) : Muss(other) {}

    // Вычисление среднего значения
    double calculateAverage() const {
        if (size == 0) {
            std::cout << "Array is empty, cannot calculate average" << std::endl;
            return 0.0;
        }
        
        double sum = 0.0;
        for (int i = 0; i < size; i++) {
            sum += data[i];
        }
        return sum / size;
    }

    // Вычисление медианного значения
    double calculateMedian() {
        if (size == 0) {
            std::cout << "Array is empty, cannot calculate median" << std::endl;
            return 0.0;
        }
        
        // Создаем копию массива для сортировки
        int* temp = new int[size];
        for (int i = 0; i < size; i++) {
            temp[i] = data[i];
        }
        
        // Сортируем массив
        std::sort(temp, temp + size);
        
        double median;
        if (size % 2 == 0) {
            // Четное количество элементов - среднее двух центральных
            median = (temp[size/2 - 1] + temp[size/2]) / 2.0;
        } else {
            // Нечетное количество элементов - центральный элемент
            median = temp[size/2];
        }
        
        delete[] temp;
        return median;
    }

    // Возврат наименьшего элемента
    int findMin() const {
        if (size == 0) {
            std::cout << "Array is empty, cannot find minimum" << std::endl;
            return 0;
        }
        
        int min_val = data[0];
        for (int i = 1; i < size; i++) {
            if (data[i] < min_val) {
                min_val = data[i];
            }
        }
        return min_val;
    }

    // Возврат наибольшего элемента
    int findMax() const {
        if (size == 0) {
            std::cout << "Array is empty, cannot find maximum" << std::endl;
            return 0;
        }
        
        int max_val = data[0];
        for (int i = 1; i < size; i++) {
            if (data[i] > max_val) {
                max_val = data[i];
            }
        }
        return max_val;
    }

    // Дополнительный метод: вывод статистики
    void printStatistics() {
        std::cout << "Array: ";
        print_data();
        std::cout << "Size: " << getsize() << std::endl;
        std::cout << "Average: " << calculateAverage() << std::endl;
        std::cout << "Median: " << calculateMedian() << std::endl;
        std::cout << "Min: " << findMin() << std::endl;
        std::cout << "Max: " << findMax() << std::endl;
        std::cout << "-------------------" << std::endl;
    }
};

// Демонстрация работы
int main(){
    int data1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int data2[] = {5, 2, 8, 1, 9, 3};
    
    std::cout << "=== Testing AdvancedMuss class ===" << std::endl;
    
    // Тестирование на упорядоченном массиве
    AdvancedMuss arr1(data1, 10);
    std::cout << "Array 1:" << std::endl;
    arr1.printStatistics();
    
    // Тестирование на неупорядоченном массиве
    AdvancedMuss arr2(data2, 6);
    std::cout << "Array 2:" << std::endl;
    arr2.printStatistics();
    
    // Тестирование на пустом массиве
    AdvancedMuss arr3;
    std::cout << "Empty array:" << std::endl;
    arr3.printStatistics();
    
    // Тестирование с одним элементом
    AdvancedMuss arr4;
    arr4.add_value(42);
    std::cout << "Single element array:" << std::endl;
    arr4.printStatistics();
    
    // Тестирование с четным количеством элементов для медианы
    int data3[] = {1, 3, 2, 4};
    AdvancedMuss arr5(data3, 4);
    std::cout << "Even-sized array:" << std::endl;
    arr5.printStatistics();

    return 0;
}