#include <windows.h>
#include <iostream>
#include <string>

int main() {
    LPCWSTR fileName = L"input.txt";

    
    HANDLE hFile = CreateFileW(
        fileName,
        GENERIC_READ,
        FILE_SHARE_READ,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Помилка: не вдалося відкрити файл." << std::endl;
        return 1;
    }

    
    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(hFile, &fileSize)) {
        std::cerr << "Помилка: не вдалося отримати розмір файлу." << std::endl;
        CloseHandle(hFile);
        return 1;
    }

    if (fileSize.QuadPart == 0) {
        std::cout << "Файл порожній." << std::endl;
        CloseHandle(hFile);
        return 0;
    }

    
    HANDLE hMap = CreateFileMappingW(
        hFile,            
        nullptr,          
        PAGE_READONLY,    
        0,                
        0,                
        nullptr           
    );

    if (hMap == nullptr) {
        std::cerr << "Помилка: не вдалося створити відображення файлу." << std::endl;
        CloseHandle(hFile);
        return 1;
    }

    
    LPVOID lpBase = MapViewOfFile(
        hMap,
        FILE_MAP_READ,  
        0,              
        0,              
        0               
    );

    if (lpBase == nullptr) {
        std::cerr << "Помилка: не вдалося відобразити файл у пам'ять." << std::endl;
        CloseHandle(hMap);
        CloseHandle(hFile);
        return 1;
    }

    
    
    const char* fileData = static_cast<const char*>(lpBase);
    size_t size = static_cast<size_t>(fileSize.QuadPart);

    
    size_t count_a = 0;
    for (size_t i = 0; i < size; ++i) {
        if (fileData[i] == 'a') {
            count_a++;
        }
    }

    std::wcout << L"Кількість символів 'a' у файлі: " << count_a << std::endl;

    
    UnmapViewOfFile(lpBase);
    CloseHandle(hMap);
    CloseHandle(hFile);

    return 0;
}
