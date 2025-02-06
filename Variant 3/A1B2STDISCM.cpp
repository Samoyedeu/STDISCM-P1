#include <iostream>
#include <thread>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <mutex>
#include <atomic>
#include <cmath>

using namespace std;

mutex primeCheckMutex;
mutex printMutex;
bool isPrimeFlag = true; 

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

void checkDivisibility(int n, int divisor, int threadID) {   
    if (n % divisor == 0) {
        std::lock_guard<std::mutex> lock(primeCheckMutex);
        isPrimeFlag = false;
        return;
    }
}

void processNumber(int n, int numThreads) {
    if (n <= 1) return;

    isPrimeFlag = true;
    std::vector<std::thread> threads;
    
    int limit = static_cast<int>(sqrt(n));
    int threadID = 0; // static id

    // Check divisibility of n by all numbers from 2 to sqrt(n) only
    for (int i = 2; i <= limit; ++i) {
        threads.emplace_back(checkDivisibility, n, i, threadID++);

        // Join the threads if the xNumThreads is more than the required
        if (threads.size() >= static_cast<size_t>(numThreads)) {
            for (auto &t : threads) t.join();
            threads.clear();
            threadID = 0; 
        }
    }

    // Join any remaining threads
    for (auto &t : threads) t.join();

    if (isPrimeFlag) {
        auto startTime = std::chrono::system_clock::now();
        auto timeStamp = std::chrono::system_clock::to_time_t(startTime);
        auto duration = startTime.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;

        std::tm timeInfo;
        localtime_s(&timeInfo, &timeStamp);
        char timeBuffer[9];
        std::strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", &timeInfo);

        {
            lock_guard<std::mutex> lock(printMutex);
            cout << "Thread ID: " << threadID
                 << "\nTime Stamp: " << getCurrentTimestamp()
                 << "\nPrime: " << n << "\n" << endl;
        }
    }
}

void processNumbers(int xNumThreads, int yEndRange) {
    for (int i = 2; i <= yEndRange; i++) {
        processNumber(i, xNumThreads);
    }
}

int main() {
    int xNumThreads, yEndRange;

    cout << "Variant 3 (A1 + B2)" << endl;
    cout << "A1 - Print immediately (Thread ID and timestamp included)" << endl;
    cout << "B2 - The search is linear, but the threads are for divisibility testing." << endl;

    if (!readConfigFile(xNumThreads, yEndRange)) {
        cerr << "Configuration error. Exiting." << endl;
        return 1;
    }

    auto startTime = getCurrentTimestamp();
    cout << "Start Time: " << startTime << endl << endl;

    processNumbers(xNumThreads, yEndRange);

    auto endTime = getCurrentTimestamp();
    cout << "\nEnd Time: " << endTime << endl;

    return 0;
}
