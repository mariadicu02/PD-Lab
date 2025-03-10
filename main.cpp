#include <windows.h>
#include <stdio.h>
#include <tchar.h>

void ReadImagePath(HKEY hKeyRoot, LPCTSTR pszPath) {

	HKEY hKey;
	DWORD dwIndex = 0;
	TCHAR szKeyName[256];
	DWORD dwKeySize = sizeof(szKeyName)/ sizeof(TCHAR);

	LONG lResult = RegOpenKeyEx(
								hKeyRoot,
								pszPath,
								0,
								KEY_READ,
								&hKey 
	);
	if (lResult != ERROR_SUCCESS) {
		_tprintf(_T("Eroare la deschiderea registrului, %ld\n"), lResult);
		return;
	}
	while (RegEnumKeyEx(
		hKey,
		dwIndex,
		szKeyName,
		&dwKeySize,
		NULL,
		NULL,
		NULL,
		NULL) == ERROR_SUCCESS)
	{
		TCHAR szFullPath[512];
		_stprintf_s(szFullPath, _T("%s\\%s"), pszPath, szKeyName);
		HKEY hSubKey;
		if (RegOpenKeyEx(hKeyRoot, szFullPath, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
		{
			DWORD dwType;
			TCHAR szImagePath[512];
			DWORD dwSize = sizeof(szImagePath);
			if (RegQueryValueEx(
								hSubKey,
								_T("Imagepath"),
								NULL,
								&dwType,
								(LPBYTE)szImagePath,
								&dwSize) == ERROR_SUCCESS)
			{
				if (dwType == REG_SZ || dwType == REG_EXPAND_SZ)
				{
					_tprintf(_T("Serviciu: %s\nPath: %s\n\n"), szKeyName, szImagePath);
				}
			}
			RegCloseKey(hSubKey);
		}
		dwKeySize = sizeof(szKeyName) / sizeof(TCHAR);
		dwIndex++;
	}
	RegCloseKey(hKey);
}

int _tmain(void) {
	ReadImagePath(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Services"));
	return 0;
}