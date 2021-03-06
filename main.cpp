#include "injection.h"

const char szProc[] = "explorer.exe";
const char szDllFile[] = "example.dll";

int main() {
	PROCESSENTRY32 PE32{ 0 };
	PE32.dwSize = sizeof(PE32);

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) {
		std::cout << "[-] CreateToolhelp32Snapshot error: 0x" << GetLastError() << std::endl;
		system("PAUSE");
		return 0;
	}

	DWORD PID = 0;
	BOOL bRet = Process32First(hSnap, &PE32);
	while (bRet) {
		if (!strcmp(szProc, PE32.szExeFile)) {
			PID = PE32.th32ProcessID;
			break;
		}
		bRet = Process32Next(hSnap, &PE32);
	}

	CloseHandle(hSnap);

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (!hProc) {
		std::cout << "[-] OpenProcess error: 0x" << GetLastError() << std::endl;
		system("PAUSE");
		return 0;
	}

	if (!ManualMap(hProc, szDllFile)) {
		std::cout << "[-] Injection error" << std::endl;
		CloseHandle(hProc);
		system("PAUSE");
		return 0;
	}

	CloseHandle(hProc);

	std::cout << "[+] Injected!" << std::endl;
	system("PAUSE");

	return 0;
}
