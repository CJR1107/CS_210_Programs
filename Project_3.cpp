#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <limits>
#include <cctype>

using namespace std;

class ItemTracker {
private:
    map<string, int> itemCounts;
    map<string, string> displayNames;

    static string ToLower(const string& s) {
        string result = s;
        for (char& ch : result) {
            ch = static_cast<char>(tolower(static_cast<unsigned char>(ch)));
        }
        return result;
    }

public:
    bool LoadFromFile(const string& inputFileName) {
        ifstream inFile(inputFileName);
        if (!inFile.is_open()) {
            return false;
        }

        string item;
        while (inFile >> item) {
            string key = ToLower(item);
            if (displayNames.find(key) == displayNames.end()) {
                displayNames[key] = item;
            }
            itemCounts[key]++;
        }
        inFile.close();
        return true;
    }

    bool WriteBackupFile(const string& outputFileName) const {
        ofstream outFile(outputFileName);
        if (!outFile.is_open()) {
            return false;
        }

        for (const auto& pair : itemCounts) {
            const string& key = pair.first;
            int count = pair.second;
            auto it = displayNames.find(key);
            string nameToPrint = (it != displayNames.end()) ? it->second : key;
            outFile << nameToPrint << " " << count << endl;
        }
        outFile.close();
        return true;
    }

    int GetItemFrequency(const string& itemName) const {
        string key = ToLower(itemName);
        auto it = itemCounts.find(key);
        return (it != itemCounts.end()) ? it->second : 0;
    }

    void PrintAllFrequencies() const {
        for (const auto& pair : itemCounts) {
            auto it = displayNames.find(pair.first);
            string nameToPrint = (it != displayNames.end()) ? it->second : pair.first;
            cout << nameToPrint << " " << pair.second << endl;
        }
    }

    void PrintHistogram(char symbol = '*') const {
        for (const auto& pair : itemCounts) {
            auto it = displayNames.find(pair.first);
            string nameToPrint = (it != displayNames.end()) ? it->second : pair.first;
            cout << nameToPrint << " ";
            for (int i = 0; i < pair.second; ++i) {
                cout << symbol;
            }
            cout << endl;
        }
    }
};

static void PrintMenu() {
    cout << "\n===== Corner Grocer Item Tracker =====\n";
    cout << "1. Look up an item frequency\n";
    cout << "2. Print all item frequencies\n";
    cout << "3. Print histogram\n";
    cout << "4. Exit\n";
    cout << "Choose an option (1-4): ";
}

static int ReadMenuChoice() {
    int choice;
    while (true) {
        if (cin >> choice && choice >= 1 && choice <= 4) {
            return choice;
        }
        cout << "Invalid input. Enter a number from 1 to 4: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int main() {
    const string inputFileName = "CS210_Project_Three_Input_File.txt";
    const string backupFileName = "frequency.dat";

    ItemTracker tracker;

    if (!tracker.LoadFromFile(inputFileName)) {
        cout << "Error opening input file." << endl;
        return 1;
    }

    tracker.WriteBackupFile(backupFileName);

    while (true) {
        PrintMenu();
        int choice = ReadMenuChoice();

        if (choice == 1) {
            cout << "Enter item name: ";
            string item;
            cin >> item;
            cout << item << " frequency: " << tracker.GetItemFrequency(item) << endl;
        }
        else if (choice == 2) {
            tracker.PrintAllFrequencies();
        }
        else if (choice == 3) {
            tracker.PrintHistogram();
        }
        else {
            cout << "Goodbye!" << endl;
            break;
        }
    }
    return 0;
}
