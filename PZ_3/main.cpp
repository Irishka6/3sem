#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdint>
#include <algorithm>
#include <numeric>
#include <random>

#ifdef _WIN32
    #include <windows.h>
    #include <direct.h>
#else
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

using namespace std;

class PGMImageProcessor {
private:
    string filename_or;
    int width, height;
    int maxValue;
    vector<uint8_t> pixelData;
    string magicNumber;
    vector<vector<string>> csv_data;
    mt19937 rng;
    
public:
    PGMImageProcessor() : rng(random_device{}()) {
        // Инициализируем CSV данные
        csv_data = {
            {"Название исходного файла", "Название отфильтрованного файла", "Размер Файла", "Метод фильтрации", "Среднее отклонение", "Сходство с оригиналом"}
        };
    }

    // Функция для создания папки
    bool createDirectory(const string& path) {
#ifdef _WIN32
        return _mkdir(path.c_str()) == 0;
#else
        return mkdir(path.c_str(), 0755) == 0;
#endif
    }

    // Функция для проверки существования папки
    bool directoryExists(const string& path) {
#ifdef _WIN32
        DWORD attrib = GetFileAttributesA(path.c_str());
        return (attrib != INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_DIRECTORY));
#else
        struct stat info;
        return stat(path.c_str(), &info) == 0 && (info.st_mode & S_IFDIR);
#endif
    }

    // Функция загрузки PGM файла
    bool loadPGM(const string& filename) {
        filename_or = filename;
        ifstream file(filename, ios::binary);
        if (!file) {
            cerr << "Ошибка: не удалось открыть файл " << filename << endl;
            return false;
        }

        file >> magicNumber;
        if (magicNumber != "P5") {
            cerr << "Ошибка: поддерживается только бинарный PGM (P5)" << endl;
            return false;
        }

        skipComments(file);
        file >> width >> height;
        
        skipComments(file);
        file >> maxValue;
        file.get();

        cout << "Информация о PGM файле:" << endl;
        cout << "  Формат: " << magicNumber << endl;
        cout << "  Размер: " << width << "x" << height << endl;
        cout << "  Макс. значение: " << maxValue << endl;

        if (maxValue != 255) {
            cerr << "Ошибка: поддерживается только max_value = 255" << endl;
            return false;
        }

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

    // Функция конвертации изображения в PGM
    bool convertToPGM(const vector<uint8_t>& imageData, int imgWidth, int imgHeight, const string& outputFilename) {
        // Создаем папку если её нет
        string folderPath = getFolderPath(outputFilename);
        if (!directoryExists(folderPath)) {
            if (!createDirectory(folderPath)) {
                cerr << "Ошибка создания папки: " << folderPath << endl;
                return false;
            }
        }
        
        ofstream file(outputFilename, ios::binary);
        if (!file) {
            cerr << "Ошибка создания файла " << outputFilename << endl;
            return false;
        }

        // Устанавливаем параметры PGM
        width = imgWidth;
        height = imgHeight;
        maxValue = 255;
        magicNumber = "P5";
        pixelData = imageData;

        file << magicNumber << "\n";
        file << width << " " << height << "\n";
        file << maxValue << "\n";
        file.write(reinterpret_cast<const char*>(pixelData.data()), pixelData.size());

        if (!file) {
            cerr << "Ошибка записи данных" << endl;
            return false;
        }

        cout << "Изображение успешно конвертировано в: " << outputFilename << endl;
        return true;
    }

    // Функция создания тестового изображения (градиент)
    bool createTestImage(int imgWidth, int imgHeight, const string& outputFilename) {
        vector<uint8_t> testImage(imgWidth * imgHeight);
        
        // Создаем градиентное изображение
        for (int y = 0; y < imgHeight; y++) {
            for (int x = 0; x < imgWidth; x++) {
                uint8_t value = static_cast<uint8_t>((x * 255) / imgWidth);
                testImage[y * imgWidth + x] = value;
            }
        }
        
        return convertToPGM(testImage, imgWidth, imgHeight, outputFilename);
    }

    // Функция добавления шумов на изображение
    void addNoise(int noisePixelCount) {
        if (pixelData.empty()) {
            cerr << "Ошибка: нет загруженного изображения" << endl;
            return;
        }
        
        if (noisePixelCount <= 0) {
            cout << "Количество шумовых пикселей должно быть положительным" << endl;
            return;
        }
        
        vector<uint8_t> originalData = pixelData;
        uniform_int_distribution<int> distCoordX(0, width - 1);
        uniform_int_distribution<int> distCoordY(0, height - 1);
        uniform_int_distribution<int> distValue(0, 255);
        
        cout << "Добавление " << noisePixelCount << " шумовых пикселей..." << endl;
        
        for (int i = 0; i < noisePixelCount; i++) {
            int x = distCoordX(rng);
            int y = distCoordY(rng);
            uint8_t noiseValue = static_cast<uint8_t>(distValue(rng));
            
            pixelData[y * width + x] = noiseValue;
        }
        
        string noisyFilename = "./rezalts/noisy_" + to_string(noisePixelCount) + "_" + getBaseName(filename_or) + ".pgm";
        savePGM(noisyFilename);
        
        // Сохраняем информацию о шумовом изображении в CSV
        csv_data.push_back({
            filename_or,
            noisyFilename,
            to_string(width) + "x" + to_string(height),
            "Шумовые пиксели: " + to_string(noisePixelCount),
            "N/A",
            "N/A"
        });
        
        cout << "Шумы добавлены. Зашумленное изображение сохранено как: " << noisyFilename << endl;
    }

    // МЕДИАННЫЙ ФИЛЬТР
    void applyMedianFilter() {
        if (pixelData.empty()) {
            cerr << "Ошибка: нет загруженного изображения" << endl;
            return;
        }
        
        int val[3] = {3, 5, 7};
        vector<uint8_t> originalData = pixelData;
        
        for (int k = 0; k < 3; k++){
            int windowSize = val[k];
            int radius = windowSize / 2;
            
            cout << "Применение медианного фильтра с окном " << windowSize << "x" << windowSize << endl;
            
            pixelData = originalData;
            vector<uint8_t> tempData = pixelData;
            
            for (int j = radius; j < height - radius; j++) {
                for (int i = radius; i < width - radius; i++) {
                    vector<uint8_t> neighbors;
                    
                    for (int dj = -radius; dj <= radius; dj++) {
                        for (int di = -radius; di <= radius; di++) {
                            neighbors.push_back(tempData[(j + dj) * width + (i + di)]);
                        }
                    }
                    
                    sort(neighbors.begin(), neighbors.end());
                    pixelData[j * width + i] = neighbors[neighbors.size() / 2];
                }
            }
            
            string outputFile = "./rezalts/med" + to_string(windowSize) + "_" + getBaseName(filename_or) + ".pgm";
            savePGM(outputFile);
            compareWithOriginal(tempData, "Медианный " + to_string(windowSize) + "x" + to_string(windowSize), outputFile);
        }
        
        pixelData = originalData;
    }

    void skipComments(ifstream& file) {
        char c;
        while (file.get(c)) {
            if (c == '#') {
                string comment;
                getline(file, comment);
                cout << "  Пропущен комментарий: #" << comment << endl;
            } else {
                file.putback(c);
                break;
            }
        }
    }

    bool savePGM(const string& filename) {
        // Создаем папку если её нет
        string folderPath = getFolderPath(filename);
        if (!directoryExists(folderPath)) {
            if (!createDirectory(folderPath)) {
                cerr << "Ошибка создания папки: " << folderPath << endl;
                return false;
            }
        }
        
        ofstream file(filename, ios::binary);
        if (!file) {
            cerr << "Ошибка создания файла " << filename << endl;
            return false;
        }

        file << magicNumber << "\n";
        file << width << " " << height << "\n";
        file << maxValue << "\n";
        file.write(reinterpret_cast<const char*>(pixelData.data()), pixelData.size());

        if (!file) {
            cerr << "Ошибка записи данных" << endl;
            return false;
        }

        cout << "Файл успешно сохранен как: " << filename << endl;
        return true;
    }

    void compareWithOriginal(const vector<uint8_t>& original, const string& filterName, const string& outputFilename) {
        if (original.size() != pixelData.size()) {
            cout << "Ошибка сравнения: размеры не совпадают!" << endl;
            return;
        }
        
        double total_diff = 0.0;
        int total_pixels = pixelData.size();
        
        for (int i = 0; i < total_pixels; i++) {
            total_diff += abs(static_cast<int>(pixelData[i]) - static_cast<int>(original[i]));
        }
        
        double avg_diff = total_diff / total_pixels;
        double similarity = (1.0 - (avg_diff / 255.0)) * 100.0;
        
        cout << "Фильтр " << filterName << ":" << endl;
        cout << "  Среднее отклонение: " << avg_diff << endl;
        cout << "  Сходство с оригиналом: " << similarity << "%" << endl;
        
        // Добавляем данные в CSV
        csv_data.push_back({
            filename_or,
            outputFilename,
            to_string(width) + "x" + to_string(height),
            filterName,
            to_string(avg_diff),
            to_string(similarity) + "%"
        });
    }

    void writeToCSV(const string& filename) {
        // Создаем папку если её нет
        string folderPath = getFolderPath(filename);
        if (!directoryExists(folderPath)) {
            if (!createDirectory(folderPath)) {
                cerr << "Ошибка создания папки для CSV: " << folderPath << endl;
            }
        }
        
        ofstream file(filename);
        
        if (!file.is_open()) {
            cerr << "Ошибка открытия файла: " << filename << endl;
            return;
        }
        
        for (const auto& row : csv_data) {
            for (size_t i = 0; i < row.size(); i++) {
                file << row[i];
                if (i < row.size() - 1) {
                    file << ";";
                }
            }
            file << "\n";
        }
        
        file.close();
        cout << "Данные записаны в: " << filename << endl;
    }

    // Геттер для получения данных изображения
    const vector<uint8_t>& getPixelData() const { return pixelData; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    string getBaseName(const string& path) {
        size_t lastSlash = path.find_last_of("/\\");
        size_t lastDot = path.find_last_of(".");
        if (lastSlash != string::npos) {
            if (lastDot != string::npos && lastDot > lastSlash) {
                return path.substr(lastSlash + 1, lastDot - lastSlash - 1);
            }
            return path.substr(lastSlash + 1);
        }
        return path;
    }
    
    string getFolderPath(const string& filename) {
        size_t lastSlash = filename.find_last_of("/\\");
        if (lastSlash != string::npos) {
            return filename.substr(0, lastSlash);
        }
        return ".";
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    
    PGMImageProcessor processor;
    
    // Создаем папку для результатов
    if (!processor.directoryExists("./rezalts")) {
        if (processor.createDirectory("./rezalts")) {
            cout << "Создана папка для результатов: ./rezalts" << endl;
        } else {
            cerr << "Ошибка создания папки ./rezalts" << endl;
        }
    }
    
    int choice;
    cout << "=== PGM ИМАДЖ ПРОЦЕССОР ===" << endl;
    cout << "1. Обработать существующие PGM файлы" << endl;
    cout << "2. Создать тестовое изображение и добавить шумы" << endl;
    cout << "Выберите опцию: ";
    cin >> choice;
    
    if (choice == 1) {
        // Обработка существующих файлов
        vector<string> files = {
            "./Date/1.pgm", 
            "./Date/2.pgm", 
            "./Date/3.pgm", 
            "./Date/4.pgm", 
            "./Date/5.pgm", 
            "./Date/6.pgm"
        };
        
        for (int i = 0; i < files.size(); i++){
            cout << "\n=== ОБРАБОТКА ФАЙЛА " << (i+1) << ": " << files[i] << " ===" << endl;
            
            if (processor.loadPGM(files[i])) {
                // Добавляем шумы
                int noiseCount;
                cout << "Введите количество шумовых пикселей: ";
                cin >> noiseCount;
                processor.addNoise(noiseCount);
                
                // Применяем медианный фильтр
                processor.applyMedianFilter();
            }
            cout << "==============================================" << endl;
        }
    } else if (choice == 2) {
        // Создание тестового изображения
        int width, height, noiseCount;
        cout << "Введите ширину изображения: ";
        cin >> width;
        cout << "Введите высоту изображения: ";
        cin >> height;
        cout << "Введите количество шумовых пикселей: ";
        cin >> noiseCount;
        
        string testFilename = "./rezalts/test_image.pgm";
        if (processor.createTestImage(width, height, testFilename)) {
            if (processor.loadPGM(testFilename)) {
                processor.addNoise(noiseCount);
                processor.applyMedianFilter();
            }
        }
    } else {
        cout << "Неверный выбор!" << endl;
        return 1;
    }
    
    // Сохраняем все результаты в один CSV файл
    processor.writeToCSV("./rezalts/Rezalt.csv");
    
    cout << "\n=== ОБРАБОТКА ЗАВЕРШЕНА ===" << endl;
    cout << "Результаты сохранены в ./rezalts/Rezalt.csv" << endl;
    
    return 0;
}