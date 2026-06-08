#pragma once

//Estructura para crear la imagen del DLL
struct ModuloInfo
{
    uintptr_t base;
    SIZE_T size;
};



HWND GetMainWindow(DWORD processId) {
    HWND hwnd = nullptr;
    do {
        hwnd = FindWindowEx(nullptr, hwnd, nullptr, nullptr);
        DWORD pidFound = 0;
        GetWindowThreadProcessId(hwnd, &pidFound);
        if (pidFound == processId && IsWindowVisible(hwnd)) {
            return hwnd;
        }
    } while (hwnd != nullptr);
    return nullptr;
}
HANDLE kg_abrir_proceso(const char* nombre, DWORD acceso) {
    PROCESSENTRY32 proceso;
    proceso.dwSize = sizeof(PROCESSENTRY32);
    HANDLE capturar = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (Process32First(capturar, &proceso) == FALSE) {
        CloseHandle(capturar);
        return INVALID_HANDLE_VALUE;
    }
    while (Process32Next(capturar, &proceso) == TRUE) {
        if (strcmp(proceso.szExeFile, nombre) == 0) {
            HANDLE processHandle = OpenProcess(acceso, FALSE, proceso.th32ProcessID);
            CloseHandle(capturar);
            pid = proceso.th32ProcessID;
            return processHandle;
        }
    }

    CloseHandle(capturar);
    return INVALID_HANDLE_VALUE;
}
ModuloInfo kg_obtener_modulo_info(DWORD processID, const wchar_t* nombre_del_modulo)
{
    ModuloInfo info = { 0, 0 };

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID);

    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32W moduleEntry;
        moduleEntry.dwSize = sizeof(moduleEntry);

        if (Module32FirstW(hSnapshot, &moduleEntry))
        {
            do
            {
                if (!_wcsicmp(moduleEntry.szModule, nombre_del_modulo))
                {
                    info.base = (uintptr_t)moduleEntry.modBaseAddr;
                    info.size = moduleEntry.modBaseSize;
                    break;
                }

            } while (Module32NextW(hSnapshot, &moduleEntry));
        }

        CloseHandle(hSnapshot);
    }

    return info;
}



