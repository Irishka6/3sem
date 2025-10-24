#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdint>
#include <algorithm>
#include <numeric>

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
    
public:
    PGMImageProcessor() {
        // �������������� CSV ������
        csv_data = {
            {"�������� ��������� �����", "�������� ���������������� �����", "������ �����", "����� ����������", "������� ����������", "�������� � ����������"}
        };
    }

    // ������� ��� �������� �����
    bool createDirectory(const string& path) {
#ifdef _WIN32
        return _mkdir(path.c_str()) == 0;
#else
        return mkdir(path.c_str(), 0755) == 0;
#endif
    }

    // ������� ��� �������� ������������� �����
    bool directoryExists(const string& path) {
#ifdef _WIN32
        DWORD attrib = GetFileAttributesA(path.c_str());
        return (attrib != INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_DIRECTORY));
#else
        struct stat info;
        return stat(path.c_str(), &info) == 0 && (info.st_mode & S_IFDIR);
#endif
    }

    bool loadPGM(const string& filename) {
        filename_or = filename;
        ifstream file(filename, ios::binary);
        if (!file) {
            cerr << "������: �� ������� ������� ���� " << filename << endl;
            return false;
        }

        file >> magicNumber;
        if (magicNumber != "P5") {
            cerr << "������: �������������� ������ �������� PGM (P5)" << endl;
            return false;
        }

        skipComments(file);
        file >> width >> height;
        
        skipComments(file);
        file >> maxValue;
        file.get();

        cout << "���������� � PGM �����:" << endl;
        cout << "  ������: " << magicNumber << endl;
        cout << "  ������: " << width << "x" << height << endl;
        cout << "  ����. ��������: " << maxValue << endl;

        if (maxValue != 255) {
            cerr << "������: �������������� ������ max_value = 255" << endl;
            return false;
        }

        int dataSize = width * height;
        pixelData.resize(dataSize);
        file.read(reinterpret_cast<char*>(pixelData.data()), dataSize);

        if (!file) {
            cerr << "������ ������ ������ ��������" << endl;
            return false;
        }

        cout << "���� ������� ��������! ��������: " << pixelData.size() << endl;
        return true;
    }

    void skipComments(ifstream& file) {
        char c;
        while (file.get(c)) {
            if (c == '#') {
                string comment;
                getline(file, comment);
                cout << "  �������� �����������: #" << comment << endl;
            } else {
                file.putback(c);
                break;
            }
        }
    }
    
    // ���������� ������� 
    void slidingAverage() {
        cout << "������ �������� ����� (���������� �������)." << endl;
        vector<uint8_t> originalData = pixelData;
        vector<uint8_t> tempData = pixelData;
        
        for (int j = 1; j < height - 1; j++) {
            for (int i = 1; i < width - 1; i++) {
                int index = j * width + i;
        
                int sum = 
                    tempData[(j-1) * width + (i-1)] + tempData[(j-1) * width + i] + tempData[(j-1) * width + (i+1)] +
                    tempData[j * width + (i-1)]     + tempData[index]               + tempData[j * width + (i+1)] +
                    tempData[(j+1) * width + (i-1)] + tempData[(j+1) * width + i] + tempData[(j+1) * width + (i+1)];
                
                pixelData[index] = static_cast<uint8_t>(sum / 9);
            }
        }
        
        string outputFile = "./rezalts/sr_" + getBaseName(filename_or) + ".pgm";
        savePGM(outputFile);
        compareWithOriginal(originalData, "���������� �������", outputFile);
        
        cout << "�������� ���������" << endl;
    }

    // ������� �������
    void run_sr(){
        cout << "������ �������� ����� (������� �������)." << endl;
        vector<uint8_t> originalData = pixelData;
        
        int testCount = 1;
        for (float k = 0.1; k < 1; k += 0.2){
            pixelData = originalData;
            vector<uint8_t> tempData = pixelData;
            
            for (int j = 0; j < height; j++) {
                float filtered = pixelData[j * width];
                for (int i = 1; i < width; i++) {
                    filtered += (pixelData[j * width + i] - filtered) * k;
                    pixelData[j * width + i] = static_cast<uint8_t>(filtered);
                }
            }
            
            string outputFile = "./rezalts/run_k" + to_string(int(k * 10)) + "_" + getBaseName(filename_or) + ".pgm";
            savePGM(outputFile);
            compareWithOriginal(tempData, "������� ������� k=" + to_string(k), outputFile);
            testCount++;
        }
        
        pixelData = originalData;
    }

    // ��������� ������
    void med(){
        int val[3] = {3, 5, 7};
        vector<uint8_t> originalData = pixelData;
        
        for (int k = 0; k < 3; k++){
            int windowSize = val[k];
            int radius = windowSize / 2;
            
            cout << "��������� ������ � ����� " << windowSize << "x" << windowSize << endl;
            
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
            compareWithOriginal(tempData, "��������� " + to_string(windowSize) + "x" + to_string(windowSize), outputFile);
        }
        
        pixelData = originalData;
    }

    bool savePGM(const string& filename) {
        // ������� ����� ���� � ���
        string folderPath = getFolderPath(filename);
        if (!directoryExists(folderPath)) {
            if (!createDirectory(folderPath)) {
                cerr << "������ �������� �����: " << folderPath << endl;
                return false;
            }
        }
        
        ofstream file(filename, ios::binary);
        if (!file) {
            cerr << "������ �������� ����� " << filename << endl;
            return false;
        }

        file << magicNumber << "\n";
        file << width << " " << height << "\n";
        file << maxValue << "\n";
        file.write(reinterpret_cast<const char*>(pixelData.data()), pixelData.size());

        if (!file) {
            cerr << "������ ������ ������" << endl;
            return false;
        }

        cout << "���� ������� �������� ���: " << filename << endl;
        return true;
    }

    void compareWithOriginal(const vector<uint8_t>& original, const string& filterName, const string& outputFilename) {
        if (original.size() != pixelData.size()) {
            cout << "������ ���������: ������� �� ���������!" << endl;
            return;
        }
        
        double total_diff = 0.0;
        int total_pixels = pixelData.size();
        
        for (int i = 0; i < total_pixels; i++) {
            total_diff += abs(static_cast<int>(pixelData[i]) - static_cast<int>(original[i]));
        }
        
        double avg_diff = total_diff / total_pixels;
        double similarity = (1.0 - (avg_diff / 255.0)) * 100.0;
        
        cout << "������ " << filterName << ":" << endl;
        cout << "  ������� ����������: " << avg_diff << endl;
        cout << "  �������� � ����������: " << similarity << "%" << endl;
        
        // ��������� ������ � CSV
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
        // ������� ����� ���� � ���
        string folderPath = getFolderPath(filename);
        if (!directoryExists(folderPath)) {
            if (!createDirectory(folderPath)) {
                cerr << "������ �������� ����� ��� CSV: " << folderPath << endl;
            }
        }
        
        ofstream file(filename);
        
        if (!file.is_open()) {
            cerr << "������ �������� �����: " << filename << endl;
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
        cout << "������ �������� �: " << filename << endl;
    }

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
    
    vector<string> files = {
        "./Date/1.pgm", 
        "./Date/2.pgm", 
        "./Date/3.pgm", 
        "./Date/4.pgm", 
        "./Date/5.pgm", 
        "./Date/6.pgm"
    };
    
    PGMImageProcessor processor;
    
    // ������� ����� ��� �����������
    if (!processor.directoryExists("./rezalts")) {
        if (processor.createDirectory("./rezalts")) {
            cout << "������� ����� ��� �����������: ./rezalts" << endl;
        } else {
            cerr << "������ �������� ����� ./rezalts" << endl;
        }
    }
    
    for (int i = 0; i < files.size(); i++){
        cout << "\n=== ��������� ����� " << (i+1) << ": " << files[i] << " ===" << endl;
        
        if (processor.loadPGM(files[i])) {
            processor.slidingAverage();
            processor.run_sr();
            processor.med();
        }
        cout << "==============================================" << endl;
    }
    
    // ��������� ��� ���������� � ���� CSV ����
    processor.writeToCSV("Rezalt.csv");
    
    cout << "\n=== ��� ����� ���������� ===" << endl;
    cout << "���������� ��������� � Rezalt.csv" << endl;
    
    return 0;
}