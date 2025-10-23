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
    string magicNumber;  // "P5" ��� ��������� PGM

public:
    // ������: ������ PGM �����
    // ������ ��������� ��������� + �������� ������
    bool loadPGM(const string& filename) {
        ifstream file(filename, ios::binary);
        if (!file) {
            cerr << "������: �� ������� ������� ���� " << filename << endl;
            return false;
        }

        // ������ magic number
        file >> magicNumber;
        if (magicNumber != "P5") {
            cerr << "������: �������������� ������ �������� PGM (P5)" << endl;
            return false;
        }

        // ������: ������� ������������ � ������ ���������
        skipComments(file);
        file >> width >> height;
        
        skipComments(file);
        file >> maxValue;
        
        // ������� ������ ����� (������ \n ��� ������)
        file.get(); 

        cout << "���������� � PGM �����:" << endl;
        cout << "  ������: " << magicNumber << endl;
        cout << "  ������: " << width << "x" << height << endl;
        cout << "  ����. ��������: " << maxValue << endl;

        // �������� ��������� �������
        if (maxValue != 255) {
            cerr << "������: �������������� ������ max_value = 255" << endl;
            return false;
        }

        // ������: ������ �������� ������
        // ������ ������ = width * height * 1 ���� (��� 8-�������)
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

    // ������: ������� ������������ � PGM
    // ����������� ���������� � # � ���� �� ����� ������
    void skipComments(ifstream& file) {
        char c;
        while (file.get(c)) {
            if (c == '#') {
                // ���������� ��� ������ �����������
                string comment;
                getline(file, comment);
                cout << "  �������� �����������: #" << comment << endl;
            } else {
                // ���������� ������ ������� � �����
                file.putback(c);
                break;
            }
        }
    }
    
    // ���������� ������� 
    void invertColors() {
        cout << "������ �������� �����." << endl;
        vector<uint8_t> tempData = pixelData;
        
        for (int j = 1; j < height - 1; j++) {
            for (int i = 1; i < width - 1; i++) {
               int index = j * width + i;
        
            // ���������� ���������� �������� ��� ���� 3x3
            int sum = 
                tempData[(j-1) * width + (i-1)] + tempData[(j-1) * width + i] + tempData[(j-1) * width + (i+1)] +
                tempData[j * width + (i-1)]     + tempData[index]               + tempData[j * width + (i+1)] +
                tempData[(j+1) * width + (i-1)] + tempData[(j+1) * width + i] + tempData[(j+1) * width + (i+1)];
            
                pixelData[index] = static_cast<uint8_t>(sum / 9);
            }
        }
        
        cout << "�������������� ���������!" << endl;
    }

    // ������� �������
    void run_sr(){
        cout << "������ �������� �����." << endl;
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
        vector<uint8_t> originalData = pixelData; // ��������� ��������
        
        for (int k = 0; k < 3; k++){
            int windowSize = val[k];
            int radius = windowSize / 2; // ������ = ������_���� / 2
            
            cout << "��������� ������ � ����� " << windowSize << "x" << windowSize << endl;
            
            pixelData = originalData; // ��������������� ��������
            vector<uint8_t> tempData = pixelData;
            
            // ������������ �������: radius ������ 1
            for (int j = radius; j < height - radius; j++) {
                for (int i = radius; i < width - radius; i++) {
                    vector<uint8_t> neighbors;
                    
                    // �������� ������� �� ���� windowSize x windowSize
                    for (int dj = -radius; dj <= radius; dj++) {
                        for (int di = -radius; di <= radius; di++) {
                            neighbors.push_back(tempData[(j + dj) * width + (i + di)]);
                        }
                    }
                    
                    sort(neighbors.begin(), neighbors.end());
                    
                    // ������������ ������ �������:
                    pixelData[j * width + i] = neighbors[neighbors.size() / 2];
                }
            }
            
            savePGM("./rezalts/med" + to_string(windowSize) + ".pgm");
        }
        
        pixelData = originalData; // ��������������� � �����
    }

    // ������: ���������� PGM �����
    // ��������� � ��� �� ������� P5
    bool savePGM(const string& filename) {
        ofstream file(filename, ios::binary);
        if (!file) {
            cerr << "������ �������� ����� " << filename << endl;
            return false;
        }

        // ������ ���������
        file << magicNumber << "\n";
        file << width << " " << height << "\n";
        file << maxValue << "\n";
        
        // ������ �������� ������
        file.write(reinterpret_cast<const char*>(pixelData.data()), pixelData.size());

        if (!file) {
            cerr << "������ ������ ������" << endl;
            return false;
        }

        cout << "���� ������� �������� ���: " << filename << endl;
        return true;
    }
};

// ������ ������������� � ����
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