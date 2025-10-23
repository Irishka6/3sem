#include <iostream>
#include <cstdlib>  // для abs()

// сылка на гит
// https://github.com/Irishka6/3sem/tree/master/PZ_1

class Muss {
private:
    int *data;
    int size;
    int capasity;


public:
    // Конструктор по умолчанию
    Muss(int *new_data, int int_capasity=10) {
        capasity = int_capasity;
        size = 0;
        data = new int[capasity];
        for (int i = 0; i < capasity; i++) {
            data[i] = 0;
        }
        for (int i = 1; i <= capasity; i++){
            if(setvalue(new_data[size - 1], size)){
                size++;  
            }
        }
    }

    Muss(const Muss& other) : capasity(other.capasity), size(other.size){
        data = new int[capasity];
        for(int i=0; i < size; i++){
            data[i] = other.data[i];
        }
    }

    // Деструктор (вызывается при уничтожении объекта)
    ~Muss() {
        std::cout << "i dont understud " << std::endl;
        print_data();
        delete [] data;
    }
    
    // Сеттер 
    bool setvalue(int value, int index){
        if (!(abs(value) <= 100)){
            std::cout << "Number not in [-100;100]"<< std::endl;
            return 0;
        }
        if (index >= capasity){
            resize(index);
            size = index;
            std::cout << "the array has been enlarged and a new value added" << std::endl;
            size = index;
        }
        data[index - 1] = value;
        return 1;
    }

    // Геттер
    int get(int index) const{
        if ((index >= capasity) || (index < 0)) {
            std::cout << "going outside the array" << std::endl;
            return 0;
        }
        return data[index];
    }

    void print_data(){
        print_alldata(data, size);
    }
    
    // функция вывода элементов массива
    void print_alldata(int *data_1, int size) const {
        std::cout << "[";
        for (int i = 0; i < size; i++) {
            std::cout << data_1[i];
            if (i < size - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }

    void resize(int newCapacity) {
        int* newData = new int[newCapacity](); // Инициализация нулями
        
        // Копируем существующие данные
        for (int i = 0; i < size && i < newCapacity; i++) {
            newData[i] = data[i];
        }
        
        delete[] data;
        data = newData;
        capasity = newCapacity;
    }

    void add_value(int value){
        setvalue(value, size + 1);
    }

    void sume(const Muss& other){
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


    void ras(const Muss& other){
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

    int getsize(){return size;}
    const int *getdata() const {return data;}
};

int main(){
    int size = 10;
    int data[size] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    Muss muss(data, size);
    Muss muss_2(muss);
    muss_2.setvalue(8, 12);
    muss_2.add_value(6);
    muss.sume(muss_2);
    muss.ras(muss_2);
    return 0;
}