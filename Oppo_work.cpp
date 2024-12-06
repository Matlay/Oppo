#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <string>
#include <algorithm>

using namespace std;

struct PressureData {
    string date;    // Дата в формате "гггг.мм.дд"
    double height;  // Высота в метрах (дробное число)
    int pressure;   // Давление в мм рт. ст. (целое число)
};

ostream& operator<<(ostream& stream, const PressureData& data);
string extractData(const string& data, const string& regex);

string extractData(const string& data, const string& regexStr) {
    regex regExp(regexStr);
    smatch match;

    if (regex_search(data, match, regExp)) {
        return match[1].str();
    }

    return "";
}

ostream& operator<<(ostream& stream, const PressureData& data) {
    stream << "Date: " << data.date << ", Height: " << data.height << " m, Pressure: " << data.pressure << " mmHg";
    return stream;
}

bool comparePressureData(const PressureData& a, const PressureData& b) {
    if (a.pressure != b.pressure) {
        return a.pressure < b.pressure;
    }
    return a.height < b.height;
}

int main() {
    ifstream ist("in.txt");
    if (!ist) {
        cerr << "Error: Unable to open file 'in.txt'" << endl;
        return 1;
    }

    vector<PressureData> measurements;
    string s;

    int minPressure, maxPressure;
    cout << "Enter minimum pressure: ";
    cin >> minPressure;
    cout << "Enter maximum pressure: ";
    cin >> maxPressure;

    while (getline(ist, s)) {
        try {

            int pressure = stoi(extractData(s, R"(((^| )(\d+)( |$)))"));

            if (pressure < minPressure || pressure > maxPressure) {
                continue;
            }

            string date = extractData(s, R"((\d{4}\.\d{2}\.\d{2}))");
            double height = stod(extractData(s, R"(((^| )\d+\.\d+($| )))"));

            measurements.push_back({ date, height, pressure });
        }
        catch (const invalid_argument& e) {
            cerr << "Error: Invalid data format in line '" << s << "'" << endl;
        }
        catch (const out_of_range& e) {
            cerr << "Error: Out of range value in line '" << s << "'" << endl;
        }
    }

    sort(measurements.begin(), measurements.end(), [](const PressureData& a, const PressureData& b) {
        if (a.pressure != b.pressure) {
            return a.pressure < b.pressure;
        }
        return a.height < b.height;
        });

    for (const auto& item : measurements) {
        cout << item << endl;
    }
}