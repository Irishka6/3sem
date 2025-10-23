#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdint>
#include <algorithm>

using namespace std;

class PGMImageProcessor {
private:
    int width, height;
    int maxValue;
    vector<uint8_t> pixelData;
    string magicNumber;  // "P5" для бинарного PGM

public:
    // ТЕОРИЯ: Чтение PGM файла
    // Формат текстовый заголовок + бинарные данные
    bool loadPGM(const string& filename) {
        ifstream file(filename, ios::binary);
        if (!file) {
            cerr << "Ошибка: не удалось открыть файл " << filename << endl;
            return false;
        }

        // Чтение magic number
        file >> magicNumber;
        if (magicNumber != "P5") {
            cerr << "Ошибка: поддерживается только бинарный PGM (P5)" << endl;
            return false;
        }

        // ТЕОРИЯ: Пропуск комментариев и чтение заголовка
        skipComments(file);
        file >> width >> height;
        
        skipComments(file);
        file >> maxValue;
        
        // Пропуск одного байта (обычно \n или пробел)
        file.get(); 

        cout << "Информация о PGM файле:" << endl;
        cout << "  Формат: " << magicNumber << endl;
        cout << "  Размер: " << width << "x" << height << endl;
        cout << "  Макс. значение: " << maxValue << endl;

        // Проверка поддержки формата
        if (maxValue != 255) {
            cerr << "Ошибка: поддерживается только max_value = 255" << endl;
            return false;
        }

        // ТЕОРИЯ: Чтение бинарных данных
        // Размер данных = width * height * 1 байт (для 8-битного)
        int dataSize = width * height;
        pixelData.resize(dataSize);
        
        file.read(reinterpret_cast<char*>(pixelData.data()), dataSize);

        if (!file) {
            cerr << "Ошибка чтения данных пикселей" << endl;
            return false;
        }

        cout << "Файл успешно загружен! Пикселей: " << pixelData.size() << endl;
        return true;
    }

    // ТЕОРИЯ: Пропуск комментариев в PGM
    // Комментарии начинаются с # и идут до конца строки
    void skipComments(ifstream& file) {
        char c;
        while (file.get(c)) {
            if (c == '#') {
                // Пропускаем всю строку комментария
                string comment;
                getline(file, comment);
                cout << "  Пропущен комментарий: #" << comment << endl;
            } else {
                // Возвращаем символ обратно в поток
                file.putback(c);
                break;
            }
        }
    }
    
    // скользящее среднее 
    void invertColors() {
        cout << "Начало удаления шумов." << endl;
        vector<uint8_t> tempData = pixelData;
        
        for (int j = 1; j < height - 1; j++) {
            for (int i = 1; i < width - 1; i++) {
               int index = j * width + i;
        
            // Правильное вычисление среднего для окна 3x3
            int sum = 
                tempData[(j-1) * width + (i-1)] + tempData[(j-1) * width + i] + tempData[(j-1) * width + (i+1)] +
                tempData[j * width + (i-1)]     + tempData[index]               + tempData[j * width + (i+1)] +
                tempData[(j+1) * width + (i-1)] + tempData[(j+1) * width + i] + tempData[(j+1) * width + (i+1)];
            
                pixelData[index] = static_cast<uint8_t>(sum / 9);
            }
        }
        
        cout << "Инвертирование завершено!" << endl;
    }

    // бегущее среднее
    void run_sr(){
        cout << "Начало удаления шумов." << endl;
        for (float k = 0.1; k < 1; k = k + 0.2){
            vector<uint8_t> tempData = pixelData;
            for (int j = 0; j < height; j++) {
                float filtred = pixelData[j*width];
                for (int i = 1; i < width; i++) {
                    filtred += (pixelData[j * width + i]- filtred) * k;
                    pixelData[j * width + i] = static_cast<uint8_t>(filtred);
                }
            }
            savePGM("./rezalts/run" + to_string(k) + ".pgm");
            pixelData = tempData;
        }
    }

    void med(){
        int val[3] = {3, 5, 7};
        vector<uint8_t> originalData = pixelData; // Сохраняем оригинал
        
        for (int k = 0; k < 3; k++){
            int windowSize = val[k];
            int radius = windowSize / 2; // РАДИУС = размер_окна / 2
            
            cout << "Медианный фильтр с окном " << windowSize << "x" << windowSize << endl;
            
            pixelData = originalData; // Восстанавливаем оригинал
            vector<uint8_t> tempData = pixelData;
            
            // ИСПРАВЛЕННЫЕ ГРАНИЦЫ: radius вместо 1
            for (int j = radius; j < height - radius; j++) {
                for (int i = radius; i < width - radius; i++) {
                    vector<uint8_t> neighbors;
                    
                    // Собираем соседей из окна windowSize x windowSize
                    for (int dj = -radius; dj <= radius; dj++) {
                        for (int di = -radius; di <= radius; di++) {
                            neighbors.push_back(tempData[(j + dj) * width + (i + di)]);
                        }
                    }
                    
                    sort(neighbors.begin(), neighbors.end());
                    
                    // ИСПРАВЛЕННЫЙ ИНДЕКС МЕДИАНЫ:
                    pixelData[j * width + i] = neighbors[neighbors.size() / 2];
                }
            }
            
            savePGM("./rezalts/med" + to_string(windowSize) + ".pgm");
        }
        
        pixelData = originalData; // Восстанавливаем в конце
    }

    // ТЕОРИЯ: Сохранение PGM файла
    // Сохраняем в том же формате P5
    bool savePGM(const string& filename) {
        ofstream file(filename, ios::binary);
        if (!file) {
            cerr << "Ошибка создания файла " << filename << endl;
            return false;
        }

        // Запись заголовка
        file << magicNumber << "\n";
        file << width << " " << height << "\n";
        file << maxValue << "\n";
        
        // Запись бинарных данных
        file.write(reinterpret_cast<const char*>(pixelData.data()), pixelData.size());

        if (!file) {
            cerr << "Ошибка записи данных" << endl;
            return false;
        }

        cout << "Файл успешно сохранен как: " << filename << endl;
        return true;
    }
};

// Пример использования с меню
int main() {
    setlocale(LC_ALL, "Russian");
    PGMImageProcessor processor;
    processor.loadPGM("./Date/Kat2.pgm");
    processor.invertColors();
    processor.savePGM("./rezalts/sr.pgm");
    PGMImageProcessor processor2;
    processor2.loadPGM("./Date/Kat2.pgm");
    processor2.run_sr();
    PGMImageProcessor processor3;
    processor3.loadPGM("./Date/Kat2.pgm");
    processor3.med();
    return 0;
}