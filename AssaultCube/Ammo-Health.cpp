#include <windows.h>
#include <iostream>

int main()
{
    uintptr_t pointerAddress = 0x0057E0A8; // Base address + relative offset
    uintptr_t ammoOffset = 0x138;
    uintptr_t healthOffset = 0xEC;
    uintptr_t valueAtPointer;

    HWND hWnd = FindWindowA(NULL, "AssaultCube");

    if (hWnd == NULL) {
        std::cerr << "Window not found!" << std::endl;
        return 1;
    }

    DWORD processID;
    GetWindowThreadProcessId(hWnd, &processID);

    if (processID == 0) {
        std::cerr << "Failed to retrieve process ID!" << std::endl;
        return 1;
    }

    std::cout << "Process ID: " << processID << std::endl;

    // Open the process with full access
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);

    if (hProcess == NULL) {
        std::cerr << "Failed to open process!" << std::endl;
        return 1;
    }

    // Read memory at pointerAddress
    if (ReadProcessMemory(hProcess, reinterpret_cast<void*>(pointerAddress), &valueAtPointer, sizeof(valueAtPointer), NULL)) {
        std::cout << "Value at address " << std::hex << pointerAddress << " is: " << std::hex << valueAtPointer << std::endl;
    }
    else {
        std::cerr << "Failed to read memory!" << std::endl;
        CloseHandle(hProcess);
        return 1;
    }

    // Calculate final address
    uintptr_t ammoMemoryLocation = valueAtPointer + ammoOffset;
    uintptr_t healthMemoryLocation = valueAtPointer + healthOffset;

    // The new value to write at the calculated memory location
    int newValue = 1337;
    SIZE_T bytesWritten;

    if (WriteProcessMemory(hProcess, reinterpret_cast<void*>(ammoMemoryLocation), &newValue, sizeof(newValue), &bytesWritten)) {
        std::cout << "Successfully wrote new value: " << newValue << " to address: " << std::hex << ammoMemoryLocation << std::endl;
    }
    else {
        std::cerr << "Failed to write memory!" << std::endl;
    }
    if (WriteProcessMemory(hProcess, reinterpret_cast<void*>(healthMemoryLocation), &newValue, sizeof(newValue), &bytesWritten)) {
        std::cout << "Successfully wrote new value: " << newValue << " to address: " << std::hex << healthMemoryLocation << std::endl;
    }
    else {
        std::cerr << "Failed to write memory!" << std::endl;
    }

    // Close the handle to the process
    CloseHandle(hProcess);

    return 0;
}
