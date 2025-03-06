#include <windows.h>
#include <iostream>

int main() {
    // Hide our own console window
    HWND hwnd = GetConsoleWindow();
    if (hwnd) {
        ShowWindow(hwnd, SW_HIDE);
    }

    // Setup startup info to have the spawned process show its window normally
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;  // Task Manager's window should be visible
    ZeroMemory(&pi, sizeof(pi));

    // Use cmd.exe to run the command as if from a batch file
    // This command mimics "start taskmgr.exe -d"
    char cmd[] = "cmd.exe /c start taskmgr.exe -d";

    // Create the process; note that we do not use CREATE_NO_WINDOW here so that
    // Task Manager can appear as expected.
    if (!CreateProcessA(
            NULL,    // No module name; use command line
            cmd,     // Command line
            NULL,    // Process handle not inheritable
            NULL,    // Thread handle not inheritable
            FALSE,   // Set handle inheritance to FALSE
            0,       // Creation flags (0 so no suppression of windows)
            NULL,    // Use parent's environment block
            NULL,    // Use parent's starting directory 
            &si,     // Pointer to STARTUPINFO structure
            &pi)     // Pointer to PROCESS_INFORMATION structure
       ) {
        std::cerr << "CreateProcess failed with error: " << GetLastError() << std::endl;
        return 1;
    }

    // Close process and thread handles as they're not needed further
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
