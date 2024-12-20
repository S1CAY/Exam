#include <windows.h>
#include <iostream>
#include <string>

int main() {
    
    LPCWSTR inputFileName = L"input.txt";
    LPCWSTR outputFileName = L"output.txt";

    
    HANDLE hInputFile = CreateFileW(
        inputFileName,
        GENERIC_READ,
        FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (hInputFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Помилка: Не вдалося відкрити вхідний файл.\n";
        return 1;
    }

    
    HANDLE hOutputFile = CreateFileW(
        outputFileName,
        GENERIC_WRITE,
        0,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (hOutputFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Помилка: Не вдалося відкрити/створити вихідний файл.\n";
        CloseHandle(hInputFile); // Не забуваємо закрити вже відкритий вхідний файл
        return 1;
    }

    const DWORD bufferSize = 4096;
    char buffer[bufferSize];
    DWORD bytesRead = 0;
    DWORD bytesWritten = 0;

    
    while (true) {
        BOOL readResult = ReadFile(hInputFile, buffer, bufferSize, &bytesRead, nullptr);
        if (!readResult) {
            std::cerr << "Помилка: Не вдалося зчитати з файлу.\n";
            break;
        }

        
        if (bytesRead == 0) {
            break; 
        }

        BOOL writeResult = WriteFile(hOutputFile, buffer, bytesRead, &bytesWritten, nullptr);
        if (!writeResult || bytesWritten != bytesRead) {
            std::cerr << "Помилка: Не вдалося записати у файл.\n";
            break;
        }
    }

    
    CloseHandle(hInputFile);
    CloseHandle(hOutputFile);

    std::cout << "Операцію успішно виконано!\n";
    return 0;
}
