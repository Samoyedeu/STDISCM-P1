#include <iostream>
#include <thread>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <mutex>
#include <cmath>

using namespace std;

mutex primeCheckMutex;
vector<int> primeNumbers;  // Store primes to print at the end

bool readConfigFile(int& xNumThreads, int& yEndRange) {
    const string fileName = "config.txt";
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Error: Unable to open the configuration file." << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        size_t pos = line.find('=');
        if (pos == string::npos) {
            cerr << "Error: Invalid configuration format." << endl;
            return false;
        }

        string key = line.substr(0, pos);
        string value = line.substr(pos + 1);

        if (key == "x") {
            try {
                xNumThreads = stoi(value);
                if (xNumThreads < 1) {
                    cerr << "Error: x must be at least 1." << endl;
                    return false;
                }
            } catch (...) {
                cerr << "Error: Invalid value for x." << endl;
                return false;
            }
        } else if (key == "y") {
            try {
                yEndRange = stoi(value);
                if (yEndRange < 2) {
                    cerr << "Error: y must be at least 2." << endl;
                    return false;
                }
            } catch (...) {
                cerr << "Error: Invalid value for y." << endl;
                return false;
            }
        } else {
            cerr << "Error: Unknown configuration key." << endl;
            return false;
        }
    }

    if (xNumThreads > yEndRange) {
        cerr << "Warning: Number of threads exceeds the range size. Adjusting x to " << yEndRange << "." << endl;
        xNumThreads = yEndRange;
    }

    return true;
}

string getCurrentTimestamp() {
    auto now = chrono::system_clock::now();
    auto nowTime = chrono::system_clock::to_time_t(now);
    auto nowMs = chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()) % 1000;

    tm localTime;
#ifdef _WIN32
    localtime_s(&localTime, &nowTime);
#else
    localtime_r(&nowTime, &localTime);
#endif

    char timeBuffer[100];
    strftime(timeBuffer, sizeof(timeBuffer), "%m/%d/%Y, %I:%M:%S", &localTime);

    char timestamp[120];
    snprintf(timestamp, sizeof(timestamp), "%s.%03d", timeBuffer, static_cast<int>(nowMs.count()));

    return string(timestamp);
}

void checkDivisibility(int n, int i, bool& isPrimeFlag, mutex& flagMutex) {
    if (n % i == 0) {
        lock_guard<mutex> lock(flagMutex);
        isPrimeFlag = false;
    }
}

void processNumber(int n, int xNumThreads) {
    if (n <= 1) return;

    bool isPrimeFlag = true;
    mutex flagMutex;
    vector<thread> threads;

    int sqrtN = sqrt(n);

    // Spawn threads to check divisibility for each number from 2 to sqrt(n)
    for (int i = 2; i <= sqrtN; ++i) {
        threads.emplace_back(checkDivisibility, n, i, ref(isPrimeFlag), ref(flagMutex));
    }

    for (auto& t : threads) {
        t.join();
    }

    if (isPrimeFlag) {
        lock_guard<mutex> lock(primeCheckMutex);
        primeNumbers.push_back(n);
    }
}

void processNumbers(int xNumThreads, int yEndRange) {
    for (int i = 2; i <= yEndRange; i++) {
        processNumber(i, xNumThreads);
    }
}

int main() {
    int xNumThreads, yEndRange;

    cout << "Variant 4 (A2 + B2)" << endl;
    cout << "A2 - Wait until all threads are done, then print everything" << endl;
    cout << "B2 - The search is linear, but the threads are for divisibility testing of individual numbers." << endl;

    if (!readConfigFile(xNumThreads, yEndRange)) {
        cerr << "Configuration error. Exiting." << endl;
        return 1;
    }

    auto startTime = getCurrentTimestamp();
    cout << "Start Time: " << startTime << endl << endl;

    processNumbers(xNumThreads, yEndRange);

    auto endTime = getCurrentTimestamp();

    cout << "Primes Found:" << endl;
    for (int prime : primeNumbers) {
        cout << prime << " ";
    }
    cout << "\n\nEnd Time: " << endTime << endl;

    return 0;
}
