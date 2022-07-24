#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <shobjidl.h> 

BOOL IsProcessRunning(DWORD pid)
{
    HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, pid);
    DWORD ret = WaitForSingleObject(process, 0);
    CloseHandle(process);
    return ret == WAIT_TIMEOUT;
}

DWORD MyGetProcessId(std::wstring ProcessName) // non-conflicting function name
{
    PROCESSENTRY32 pt;
    HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pt.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hsnap, &pt)) { // must call this first
        do {
            LPCTSTR proc = ProcessName.c_str();
            if (!lstrcmpi(pt.szExeFile, proc)) {
                CloseHandle(hsnap);
                return pt.th32ProcessID;
            }
        } while (Process32Next(hsnap, &pt));
    }
    CloseHandle(hsnap);
    return 0;
}

const char* OpenDialog()
{
    COMDLG_FILTERSPEC ComDlgFS[1] = { {L"DLL files", L"*.dll"} };
    IFileOpenDialog* pFileOpen = NULL;
    IShellItem* pShellItem = NULL;
    wchar_t* ppszName = NULL;
    CoInitialize(NULL);
    CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_IFileOpenDialog, (void**)(&pFileOpen));
    pFileOpen->SetFileTypes(1, ComDlgFS);
    pFileOpen->SetTitle(L"DLL File to inject");
    pFileOpen->Show(0);
    pFileOpen->GetResult(&pShellItem);
    pShellItem->GetDisplayName(SIGDN_FILESYSPATH, &ppszName);
    wprintf(L"%s\n", ppszName);
    CoTaskMemFree(ppszName);
    pShellItem->Release();
    pFileOpen->Release();
    CoUninitialize();
    getchar();
    return 0;
}

void Inject(DWORD pid, std::string way)
{
    if (way == "LLA")
    {
        std::cout << "DLL Location -> ";
        std::string dllName;
        std::cin >> dllName;
        size_t sz = strlen(dllName.c_str());
        HANDLE hProcess;
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pid);
        LPVOID lpBaseAddress;
        lpBaseAddress = VirtualAllocEx(hProcess, NULL, sz, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        WriteProcessMemory(hProcess, lpBaseAddress, dllName.c_str(), sz, NULL);
        HMODULE hModule = GetModuleHandle(L"kernel32.dll");
        LPVOID lpStartAddress = GetProcAddress(hModule, "LoadLibraryA");
        HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lpStartAddress, lpBaseAddress, 0, NULL);
        if (hThread)
        {
            std::cout << "Successfully Injected " << dllName << "\n";
            std::cout << "App will be closed in 5 seconds.\n";
            Sleep(5000);
        }
        else
        {
            std::cout << "Failed to Inject " << dllName << "\n";
            std::cout << "App will be closed in 5 seconds.\n";
            Sleep(5000);
        }
    }
    else if (way == "TH")
    {
        std::cout << "\nSoon..\n";
        std::cout << "App will be closed in 5 seconds.\n";
        Sleep(5000);
    }
    else if (way == "MM")
    {
        std::cout << "\nSoon..\n";
        std::cout << "App will be closed in 5 seconds.\n";
        Sleep(5000);
    }
    else
    {
        std::cout << "Injection type is invalid, Please contact the application developer.";
        std::cout << "App will be closed in 5 seconds.\n";
        Sleep(5000);
    }
}

void Kill(DWORD pid)
{
    HANDLE handy;
    handy = OpenProcess(SYNCHRONIZE | PROCESS_TERMINATE, TRUE, pid);
    TerminateProcess(handy, 0);
    std::cout << "Process Has Been Killed.\n";
    std::cout << "App will be closed in 5 seconds.\n";
    Sleep(5000);
}

int main()
{
    SetConsoleTitleA("Rains DLL Injector (x64)");
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, 4);
    std::cout << "Rains DLL Injector\n";
    std::cout << "Made by <Ra1n6666/>#6666\n\n";
    SetConsoleTextAttribute(h, 15);
    std::cout << "Enter Process Name -> ";
    std::wstring process;
    std::wcin >> process;
    DWORD procid = MyGetProcessId(process);
    if (procid != 0)
    {
        SetConsoleTextAttribute(h, 2);
        std::cout << "Process Is Valid.\n";
        SetConsoleTextAttribute(h, 15);
        std::cout << "\n----------Process----------\n";
        std::cout << "Process Bits -> ";
        SetConsoleTextAttribute(h, 2);
        std::wcout << "Soon..\n";
        SetConsoleTextAttribute(h, 15);
        std::cout << "Process Name -> ";
        SetConsoleTextAttribute(h, 2);
        std::wcout << process << "\n";
        SetConsoleTextAttribute(h, 15);
        std::cout << "Process ID -> ";
        SetConsoleTextAttribute(h, 2);
        std::cout << procid << "\n";
        SetConsoleTextAttribute(h, 15);
        std::cout << "---------------------------\n\n";
        SetConsoleTextAttribute(h, 15);
        std::cout << "[";
        SetConsoleTextAttribute(h, 2);
        std::cout << "1";
        SetConsoleTextAttribute(h, 15);
        std::cout << "] ";
        std::cout << "Inject DLL\n";
        SetConsoleTextAttribute(h, 15);
        std::cout << "[";
        SetConsoleTextAttribute(h, 4);
        std::cout << "2";
        SetConsoleTextAttribute(h, 15);
        std::cout << "] ";
        std::cout << "Kill Process\n";
        std::cout << "Choose An Option -> ";
        std::string option;
        std::cin >> option;
        if (option == "1")
        {
            SetConsoleTextAttribute(h, 15);
            std::cout << "\n[";
            SetConsoleTextAttribute(h, 2);
            std::cout << "1";
            SetConsoleTextAttribute(h, 15);
            std::cout << "] ";
            std::cout << "Load Library\n";
            SetConsoleTextAttribute(h, 15);
            std::cout << "[";
            SetConsoleTextAttribute(h, 2);
            std::cout << "2";
            SetConsoleTextAttribute(h, 15);
            std::cout << "] ";
            std::cout << "Thread Hijacking\n";
            SetConsoleTextAttribute(h, 15);
            std::cout << "[";
            SetConsoleTextAttribute(h, 2);
            std::cout << "3";
            SetConsoleTextAttribute(h, 15);
            std::cout << "] ";
            std::cout << "Manual Map\n";
            std::cout << "Injection Type -> ";
            std::string type;
            std::cin >> type;
            if (type == "1")
            {
                Inject(procid, "LLA");
            }
            else if (type == "2")
            {
                Inject(procid, "TH");
            }
            else if (type == "3")
            {
                Inject(procid, "MM");
            }
            else
            {
                std::cout << "Choice is invalid.\n";
                std::cout << "App will be closed in 5 seconds.\n";
                Sleep(5000);
            }
        }
        else if (option == "2")
        {
            Kill(procid);
        }
        else
        {
            std::cout << "Choice is invalid.\n";
            std::cout << "App will be closed in 5 seconds.\n";
            Sleep(5000);
        }
    }
    else
    {
        SetConsoleTextAttribute(h, 4);
        std::cout << "\nProcess Is Invalid.\n";
        SetConsoleTextAttribute(h, 15);
        std::cout << "App will be closed in 5 seconds.\n";
        Sleep(5000);
    }
}